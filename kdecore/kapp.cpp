/* This file is part of the KDE libraries
    Copyright (C) 1997 Matthias Kalle Dalheimer (kalle@kde.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
	*/

// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <qdir.h> // must be at the front
#include <qcollection.h>
#include <qwidgetlist.h>
#include <qstrlist.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qregexp.h>
#include <qkeycode.h>
#include <qpopupmenu.h>
#include <qsessionmanager.h>
#include <qlist.h>
#include <qtranslator.h>
#include <qpixmapcache.h>

#include <kapp.h>
#include <kglobal.h>
#include <kstddirs.h>
#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kconfig.h>
#include <ksimpleconfig.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kglobalsettings.h>
#include <kipc.h>
#include <kcrash.h>

#include <kstyle.h>
#include <qplatinumstyle.h>
#include <qcdestyle.h>
#include <kdestyle.h>

#include <dcopclient.h>

#include <sys/types.h>
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#include <sys/wait.h>

#include "ltdl.h"

#include "kwm.h"

#include <fcntl.h>
#include <stdlib.h> // getenv(), srand(), rand()
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>

#include "kprocctrl.h"

#ifdef HAVE_PATHS_H
#include <paths.h>
#endif

#ifndef _PATH_TMP
#define _PATH_TMP "/tmp/"
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/SM/SMlib.h>

// defined by X11 headers
#ifdef KeyPress
#undef KeyPress
#endif

KApplication* KApplication::KApp = 0L;
//extern bool bAreaCalculated;

template class QList<KSessionManaged>;

extern "C" {
static int kde_xio_errhandler( Display * )
{
  return kapp->xioErrhandler();
}

static int kde_x_errhandler( Display *dpy, XErrorEvent *err )
{
    char errstr[256];
    XGetErrorText( dpy, err->error_code, errstr, 256 );
    if ( err->error_code != BadWindow )
	qWarning( "KDE detected X Error: %s %d\n  Major opcode:  %d", errstr, err->error_code, err->request_code );
    return 0;
}
}

/*
  Private data to make keeping binary compatibility easier
 */
class KApplicationPrivate
{
public:
    KApplicationPrivate()
    {
    }
    ~KApplicationPrivate()
    {
    }
};


static QList<QWidget>*x11Filter = 0;
void KApplication::installX11EventFilter( QWidget* filter )
{
    if ( !filter )
	return;
    if (!x11Filter)
	x11Filter = new QList<QWidget>;
    connect ( filter, SIGNAL( destroyed() ), this, SLOT( x11FilterDestroyed() ) );
    x11Filter->append( filter );
}

void KApplication::x11FilterDestroyed()
{
    if ( !x11Filter || !sender() )
	return;
    QWidget* w = (QWidget*) sender();
    x11Filter->removeRef( w );
    if ( x11Filter->isEmpty() ) {
	delete x11Filter;
	x11Filter = 0;
    }
}


// the help class for session management communication
static QList<KSessionManaged>* sessionClients()
{
    static QList<KSessionManaged>* session_clients = 0L;
    if ( !session_clients )
	session_clients = new QList<KSessionManaged>;
    return session_clients;
}

/*
  Auxiliary function to calculate a a session config name used for the
  instance specific config object.
  Syntax:  "<appname>:<sessionId>"
 */
static QString sessionConfigName()
{
  QString aSessionConfigName;
  QTextOStream ts( &aSessionConfigName );
  ts << qApp->name() << ":" << qApp->sessionId();
  return aSessionConfigName;
}

static SmcConn mySmcConnection = 0;
static QTime* smModificationTime = 0;

KApplication::KApplication( int& argc, char** argv, const QCString& rAppName,
                            bool allowStyles, bool GUIenabled ) :
    QApplication( argc, argv, GUIenabled ), KInstance(rAppName)
{
    if (!GUIenabled)
       allowStyles = false;
    useStyles = allowStyles;
    ASSERT (!rAppName.isEmpty());
    setName(rAppName);
    pAppData = new KApplicationPrivate;

    init(GUIenabled);
    parseCommandLine( argc, argv );
}

KApplication::KApplication( bool allowStyles, bool GUIenabled ) :
  QApplication( *KCmdLineArgs::qt_argc(), *KCmdLineArgs::qt_argv(),
                GUIenabled ),
  KInstance( KCmdLineArgs::about)
{
    if (!GUIenabled)
       allowStyles = false;
    useStyles = allowStyles;
    setName( instanceName() );
    pAppData = new KApplicationPrivate;

    init(GUIenabled);
    parseCommandLine( );
}

KApplication::KApplication(Display *display, int& argc, char** argv, const QCString& rAppName,
                           bool allowStyles, bool GUIenabled ) :
  QApplication( display ), KInstance(rAppName)
{
    if (!GUIenabled)
       allowStyles = false;
    useStyles = allowStyles;

    ASSERT (!rAppName.isEmpty());
    setName(rAppName);
    pAppData = new KApplicationPrivate;

    init(GUIenabled);
    parseCommandLine( argc, argv );
}

int KApplication::xioErrhandler()
{
  emit shutDown();
  exit( 1 );
  return 0;
}

class KDETranslator : public QTranslator
{
public:
  KDETranslator(QObject *parent) : QTranslator(parent, "kdetranslator") {}
  virtual QString find(const char*, const char* message) const {
    return i18n(message);
  }
};

void KApplication::init(bool GUIenabled)
{
  QApplication::setDesktopSettingsAware( false );

  KApp = this;

  styleHandle = 0;
  pKStyle = 0;
  smw = 0;

  // Initial KIPC event mask.
  kipcEventMask = (1 << KIPC::StyleChanged) | (1 << KIPC::PaletteChanged) |
	          (1 << KIPC::FontChanged) | (1 << KIPC::BackgroundChanged);

  if (GUIenabled)
  {
    // this is important since we fork() to launch the help (Matthias)
    fcntl(ConnectionNumber(qt_xdisplay()), F_SETFD, FD_CLOEXEC);
    // set up the fancy (=robust and error ignoring ) KDE xio error handlers (Matthias)
    XSetErrorHandler( kde_x_errhandler );
    XSetIOErrorHandler( kde_xio_errhandler );

    connect( this, SIGNAL( aboutToQuit() ), this, SIGNAL( shutDown() ) );

    display = desktop()->x11Display();
    kipcCommAtom = XInternAtom(display, "KIPC_COMM_ATOM", false);

    // GJ: Work around a bug in kconfig or kinstance.
    KGlobal::config()->reparseConfiguration();
    kdisplaySetStyle();
    kdisplaySetFont();
    kdisplaySetPalette();
  }

  installTranslator(new KDETranslator(this));

  // install appdata resource type
  KGlobal::dirs()->addResourceType("appdata", KStandardDirs::kde_default("data")
                                   + QString::fromLatin1(name()) + '/');
  pSessionConfig = 0L;
  pDCOPClient = 0L; // don't instantiate until asked to do so.
  bSessionManagement = true;

  // register a communication window for desktop changes (Matthias)
  if (GUIenabled)
  {
    Atom a = XInternAtom(qt_xdisplay(), "KDE_DESKTOP_WINDOW", false);
    smw = new QWidget(0,0);
    long data = 1;
    XChangeProperty(qt_xdisplay(), smw->winId(), a, a, 32,
					PropModeReplace, (unsigned char *)&data, 1);
  }

  captionLayout = CaptionAppLast;
}

static int my_system (const char *command) {
   int pid, status;

   QApplication::flushX();
   pid = fork();
   if (pid == -1)
      return -1;
   if (pid == 0) {
      setuid( getuid() ); // Make sure a set-user-id prog. is not root anymore
      setgid( getgid() );
      const char* shell = "/bin/sh";
      if (getenv("SHELL"))
         shell = getenv("SHELL");
      execl(shell, shell, "-c", command, 0L);
      exit(127);
   }
   do {
      if (waitpid(pid, &status, 0) == -1) {
         if (errno != EINTR)
            return -1;
       } else
            return status;
   } while(1);
}


DCOPClient *KApplication::dcopClient()
{
  if (pDCOPClient)
    return pDCOPClient;

  // create an instance specific DCOP client object
  // if dcopserver lockfile not present, start the server.
  // GJ: Do we really want to do this?
  QCString fName(::getenv("DCOPSERVER"));
  if (fName.isEmpty()) {
    fName = ::getenv("HOME");
    fName += "/.DCOPserver";
    if (::access(fName.data(), R_OK) == -1) {
      QString srv = KStandardDirs::findExe(QString::fromLatin1("kdeinit"));
      my_system(srv.latin1());
    }
  }

  pDCOPClient = new DCOPClient();
  connect(pDCOPClient, SIGNAL(attachFailed(const QString &)),
	  SLOT(dcopFailure(const QString &)));

  return pDCOPClient;
}

KConfig* KApplication::sessionConfig()
{
    if (pSessionConfig)
	return pSessionConfig;

    // create an instance specific config object
    pSessionConfig = new KConfig( sessionConfigName(), false, false);

    return pSessionConfig;
}


KSessionManaged::KSessionManaged()
{
    sessionClients()->remove( this );
    sessionClients()->append( this );
}

KSessionManaged::~KSessionManaged()
{
    sessionClients()->remove( this );
}

bool KSessionManaged::saveState(QSessionManager&)
{
    return true;
}

bool KSessionManaged::commitData(QSessionManager&)
{
    return true;
}


void KApplication::disableSessionManagement() {
  bSessionManagement = false;
}


bool KApplication::requestShutDown()
{
    if ( mySmcConnection ) {
	// we already have a connection to the session manager, use it.
	SmcRequestSaveYourself( mySmcConnection, SmSaveBoth, True, SmInteractStyleAny, False, True );
	return TRUE;
    }

    // open a temporary connection, if possible

    propagateSessionManager();
    QCString smEnv = ::getenv("SESSION_MANAGER");
    if (smEnv.isEmpty())
	return FALSE;

    char cerror[256];
    char* myId = 0;
    char* prevId = 0;
    SmcCallbacks cb;
    SmcConn smcConnection = SmcOpenConnection( 0, 0, 1, 0,
					       0, &cb,
					       prevId,
					       &myId,
					       255,
					       cerror );
    ::free( myId ); // it was allocated by C
    if (!smcConnection )
	return FALSE;

    SmcRequestSaveYourself( smcConnection, SmSaveBoth, True, SmInteractStyleAny, False, True );
    SmcCloseConnection( smcConnection, 0, 0 );
    return TRUE;
}

void KApplication::propagateSessionManager()
{
    QCString fName = ::getenv("HOME");
    fName += "/.KSMserver";
    QCString smEnv = ::getenv("SESSION_MANAGER");
    bool check = smEnv.isEmpty();
    if ( !check && smModificationTime ) {
	 QFileInfo info( fName );
	 QTime current = info.lastModified().time();
	 check = current > *smModificationTime;
    }
    if ( check ) {
	delete smModificationTime;
	QFile f( fName );
	if ( !f.open( IO_ReadOnly ) )
	    return;
	QFileInfo info ( f );
	smModificationTime = new QTime( info.lastModified().time() );
	QTextStream t(&f);
	QString s = t.readLine();
	f.close();
	::setenv( "SESSION_MANAGER", s.latin1(), TRUE  );
    }
}

void KApplication::commitData( QSessionManager& sm )
{
    bool cancelled = false;
    for (KSessionManaged* it = sessionClients()->first();
	 it && !cancelled;
	 it = sessionClients()->next() ) {
	cancelled = !it->commitData( sm );
    }
    if ( cancelled )
	sm.cancel();

    if ( sm.allowsInteraction() ) {
	QWidgetList done;
	QWidgetList *list = QApplication::topLevelWidgets();
	bool cancelled = FALSE;
	QWidget* w = list->first();
	while ( !cancelled && w ) {
	    if ( !w->testWState( WState_ForceHide ) && !w->inherits("KTMainWindow") ) {
		QCloseEvent e;
		sendEvent( w, &e );
		cancelled = !e.isAccepted();
		if ( !cancelled )
		    done.append( w );
		delete list; // one never knows...
		list = QApplication::topLevelWidgets();
		w = list->first();
	    } else {
		w = list->next();
	    }
	    while ( w && done.containsRef( w ) )
		w = list->next();
	}
	delete list;
    }


    if ( !bSessionManagement ) {
	sm.setRestartHint( QSessionManager::RestartNever );
	return;
    }
}

void KApplication::saveState( QSessionManager& sm )
{
    static bool firstTime = false;
    mySmcConnection = (SmcConn) sm.handle();

    if ( !bSessionManagement ) {
	sm.setRestartHint( QSessionManager::RestartNever );
	return;
    }

    if ( firstTime ) {
	firstTime = false;
	return; // no need to save the state.
    }

    QString aLocalFileName = KGlobal::dirs()->saveLocation("config") +
	sessionConfigName();

    // remove former session config if still existing, we want a new and fresh one
    if ( pSessionConfig ) {
	delete pSessionConfig;
	pSessionConfig = 0;
	QFile f ( aLocalFileName );
	if ( f.exists() )
	    f.remove();
    }

    // tell the session manager about our new lifecycle
    QStringList restartCommand = sm.restartCommand();
    sm.setRestartCommand( restartCommand );

    // finally: do session management
    emit saveYourself(); // for compatiblity
    bool cancelled = false;
    for (KSessionManaged* it = sessionClients()->first();
	 it && !cancelled;
	 it = sessionClients()->next() ) {
	cancelled = !it->saveState( sm );
    }

    // if we created a new session config object, register a proper discard command
    if ( pSessionConfig ) {
	pSessionConfig->sync();
	QStringList discard;
//	discard  << ( "rm "+aLocalFileName ); // only one argument  due to broken xsm
        discard  << "rm" << aLocalFileName; // WABA: Screw xsm
	sm.setDiscardCommand( discard );
    }

    if ( cancelled )
	sm.cancel();
}

#if 0
//
// 1999-10-21 Espen Sand:
// The functionality is moved to ktmainwindow. I have decided that I will
// not remove the code from kapp yet. You should only uncomment this is you
// really need to do so. In 99% of the cases it is simpler to modify the
// code that depend on this See KDE2PORTING
//
// This code WILL be delected quite soon.
//
QPopupMenu* KApplication::helpMenu( bool /*bAboutQtMenu*/,
	   const QString& aboutAppText )
{
  int id = 0;
  QPopupMenu* pMenu = new QPopupMenu();

  id = pMenu->insertItem( i18n( "&Contents" ) );
  pMenu->connectItem( id, this, SLOT( appHelpActivated() ) );
  pMenu->setAccel( Key_F1, id );

  pMenu->insertSeparator();

  id = pMenu->insertItem(i18n("&About %1...").arg(aAppName));
  if( !aboutAppText.isNull() )
	{
	  pMenu->connectItem( id, this, SLOT( aboutApp() ) );
	  aAppAboutString = aboutAppText;
	}

  id = pMenu->insertItem( i18n( "About &KDE..." ) );
  pMenu->connectItem( id, this, SLOT( aboutKDE() ) );
  /*
	if( bAboutQtMenu )
	{
	id = pMenu->insertItem( i18n( "About Qt" ) );
	pMenu->connectItem( id, this, SLOT( aboutQt() ) );
	}
  */
  return pMenu;
}


void KApplication::appHelpActivated()
{
  invokeHTMLHelp( QString(name()) + "/" + "index.html", "" );
}


void KApplication::aboutKDE()
{
  QMessageBox about(i18n( "About KDE" ),
		    i18n(
			 "\nThe KDE Desktop Environment was written by the KDE Team,\n"
			 "a world-wide network of software engineers committed to\n"
			 "free software development.\n\n"
			 "Visit http://www.kde.org for more information on the KDE\n"
			 "Project. Please consider joining and supporting KDE.\n\n"
			 "Please report bugs at http://bugs.kde.org.\n"),
		    QMessageBox::Information,
		    QMessageBox::Ok + QMessageBox::Default, 0, 0,
		    0, "aboutkde");
  about.setButtonText(0, i18n("&OK"));
  about.exec();
}

void KApplication::aboutApp()
{
  QWidget* w = activeWindow();
  QString caption = i18n("About %1").arg(kapp->caption());
  QMessageBox about(caption, aAppAboutString, QMessageBox::Information,
		   QMessageBox::Ok + QMessageBox::Default, 0, 0, w, "aboutapp");
  about.setButtonText(0, i18n("&OK"));
  about.setIconPixmap(icon());
  about.exec();
}


void KApplication::aboutQt(){
   //  QWidget* w = activeWindow();
  //  QMessageBox::aboutQt( w, caption() );
}
#endif


void KApplication::dcopFailure(const QString &msg)
{
  QString msgStr(i18n("There was some error setting up inter-process\n"
		      "communications for KDE.  The message returned\n"
		      "by the system was:\n\n"));
  msgStr += msg;
  msgStr += i18n("\n\nPlease check that the \"dcopserver\" program is running!");

  QMessageBox::critical(kapp->mainWidget(),
			i18n("DCOP communications error (%1)").arg(kapp->caption()),
			msgStr, i18n("OK"));
}

void KApplication::parseCommandLine( int& argc, char** argv )
{
    enum parameter_code { unknown = 0, caption, icon, miniicon, dcopserver };
    const char* parameter_strings[] = { "-caption", "-icon", "-miniicon", "-dcopserver", 0 };

    aDummyString2 = " ";
    int i = 1;
    parameter_code parameter;
    while( i < argc ) {
	parameter = unknown;

	for ( int p = 0 ; parameter_strings[p]; p++)
	    if ( !strcmp( argv[i], parameter_strings[p]) ) {
		parameter = static_cast<parameter_code>(p + 1);
		break;
	    }

	if ( parameter != unknown && argc < i +2 ) { // last argument without parameters
	    argc -= 1;
	    break; // jump out of the while loop
	}

	switch (parameter) {
	case caption:
	    aCaption = argv[i+1];
	    aDummyString2 += parameter_strings[caption-1];
	    aDummyString2 += " \"";
	    aDummyString2 += argv[i+1];
	    aDummyString2 += "\" ";
	    break;
	case icon:
	    if (argv[i+1][0] == '/')
		aIconPixmap = QPixmap(argv[i+1]);
	    else
		aIconPixmap = DesktopIcon(argv[i+1]);
	    if (aMiniIconPixmap.isNull())
		aMiniIconPixmap = SmallIcon(argv[i+1]);
	    aDummyString2 += parameter_strings[icon-1];
	    aDummyString2 += " ";
	    aDummyString2 += argv[i+1];
	    aDummyString2 += " ";
	    break;
	case miniicon:
	    aMiniIconPixmap = SmallIcon(argv[i+1]);
	    aDummyString2 += parameter_strings[miniicon-1];
	    aDummyString2 += " ";
	    aDummyString2 += argv[i+1];
	    aDummyString2 += " ";
	    break;
	case dcopserver:
	    dcopClient()->setServerAddress(argv[i+1]);
	    break;
	case unknown:
	    i++;
	}

	if ( parameter != unknown ) { // remove arguments

	    for( int j = i;  j < argc-2; j++ )
		argv[j] = argv[j+2];

	    argc -=2 ;
	}

    }
    pArgc = argc;
}

static const KCmdLineOptions qt_options[] =
{
   { "display <displayname>", I18N_NOOP("Use the X-server display 'displayname'"), 0},
   { "font <fontname>",  I18N_NOOP("Set default font to 'fontname'"), 0},
   { "session <sessionId>", I18N_NOOP("Restore the application for the given 'sessionId'"), 0},
   { "cmap", I18N_NOOP("Causes the application to install a private color map on an 8-bit display"), 0},
   { "ncols <count>", I18N_NOOP("Limits the number of colors allocated in the color cube on a 8-bit display,\nif the application is using the QApplication::ManyColor color specification"), 0},
   { 0, 0, 0 }
};
// NOTE: Other Qt options include (color options omitted) :
// name, title, geometry, im, [no]xim, iconic, visual, inputstyle,
// and in debug mode : sync, nograb, dograb
// Shouldn't all of those be added ? (David)

static const KCmdLineOptions kde_options[] =
{
   { "caption <caption>",	I18N_NOOP("Use 'caption' as name in the titlebar"), 0},
   { "icon <icon>",  		I18N_NOOP("Use 'icon' as the application icon"), 0},
   { "miniicon <icon>", 	I18N_NOOP("Use 'icon' as the icon in the titlebar"), 0},
   { "dcopserver <server>",	I18N_NOOP("Use the DCOP Server specified by 'server'"), 0},
   { "nocrashhandler",		I18N_NOOP("Disable crash handler, to get core dumps"), 0},
   { 0, 0, 0 }
};

void
KApplication::addCmdLineOptions()
{
   KCmdLineArgs::addCmdLineOptions(qt_options, "Qt", "qt");
   KCmdLineArgs::addCmdLineOptions(kde_options, "KDE", "kde");
}

void KApplication::parseCommandLine( )
{
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs("kde");

    if (args->isSet("caption"))
    {
       aCaption = args->getOption("caption");
    }

    if (args->isSet("miniicon"))
    {
       const char *tmp = args->getOption("miniicon");
       aMiniIconPixmap = SmallIcon(tmp);
    }

    if (args->isSet("icon"))
    {
       const char *tmp = args->getOption("icon");
       aIconPixmap = DesktopIcon( tmp );
       if (aMiniIconPixmap.isNull())
          aMiniIconPixmap = SmallIcon( tmp );
    }

    if (args->isSet("dcopserver"))
    {
       dcopClient()->setServerAddress( args->getOption("dcopserver"));
    }

    bool nocrashhandler = (getenv("KDE_DEBUG") != NULL);
    if (!nocrashhandler && args->isSet("crashhandler"))
    {
        // set default crash handler / set emergency save function to nothing
        KCrash::setCrashHandler(KCrash::defaultCrashHandler);
        KCrash::setEmergencySaveFunction(NULL);

// WABA: What's the use of a path if you can't guarantee that 
// it is the right one?
// KCrash::setApplicationPath(KStandardDirs::findExe(QCString(args->appName()),NULL,0));

        KCrash::setApplicationName(QString(args->appName()));
    }

    delete args; // Throw away
}

QPixmap KApplication::icon() const
{
  if( aIconPixmap.isNull()) {
      KApplication *that = const_cast<KApplication*>(this);
      that->aIconPixmap = DesktopIcon( instanceName() );
  }
  return aIconPixmap;
}

QPixmap KApplication::miniIcon() const
{
  if (aMiniIconPixmap.isNull()) {
      KApplication *that = const_cast<KApplication*>(this);
      that->aMiniIconPixmap = SmallIcon( instanceName() );
  }
  return aMiniIconPixmap;
}
KApplication::~KApplication()
{
  delete smw;

  // close down IPC
  delete pDCOPClient;

  // Carefully shut down the process controller: It is very likely
  // that we receive a SIGCHLD while the destructor is running
  // (since we are in the process of shutting down, an opportunity
  // at which child process are being killed). So we first mark
  // the controller deleted (so that the SIGCHLD handler thinks it
  // is already gone) before we actually delete it.
  KProcessController* ctrl = KProcessController::theKProcessController;
  KProcessController::theKProcessController = 0;
  delete ctrl; // Stephan: "there can be only one" ;)

  delete pAppData;
  KApp = 0;

  mySmcConnection = 0;
  delete smModificationTime;
  smModificationTime = 0;
}


class KAppX11HackWidget: public QWidget
{
public:
    bool publicx11Event( XEvent * e) { return x11Event( e ); }
};


bool KApplication::x11EventFilter( XEvent *_event )
{
    if (x11Filter)
    {
	for (QWidget* w=x11Filter->first(); w; w=x11Filter->next())
	{
	    if (((KAppX11HackWidget*) w)->publicx11Event(_event))
		return true;
	}
    }

    if ((_event->type == ClientMessage) &&
	    (_event->xclient.message_type == kipcCommAtom))
    {
	XClientMessageEvent *cme = (XClientMessageEvent *) _event;

	int id = cme->data.l[0];
	int arg = cme->data.l[1];
	if ((id < 32) && (kipcEventMask & (1 << id)))
	{
	    switch (id)
	    {
	    case KIPC::StyleChanged:
		KGlobal::config()->reparseConfiguration();
		kdisplaySetStyle();
		break;

	    case KIPC::PaletteChanged:
		KGlobal::config()->reparseConfiguration();
		kdisplaySetPalette();
		break;

	    case KIPC::FontChanged:
		KGlobal::config()->reparseConfiguration();
		kdisplaySetFont();
		break;

	    case KIPC::BackgroundChanged:
		emit backgroundChanged(arg);
		break;

	    case KIPC::SettingsChanged:
		KGlobal::config()->reparseConfiguration();
                emit settingsChanged(arg);
		break;

	    case KIPC::IconChanged:
		QPixmapCache::clear();
		KGlobal::config()->reparseConfiguration();
		KGlobal::instance()->newIconLoader();
		emit iconChanged(arg);
		break;
	    }
	}
	else if (id >= 32)
	{
	    emit kipcMessage(id, arg);
	}
	return true;
    }

    return false;
}

void KApplication::addKipcEventMask(int id)
{
    if (id >= 32)
    {
	kdDebug(101) << "Cannot use KIPC event mask for message IDs >= 32\n";
	return;
    }
    kipcEventMask |= (1 << id);
}

void KApplication::removeKipcEventMask(int id)
{
    if (id >= 32)
    {
	kdDebug(101) << "Cannot use KIPC event mask for message IDs >= 32\n";
	return;
    }
    kipcEventMask &= ~(1 << id);
}

void KApplication::enableStyles()
{
    if (!useStyles)
    {
        useStyles = true;
        applyGUIStyle(Qt::WindowsStyle);
    }
}

void KApplication::applyGUIStyle(GUIStyle /* pointless */) {
    /* Hey, we actually do stuff here now :)
     * The widgetStyle key is used as a style string. If it matches a
     * Qt internal style that is used, otherwise it is checked to see
     * if it matches a lib name in either $(KDEDIR)/lib or
     * ~/.kde/share/apps/kstyle/modules. If it does we assume it's a style
     * plugin and try to dlopen and allocate a KStyle. If libtool dlopen
     * isn't supported that's no problem, plugins just won't work and you'll
     * be restricted to the internal styles.
     *
     * mosfet@jorsm.com
     */

    static bool dlregistered = false;
    KSimpleConfig pConfig( "kstylerc", true );
    QString oldGroup = pConfig.group();
    pConfig.setGroup("KDE");
    QString styleStr = pConfig.readEntry("widgetStyle", "Default");

    void *oldHandle = styleHandle;

    if(styleStr == "Default"){
        pKStyle = new KDEStyle;
        setStyle(pKStyle);
        styleHandle=0;
    }
    else if(styleStr == "Platinum"){
        pKStyle=0;
        styleHandle=0;
        setStyle(new QPlatinumStyle);
    }
    else if(styleStr == "Windows 95"){
        pKStyle=0;
        styleHandle=0;
        setStyle(new QWindowsStyle);
    }
    else if(styleStr == "CDE"){
        pKStyle=0;
        styleHandle=0;
        setStyle(new QCDEStyle);
    }
    else if(styleStr == "Motif"){
        pKStyle=0;
        styleHandle=0;
        setStyle(new QMotifStyle);
    }
    else if(useStyles){
        if(!dlregistered){
            dlregistered = true;
            lt_dlinit();
        }

        if(!locate("lib", styleStr).isNull()) {
            styleStr = locate("lib", styleStr);
            styleHandle = lt_dlopen(QFile::encodeName(styleStr));
        }
        else {
            warning("KApp: Unable to find style plugin %s.", styleStr.local8Bit().data());
            pKStyle = new KDEStyle;
            setStyle(pKStyle);
            styleHandle=0;
            return;
        }

        if(!styleHandle){
            warning("KApp: Unable to open style plugin %s (%s).",
                    styleStr.local8Bit().data(), lt_dlerror());

            pKStyle = new KDEStyle;
            setStyle(pKStyle);
        }
        else{
            lt_ptr_t alloc_func = lt_dlsym(styleHandle,
                                           "allocate");

            if(!alloc_func){
                warning("KApp: Unable to init style plugin %s (%s).",
                        styleStr.local8Bit().data(), lt_dlerror());
                pKStyle = new KDEStyle;
                setStyle(pKStyle);
                lt_dlclose(styleHandle);
                styleHandle = 0;
            }
            else{
                KStyle* (*alloc_ptr)();
                alloc_ptr = (KStyle* (*)())(alloc_func);
                pKStyle = alloc_ptr();
                if(pKStyle){
                    setStyle(pKStyle);
                }
                else{
                    warning("KApp: Style plugin unable to allocate style.");
                    pKStyle = new KDEStyle;
                    setStyle(pKStyle);
                    lt_dlclose(styleHandle);
                    styleHandle = 0;
                }
            }
        }
    }
    else{
        pKStyle = new KDEStyle;
        setStyle(pKStyle);
        styleHandle=0;
    }
    if(oldHandle){
        lt_dlclose((lt_dlhandle*)oldHandle);
    }

}


QString KApplication::caption() const
{
  // Caption set from command line ?
  if( !aCaption.isNull() )
	return aCaption;
  else
      // We have some about data ?
      if ( KGlobal::instance()->aboutData() )
        return KGlobal::instance()->aboutData()->programName();
      else
        // Last resort : application name
	return name();
}


//
// 1999-09-20: Espen Sand
// An attempt to simplify consistent captions.
//
QString KApplication::makeStdCaption( const QString &userCaption,
	                              bool withAppName, bool modified ) const
{
  if( userCaption.isNull() == true )
  {
    return( caption() );
  }

  //
  // This string should be collected from a global object.
  //
  QString modString = i18n("**");
  if( modified == true )
  {
    modified = modString.isNull() == true ? false : true;
  }

  // WABA:
  // Always put the "userCaption" in as last arg because it can
  // can contain the text "%1" or "%2" which will mess up the rest
  // of the arguments.

  if( withAppName == true )
  {
    if( captionLayout == CaptionAppLast )
    {
      if( modified == true )
      { 
	// userCaption must be last arg!!!
	return( QString("%1 %3 - %2").arg(modString).
		arg(caption()).arg(userCaption)); // useCaption last!!!
      }
      else {
 
	// userCaption must be last arg!!!	
	return( QString("%2 - %1").arg(caption()).arg(userCaption));
      }
    }
    else if( captionLayout == CaptionAppFirst )
    {
      if( modified == true )
      {
 
	// userCaption must be last!!!
	return( QString("%1: %2 %3").arg(caption()).arg(modString).
		arg(userCaption) );
      }
      else
      {
	// userCaption must be last!!!
	return( QString("%1: %2").arg(caption()).arg(userCaption) );
      }
    }
  }

  if( modified == true )
  {
    // userCaption must be last!!!
    return( QString("%1 %2").arg(modString).arg(userCaption) );
  }
  else
  {
    return( userCaption );
  }
}

void KApplication::kdisplaySetPalette()
{
    // the following is temporary and will soon dissappear (Matthias, 3.August 1999 )
    KConfigBase* config = KGlobal::config();
    KConfigGroupSaver saver( config, "General" );

    QColor buttonFallback = config->readColorEntry( "background", &lightGray );
    QColor button = config->readColorEntry( "buttonBackground", &buttonFallback );
    QColor buttonTextFallback = config->readColorEntry( "foreground", &black );
    QColor buttonText = config->readColorEntry( "buttonForeground", &buttonTextFallback );
    QColor background = config->readColorEntry( "background", &lightGray );
    QColor highlight = config->readColorEntry( "selectBackground", &darkBlue);
    QColor highlightedText = config->readColorEntry( "selectForeground", &white );
    QColor base = config->readColorEntry( "windowBackground", &white );
    QColor foreground = config->readColorEntry( "windowForeground", &black );

    config->setGroup( "WM");
    inactiveTitleColor_ = config->readColorEntry( "inactiveBackground", &lightGray );
    inactiveTextColor_ = config->readColorEntry( "inactiveForeground", &darkGray );
    activeTitleColor_ = config->readColorEntry( "activeBackground", &darkBlue );
    activeTextColor_ = config->readColorEntry( "activeForeground", &white );

    config->setGroup( "KDE");
    contrast_ = config->readNumEntry( "contrast", 7 );

    int highlightVal, lowlightVal;
    highlightVal = 100 + (2*contrast_+4)*16/10;
    lowlightVal = 100 + (2*contrast_+4)*10;

    QColorGroup disabledgrp(foreground, background,
                            background.light(highlightVal),
                            background.dark(lowlightVal),
                            background.dark(120),
                            background.dark(120), base);

    QColorGroup colgrp(foreground, background, background.light(highlightVal),
                       background.dark(lowlightVal),
                       background.dark(120),
                       foreground, base);

    int inlowlightVal = lowlightVal-25;
    if(inlowlightVal < 120)
        inlowlightVal = 120;

    colgrp.setColor(QColorGroup::Highlight, highlight);
    colgrp.setColor(QColorGroup::HighlightedText, highlightedText);
    colgrp.setColor(QColorGroup::Button, button);
    colgrp.setColor(QColorGroup::ButtonText, buttonText);
    colgrp.setColor(QColorGroup::Midlight, background.light(110));

    disabledgrp.setColor(QColorGroup::Button, button);
    disabledgrp.setColor(QColorGroup::ButtonText, buttonText);
    disabledgrp.setColor(QColorGroup::Midlight, background.light(110));

    QPalette newPal(colgrp, disabledgrp, colgrp);
/* 
    if(QPixmap::defaultDepth() > 8){
        QColorGroup iGrp(colgrp);
        iGrp.setColor(QColorGroup::Button, colgrp.button().light(115));
        iGrp.setColor(QColorGroup::ButtonText, colgrp.buttonText().light(115));
        iGrp.setColor(QColorGroup::Text, colgrp.text().light(115));
        iGrp.setColor(QColorGroup::Dark, colgrp.dark().light(115));
        iGrp.setColor(QColorGroup::Mid, colgrp.mid().light(115));
        iGrp.setColor(QColorGroup::Midlight, colgrp.midlight().light(115));
        iGrp.setColor(QColorGroup::Light, colgrp.light().light(115));
        newPal.setInactive(iGrp);
    }
*/
    setPalette(newPal, true);
    
    // GJ: The cursor blink rate doesn't belong here. It should get it's own
    // change message but it doesn't really matter because it isn't set.
    int num = config->readNumEntry("cursorBlinkRate", cursorFlashTime());
    if (num < 200)
	num = 200;
    if (num > 2000)
	num = 2000;
    setCursorFlashTime(num);

    //style().polish(newPal);
    emit kdisplayPaletteChanged();
    emit appearanceChanged();
}

void KApplication::kdisplaySetFont()
{
    delete KGlobal::_generalFont;
    KGlobal::_generalFont = 0L;
    delete KGlobal::_fixedFont;
    KGlobal::_fixedFont = 0L;
    delete KGlobal::_menuFont;
    KGlobal::_menuFont = 0L;
    delete KGlobal::_toolBarFont;
    KGlobal::_toolBarFont = 0L;

    QApplication::setFont(KGlobal::generalFont(), true);
    emit kdisplayFontChanged();
    emit appearanceChanged();
}


void KApplication::kdisplaySetStyle()
{
    applyGUIStyle(WindowsStyle);
    emit kdisplayStyleChanged();
    emit appearanceChanged();
}


void KApplication::invokeHTMLHelp( QString filename, QString topic ) const
{
   if( filename.isEmpty() )
      filename = QString(name()) + "/index.html";

   QString url;
   if (!topic.isEmpty())
     url = QString("help:/%1#%2").arg(filename).arg(topic);
   else
     url = QString("help:/%1").arg(filename);

   QCString dcopService;
   QString error;

   if (startServiceByDesktopName("khelpcenter", url, dcopService, error))
   {
      warning("Could not launch help:\n%s\n", error.local8Bit().data());
      return;
   }
}


void KApplication::invokeMailer(const QString &address,const QString &subject )
{
   QCString dcopService;
   QString error;
   QString mailClient( "kmail");
   //TODO: subject needs to be passed as well!! 
   if (startServiceByDesktopName(mailClient, address, dcopService, error))
   {
      warning("Could not launch mail client:\n%s\n", error.local8Bit().data());
      return;
   }
}


void KApplication::invokeBrowser( const QString &url )
{
   QCString dcopService;
   QString error;

   if (startServiceByDesktopName("kfmclient", url, dcopService, error))
   {
      warning("Could not launch browser:\n%s\n", error.local8Bit().data());
      return;
   }
}

QCString
KApplication::launcher()
{
   char host[200];
   if (gethostname(host, 200) == -1) {
      kdDebug(101) << "gethostname(): " << strerror(errno) << "\n";
      return 0;
   }
   // Try to resolve to FQDN
   struct hostent *h = gethostbyname(host);
   if (h == 0L) {
      kdDebug(101) << "gethostbyname() returned with an error!\n";
   } else {
      if (strlen(h->h_name) < 200)
         strcpy(host, h->h_name);
      else
         kdDebug(101) << "host name too long\n";
   }
   QCString name;
   name.sprintf("klauncher_%s_%d", host, getuid());
   return name;
}

static int
startServiceInternal( const QCString &function,
                      const QString& _name, const QString &URL,
                      QCString &dcopService, QString &error )
{
   typedef struct serviceResult
   {
      int result;
      QCString dcopName;
      QString error;
   };

   // Register app as able to send DCOP messages
   DCOPClient *dcopClient;
   if (kapp)
      dcopClient = kapp->dcopClient();
   else
      dcopClient = new DCOPClient;

   if (!dcopClient->isAttached())
   {
      if (!dcopClient->attach())
      {
         error = i18n("Could not register with DCOP.\n");
         return -1;
      }
   }
   QByteArray params;
   QDataStream stream(params, IO_WriteOnly);
   stream << _name << URL;
   QCString replyType;
   QByteArray replyData;
   QCString _launcher = KApplication::launcher();
   if (!dcopClient->call(_launcher, _launcher,
	function, params, replyType, replyData))
   {
	error = i18n("KLauncher could not be reached via DCOP.\n");
        if (!kapp)
           delete dcopClient;
        return -1;
   }
   if (!kapp)
      delete dcopClient;

   QDataStream stream2(replyData, IO_ReadOnly);
   serviceResult result;
   stream2 >> result.result >> result.dcopName >> result.error;
   dcopService = result.dcopName;
   error = result.error;
   return result.result;
}

int
KApplication::startServiceByName( const QString& _name, const QString &URL,
                              QCString &dcopService, QString &error )
{
   return startServiceInternal(
                      "start_service_by_name(QString,QString)",
                      _name, URL, dcopService, error);
}

int
KApplication::startServiceByDesktopPath( const QString& _name, const QString &URL,
                              QCString &dcopService, QString &error )
{
   return startServiceInternal(
                      "start_service_by_desktop_path(QString,QString)",
                      _name, URL, dcopService, error);
}

int
KApplication::startServiceByDesktopName( const QString& _name, const QString &URL,
                              QCString &dcopService, QString &error )
{
   return startServiceInternal(
                      "start_service_by_desktop_name(QString,QString)",
                      _name, URL, dcopService, error);
}


bool KApplication::kdeFonts(QStringList &fontlist) const
{
  QString fontfilename = KGlobal::dirs()->saveLocation("config") + "kdefonts";
  QFile fontfile(fontfilename);

  if (!fontfile.exists())
    return false;

  if(!fontfile.open(IO_ReadOnly)){
    return false;
  }

  QTextStream t(&fontfile);


  while ( !t.eof() ) {
    QString s = t.readLine();
    if(!s.isEmpty())
      fontlist.append( s );
  }

  fontfile.close();

  return true;
}


QString KApplication::tempSaveName( const QString& pFilename ) const
{
  QString aFilename;

  if( pFilename[0] != '/' )
    {
      kdWarning(101) << "Relative filename passed to KApplication::tempSaveName" << endl;
      aFilename = QFileInfo( QDir( "." ), pFilename ).absFilePath();
    }
  else
    aFilename = pFilename;

  QDir aAutosaveDir( QDir::homeDirPath() + "/autosave/" );
  if( !aAutosaveDir.exists() )
    {
      if( !aAutosaveDir.mkdir( aAutosaveDir.absPath() ) )
	{
	  // Last chance: use _PATH_TMP
	  aAutosaveDir.setPath( _PATH_TMP );
	}
    }

  aFilename.replace( QRegExp( "/" ),"\\!" ).prepend( "#" ).append( "#" ).prepend( "/" ).prepend( aAutosaveDir.absPath() );

  return aFilename;
}


QString KApplication::checkRecoverFile( const QString& pFilename,
        bool& bRecover ) const
{
  QString aFilename;

  if( pFilename[0] != '/' )
    {
      kdWarning(101) << "Relative filename passed to KApplication::tempSaveName" << endl;
      aFilename = QFileInfo( QDir( "." ), pFilename ).absFilePath();
    }
  else
    aFilename = pFilename;

  QDir aAutosaveDir( QDir::homeDirPath() + "/autosave/" );
  if( !aAutosaveDir.exists() )
    {
      if( !aAutosaveDir.mkdir( aAutosaveDir.absPath() ) )
	{
	  // Last chance: use _PATH_TMP
	  aAutosaveDir.setPath( _PATH_TMP );
	}
    }

  aFilename.replace( QRegExp( "/" ), "\\!" ).prepend( "#" ).append( "#" ).prepend( "/" ).prepend( aAutosaveDir.absPath() );

  if( QFile( aFilename ).exists() )
    {
      bRecover = true;
      return aFilename;
    }
  else
    {
      bRecover = false;
      return pFilename;
    }
}


bool checkAccess(const QString& pathname, int mode)
{
  int accessOK = access( QFile::encodeName(pathname), mode );
  if ( accessOK == 0 )
    return true;  // OK, I can really access the file

  // else
  // if we want to write the file would be created. Check, if the
  // user may write to the directory to create the file.
  if ( mode & W_OK == 0 )
    return false;   // Check for write access is not part of mode => bail out


  if (!access( QFile::encodeName(pathname), F_OK)) // if it already exists
      return false;

  //strip the filename (everything until '/' from the end
  QString dirName(pathname);
  int pos = dirName.findRev('/');
  if ( pos == -1 )
    return false;   // No path in argument. This is evil, we won't allow this

  dirName.truncate(pos); // strip everything starting from the last '/'

  accessOK = access( QFile::encodeName(dirName), W_OK );
  // -?- Can I write to the accessed diretory
  if ( accessOK == 0 )
    return true;  // Yes
  else
    return false; // No
}


void KApplication::setTopWidget( QWidget *topWidget )
{
  if( topWidget != 0 )
  {
    Window leader = topWidget->winId();
    QCString string_buffer = instanceName().data(); // copies it

    char * argv = string_buffer.data();

    XSetCommand(display, leader, &argv, 1);

    XClassHint hint;
    hint.res_name = string_buffer.data();
    hint.res_class = const_cast<char*>("toplevel");
    XSetClassHint(display, leader, &hint);

    XWMHints * hints = XAllocWMHints();
    hints->window_group = leader;
    hints->flags = WindowGroupHint;
    XSetWMHints(display, leader, hints);
    XFree( (char*)hints);

    // set the specified caption
    topWidget->setCaption( caption() );
    // set the specified icons
    KWM::setIcon(topWidget->winId(), icon());
    KWM::setMiniIcon(topWidget->winId(), miniIcon());
    // set a short icon text
    // TODO: perhaps using .ascii() isn't right here as this may be seen by
    // a user?
    XSetIconName( qt_xdisplay(), topWidget->winId(), caption().ascii() );
  }
}

QColor KApplication::inactiveTitleColor() const
{
    return inactiveTitleColor_;
}


QColor KApplication::inactiveTextColor() const
{
    return inactiveTextColor_;
}


QColor KApplication::activeTitleColor() const
{
    return activeTitleColor_;
}


QColor KApplication::activeTextColor() const
{
    return activeTextColor_;
}

int KApplication::contrast() const
{
    return contrast_;
}

int KApplication::random()
{
   static int init = false;
   if (!init)
   {
      unsigned int seed;
      init = true;
      int fd = open("/dev/urandom", O_RDONLY);
      if (fd <= 0 || ::read(fd, &seed, sizeof(seed)) != sizeof(seed))
      {
            // No /dev/urandom... try something else.
            srand(getpid());
            seed = rand()+time(0);
      }
      if (fd >= 0) close(fd);
      srand(seed);
   }
   return rand();
}

QString KApplication::randomString(int length)
{
   if (!length) return QString::null;
   if (length<0)
      length=random();

   char *string=new char[length+1];	
   while (length--)
   {
      int r=random() % 62;
      r+=48;
      if (r>57) r+=7;
      if (r>90) r+=6;
      string[length]=(char)r;
      // so what if I work backwards?
   }
   QString str(string);
   delete [] string;
   return str;
}

#include "kapp.moc"
