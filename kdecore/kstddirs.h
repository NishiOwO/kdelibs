/*
* kstddirs.h -- Declaration of class KStandardDirs.
* Generated by newclass on Thu Mar  5 16:05:28 EST 1998.
*/

#ifndef SSK_KSTDDIRS_H
#define SSK_KSTDDIRS_H

#include <qstring.h>
#include <qdict.h>
#include <qstringlist.h>

class KConfig;

/**
* Site-independent access to standard KDE directories.
*
* The various directory accessors allow you to access the various
* KDE FSSTND directories in a portable way.
*
* Since the KDE FSSTND is heirarchical, the accessors provide the
* ability to get the directory in the most to the least specific
* location (in order, Application/User/System);
*
* Additionally, each accessor has the ability to find the most
* specific directory that actually exists. The default is no check
* whether the directory exists.
* 
* Ordinarily, you will not want to manually instantiate this class.
* The global @ref KApplication object instantiates and provides const 
* access to a KStandardDirs object via the 
* @ref KApplication::dirs method.
*
* standard resources that kdelibs allocates are:
*
* @li appdata - application specific data dir (if instantiated from KGlobal)
* @li apps - applications menu (.desktop files)
* @li cgi - CGIs to run from kdehelp
* @li config - configuration files
* @li data - where applications store data
* @li exe - executables in $prefix/bin. @ref #findExe for a function that takes $PATH into account 
* @li html - HTML documentation 
* @li icon - icons
* @li lib - libs
* @li locale - translation files for KLocale
* @li mime - mime types
* @li mini - miniature icons
* @li services - services
* @li servicetypes - service types
* @li sound - application sounds
* @li toolbar - toolbar pictures
* @li wallpaper - wallpapers
* 
* @author Sirtaj Singh Kang <taj@kde.org>
* @version $Id$
*/
class KStandardDirs
{
public:
        /**
	 * KStandardDirs Constructor.
	 * @param appName The name of the application, which will be
	 *		used for searching the "apps" directory.
	 */
	KStandardDirs( const QString& appName = QString::null );

	/**
	 * KStandardDirs Destructor.
	 */
	virtual ~KStandardDirs();

	/*
	 * This adds another search dir to front of the fsstnd list.
	 * @li when compiling kdelibs, the prefix is added to this.
	 * @li when compiling a separate app, kapp adds the new prefix
	 * @li additional dirs may be loaded from share/config/kdeglobals (?)
	 *
	 * @param tosave specifies whether the dir will be saved to kderc.
	 * Any hardcoded dirs should not be saved, so kapp would make this
	 * false.
	 */
	void addPrefix( QString dir, bool tosave = false );


	/*
	 * with this we add types
	 * all basic types ("html", "config", etc) are added by kdelibs.
	 *
	 * @param type specifies a short descriptive string to access
	 * files of this type.
	 * @param relativename specifies a directory relative to the root
	 * of the KFSSTND
	 */
	bool addResourceType( const QString& type,
			      const QString& relativename );


	/**
	 * Adds hard path to the front of the search path for
	 * particular types for example in case of icons where
	 * the user specifies extra paths.
	 * 
	 * @param type specifies a short descriptive string to access files 
	 * of this type.
	 * @param absdir points to directory where to look for this specific
	 * type. Non-existant directories may be saved but pruned.
	 * @param tosave is same as with addPrefix.
	 * 
	 */
	bool addResourceDir( const QString& type, 
			     const QString& absdir, 
			     bool tosave = false );

	/**
	 * Tries to find resource in the following order:
	 * @li all hard paths (most recent first)
	 * @li all PREFIX/<relativename> paths (most recent first)
	 *
	 * @return a full path to the filename specified in the second
	 *         argument.
	 */
	QString findResource( const QString& type, 
			      const QString& filename ) const;

	/**
	 * Tries to find all directories whose name consists of the
	 * specified type and a relative path.
	 *
	 * @param type the type of the base directory.
	 * @param reldir relative directory.
	 *
	 * @return a list of matching directories.
	 */
	QStringList findDirs( const QString& type, 
                              const QString& reldir ) const;

	/**
	 * Tries to find the directory the file is in.
	 * It works the same as findResource, but it doesn't
	 * return the filename but the name of the directory.
	 * This way the application can access a couple of files
	 * that have been installed into the same directory without
	 * having to look for each file.
	 *
	 * @return the directory where the file specified in the second
	 *         argument is located.
	 */
	QString findResourceDir( const QString& type,
				 const QString& filename) const;


	/**
	 * Tries to find all resources with the specified type.
	 * The function will look into all specified directories
	 * and returns all filenames in these directories.
	 *
	 * @param type the type of resource to locate directories for.
	 * @param recursive specifies if the function should decend
	 *        into subdirectories.
	 * @param only accept filenames that fit to filter. Currently
	 *        only full match is implemented, but it may be extended
	 *        for regular expressions
	 * @return a list of directories matching the resource specified.
	 */
	QStringList findAllResources( const QString& type, 
				      const QString& filter = QString::null,
				      bool recursive = false) const;

	/** 
	 * Finds the executable in the system path. A valid executable must
	 * be a file and have its executable bit set.
	 *
	 * @see #findAllExe
	 * @param appname the name of the executable file for which to search.
	 * @param pathstr The path which will be searched. If this is 
	 * 		0 (default), the $PATH environment variable will 
	 *		be searched.
	 * @param ignoreExecBit	If true, an existing file will be returned
	 *			even if its executable bit is not set.
	 *
	 * @return The path of the executable. If it was not found, this string 
	 *	will be null.
	 */
	static QString findExe( const QString& appname, 
				const QString& pathstr=QString::null,
				bool ignoreExecBit=false );

	/** 
	 * Finds all occurences of an executable in the system path.
	 *
	 * @see	#findExe
	 *
	 * @param list	will be filled with the pathnames of all the
	 *		executables found. Will be empty if the executable
	 *		was not found.
	 * @param appname	The name of the executable for which to
	 *	 		search.
	 * @param pathstr	The path list which will be searched. If this
	 *		is 0 (default), the $PATH environment variable will
	 *		be searched.
	 * @param ignoreExecBit If true, an existing file will be returned
	 *			even if its executable bit is not set.
	 *
	 * @return The number of executables found, 0 if none were found.
	 */
	static int findAllExe( QStringList& list, const QString& appname,
			       const QString& pathstr=QString::null, 
			       bool ignoreExecBit=false );

	/**
	 * This function adds the defaults that are used by the current
	 * KDE version. It's a serie of addResourceTypes and addPrefix calls.
	 * You normally wouldn't call this function, it's called for you from
	 * KGlobal.
	 */
	void addKDEDefaults();

	/**
	 * Reads customized entries out of the given config object and adds
	 * them via addResourceDirs
	 * 
	 * @param config the object the entries are read from. This should
	 *        contain global config files
	 * @return if new config paths have been added that would reflect
	 * to config
	 **/
	bool addCustomized(KConfig *config);

	/**
	 * returns the list of possible directories for the type 
	 * The functions updates the cache if possible
	 */
	QStringList getResourceDirs(const QString& type) const;

	/**
	 * Finds a location to save files into for the given type
	 * in the user's home directory.
	 * 
	 * @param suffix makes it easier for you to create subdirectories.
		       You can't pass filenames here, you _have_ to pass
		       directory names only and add possible filename in
		       that directory yourself
	 * @param create if set, getSaveLocation will create the directories
	 *        needed (including those given by suffix)
	 */
	 QString getSaveLocation(const QString& type,
				 const QString& suffix = QString::null, 
				 bool create = true) const;

	/**
	 * Recursively creates still missing directories in the given path.
	 * The resulting permissions will depend on the current umask setting.
	 * permission = mode & ~umask.
	 *
	 * Since we are paranoid we use 0700 as default permission. 
	 *
	 * @param dir absolute path of the directory to be made
	 * @param mode directory permissions
	 */
	static bool makeDir(const QString& dir, int mode = 0700);

	static QString kde_default(const QString& type);

 private:

	QString localkdedir() const;

	QStringList prefixes;

	// Directory dictionaries
	QDict<QStringList> absolutes;
	QDict<QStringList> relatives;
	
	mutable QDict<QStringList> dircache;

	// Disallow assignment and copy-construction
	KStandardDirs( const KStandardDirs& );
	KStandardDirs& operator= ( const KStandardDirs& );

	bool addedCustoms;
};

/**
 * On The Usage Of 'locate' and 'locateLocal'
 *
 * Typical KDE applications use resource files in one out of
 * three ways:
 *
 * 1) A resource file is read but is never written. A system
 *    default is supplied but the user can override this
 *    default in his local .kde directory:
 *   
 *    // Code example
 *    myFile = locate("appdata", "groups.lst")
 *    myData =  myReadGroups(myFile);
 *
 * 2) A resource file is read and written. If the user has no
 *    local version of the file the system default is used.
 *    The resource file is always written to the users local
 *    .kde directory.
 *
 *    // Code example
 *    myFile = locate("appdata", "groups.lst")
 *    myData =  myReadGroups(myFile);
 *    ...
 *    doSomething(myData);
 *    ...
 *    myFile = locateLocal("appdata", "groups.lst")
 *    myWriteGroups(myFile, myData);
 *
 * 3) A resource file is read and written. No system default
 *    is used if the user has no local version of the file.
 *    The resource file is always written to the users local
 *    .kde directory.
 *
 *    // Code example
 *    myFile = locateLocal("appdata", "groups.lst")
 *    myData =  myReadGroups(myFile);
 *    ...
 *    doSomething(myData);
 *    ...
 *    myFile = locateLocal("appdata", "groups.lst")
 *    myWriteGroups(myFile, myData);
 **/
 
/**
 * This function is just for convience. It simply calls 
 * KGlobal::dirs()->findResource(type, filename)
 **/
QString locate( const QString& type, const QString& filename );

/**
 * This function is much like locate. However it returns a
 * filename suitable for writing to. No check is made if the
 * specified filename actually exists. Missing directories
 * are created. If filename is only a directory, without a
 * specific file, filename must have a trailing slash. 
 *
 **/
QString locateLocal( const QString& type, QString filename );
#endif // SSK_KSTDDIRS_H
