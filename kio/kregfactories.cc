#include "kregfactories.h"
#include "kmimetypes.h"

#include <qstring.h>
#include <qmessagebox.h>

#include <kapp.h>
#include <klocale.h>
#include <kdebug.h>
#include <kglobal.h>
#include <kstddirs.h>

#include <stdlib.h>

/**************************************************
 *
 * Helper
 *
 **************************************************/

/*
QString systemShareDir()
{
// This is really dirty. We can't use existing kapp functions because
// we want to add anything after share/. We should have kde_sharedir()
// but anyway we'll switch to KStdDirs one day ... until then : dirt ;) 

  QString tmp( "/opt/kde" );
  const char* env = getenv( "KDEDIR" );
  if ( env )
    tmp = env;

  tmp += "/share";

  return tmp;
}

QString userShareDir()
{
  return ( kapp->localkdedir() + "/share" );
}
*/

/**************************************************
 *
 * KServiceTypeEntry
 *
 **************************************************/

KServiceTypeEntry::KServiceTypeEntry( KRegistry* _reg, const QString& _file, KServiceType *_mime, KServiceTypeFactory *factory )
  : KRegEntry( _reg, _file )
{
  m_pServiceType = _mime;
  m_pFactory = factory;
  m_pFactory->addEntryNotify( this );
}

KServiceTypeEntry::~KServiceTypeEntry()
{
  m_pFactory->removeEntryNotify( this );
}

void KServiceTypeEntry::save( QDataStream& _str ) const
{
  _str << (Q_UINT32)m_pServiceType->typeCode();
  _str << *m_pServiceType;

  KRegEntry::save( _str );
}

/**************************************************
 *
 * KServiceTypeFactory
 *
 **************************************************/

KServiceTypeFactory::KServiceTypeFactory()
{
  m_pathList += KGlobal::dirs()->getResourceDirs( "mime" );
  m_pathList += KGlobal::dirs()->getResourceDirs( "servicetypes" );
}

KServiceTypeFactory::KServiceTypeFactory( const QStringList& _path_list )
{
  m_pathList = _path_list;
}

/*
KServiceTypeFactory::KServiceTypeFactory( const QString& _system_path, 
					  const QString& _user_path )
{
  if ( !_system_path.isEmpty() )
    m_pathList.append( systemShareDir() + _system_path );
  QString user = _user_path;
  if ( !user.isEmpty() )
    user = _system_path;
  if ( !user.isEmpty() )
    m_pathList.append( userShareDir() + user );
}
*/

KRegEntry* KServiceTypeFactory::create( KRegistry* _reg, const QString& _file, QDataStream& _str )
{
  kdebug(KDEBUG_INFO, 7012, "KServiceTypeFactory::create from datastream for %s",_file.ascii());
  // Read typecode
  Q_UINT32 u;
  _str >> u;

  KServiceType *e;

  switch( u )
    {
    case TC_KServiceType:
      e = new KServiceType( _str );
      break;
    case TC_KMimeType:
      e = new KMimeType( _str );
      break;
    case TC_KFolderType:
      e = new KFolderType( _str );
      break;
    case TC_KDEDesktopMimeType:
      e = new KDEDesktopMimeType( _str );
      break;
    case TC_KExecMimeType:
      e = new KExecMimeType( _str );
      break;
    default:
      ASSERT( 0 );
      return 0;
    }

  if ( !e->isValid() )
  {
    kdebug( KDEBUG_WARN, 7012, "Invalid ServiceType : %s", _file.ascii() );
    delete e;
    return 0;
  }

  KServiceTypeEntry* res = new KServiceTypeEntry( _reg, _file, e, this );
  e->deref();
  res->load( _str );

  return res;
}

KRegEntry* KServiceTypeFactory::create( KRegistry* _reg, const QString& _file, KSimpleConfig &_cfg )
{
  QString service = _cfg.readEntry( "X-KDE-ServiceType" );
  QString mime = _cfg.readEntry( "MimeType" );

  if ( mime.isEmpty() && service.isEmpty() )
  {
    QString tmp = i18n( "The service/mime type config file\n%1\n"
			"does not contain a ServiceType=...\nor MimeType=... entry").arg( _file );
    QMessageBox::critical( 0L, i18n( "KFM Error" ), tmp, i18n( "OK" ) );
    return 0;
  }

  KServiceType* e;

  if ( mime == "inode/directory" )
    e = new KFolderType( _cfg );
  else if ( mime == "application/x-desktop" )
    e = new KDEDesktopMimeType( _cfg );
  else if ( mime == "application/x-executable" || mime == "application/x-shellscript" )
    e = new KExecMimeType( _cfg );
  else if ( !mime.isEmpty() )
    e = new KMimeType( _cfg );
  else
    e = new KServiceType( _cfg );

  if ( !e->isValid() )
  {
    kdebug( KDEBUG_WARN, 7012, "Invalid ServiceType : %s", _file.ascii() );
    delete e;
    return 0;
  }

  KServiceTypeEntry* res = new KServiceTypeEntry( _reg, _file, e, this );
  e->deref();

  return res;
}

/**************************************************
 *
 * KServiceEntry
 *
 **************************************************/

KServiceEntry::KServiceEntry( KRegistry* _reg, const QString& _file, KService *_service, KServiceFactory *factory )
  : KRegEntry( _reg, _file )
{
  m_pService = _service;
  m_pFactory = factory;
  m_pFactory->addEntryNotify( this );
}

KServiceEntry::~KServiceEntry()
{
  m_pFactory->removeEntryNotify( this );
}

void KServiceEntry::save( QDataStream& _str ) const
{
  _str << (Q_UINT32)m_pService->typeCode();
  _str << *m_pService;

  KRegEntry::save( _str );
}

/**************************************************
 *
 * KServiceFactory
 *
 **************************************************/

KServiceFactory::KServiceFactory()
{
  m_pathList += KGlobal::dirs()->getResourceDirs( "apps" );
  m_pathList += KGlobal::dirs()->getResourceDirs( "services" );
}

KServiceFactory::KServiceFactory( const QStringList& _path_list )
{
  m_pathList = _path_list;
}

/*
KServiceFactory::KServiceFactory( const QString& _system_path, 
				  const QString& _user_path )
{
  if ( !_system_path.isEmpty() )
    m_pathList.append( systemShareDir() + _system_path );
  QString user = _user_path;
  if ( !user.isEmpty() )
    user = _system_path;
  if ( !user.isEmpty() )
    m_pathList.append( userShareDir() + user );
}
*/
KRegEntry* KServiceFactory::create( KRegistry* _reg, const QString& _file, QDataStream& _str )
{
  Q_UINT32 u;
  _str >> u;

  ASSERT( u == TC_KService );

  KService *s = new KService( _str );
  if ( !s->isValid() )
  {
    kdebug( KDEBUG_WARN, 7012, "Invalid Service : %s", _file.ascii() );
    delete s;
    return 0;
  }

  KServiceEntry* e = new KServiceEntry( _reg, _file, s, this );
  s->deref();

  e->load( _str );

  return e;
}

KRegEntry* KServiceFactory::create( KRegistry* _reg, const QString& _file, KSimpleConfig &_cfg )
{
  KService *service = new KService( _cfg );
  if ( !service->isValid() )
  {
    kdebug( KDEBUG_WARN, 7012, "Invalid Service : %s", _file.ascii() );
    delete service;
    return 0;
  }

  KServiceEntry* e = new KServiceEntry( _reg, _file, service, this );
  service->deref();

  return e;
}

#include "kregfactories.moc"

