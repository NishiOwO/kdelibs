/* This file is part of the KDE libraries
   Copyright (C) 1999 Torben Weis <weis@kde.org>
   Copyright (C) 2000- Waldo Bastain <bastain@kde.org>
   Copyright (C) 2000- Dawit Alemayehu <adawit@kde.org>

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

#include <string.h>
#include <sys/utsname.h>

#include <kglobal.h>
#include <klocale.h>
#include <kconfig.h>
#include <kio/kpac.h>
#include <kstddirs.h>
#include <klibloader.h>
#include <kstringhandler.h>
#include <kstaticdeleter.h>
#include <kio/slaveconfig.h>
#include <kio/ioslave_defaults.h>
#include <kio/http_slave_defaults.h>

#include "kprotocolmanager.h"

class
KProtocolManagerPrivate
{
public:
   KProtocolManagerPrivate();

   ~KProtocolManagerPrivate();

   KConfig *config;
   KConfig *http_config;
   KPAC *pac;
   bool init_busy;
   KURL url;
   QString protocol;
   QString proxy;
   QString modifiers;
   QString useragent;
};

static KStaticDeleter<KProtocolManagerPrivate> kpmpksd;

KProtocolManagerPrivate::KProtocolManagerPrivate()
                        :config(0), http_config(0), pac(0), init_busy(false)
{
   kpmpksd.setObject(this);
}

KProtocolManagerPrivate::~KProtocolManagerPrivate()
{
   delete config;
   delete http_config;
   delete pac;
   kpmpksd.setObject(0);
}

static KProtocolManagerPrivate* d = 0;

// DEFUALT USERAGENT STRING
#define CFG_DEFAULT_UAGENT(X) \
QString("Mozilla/5.0 (compatible; Konqueror/%1%2)").arg(KDE_VERSION_STRING).arg(X)

void KProtocolManager::reparseConfiguration()
{
  delete d;
  d = 0;
}

KConfig *KProtocolManager::config()
{
  if (!d)
     d = new KProtocolManagerPrivate;

  if (!d->config)
  {
     d->config = new KConfig("kioslaverc", false, false);
  }
  return d->config;
}

KConfig *KProtocolManager::http_config()
{
  if (!d)
     d = new KProtocolManagerPrivate;

  if (!d->http_config)
  {
     d->http_config = new KConfig("kio_httprc", false, false);
  }
  return d->http_config;
}

KPAC *KProtocolManager::pac()
{
  ProxyType type = proxyType();
  if (type < PACProxy)
    return 0;

  if (!d->pac)
  {
    if (d->init_busy) return 0;
    d->init_busy = true;

    KLibrary *lib = KLibLoader::self()->library("libkpac");
    if (lib)
    {
      KPAC *(*create_pac)() = (KPAC *(*)())(lib->symbol("create_pac"));
      if (create_pac)
      {
        KPAC *newPAC = create_pac();
        switch (type)
        {
          case PACProxy:
            newPAC->init( proxyConfigScript() );
            break;
          case WPADProxy:
            newPAC->discover();
          default:
            break;
        }
        d->pac = newPAC;
      }
    }
    d->init_busy = false;
  }
  return d->pac;
}

/*=============================== TIMEOUT SETTINGS ==========================*/

int KProtocolManager::readTimeout()
{
  KConfig *cfg = config();
  cfg->setGroup( QString::null );
  int val = cfg->readNumEntry( "ReadTimeout", DEFAULT_READ_TIMEOUT );
  return QMAX(MIN_TIMEOUT_VALUE, val);
}

int KProtocolManager::connectTimeout()
{
  KConfig *cfg = config();
  cfg->setGroup( QString::null );
  int val = cfg->readNumEntry( "ConnectTimeout", DEFAULT_CONNECT_TIMEOUT );
  return QMAX(MIN_TIMEOUT_VALUE, val);
}

int KProtocolManager::proxyConnectTimeout()
{
  KConfig *cfg = config();
  cfg->setGroup( QString::null );
  int val = cfg->readNumEntry( "ProxyConnectTimeout", DEFAULT_PROXY_CONNECT_TIMEOUT );
  return QMAX(MIN_TIMEOUT_VALUE, val);
}

int KProtocolManager::responseTimeout()
{
  KConfig *cfg = config();
  cfg->setGroup( QString::null );
  int val = cfg->readNumEntry( "ResponseTimeout", DEFAULT_RESPONSE_TIMEOUT );
  return QMAX(MIN_TIMEOUT_VALUE, val);
}

/*========================== PROXY SETTINGS =================================*/

bool KProtocolManager::useProxy()
{
  return proxyType() != NoProxy;
}

bool KProtocolManager::useReverseProxy()
{
  KConfig *cfg = config();
  cfg->setGroup( "Proxy Settings" );
  return cfg->readBoolEntry("ReversedException", false);
}

KProtocolManager::ProxyType KProtocolManager::proxyType()
{
  KConfig *cfg = config();
  cfg->setGroup( "Proxy Settings" );
  return static_cast<ProxyType>(cfg->readNumEntry( "ProxyType" ));
}

KProtocolManager::ProxyAuthMode KProtocolManager::proxyAuthMode()
{
  KConfig *cfg = config();
  cfg->setGroup( "Proxy Settings" );
  return static_cast<ProxyAuthMode>(cfg->readNumEntry( "AuthMode" ));
}

/*========================== CACHING =====================================*/

bool KProtocolManager::useCache()
{
  KConfig *cfg = http_config();
  return cfg->readBoolEntry( "UseCache", true );
}

KIO::CacheControl KProtocolManager::cacheControl()
{
  KConfig *cfg = http_config();
  QString tmp = cfg->readEntry("cache");
  if (tmp.isEmpty())
    return DEFAULT_CACHE_CONTROL;
  return KIO::parseCacheControl(tmp);
}

QString KProtocolManager::cacheDir()
{
  KConfig *cfg = http_config();
  return cfg->readEntry("CacheDir", KGlobal::dirs()->saveLocation("data","kio_http/cache"));
}

int KProtocolManager::maxCacheAge()
{
  KConfig *cfg = http_config();
  return cfg->readNumEntry( "MaxCacheAge", DEFAULT_MAX_CACHE_AGE ); // 14 days
}

int KProtocolManager::maxCacheSize()
{
  KConfig *cfg = http_config();
  return cfg->readNumEntry( "MaxCacheSize", DEFAULT_MAX_CACHE_SIZE ); // 5 MB
}

QString KProtocolManager::noProxyFor()
{
  KConfig *cfg = config();
  cfg->setGroup( "Proxy Settings" );
  return cfg->readEntry( "NoProxyFor" );
}

QString KProtocolManager::proxyFor( const QString& protocol )
{
  KConfig *cfg = config();
  cfg->setGroup( "Proxy Settings" );
  return cfg->readEntry( protocol.lower() + "Proxy" );
}

QString KProtocolManager::proxyForURL( const KURL &url )
{
  QString proxy;
  ProxyType pt = proxyType();

  switch (pt)
  {
      case PACProxy:
      case WPADProxy:
          if (!url.host().isEmpty() && pac())
              proxy = pac()->proxyForURL( url );
          break;
      case EnvVarProxy:
          proxy = QString::fromLocal8Bit(getenv(proxyFor(url.protocol()).local8Bit()));
          break;
      case ManualProxy:
          proxy = proxyFor( url.protocol() );
          break;
      case NoProxy:
      default:
          break;
  }

  return (proxy.isEmpty() ? QString::fromLatin1("DIRECT") : proxy);
}

void KProtocolManager::badProxy( const QString &proxy )
{
  if ( d && d->pac ) // don't load KPAC here if it isn't already
    d->pac->badProxy( proxy );
}

QString KProtocolManager::slaveProtocol( const QString & protocol )
{
  if ( useProxy() )
  {
    KURL u = proxyFor(protocol);
    if ( u.isValid() )
    {
      // HACK: This will be removed once kio_http
      // gets ported over to TCPSlaveBase!!
      QString p = u.protocol();
      if ( p == QString::fromLatin1("http") &&
           protocol == QString::fromLatin1("https") )
        p = protocol;
      return p;
    }
  }
  return protocol;
}

/*
    Domain suffix match. E.g. return true if host is "cuzco.inka.de" and
    nplist is "inka.de,hadiko.de" or if host is "localhost" and nplist is
    "localhost".
*/
static bool revmatch(const char *host, const char *nplist)
{
  if (0 == host)
    return false;

  const char *hptr = host + strlen( host ) - 1;
  const char *nptr = nplist + strlen( nplist ) - 1;
  const char *shptr = hptr;

  while ( nptr >= nplist )
  {
    if ( *hptr != *nptr )
    {
      hptr = shptr;
      // Try to find another domain or host in the list
      while(--nptr>=nplist && *nptr!=',' && *nptr!=' ') ;
      // Strip out multiple spaces and commas
      while(--nptr>=nplist && (*nptr==',' || *nptr==' ')) ;
    }
    else
    {
      if ( nptr==nplist || nptr[-1]==',' || nptr[-1]==' ')
        return true;
      hptr--; nptr--;
    }
  }

  return false;
}

QString KProtocolManager::slaveProtocol(const KURL &url, QString &proxy)
{
  if (d && d->url == url)
  {
     proxy = d->proxy;
     return d->protocol;
  }

  if (useProxy())
  {
     proxy = proxyForURL(url);
     if ((proxy != "DIRECT") && (!proxy.isEmpty()))
     {
        QString noProxy = noProxyFor();
        ProxyType type = proxyType();
        bool useRevProxy = ( (type == ManualProxy || type == EnvVarProxy) &&
                            useReverseProxy() );
        bool isRevMatch = (!noProxy.isEmpty() &&
                           revmatch(url.host().lower().latin1(),
                                    noProxy.lower().latin1()));
        if ( (!useRevProxy && !isRevMatch) ||
             (useRevProxy && isRevMatch) )
        {
           // Let's not assume all proxying is done through
           // the http io-slave.  Instead attempt to determine
           // the protocol to use from the proxy URL itself.
           // If the proxy URL is then determined to be invalid,
           // bypass it all together.
           d->url = proxy;
           if ( d->url.isValid() )
           {
              d->protocol = d->url.protocol();
              // HACK: This will be removed once kio_http
              // gets ported over to TCPSlaveBase!!
              if ( url.protocol() == QString::fromLatin1("https") &&
                   d->protocol == QString::fromLatin1("http") )
                d->protocol = url.protocol();
              d->url = url;
              d->proxy = proxy;
              return d->protocol;
           }
        }
     }
  }
  d->url = url;
  d->proxy = proxy = QString::null;
  d->protocol = url.protocol();
  return d->protocol;
}


/*==================================== OTHERS ===============================*/

bool KProtocolManager::markPartial()
{
  KConfig *cfg = config();
  cfg->setGroup( QString::null );
  return cfg->readBoolEntry( "MarkPartial", true );
}

int KProtocolManager::minimumKeepSize()
{
  KConfig *cfg = config();
  cfg->setGroup( QString::null );
  return cfg->readNumEntry( "MinimumKeepSize",
                            DEFAULT_MINIMUM_KEEP_SIZE ); // 5000 byte
}

bool KProtocolManager::autoResume()
{
  KConfig *cfg = config();
  cfg->setGroup( QString::null );
  return cfg->readBoolEntry( "AutoResume", false );
}

bool KProtocolManager::persistentConnections()
{
  KConfig *cfg = config();
  cfg->setGroup( QString::null );
  return cfg->readBoolEntry( "PersistentConnections", true );
}

QString KProtocolManager::proxyConfigScript()
{
  KConfig *cfg = config();
  cfg->setGroup( "Proxy Settings" );
  return cfg->readEntry( "Proxy Config Script" );
}

QString KProtocolManager::userAgentForHost( const QString& hostname )
{
  QString user_agent = KIO::SlaveConfig::self()->configData("http", hostname, "UserAgent");

  if (user_agent.isEmpty())
    user_agent = defaultUserAgent();

  return user_agent;
}

/*================================= USER-AGENT SETTINGS =====================*/

QString KProtocolManager::defaultUserAgent( )
{
  QString modifiers = KIO::SlaveConfig::self()->configData("http", QString::null, "UserAgentKeys");
  return defaultUserAgent(modifiers);
}

QString KProtocolManager::defaultUserAgent( const QString &_modifiers )
{
  if (!d)
     d = new KProtocolManagerPrivate;

  QString modifiers = _modifiers.lower();
  if (modifiers.isEmpty())
     modifiers = DEFAULT_USER_AGENT_KEYS;

  if (d->modifiers == modifiers)
     return d->useragent;

  QString supp;
  struct utsname nam;
  if( uname(&nam) >= 0 )
  {
    if( modifiers.contains('o') )
    {
      supp += QString("; %1").arg(nam.sysname);
      if ( modifiers.contains('v') )
        supp += QString(" %1").arg(nam.release);
    }
    if( modifiers.contains('p') )
    {
      supp += QString::fromLatin1("; X11");  // TODO: determine this valye instead of hardcoding...
    }
    if( modifiers.contains('m') )
    {
      supp += QString("; %1").arg(nam.machine);
    }
    if( modifiers.contains('l') )
    {
      QStringList languageList = KGlobal::locale()->languageList();
      QStringList::Iterator it = languageList.find( QString::fromLatin1("C") );
      if( it != languageList.end() )
      {
        if( languageList.contains( QString::fromLatin1("en") ) > 0 )
          languageList.remove( it );
        else
          (*it) = QString::fromLatin1("en");
      }
      if( languageList.count() )
        supp += QString("; %1").arg(languageList.join(", "));
    }
  }
  d->modifiers = modifiers;
  d->useragent = CFG_DEFAULT_UAGENT(supp);
  return d->useragent;
}

/*================================ !!DEPRECATED!! ===========================*/

// obsolete, remove me in KDE 3.0
void KProtocolManager::setReadTimeout( int ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setConnectTimeout( int ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setProxyConnectTimeout( int ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setResponseTimeout( int ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setMarkPartial( bool ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setMinimumKeepSize( int ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setAutoResume( bool ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setPersistentConnections( bool ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setUseCache( bool ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setMaxCacheSize( int ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setMaxCacheAge( int ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setUseProxy( bool ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setUseReverseProxy( bool ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setProxyType( ProxyType ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setProxyAuthMode( ProxyAuthMode ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setFtpProxy( const QString& ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setHttpProxy( const QString& ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setNoProxyFor( const QString& ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setProxyFor( const QString&, const QString& ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setCacheControl( KIO::CacheControl ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setUserAgentList( const QStringList& ){}
// obsolete, remove me in KDE 3.0
void KProtocolManager::setProxyConfigScript( const QString& ){}
// obsolete, remove me in KDE 3.0
QStringList KProtocolManager::userAgentList()
{ return QStringList(); }
// obsolete, remove me in KDE 3.0
int KProtocolManager::defaultConnectTimeout()
{ return DEFAULT_CONNECT_TIMEOUT; }
// obsolete, remove me in KDE 3.0
int KProtocolManager::defaultProxyConnectTimeout()
{ return DEFAULT_PROXY_CONNECT_TIMEOUT; }
// obsolete, remove me in KDE 3.0
int KProtocolManager::defaultResponseTimeout()
{ return DEFAULT_RESPONSE_TIMEOUT; }
// obsolete, remove me in KDE 3.0
int KProtocolManager::defaultReadTimeout()
{ return DEFAULT_READ_TIMEOUT; }
// obsolete, remove me in KDE 3.0
int KProtocolManager::minimumTimeoutThreshold()
{ return MIN_TIMEOUT_VALUE; }
// obsolete, remove me in KDE 3.0
QString KProtocolManager::ftpProxy()
{ return proxyFor( "ftp" ); }
// obsolete, remove me in KDE 3.0
QString KProtocolManager::httpProxy()
{ return proxyFor( "http" ); }
