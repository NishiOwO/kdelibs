/* This file is part of the KDE project
 *
 * Copyright (C) 2000 Simon Hausmann <hausmann@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "khtml_factory.h"
#include "khtml_part.h"

#include <kinstance.h>
#include <kaboutdata.h>
#include <klocale.h>

#include <assert.h>

extern "C"
{
  void *init_libkhtml()
  {
    return new KHTMLFactory;
  }
};

KInstance *KHTMLFactory::s_instance = 0L;
KAboutData *KHTMLFactory::s_about = 0L;

KHTMLFactory::KHTMLFactory()
{
}

KHTMLFactory::~KHTMLFactory()
{
  if ( s_instance )
    delete s_instance;
  if ( s_about )
    delete s_about;
}

KParts::Part *KHTMLFactory::createPart( QWidget *parentWidget, const char *widgetName, QObject *parent, const char *name, const char *className, const QStringList &args )
{
  KHTMLPart *part = new KHTMLPart( parentWidget, widgetName, parent, name );
  emit objectCreated( part ) ;
  return part;
}

KInstance *KHTMLFactory::instance()
{
  if ( !s_instance )
  {
    s_about = new KAboutData( "khtml", I18N_NOOP( "KHTML" ), "3.0",
			      I18N_NOOP( "Embeddable HTML component" ),
			      KAboutData::License_LGPL );
    s_about->addAuthor( "Lars Knoll", 0, "knoll@kde.org" );
    s_about->addAuthor( "Antti Koivisto", 0, "koivisto@kde.org" );
    s_about->addAuthor( "Waldo Bastian", 0, "bastian@kde.org" );
    s_about->addAuthor( "Torben Weis", 0, "weis@kde.org" );
    s_about->addAuthor( "Martin Jones", 0, "mjones@kde.org" );
    s_about->addAuthor( "Simon Hausmann", 0, "hausmann@kde.org" );

    s_instance = new KInstance( s_about );
  }

  return s_instance;
}
