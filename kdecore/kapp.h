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
// Revision 1.41  1998/01/06 22:54:29  kulow
// $Log$
//
// Revision 1.39  1997/12/28 21:32:15  kulow
// last time I forgot two functions kde_mimedir() and kde_confdir()
// I will move this functions very soon to static functions of a new
// class KPaths. Kapplication is not the optimal class to contain this
// functions, since I need a DISPLAY to find out the paths. But I think,
// we can create some inline methods in kapp then
//
// Revision 1.38  1997/12/13 15:08:58  jacek
// KCharsets support added
// WARNING: kderc file format has changed. Since now charset name
// instead of id is stored.
//
//
// Revision 1.37  1997/11/20 22:24:31  kalle
// new static methods for the various directories
//
// Revision 1.36  1997/11/18 21:40:43  kalle
// KApplication::localconfigdir()
// KApplication::localkdedir()
// KConfig searches in $KDEDIR/share/config/kderc
//
// Revision 1.35  1997/10/22 20:42:56  kalle
// Help menu works as advertised
//
// Revision 1.34  1997/10/17 17:32:08  ettrich
// Matthias: typo in documentation :)
//
// Revision 1.33  1997/10/17 13:30:18  ettrich
// Matthias: registerTopWidget/unregisterTopWidget are obsolete and empty now.
//           Introduced new registration model
//
// Revision 1.32  1997/10/16 11:14:28  torben
// Kalle: Copyright headers
// kdoctoolbar removed
//
// Revision 1.31  1997/10/12 14:37:22  kalle
// Documentation correction
//
// Revision 1.29  1997/10/11 22:39:30  ettrich
// Matthias: BINARY INCOMPATIBLE CHANGES
//     - removed senseless method setUnsavedData
//
// Revision 1.28  1997/10/11 13:32:14  kalle
// pAppData in KApplication
// KTabListBox now in libkdeui
//
// Revision 1.27  1997/10/10 22:09:18  ettrich
// Matthias: BINARY INCOMPATIBLE CHANGES: extended session management support
//
// Revision 1.26  1997/10/05 12:52:41  kalle
// Three new methods from Mark Donohoe
//
// Revision 1.25  1997/10/04 19:42:45  kalle
// new KConfig
//
// Revision 1.24  1997/09/29 19:27:03  kalle
// Save icon and mini-icon pixmap
// SORRY, BUTTHH THIS IS AGAIN BINARY INCOMPATIBLE!!!!!!!!!!!!!!!
//
// Revision 1.23  1997/09/29 18:57:36  kalle
// BINARY INCOMPATIBLE!!!!!!!!!!!!!!!!!!!!!!
// Support for session management
// make KDebug compile on HP-UX
//
// Revision 1.22  1997/09/26 07:01:13  kalle
// Here are the promised dummies for session management:
//
// - KApplication::saveYourself (signal)
// - KApplication::tempFileName()
// - KApplication::checkRecoverFile()
//
// A short documentation is in kapp.h, more docs will follow.
//
// MD: Implemented reading of the colour scheme contrast variable. This allows
// you to choose the highlights and lowlights used to draw widgets and has been
// part of the colour scheme specification for some time
//
// Sorry if this breaks anything; it is the last of the binary incompatible
// changes that Kalle announced yesterday.
// Revision 1.20  1997/09/11 19:44:54  kalle
// New debugging scheme for KDE (binary incompatible!)


#ifndef klocale
#define klocale KApplication::getKApplication()->getLocale()
#endif
#define i18n KApplication::getKApplication()->getLocale()->translate
// klocale->translate is much to long
#ifndef i18n
* @version $Id$
#endif

class KIconLoader;
class KCharsets;

//#ifndef _KLOCALE_H
#include <klocale.h>
//#endif

#include <drag.h>
* @version $Id$
#include <qapp.h>
#include <qfile.h>
/** 
* Constructor. Pass command-line arguments. 
*
* A KConfig object is
* created that contains an application-specific config file whose
* name is "~/." + argv[0] + "rc". This constructor should be considered
* obsolete. The state of the application-specific config file may be
* queried afterwards with getConfigState(). 
*/
* accelerators, common menu entries, a KConfig object
* etc. KApplication installs a signal handler for the SIGCHLD signal
/** 
* Constructor. Pass command-line arguments. 
*
* A KConfig object is
* created that contains an application-specific config file whose
* name is "~/." + rAppName + "rc". The state of the application-specific 
* config file may be queried afterwards with getConfigState(). 
*/
*/ 
class KApplication : public QApplication
/** 
* Destructor 
*/
  /** 
	* Constructor. Pass command-line arguments. 
	*
	* A KConfig object is
	* created that contains an application-specific config file whose
	* name is "~/." + argv[0] + "rc". This constructor should be considered
	* obsolete. The state of the application-specific config file may be
/** 
* Return the current application object.
*
* This is similar to the global QApplication pointer qApp. It allows access
* to the single global KApplication object, since more than one cannot be
* created in the same application. It saves the trouble of having to pass
* the pointer to it explicitly to every function that may require it.
*/
	* created that contains an application-specific config file whose
	* name is "~/." + rAppName + "rc". The state of the application-specific 
/** 
* Return the logical application name as set in the constructor.
*/

  /** 
/** 
* Retrieve the application config object. 
*
* @return a pointer to the application's global KConfig object.
* @see KConfig
*/
	*/
  virtual bool eventFilter( QObject*, QEvent* );
/** 
* Retrieve the application session config object. 
*
* @return a pointer to the application's instance specific KConfig object.
* @see KConfig
*/
	* created in the same application. It saves the trouble of having to pass
	* the pointer to it explicitly to every function that may require it.
/** 
* Is the application restored from the session manager? 
*
* @ In this case the getSessionConfig()-object probably contains 
* @ important data.
*/
  const QString& appName() const { return aAppName; }
* @ Session management will apply to the main widget.
/** 
* Enable session management
*
* @ If userdefined = True then the WmCommand can be defined with setWmCommand.
* @ Note that you do not get an instance specific config object with
* @ getSessionConfig() in this case!
* @
* @ Session management will apply to the top widget.
*/
	* Retrieve the application session config object. 
	*
/** 
* Set the WmCommand for the session manager.
*
* @ This has an effekt if either session management is disabled (then it
* @ is used for pseudo session managemt) or ith session management is
* @ enabled with userdefined=True.
*/
	*
	QPopupMenu* getHelpMenu( bool bAboutQtMenu = false );
	* @ important data.
	*/
  bool isRestored() const { return bIsRestored; }

  /** 
	* Enable session management
	*
	QPopupMenu* getHelpMenu( bool bAboutQtMenu, const char* appAboutText );
	* @ Note that you do not get an instance specific config object with
	* @ getSessionConfig() in this case!
	* @
	* @ Session management will apply to the top widget.
	*/
  void enableSessionManagement(bool userdefined = FALSE);

  /** 
	* Set the WmCommand for the session manager.
	*
	* @ This has an effekt if either session management is disabled (then it
	* @ is used for pseudo session managemt) or ith session management is
	* @ enabled with userdefined=True.
	*/
  void setWmCommand(const char*);
  

  /**
	* Return a standard help menu
	*
	* @param bAboutQtMenu If true, there is a menu entry for About Qt
  
    * @return a pointer to the Iconloader of the application
    * @see KIconLoader
    */
  KIconLoader* getIconLoader();

  /**
    * Get a KLocale object for the application. If it does not yet exist,
    * create one.
    * @return a pointer to the KLocale object of the application
    * @see KLocale
    */
  KLocale* getLocale();

  /**
    * Get a KCharsets object for the application. 
    * @return a pointer to the KCharsets object of the application
    * @see KCharsets
    */
  KCharsets* getCharsets()const
	{ return pCharsets; }
     
  /**
	* Get the icon for the application.
	* @return a QPixmap with the icon.
	* @see QPixmap
	*/
  QPixmap getIcon() const
	{ return aIconPixmap; }
  
  
  /**
	* Get the mini-icon for the application.
	* @return a QPixmap with the icon.
/** 
* Possible return values for getConfigState().
*
* @see #getConfigState
*/

  /** Sets the top widget of the application . This widget will
    * be used for communication with the session manager.
/** 
* Retrieve the state of the app-config object. 
*
* Possible return values
* are APPCONFIG_NONE (the application-specific config file could not be
* opened neither read-write nor read-only), APPCONFIG_READONLY (the
* application-specific config file is opened read-only, but not
* read-write) and APPCONFIG_READWRITE (the application-specific config
* file is opened read-write).
*
* @see #ConfigState
*/
  void registerTopWidget();
  /* obsolete, will dissappear (Matthias) */
/**
* Invoke the kdehelp HTML help viewer. 
*
* @param aFilename	The filename that is to be loaded. Its location
*			is computed automatically according to the KFSSTND. 
*			If aFilename is empty, the logical appname with .html 
*			appended to it is used.
* @param aTopic		This allows context-sensitive help. Its value
*			will be appended to the filename, prefixed with
*			a "#" (hash) character.
*/
	* Retrieve the state of the app-config object. 
	*
/** 
* Get the KDE base dir. 
*
* This is the value of the KDEDIR
* environment variable if it is set in the process' environment,
	* Returns the directory where config files are stored
	* @return the name of the directory
	*/
  static const QString& kde_configdir();

/** 
* Find a file using standard KDE search paths. 
*
* Possible search paths
* include $KDEDIR, $KDEPATH, and "[KDE Setup]:Path=" entry in a config
* file. If file is not found, isEmpty() will return True
*/
  /**
	* Get the local KDE base dir
/** 
* Get the KDE font list.
*
* This method allows you to get the KDE font 
* list which was composed by the user with kfontmanager. Usually you should 
* work only with those fonts in your kapplication. 
*  
*  @return true on success.
*/
	* Get the local KDE config dir
	*
	* This is usually $HOME/.kde/share/config
/**
* Return a text for the window caption.
*
* This would be set either by
* "-caption", otherwise it will be equivalent to the name of the
* executable.
*/
   const char* getCaption() const;
	* file. If file is not found, isEmpty() will return True
	*/
  static QString findFile( const char *file );

  /** 
	* Get the KDE font list.
	*
	* This method allows you to get the KDE font 
	* list which was composed by the user with kfontmanager. Usually you should 
	* work only with those fonts in your kapplication. 
	*  
	*  @return true on success.
	*/
  bool getKDEFonts(QStrList *fontlist);


  /**
	* Return a text for the window caption.
	*
	* This would be set either by
	* "-caption", otherwise it will be equivalent to the name of the
	* executable.
	*/
  const char* getCaption() const;

  /** Get a file name in order to make a temporary copy of your
	* document.
	*
	* @param pFilename The full path to the current file opf your
	* document.
	* @return A new filename for auto-saving. You have to free() this
	* yourself, otherwise you have a memory leak!
  * An X11 atom used for IPC
  */

  /** Check if there is an auto-save file for the document you want to
  * An X11 atom used for IPC
  */
	* @param pFilename The full path to the document you want to open.
	* @param bRecover  This gets set to true if there was a recover
  * An X11 atom used for IPC
  */
	* pointer yourself, otherwise you have a memory leak.
	*/
  * An X11 atom used for IPC
  */

  /**
  * An X11 atom used for IPC
  */
	*
	* @return whether the KLocale object has already been constructed
	*/
  * Get the X11 display
  */
  /**
	* An X11 atom used for IPC
  * Used by KDNDDropZone to register
  */
  /**
	* An X11 atom used for IPC
  * Used by KDNDDropZone during shutdown
  */
  /**
	* An X11 atom used for IPC
  * Set the DropZone which reveives root drop events.
  */
  /**
	* An X11 atom used for IPC
	*/
/**
* List of all DropZones.
*/
	*/
  Atom getDndRootProtocolAtom() { return DndRootProtocol; }    
/** 
* The last drop zone the mouse was over.
*
* If we get a DndLeaveProtocol we must inform 'lastEnteredDropZone'
* that the mouse left the DropZone.
*/
	* Used by KDNDDropZone to register
	*/
/**
* The DropZone which receives root drop events.
*/
	*/
  virtual void removeDropZone( KDNDDropZone *_z ) { dropZones.remove( _z ); }
/**
* Used to catch X11 events
*/
  virtual void setRootDropZone( KDNDDropZone *_z ) { rootDropZone = _z; }

protected:
  /**
	* List of all DropZones.
	*/
  QList<KDNDDropZone> dropZones;

  /** 
	* The last drop zone the mouse was over.
	*
	* If we get a DndLeaveProtocol we must inform 'lastEnteredDropZone'
	* that the mouse left the DropZone.
	*/
  KDNDDropZone *lastEnteredDropZone;

  /**
/**
* The X11 display
*/

  /**
	* Used to catch X11 events
	*/

  /**
	* Two X11 atoms used for session management
	*/
private slots:
  void appHelpActivated();

  /**
	* X11 atoms used for IPC
	*/
  Atom DndSelection;
  Atom DndProtocol;
  Atom DndEnterProtocol;
  Atom DndLeaveProtocol;
  Atom DndRootProtocol;

  /**
	* The X11 display
  QString aDummyString1; // do not touch
  Display *display;

  Atom KDEChangePalette;
  Atom KDEChangeGeneral;
  Atom KDEChangeStyle;

  /// Current application object.

 private slots:
 void appHelpActivated();
  void aboutKDE();
  void aboutApp();
  void aboutQt();

private:
  void* pAppData; // don't touch this without Kalles permission
  KConfig* pConfig; // application config object
  KConfig* pSessionConfig; //instance specific application config object
  QString aSessionName; // logical name of the instance specific config file
  QWidget* pTopWidget;
  QString aAppName; // logical application name
  QString aCaption; // the name for the window title
  QString aWmCommand; // for userdefined session management
  ConfigState eConfigState;
  static QStrList* pSearchPaths;
  KIconLoader* pIconLoader; // the application's own icon loader
  KLocale* pLocale;  
  static KCharsets* pCharsets;  // it shouldn't be static, but you would loose binary compability 
  void* dummy2; // do not use these without asking kalle@kde.org
  void* dummy3;
  void* dummy4;
  QString aAppAboutString; // The text for the about box
  QString aDummyString2; // do not touch
  QString aDummyString3; // do not touch
  QString aDummyString4; // do not touch
  bool bLocaleConstructed; // has the KLocale object already been constructed
  bool bIsRestored; // is the application restored from the session manager?
  bool bSessionManagement; 
  bool bSessionManagementUserDefined;
  QPixmap aIconPixmap;
  QPixmap aMiniIconPixmap;

  void init( );
  void parseCommandLine( int&, char** ); // search for special KDE arguments

  void buildSearchPaths();
  void appendSearchPath( const char *path );

signals:
/** 
* KApplication has changed its Palette due to a KDisplay request.
*
* Normally, widgets will update their palettes automatically, but you
* should connect to this to program special behaviour.
*/

public:
/** 
* KApplication has changed its GUI Style due to a KDisplay request.
*
* Normally, widgets will update their styles automatically (as they would
* respond to an explicit setGUIStyle() call), but you should connect to
* this to program special behaviour.
*/
  QColor selectColor;
  QColor selectTextColor;
/**
* KApplication has changed its Font due to a KDisplay request.
*
* Normally widgets will update their fonts automatically, but you should
* connect to this to monitor global font changes, especially if you are
* using explicit fonts.
*/
  /** 
	* KApplication has changed its Palette due to a KDisplay request.
	*
	* Normally, widgets will update their palettes automatically, but you
	* should connect to this to program special behaviour.
	*/
  void kdisplayPaletteChanged();

  /** 
	* KApplication has changed its GUI Style due to a KDisplay request.
	*
	* Normally, widgets will update their styles automatically (as they would
	* respond to an explicit setGUIStyle() call), but you should connect to
	* this to program special behaviour.
	*/
  void kdisplayStyleChanged();

  /**
	* KApplication has changed its Font due to a KDisplay request.
	*
	* Normally widgets will update their fonts automatically, but you should
	* connect to this to monitor global font changes, especially if you are
	* using explicit fonts.
	*/
  void kdisplayFontChanged();

  /** 
	* KApplication has changed either its GUI style, its font or its palette
	* due to a kdisplay request. Normally, widgets will update their styles
	* automatically, but yout should connect to this to program special
	* behavior. */
  void appearanceChanged();

  /** Session management is about to close your application.
	*
	* Connect to this signal in order to save your data. Do NOT
	* manipulate the UI in that slot, it is blocked by kwm.
	*
	* Use the getSessionConfig() Kconfig object to store all
	* your instance specific datas.
	*
	* Note: you should not do that if you are using the KTopLevelWidget.
	*       Overload saveProperties(...)/readProperties(...) in that case.
	*       This gives you also the possibillity to handle multiple windows
	*       application in an easy manner.
	*/
  void saveYourself();
};

#endif
