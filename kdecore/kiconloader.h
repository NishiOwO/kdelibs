/* 
   $Id$
   
   This file is part of the KDE libraries
   Copyright (C) 1997 Christoph Neerfeld (chris@kde.org)
   
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
   
   $Log$
   Revision 1.33  1999/06/16 21:01:24  kulow
   kiconloader doesn't use it's own directories, but use KStandardDirs from now
   on - for this I removed insertDirectory and appendDirectory from it's API.
   Afaik only koffice used it.
   And I also added KStandardDirs::kde_data_relative() which just returns
   /share/apps to add a central place to define such paths. I think about
   adding more of these static functions to make the whole thing as configurable
   as it used to be.

   Revision 1.32  1999/06/05 01:15:11  dmuell
   global configuration will now be searched in the following paths:

   KDEDIR/share/config/kdeglobals
   /usr/lib/KDE/system.kdeglobals
   /usr/local/lib/KDE/system.kdeglobals
   ~/.kde/share/config/kdeglobals

   it did it previously in

   KDEDIR/share/config/kderc
   /usr/lib/KDE/system.kderc
   /usr/local/lib/KDE/system.kderc
   ~/.kderc

   Note: It's better not to use a "rc" suffix - there might be an
   application that has that name.

   Revision 1.31  1999/06/03 09:21:32  ssk
   Added arg to loadInternal to ignore cache if requested.
   Updated reloadIcon to use it.

   Revision 1.30  1999/05/26 18:02:14  kulow
   David is right when he says addIcons is overkill. Removed the function
   again and renamed the old function setDir to changeDirs as this is what
   it does. For this I added getResourceDirs(type) to kstddirs. Don't use
   it if you can avoid it ;)

   Revision 1.29  1999/05/26 12:29:32  kulow
   adding getIconList() - the iconloader will find all icons itself. Currently
   it's just a call to findAllResources("toolbar"), but in this case we urgently
   need an abstraction layer as KIconLoader may be extended to look for large
   icons too

   Revision 1.28  1999/05/26 08:14:05  dfaure
   Docu improvements. Moved more to the top comment (like the search path) ;
   Added a line about "large/", explain that both constructors keep the default
   search path,  and some more fixes.
   BTW : reloadIcon calls flush which is deprecated...

   Revision 1.27  1999/05/25 16:17:24  kulow
   two changes:
     kde_icondir() has been removed. Use locate("icon", pixmap) instead
     KIconLoader::loadMiniIcon has been removed. It does the same as
     loadApplicationMiniIcon and the later is less confusing in what it
     does (loading icons out of share/icons/mini)

   Revision 1.26  1999/05/23 17:04:52  kulow
   let KGlobal create the IconLoader instance. Removed KApplication::getIconLoader.
   Steffen's idea - just more consequent ;)

   removing some little used kde_*dir functions. May break kdebase parts,
   I'm compiling right now

   Revision 1.25  1999/05/09 23:58:34  pbrown
   was broken with QStringList.  fixed.

   Revision 1.24  1999/05/09 17:22:54  ssk
   API_CHANGE: KIconLoader::getDirList now returns QStringList.
   KIconLoader now uses QPixmapCache.
   Added a new appendDirectory method.

   Revision 1.23  1999/05/07 16:45:13  kulow
   adding more explicit calls to ascii()

   Revision 1.22  1999/05/07 15:42:36  kulow
   making some changes to the code and partly to the API to make it
   -DQT_NO_ASCII_CAST compatible.
   The job is quite boring, but triggers some abuses of QString. BTW:
   I added some TODOs to the code where I was too lazy to continue.
   Someone should start a grep for TODO in the code on a regular base ;)

   Revision 1.21  1999/04/18 19:55:42  kulow
   CVS_SILENT some more fixes

   Revision 1.20  1999/04/06 12:54:11  dfaure
   warning: extra qualification `KIconLoader::' on member `getIconPath' ignored

   Revision 1.19  1999/03/29 04:14:37  ssk
   Rewrote some docs.
*/


#ifndef KICONLOADER_H
#define KICONLOADER_H

class KConfig;
#include <qobject.h>
#include <qlist.h>
#include <qpixmap.h>
#include <qstringlist.h>
#include <qstring.h>

#define Icon(x) KGlobal::iconLoader()->loadIcon(x,0,0,false)
#define ICON(x) KGlobal::iconLoader()->loadIcon(x,0,0,false)

/**
	Icon loader with caching.

	Multiples loads of the same icons using this class will be cached
	using @ref QPixmapCache, saving memory and loading time. 
	
	Icons are searched for according to the KDE file system standard.
	The default search path is :
  
	@li $HOME/.kde/share/apps/<appName>/pics
	@li $KDEDIR/share/apps/<appName>/pics
	@li $HOME/.kde/share/apps/<appName>/toolbar
	@li $KDEDIR/share/apps/<appName>/toolbar

	@li $HOME/.kde/share/icons
	@li $HOME/.kde/share/toolbar

	@li $KDEDIR/share/icons
	@li $KDEDIR/share/toolbar

	@li list of directories given by config file (see the constructors)
	
	Some large/ directories are added to the search path for special
	apps that support large icons (kpanel, kfm) or for all apps
	depending on the settings in ~/.kde/share/config/kdeglobals

	Extra directories can be added, see @ref insertDirectory.

	All keys used in QPixmapCache by this class have the "$kico_.." prefix.

	@author Christoph Neerfeld (chris@kde.org)
	@version $Id$
*/
class KIconLoader : public QObject
{
  Q_OBJECT

public:
  /** Default constructor. 
   * Adds to the search path the ones listed in [KDE Setup]/IconPath. 
   * (in .kderc or the application config file)
   */
  KIconLoader();

  /**
    Constructor.
    If you want to use another path in your application then write into
    a config file :

    <pre>
    [MyApplication]
    PixmapPath=/..../my_pixmap_path
    </pre>

    and call KIconLoader( config, "MyApplication", "PixmapPath" ).

    @param conf		Pointer to a KConfig object which will be searched
    for additional paths.
    @param app_name	Group to search for paths.
    @param var_name	Key to search for paths.

  */
  KIconLoader ( KConfig *conf, const QString &app_name, 
		const QString &var_name = "PixmapPath");

  /** Destructor. */
  ~KIconLoader () {}

  /** 
  	Load an icon from disk or cache.

	@param name	The name of the icon to load. Absolute pathnames are
	 		allowed.
  	@param w	The max width of the resulting pixmap. Larger icons
			are scaled down. The default is no maximum.
  	@param h	The max height of the resulting pixmap. Larger icons
			are scaled down. The default is no maximum.
	@param canReturnNull	If this is false, this function will return
		the "unknown.xpm" icon if the requested icon is not found.
		The default is to return null.

	@return	The loaded icon.
  */
  QPixmap loadIcon( const QString& name, int w = 0, int h = 0, 
  		bool canReturnNull = true );


  /** 
  	Load an icon from disk without cache.

	This is useful if the icon has changed on the filesystem and
	you want to be sure that you get the new version, not the old
	one from the cache.

	@see loadIcon
  */
  QPixmap reloadIcon( const QString& name, int w = 0, int h = 0);

  /**
   * The loadApplication-Icon functions are similar to the 
   * usual loadIcon functions except that they look in
   * kdedir()/share/icon first.
   *
   * These function should be used if you are loading the
   * application icons. Normally KApplication does this for
   * you, but special programs like kpanel or kmenuedit
   * need to load the application icons of foreign applications.
   *
   * @see loadIcon
   */
  QPixmap loadApplicationIcon( const QString& name, int w = 0, int h = 0 );

  /**
  	Similar to loadMiniIcon, but searches for a mini icon.

	@see loadMiniIcon, loadApplicationIcon
  */
  QPixmap loadApplicationMiniIcon( const QString& name, int w = 0, int h = 0 );

  /** 
	Get the complete path for an icon name.

	@param name	The name of the icon to search for.
	@param always_valid If true, the function will return the path to
		unknown.xpm if the icon is not found. Note that it will
		return null if unknown.xpm was also not found.

	@return the physical path to the named icon.
  */
  QString getIconPath( const QString& name, 
		       bool always_valid=false);


  /** 
   * Remove an icon from the cache. This is no longer required since @ref
   * QPixmapCache does this for us.
   * @deprecated
   */
  void flush( const QString &name ); 

protected:

  KConfig		*config;

  /**
	honourcache will check if the icon is contained in the cache before
	trying to load it. 
  */
  QPixmap loadInternal( const QString& name, int w = 0, int h = 0,
			bool honourcache = true );

  QString appname;
  QString varname;

private:
  void initPath();
  void addPath(QString path);

  // Disallow assignment and copy-construction
  KIconLoader( const KIconLoader& );
  KIconLoader& operator= ( const KIconLoader& );

};

#endif // KICONLOADER_H




