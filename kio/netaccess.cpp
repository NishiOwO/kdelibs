/*  $Id$

    This file is part of the KDE libraries
    Copyright (C) 1997 Torben Weis (weis@kde.org)
    Copyright (C) 1998 Matthias Ettrich (ettrich@kde.org)
    Copyright (C) 1999 David Faure (faure@kde.org)

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

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <qstring.h>
#include <qapplication.h>

#include <kdebug.h>
#include <kurl.h>
#include <kio_job.h>

#include "kio_netaccess.h"

using namespace KIO;

bool NetAccess::download(const KURL& u, QString & target, Job *job)
{
  if (u.isLocalFile()) {
    // file protocol. We do not need the network
    target = u.path();
    return true;
  }

  NetAccess kioNet;
  KURL path( target );
  bool result = kioNet.downloadInternal( u, path, job );
  target = path.path(); // set by downloadInternal
  return result;
}

bool NetAccess::upload(const QString& src, const KURL& target, Job *job)
{
  if (target.isEmpty())
    return false;

  // If !target.isEmpty(), then downloadInternal just
  // copies src to target.  Great!
  NetAccess kioNet;
  KURL path( target ); // need that for non-constness
  return kioNet.downloadInternal( KURL(src), path, job);
}

QStringList* NetAccess::tmpfiles = 0L;

void NetAccess::removeTempFile(const QString& name)
{
  if (!tmpfiles)
    return;
  if (tmpfiles->contains(name))
  {
    unlink(name);
    tmpfiles->remove(name);
  }
}

bool NetAccess::downloadInternal(const KURL& src, KURL& target, Job *job)
{
  if (target.isEmpty())
  {
      QString path = tmpnam(0);
      target = path;
      if (!tmpfiles)
	  tmpfiles = new QStringList;
      tmpfiles->append(path);
  }
  bDownloadOk = true; // success unless further error occurs
  
  if (!job) kDebugFatal("Setting the job in KIO::NetAccess is not allowed anymore!");
 
  job = KIO::file_copy( src, target );
  connect( job, SIGNAL( result (KIO::Job *) ), 
           this, SLOT( slotResult (KIO::Job *) ) );

  qApp->enter_loop();
  return bDownloadOk;
}

void NetAccess::slotResult( KIO::Job * job )
{
  bDownloadOk = !job->error();
  qApp->exit_loop();
}

#include "kio_netaccess.moc"
