/*  This file is part of the KDE libraries
 *  Copyright (C) 1999 David Faure <faure@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation;
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA 02111-1307, USA.
 **/

#include "kservicefactory.h"
#include "ksycoca.h"
#include "ksycocatype.h"
#include "ksycocadict.h"
#include "kservice.h"

#include <qstring.h>
#include <kmessagebox.h>

#include <klocale.h>
#include <kdebug.h>
#include <kglobal.h>
#include <kstddirs.h>

KServiceFactory::KServiceFactory()
 : KSycocaFactory( KST_KServiceFactory )
{
   if (KSycoca::isBuilding())
   {
      (*m_pathList) += KGlobal::dirs()->resourceDirs( "apps" );
      (*m_pathList) += KGlobal::dirs()->resourceDirs( "services" );
   }
   self = this;
}

KServiceFactory::~KServiceFactory()
{
}

KSycocaEntry* KServiceFactory::createEntry( const QString& file )
{
  // Just a backup file ?
  if ( file.right(1) == "~" || file.right(4) == ".bak" || ( file[0] == '%' && file.right(1) == "%" ) )
      return 0;

  KSimpleConfig cfg( file, true);
  cfg.setDesktopGroup();
  KService * serv = new KService( cfg );

  if ( serv->isValid() )
     return serv;
  else {
     kdebug( KDEBUG_WARN, 7012, "Invalid Service : %s", file.ascii() );
     delete serv;
     return 0L;
  }
}

// Static function!
KService *
KServiceFactory::findServiceByName(const QString &_name)
{
   if (!self)
      self = new KServiceFactory();
   return self->_findServiceByName(_name);   
}

KService *
KServiceFactory::_findServiceByName(const QString &_name)
{
   if (!m_sycocaDict) return 0; // Error!

   // Warning : this assumes we're NOT building a database
   // But since findServiceByName isn't called in that case...
   // [ see KServiceTypeFactory for how to do it if needed ]

   int offset = m_sycocaDict->find_string( _name );
   if (!offset) return 0; // Not found

   KService *newService = createService(offset);

   // Check whether the dictionary was right.
   if (newService && (newService->name() != _name))
   {
      // No it wasn't...
      delete newService;
      newService = 0; // Not found
   }
   return newService;
}

KService *
KServiceFactory::createService(int offset)
{
   KService *newEntry = 0;
   KSycocaType type; 
   QDataStream *str = KSycoca::findEntry(offset, type);
   switch(type)
   {
     case KST_KService:
        newEntry = new KService(*str, offset);
        break;
        
     default:
        kdebug( KDEBUG_ERROR, 7011, QString("KServiceFactory: unexpected object entry in KSycoca database (type = %1)").arg((int)type) );
        break;
   }
   if (!newEntry->isValid())
   {
      kdebug( KDEBUG_ERROR, 7011, "KServiceFactory: corrupt object in KSycoca database!\n");
      delete newEntry;
      newEntry = 0;
   }   
   return newEntry;
}

//static
KServiceList *
KServiceFactory::allServices()
{
   if (!self)
      self = new KServiceFactory();
   return self->_allServices();   
}

KServiceList *
KServiceFactory::_allServices()
{
   KServiceList * list = new KServiceList;
   // Assume we're NOT building a database
   // Get stream to factory start
   QDataStream *str = KSycoca::registerFactory( factoryId() );
   // Read the dict offset - will serve as an end point for the list of entries
   Q_INT32 sycocaDictOffset;
   (*str) >> sycocaDictOffset;

   int offset = str->device()->at();
   KService *newService;
   while ( offset < sycocaDictOffset )
   {
      newService = createService(offset);
      if (newService)
         list->append( newService );
      offset = str->device()->at();
   }
   return list;
}

// static
KServiceList *KServiceFactory::offers( int serviceTypeOffset )
{
   if (!self)
      self = new KServiceFactory();
   return self->_offers( serviceTypeOffset );   
}

KServiceList *KServiceFactory::_offers( int serviceTypeOffset )
{
  kdebug(KDEBUG_INFO, 7011, QString("KServiceFactory::offers ( %1 )").arg(serviceTypeOffset,8,16));
  KServiceList * list = new KServiceList;
  // Jump to offer list
  QDataStream *str = KSycoca::findOfferList();
  Q_INT32 aServiceTypeOffset;
  Q_INT32 aServiceOffset;
  // We might want to do a binary search instead of a linear search
  // since servicetype offsets are sorted. Bah.
  while (true)
  {
      (*str) >> aServiceTypeOffset;
      if ( aServiceTypeOffset )
      {
        (*str) >> aServiceOffset;
        if ( aServiceTypeOffset == serviceTypeOffset )
        {
          kdebug(KDEBUG_INFO, 7011, QString("KServiceFactory::offers : Found !"));
          // Save stream position !
          int savedPos = str->device()->at();
          // Create Service
          list->append( createService( aServiceOffset ) );
          // Restore position
          str->device()->at( savedPos );
        } else if ( aServiceTypeOffset > (Q_INT32)serviceTypeOffset )
          break; // too far
      }
      else
        break; // 0 => end of list
  }
  return list;
}

KServiceFactory *KServiceFactory::self = 0;

#include "kservicefactory.moc"
