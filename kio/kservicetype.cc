#include "kservicetype.h"

#include <assert.h>
#include <qsmartptr.h>

QList<KServiceType>* KServiceType::s_lstServiceTypes = 0L;

void KServiceType::initStatic()
{
  if ( s_lstServiceTypes == 0 )
    s_lstServiceTypes = new QList<KServiceType>;
}

KServiceType::KServiceType( KSimpleConfig& _cfg )
{
  initStatic();
  s_lstServiceTypes->append( this );
  _cfg.setDesktopGroup();

  // Is this a bug or an intentionnal change ? Currently mimetype files
  // contain "MimeType=", not "Name=" !  (David)
  // m_strName = _cfg.readEntry( "Name" );

  // -> reverted to "MimeType=" temporarily, but seems wrong since
  // some servicetype are not mimetypes, if I got it correctly (any example, Torben ? ;) )
  m_strName = _cfg.readEntry( "MimeType" );

  if ( m_strName.isEmpty() )
    m_strName = _cfg.readEntry( "X-KDE-ServiceType" );

  m_strComment = _cfg.readEntry( "Comment" );
  m_strIcon = _cfg.readEntry( "Icon" );

  QStringList tmpList = _cfg.groupList();
  QStringList::Iterator gIt = tmpList.begin();

  for( ; gIt != tmpList.end(); ++gIt )
  {
    if ( (*gIt).find( "Property::" ) == 0 )
    {
      _cfg.setGroup( *gIt );
      m_mapProps.insert( (*gIt).mid( 10 ),
			 _cfg.readPropertyEntry( "Value",
						 QVariant::nameToType( _cfg.readEntry( "Type" ) ) ) );
    }
  }

  gIt = tmpList.begin();
  for( ; gIt != tmpList.end(); ++gIt )
  {
    if( (*gIt).find( "PropertyDef::" ) == 0 )
    {
      _cfg.setGroup( *gIt );
      m_mapPropDefs.insert( (*gIt).mid( 13 ),
			    QVariant::nameToType( _cfg.readEntry( "Type" ) ) );
    }
  }

  m_bValid = !m_strName.isEmpty();
}

KServiceType::KServiceType( const QString& _type, const QString& _icon, const QString& _comment )
{
  initStatic();
  s_lstServiceTypes->append( this );

  m_strName = _type;
  m_strIcon = _icon;
  m_strComment = _comment;
  m_bValid = !m_strName.isEmpty();
}

KServiceType::~KServiceType()
{
  s_lstServiceTypes->removeRef( this );
}

KServiceType::PropertyPtr KServiceType::property( const QString& _name ) const
{
  QVariant* p = 0;

  if ( _name == "Name" )
    p = new QVariant( m_strName );
  if ( _name == "Icon" )
    p = new QVariant( m_strIcon );
  if ( _name == "Comment" )
    p = new QVariant( m_strComment );

  if ( p )
    return KServiceType::PropertyPtr( p );

  QMap<QString,QVariant>::ConstIterator it = m_mapProps.find( _name );
  if ( it == m_mapProps.end() )
    return (QVariant*)0;

  p = (QVariant*)(&(it.data()));

  p->ref();
  return KServiceType::PropertyPtr( p );
}

QStringList KServiceType::propertyNames() const
{
  QStringList res;

  QMap<QString,QVariant>::ConstIterator it = m_mapProps.begin();
  for( ; it != m_mapProps.end(); ++it )
    res.append( it.key() );

  res.append( "Name" );
  res.append( "Comment" );
  res.append( "Icon" );

  return res;
}

QVariant::Type KServiceType::propertyDef( const QString& _name ) const
{
  QMap<QString,QVariant::Type>::ConstIterator it = m_mapPropDefs.find( _name );
  if ( it == m_mapPropDefs.end() )
    return QVariant::Empty;
  return it.data();
}

QStringList KServiceType::propertyDefNames() const
{
  QStringList l;

  QMap<QString,QVariant::Type>::ConstIterator it = m_mapPropDefs.begin();
  for( ; it != m_mapPropDefs.end(); ++it )
    l.append( it.key() );

  return l;
}

void KServiceType::load( QDataStream& _str )
{
  Q_INT8 b;
  _str >> m_strName >> m_strIcon >> m_strComment >> m_mapProps >> m_mapPropDefs >> b;
  m_bValid = b;
}

void KServiceType::save( QDataStream& _str ) const
{
  _str << m_strName << m_strIcon << m_strComment << m_mapProps << m_mapPropDefs << (Q_INT8)m_bValid;
}

KServiceType* KServiceType::serviceType( const QString& _name )
{
  assert( s_lstServiceTypes );
  QListIterator<KServiceType> it( *s_lstServiceTypes );
  for( ; it.current(); ++it )
    if ( it.current()->name() == _name )
      return it.current();

  return 0;
}

QDataStream& operator>>( QDataStream& _str, KServiceType& s )
{
  s.load( _str );
  return _str;
}

QDataStream& operator<<( QDataStream& _str, const KServiceType& s )
{
  s.save( _str );
  return _str;
}
