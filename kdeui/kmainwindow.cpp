 /* This file is part of the KDE libraries
     Copyright
     (C) 2000 Reginald Stadlbauer (reggie@kde.org)
     (C) 1997 Stephan Kulow (coolo@kde.org)
     (C) 1997-2000 Sven Radej (radej@kde.org)
     (C) 1997-2000 Matthias Ettrich (ettrich@kde.org)
     (C) 1999 Chris Schlaeger (cs@kde.org)

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Library General Public
     License version 2 as published by the Free Software Foundation.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Library General Public License for more details.

     You should have received a copy of the GNU Library General Public License
     along with this library; see the file COPYING.LIB.  If not, write to
     the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
     Boston, MA 02111-1307, USA.
 */

#include "kmainwindow.h"

#include <qsessionmanager.h>
#include <qobjectlist.h>

#include <kapp.h>
#include <kconfig.h>
#include <kdebug.h>
#include <khelpmenu.h>
#include <kmenubar.h>
#include <kstatusbar.h>
#include <qlayout.h>

#include <klocale.h>
#include <kstddirs.h>
#include <kstaticdeleter.h>
#include <netwm.h>

#include <stdlib.h>
#include <ctype.h>

class KMainWindowPrivate {
public:
    bool showHelpMenu:1;

    bool autoSaveSettings:1;
    bool settingsDirty:1;
    bool autoSaveWindowSize:1;
    QString autoSaveGroup;
};

QList<KMainWindow>* KMainWindow::memberList = 0L;
static bool no_query_exit = false;
static KMWSessionManaged* ksm = 0;
static KStaticDeleter<KMWSessionManaged> ksmd;

class KMWSessionManaged : public KSessionManaged
{
public:
    KMWSessionManaged()
    {
    };
    ~KMWSessionManaged()
    {
    }
    bool saveState( QSessionManager& )
    {
        KConfig* config = KApplication::kApplication()->sessionConfig();
        if ( KMainWindow::memberList->first() ){
            // According to Jochen Wilhelmy <digisnap@cs.tu-berlin.de>, this
            // hook is usefull for better document orientation
            KMainWindow::memberList->first()->saveGlobalProperties(config);
        }

        QListIterator<KMainWindow> it(*KMainWindow::memberList);
        int n = 0;
        config->setGroup(QString::fromLatin1("Number"));
        config->writeEntry(QString::fromLatin1("NumberOfWindows"), KMainWindow::memberList->count());
        for (it.toFirst(); it.current(); ++it){
            n++;
            it.current()->savePropertiesInternal(config, n);
        }
        return TRUE;
    }

    bool commitData( QSessionManager& sm )
    {
        // not really a fast method but the only compatible one
        if ( sm.allowsInteraction() ) {
            bool cancelled = false;
            QListIterator<KMainWindow> it(*KMainWindow::memberList);
            KMainWindow* last = 0;
            ::no_query_exit = true;
            for (it.toFirst(); it.current() && !cancelled; ++it){
                if ( !it.current()->testWState( Qt::WState_ForceHide ) ) {
                    last = it.current();
                    QCloseEvent e;
                    QApplication::sendEvent( last, &e );
                    cancelled = !e.isAccepted();
//                     if ( !cancelled && it.current()->testWFlags( Qt::WDestructiveClose ) )
//                       delete it.current();
                }
            }
            no_query_exit = FALSE;
            if ( !cancelled && last )
                cancelled = !last->queryExit();
            return !cancelled;
        }

        // the user wants it, the user gets it
        return TRUE;
    }
};

KMainWindow::KMainWindow( QWidget* parent, const char *name, WFlags f )
    : QMainWindow( parent, name, f ), KXMLGUIBuilder( this ), helpMenu2( 0 ), factory_( 0 )
{
    setDockMenuEnabled( FALSE );
    mHelpMenu = 0;
    kapp->setTopWidget( this );
    connect(kapp, SIGNAL(shutDown()), this, SLOT(shuttingDown()));
    if( !memberList )
        memberList = new QList<KMainWindow>;

    if ( !ksm )
        ksm = ksmd.setObject(new KMWSessionManaged());
    memberList->append( this );
    if ( !name ) {
        // set a unique object name. Required by session management.
        QCString s;
        s.setNum( memberList->count() );
        setName( kapp->instanceName() + "-mainwindow#" + s );
    }

    d = new KMainWindowPrivate;
    d->showHelpMenu = true;
    d->settingsDirty = false;
    d->autoSaveSettings = false;
    d->autoSaveWindowSize = true; // for compatibility

    setCaption( kapp->caption() );
}

KMainWindow::~KMainWindow()
{
    QMenuBar* mb = internalMenuBar();
    delete mb;
    delete d;
    memberList->remove( this );
}

KPopupMenu* KMainWindow::helpMenu( const QString &aboutAppText, bool showWhatsThis )
{
    if( mHelpMenu == 0 ) {
        if ( aboutAppText.isEmpty() )
            mHelpMenu = new KHelpMenu( this, instance()->aboutData(), showWhatsThis);
        else
            mHelpMenu = new KHelpMenu( this, aboutAppText, showWhatsThis );

        if ( mHelpMenu == 0 )
            return 0;
        connect( mHelpMenu, SIGNAL( showAboutApplication() ),
                 this, SLOT( showAboutApplication() ) );
    }

    return mHelpMenu->menu();
}

KPopupMenu* KMainWindow::customHelpMenu( bool showWhatsThis )
{
    if( mHelpMenu == 0 ) {
        mHelpMenu = new KHelpMenu( this, QString::null, showWhatsThis );
        connect( mHelpMenu, SIGNAL( showAboutApplication() ),
                 this, SLOT( showAboutApplication() ) );
    }

    return mHelpMenu->menu();
}

bool KMainWindow::canBeRestored( int number )
{
    if ( !kapp->isRestored() )
        return FALSE;
    KConfig *config = kapp->sessionConfig();
    if ( !config )
        return FALSE;
    config->setGroup( QString::fromLatin1("Number") );
    int n = config->readNumEntry( QString::fromLatin1("NumberOfWindows") , 0 );
    return number >= 1 && number <= n;
}

const QString KMainWindow::classNameOfToplevel( int number )
{
    if ( !kapp->isRestored() )
        return QString::null;
    KConfig *config = kapp->sessionConfig();
    if ( !config )
        return QString::null;
    QString s;
    s.setNum( number );
    s.prepend( QString::fromLatin1("WindowProperties") );
    config->setGroup( s );
    if ( !config->hasKey( QString::fromLatin1("ClassName") ) )
        return QString::null;
    else
        return config->readEntry( QString::fromLatin1("ClassName") );
}

bool KMainWindow::restore( int number, bool show )
{
    if ( !canBeRestored( number ) )
        return FALSE;
    KConfig *config = kapp->sessionConfig();
    if ( readPropertiesInternal( config, number ) ){
        if ( show )
            KMainWindow::show();
        return FALSE;
    }
    return FALSE;
}

KXMLGUIFactory *KMainWindow::guiFactory()
{
    if ( !factory_ )
        factory_ = new KXMLGUIFactory( this, this, "guifactory" );
    return factory_;
}

void KMainWindow::createGUI( const QString &xmlfile, bool _conserveMemory )
{
    // disabling the updates prevents unnecessary redraws
    setUpdatesEnabled( false );

    // just in case we are rebuilding, let's remove our old client
    guiFactory()->removeClient( this );

    // make sure to have an empty GUI
    if ( internalMenuBar() )
        internalMenuBar()->clear();

    (void)toolBarIterator(); // make sure toolbarList is most-up-to-date
    toolbarList.setAutoDelete( true );
    toolbarList.clear();
    toolbarList.setAutoDelete( false );

    // don't build a help menu unless the user ask for it
    if (d->showHelpMenu) {
        // we always want a help menu
        if (helpMenu2 == 0)
            helpMenu2 = new KHelpMenu(this, instance()->aboutData(), true,
                                      actionCollection());
    }

    // we always want to load in our global standards file
    setXMLFile( locate( "config", "ui/ui_standards.rc", instance() ) );

    // now, merge in our local xml file.  if this is null, then that
    // means that we will be only using the global file
    if ( !xmlfile.isNull() ) {
        setXMLFile( xmlfile, true );
    } else {
        QString auto_file(instance()->instanceName() + "ui.rc");
        setXMLFile( auto_file, true );
    }

    // make sure we don't have any state saved already
    setXMLGUIBuildDocument( QDomDocument() );

    // do the actual GUI building
    guiFactory()->addClient( this );

    // try and get back *some* of our memory
    if ( _conserveMemory )
    {
      // before freeing the memory allocated by the DOM document we also
      // free all memory allocated internally in the KXMLGUIFactory for
      // the menubar and the toolbars . This however implies that we
      // have to take care of deleting those widgets ourselves. For
      // destruction this is no problem, but when rebuilding we have
      // to take care of that (and we want to rebuild the GUI when
      // using stuff like the toolbar editor ).
      // In addition we have to take care of not removing containers
      // like popupmenus, defined in the XML document.
      // this code should probably go into a separate method in KMainWindow.
      // there's just one problem: I'm bad in finding names ;-) , so
      // I skipped this ;-)

      QDomDocument doc = domDocument();

      QDomElement e = doc.documentElement().firstChild().toElement();
      for (; !e.isNull(); e = e.nextSibling().toElement() ) {
          if ( e.tagName().lower() == "toolbar" )
              factory_->resetContainer( e.attribute( "name" ) );
          else if ( e.tagName().lower() == "menubar" )
              factory_->resetContainer( e.tagName(), true );
      }

      // ### We cannot free the DOM document here, because then the toolbar state saving
      // doesn't work (Simon)
//      conserveMemory();
    }

    setUpdatesEnabled( true );
    updateGeometry();
}

void KMainWindow::setHelpMenuEnabled(bool showHelpMenu)
{
    d->showHelpMenu = showHelpMenu;
}

bool KMainWindow::isHelpMenuEnabled()
{
    return d->showHelpMenu;
}

void KMainWindow::setCaption( const QString &caption )
{
    setPlainCaption( kapp->makeStdCaption(caption) );
}

void KMainWindow::setCaption( const QString &caption, bool modified )
{
    setPlainCaption( kapp->makeStdCaption(caption, true, modified) );
}

void KMainWindow::setPlainCaption( const QString &caption )
{
    QMainWindow::setCaption( caption );
    NETWinInfo info( qt_xdisplay(), winId(), qt_xrootwin(), 0 );
    info.setName( caption.utf8().data() );
}

void KMainWindow::appHelpActivated( void )
{
    if( mHelpMenu == 0 ) {
        mHelpMenu = new KHelpMenu( this );
        if ( mHelpMenu == 0 )
            return;
    }
    mHelpMenu->appHelpActivated();
}


void KMainWindow::closeEvent ( QCloseEvent *e )
{
    if (queryClose()) {
        e->accept();

        // Save settings if auto-save is enabled, and settings have changed
        if (d->settingsDirty && d->autoSaveSettings)
        {
            //kdDebug(200) << "KMainWindow::closeEvent -> saving settings" << endl;
            saveMainWindowSettings( KGlobal::config(), d->autoSaveGroup );
            KGlobal::config()->sync();
            d->settingsDirty = false;
        }

        int not_withdrawn = 0;
        QListIterator<KMainWindow> it(*KMainWindow::memberList);
        for (it.toFirst(); it.current(); ++it){
            if ( !it.current()->isHidden() && it.current()->isTopLevel() && it.current() != this )
                not_withdrawn++;
        }

        if ( !no_query_exit && not_withdrawn <= 0 ) { // last window close accepted?
            if ( queryExit() ) {            // Yes, Quit app?
                // We saved the toolbars already
                disconnect(kapp, SIGNAL(shutDown()), this, SLOT(shuttingDown()));
                kapp->deref();             // ...and quit aplication.
            }  else {
                // cancel closing, it's stupid to end up with no windows at all....
                e->ignore();
            }
        }
    }
}

bool KMainWindow::queryExit()
{
    return TRUE;
}

bool KMainWindow::queryClose()
{
    return TRUE;
}

void KMainWindow::saveGlobalProperties( KConfig*  )
{
}

void KMainWindow::readGlobalProperties( KConfig*  )
{
}

#if defined(KDE_COMPAT)
void KMainWindow::updateRects()
{
}
#endif

void KMainWindow::showAboutApplication( void )
{
}

void KMainWindow::savePropertiesInternal( KConfig *config, int number )
{
    // in order they are in toolbar list

    QString s;
    s.setNum(number);
    s.prepend(QString::fromLatin1("WindowProperties"));
    saveMainWindowSettings(config, s); // Menubar, statusbar and Toolbar settings.

    s.setNum(number);
    config->setGroup(s);
    saveProperties(config);
}

void KMainWindow::saveMainWindowSettings(KConfig *config, const QString &configGroup)
{
    kdDebug(200) << "KMainWindow::saveMainWindowSettings " << configGroup << endl;
    QString entry;
    QStrList entryList;

    if (!configGroup.isEmpty())
       config->setGroup(configGroup);

    // Called by session management - or if we want to save the window size anyway
    if (!configGroup.isEmpty() || d->autoSaveWindowSize)
    {
       // store objectName, className, Width and Height  for later restoring
       config->writeEntry(QString::fromLatin1("ObjectName"), name());
       config->writeEntry(QString::fromLatin1("ClassName"), className());
       QWidget *desk = KApplication::desktop();
       config->writeEntry(QString::fromLatin1("Width %1").arg(desk->width()), width() );
       config->writeEntry(QString::fromLatin1("Height %1").arg(desk->height()), height() );
    }

    if (internalStatusBar()) {
        entryList.clear();
        if ( internalStatusBar()->isHidden() )
            entryList.append("Disabled");
        else
            entryList.append("Enabled");
        config->writeEntry(QString::fromLatin1("StatusBar"), entryList, ';');
    }

    if (internalMenuBar()) {
        entryList.clear();
        if ( internalMenuBar()->isHidden() )
            entryList.append("Disabled");
        else
            entryList.append("Enabled");
        config->writeEntry(QString::fromLatin1("MenuBar"), entryList, ';');
    }

    int n = 1; // Toolbar counter. toolbars are counted from 1,
    KToolBar *toolbar = 0;
    QString toolKey;
    QListIterator<KToolBar> it( toolBarIterator() );
    while ( ( toolbar = it.current() ) ) {
        ++it;
        QString group;
        if (!configGroup.isEmpty())
        {
           // Give a number to the toolbar, but prefer a name if there is one,
           // because there's no real guarantee on the ordering of toolbars
           group = (!strcmp(toolbar->name(), "unnamed") ? QString::number(n) : QString(" ")+toolbar->name());
           group.prepend(" Toolbar");
           group.prepend(configGroup);
        }
        toolbar->saveSettings(config, group);
        n++;
    }
}

bool KMainWindow::readPropertiesInternal( KConfig *config, int number )
{
    if ( number == 1 )
        readGlobalProperties( config );

    // in order they are in toolbar list
    QString s;
    s.setNum(number);
    s.prepend(QString::fromLatin1("WindowProperties"));

    applyMainWindowSettings(config, s); // Menubar, statusbar and toolbar settings.

    s.setNum(number);
    config->setGroup(s);
    readProperties(config);
    return true;
}

void KMainWindow::applyMainWindowSettings(KConfig *config, const QString &configGroup)
{
    kdDebug(200) << "KMainWindow::applyMainWindowSettings" << endl;
    QString entry;
    QStrList entryList;
    int i = 0; // Number of entries in list

    if (!configGroup.isEmpty())
       config->setGroup(configGroup);

    // restore the object name (window role)
    if ( config->hasKey(QString::fromLatin1("ObjectName" )) )
        setName( config->readEntry(QString::fromLatin1("ObjectName")).latin1()); // latin1 is right here

    // restore the size
    QWidget *desk = KApplication::desktop();
    QSize size( config->readNumEntry( QString::fromLatin1("Width %1").arg(desk->width()), 0 ),
                config->readNumEntry( QString::fromLatin1("Height %1").arg(desk->height()), 0 ) );
    if (size.isEmpty()) {
        // try the KDE 2.0 way
        size = QSize( config->readNumEntry( QString::fromLatin1("Width"), 0 ),
                      config->readNumEntry( QString::fromLatin1("Height"), 0 ) );
        if (!size.isEmpty()) {
            // make sure the other resolutions don't get old settings
            config->writeEntry( QString::fromLatin1("Width"), 0 );
            config->writeEntry( QString::fromLatin1("Height"), 0 );
        }
    }
    if ( !size.isEmpty() )
        resize( size );

    if (internalStatusBar()) {
        entryList.clear();
        i = config->readListEntry (QString::fromLatin1("StatusBar"), entryList, ';');
        entry = entryList.first();
        if (entry == QString::fromLatin1("Disabled"))
            internalStatusBar()->hide();
        else
            internalStatusBar()->show();
    }

    if (internalMenuBar()) {
        entryList.clear();
        i = config->readListEntry (QString::fromLatin1("MenuBar"), entryList, ';');
        entry = entryList.first();
        if (entry==QString::fromLatin1("Disabled"))
            internalMenuBar()->hide();
        else
            internalMenuBar()->show();
    }

    int n = 1; // Toolbar counter. toolbars are counted from 1,
    KToolBar *toolbar;
    QString toolKey;
    QListIterator<KToolBar> it( toolBarIterator() ); // must use own iterator

    for ( ; it.current(); ++it) {
        toolbar= it.current();
        QString group;
        if (!configGroup.isEmpty())
        {
           // Give a number to the toolbar, but prefer a name if there is one,
           // because there's no real guarantee on the ordering of toolbars
           group = (!strcmp(toolbar->name(), "unnamed") ? QString::number(n) : QString(" ")+toolbar->name());
           group.prepend(" Toolbar");
           group.prepend(configGroup);
        }
        toolbar->applySettings(config, group);
        n++;
    }

    finalizeGUI( true );
}

void KMainWindow::finalizeGUI( bool force )
{
    //kdDebug(200) << "KMainWindow::finalizeGUI force=" << force << endl;
    // The whole reason for this is that moveToolBar relies on the indexes
    // of the other toolbars, so in theory it should be called only once per
    // toolbar, but in increasing order of indexes.
    // Since we can't do that immediately, we move them, and _then_
    // we call positionYourself again for each of them, but this time
    // the toolbariterator should give them in the proper order.
    // Both the XMLGUI and applySettings call this, hence "force" for the latter.
    QListIterator<KToolBar> it( toolBarIterator() );
    for ( ; it.current() ; ++ it )
        it.current()->positionYourself( force );

    d->settingsDirty = false;
}

void KMainWindow::setSettingsDirty()
{
    //kdDebug(200) << "KMainWindow::setSettingsDirty" << endl;
    d->settingsDirty = true;
}

bool KMainWindow::settingsDirty() const
{
    return d->settingsDirty;
}

QString KMainWindow::settingsGroup() const
{
    return d->autoSaveGroup;
}

void KMainWindow::setAutoSaveSettings( const QString & groupName, bool saveWindowSize )
{
    d->autoSaveSettings = true;
    d->autoSaveGroup = groupName;
    d->autoSaveWindowSize = saveWindowSize;
    // Get notified when the user moves a toolbar around
    connect( this, SIGNAL( endMovingToolBar( QToolBar * ) ),
             this, SLOT( setSettingsDirty() ) );
    // Now read the previously saved settings
    applyMainWindowSettings( KGlobal::config(), groupName );
}

void KMainWindow::resetAutoSaveSettings()
{
    d->autoSaveSettings = false;
}

void KMainWindow::resizeEvent( QResizeEvent * )
{
    if ( d->autoSaveWindowSize )
        setSettingsDirty();
}

KMenuBar *KMainWindow::menuBar()
{
    if ( !internalMenuBar() )
        return new KMenuBar( this );
    return (KMenuBar*)internalMenuBar();
}

KStatusBar *KMainWindow::statusBar()
{
    if ( !internalStatusBar() )
        return new KStatusBar( this );
    return (KStatusBar*)internalStatusBar();
}

void KMainWindow::shuttingDown()
{
    // call the virtual queryExit
    queryExit();
}

QMenuBar *KMainWindow::internalMenuBar()
{
    QObjectList *l = queryList( "QMenuBar" );
    if ( !l || !l->first() ) {
        delete l;
        return 0;
    }

    QMenuBar *m = (QMenuBar*)l->first();
    delete l;
    return m;
}

QStatusBar *KMainWindow::internalStatusBar()
{
    QObjectList *l = queryList( "QStatusBar" );
    if ( !l || !l->first() ) {
        delete l;
        return 0;
    }

    QStatusBar *s = (QStatusBar*)l->first();
    delete l;
    return s;
}

void KMainWindow::childEvent( QChildEvent* e)
{
    QMainWindow::childEvent( e );
}

KToolBar *KMainWindow::toolBar( const char * name )
{
    if (!name)
       name = "mainToolBar";
    KToolBar *tb = (KToolBar*)child( name, "KToolBar" );
    if ( tb )
        return tb;
    bool honor_mode = (name == "mainToolBar");

    if ( builderClient() )
	return new KToolBar(this, name, honor_mode); // XMLGUI constructor
    else
	return new KToolBar(this, Top, false, name, honor_mode ); // non-XMLGUI
}

QListIterator<KToolBar> KMainWindow::toolBarIterator()
{
    toolbarList.clear();
    QList<QToolBar> lst;
    for ( int i = (int)QMainWindow::Unmanaged; i <= (int)Minimized; ++i ) {
        lst = toolBars( (ToolBarDock)i );
        for ( QToolBar *tb = lst.first(); tb; tb = lst.next() ) {
            if ( !tb->inherits( "KToolBar" ) )
                continue;
            toolbarList.append( (KToolBar*)tb );
        }
    }
    return QListIterator<KToolBar>( toolbarList );
}

void KMainWindow::paintEvent( QPaintEvent * )
{
    // do nothing
}


#include "kmainwindow.moc"

