/*
* kglobal.h -- Declaration of class KGlobal.
* Generated by newclass on Sat May  1 02:08:43 EST 1999.
*/
#ifndef SSK_KGLOBAL_H
#define SSK_KGLOBAL_H

class KApplication;
class KStandardDirs;
class KConfig;
class KLocale;
class KIconLoader;
class KCharsets;
class QFont;

/**
* Accessors to KDE global objects.
* 
* This class is never instantiated. Instead, it is initialized by
* @ref KApplication.
*
* WARNING: Do NOT access these functions without first constructing a 
* @ref KApplication object.
* 
* @author Sirtaj Singh Kang (taj@kde.org)
* @version $Id$
*/
class KGlobal
{
public:

	static KApplication	*kApp();
	static KStandardDirs	*dirs();
	
	static KConfig		*config();
	static KConfig		*instanceConfig();
	
	static KIconLoader	*iconLoader();

	static KLocale		*locale();
	static KCharsets	*charsets();

	static QFont            generalFont();
	static QFont            fixedFont();

		
	KGlobal();
	KGlobal( const KGlobal& );

	static 	KApplication	*_kapp;
	static 	KStandardDirs	*_dirs;

	static 	KConfig		*_config;
	static 	KConfig		*_instanceConfig;
	static 	KIconLoader	*_iconLoader;

	static 	KLocale		*_locale;
	static 	KCharsets	*_charsets;

	static  QFont           *_generalFont;
	static  QFont           *_fixedFont;


protected:
	friend class KApplication;

	/**
	 * This will free all locally allocated objects and memory.
	 * Access to any method after this will cause a reallocation.
	 */
	static void freeAll();

	static bool newAppConfig();
	static bool addedConfig;
	
};

#endif // SSK_KGLOBAL_H
