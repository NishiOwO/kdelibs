// -*- c++ -*-
/* This file is part of the KDE libraries
    Copyright (C) 1997, 1998 Richard Moore <rich@kde.org>
                  1998 Stephan Kulow <coolo@kde.org>
                  1998 Daniel Grana <grana@ie.iwi.unibe.ch>

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

#include "kfileinfo.h"

#include <sys/types.h>
#include <dirent.h>
#include <grp.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>
#include <time.h>

#include <qfileinfo.h>
#include <qregexp.h>

#include <kapp.h>
#include <kglobal.h>
#include <klocale.h>
#include <kurl.h>
#include "config-kfile.h"

PasswdMapper *KFileInfo::passwdMap = 0L;
GroupMapper  *KFileInfo::groupMap  = 0L;

KFileInfo::KFileInfo(const KUDSEntry &e)
{
    myIsDir = false;
    myIsFile = true;
    myIsSymLink = false;
    myPermissions = 0755;
    mySize = 0;
    myIsReadable = true;

    debugC("KFileInfo::KFileInfo");
    KURL url;
    KUDSEntry::ConstIterator it = e.begin();
    for( ; it != e.end(); it++ ) {
	switch (( *it ).m_uds) {
	case UDS_NAME:
	    myName = ( *it ).m_str;
	    break;
	case UDS_SIZE:
	    mySize = ( *it ).m_long;
	    break;
	case UDS_MODIFICATION_TIME:
	    myDate = dateTime(( *it ).m_long);
	    break;
	case UDS_USER:
	    myOwner = ( *it ).m_str;
	    break;
	case UDS_GROUP:
	    myGroup = ( *it ).m_str;
	    break;
	case UDS_ACCESS:
	    myPermissions = ( *it ).m_long;
	    parsePermissions(myPermissions);
	    break;
	case UDS_FILE_TYPE:
	    myIsDir = (( *it ).m_long & S_IFDIR) != 0;
	    myIsFile = !myIsDir;
	    break;
	case UDS_LINK_DEST:
	    myIsSymLink = (( *it ).m_str.length());
	    break;
	case UDS_URL:
	    url = ( *it ).m_str;
	    myBaseURL = url.path( 1 ); // we want a trailing "/"
	    break;
	default:
	    debug("got %ld", ( *it ).m_uds);
	};
    }

    debugC("COMPLETE %s %d %d", myName.ascii(), myIsDir, mySize);
    //    myBaseURL = "";
    myName.replace( QRegExp("/$"), "" );
    myFilePath = myBaseURL + myName;
    myIsReadable = true;
}

KFileInfo::KFileInfo(const QFileInfo &e)
{
    myName = e.fileName();
    mySize = e.size();
    myBaseURL = e.dirPath(true);
    myDate= (e.lastModified()).toString();
    myOwner = e.owner();
    myGroup = e.group();
    myIsDir = e.isDir();
    myIsFile = e.isFile();
    myIsSymLink = e.isSymLink();
    myPermissions = 755;
    if ( myBaseURL.right(1) != "/" )
        myBaseURL += "/";
    myFilePath = myBaseURL + myName;
}

KFileInfo::KFileInfo(const QString& dir, const QString& name)
{
    myName = name;
    myBaseURL = dir;
    if ( myBaseURL.right(1) != "/" )
        myBaseURL += "/";

    if (!passwdMap) {
      passwdMap = new PasswdMapper;
      groupMap = new GroupMapper;
      readUserInfo();
    }

    struct stat buf;
    myIsSymLink = false;

    myFilePath = dir + myName;

    if (lstat(myFilePath.local8Bit(), &buf) == 0) {
	myIsDir = (buf.st_mode & S_IFDIR) != 0;
        // check if this is a symlink to a directory
	if (S_ISLNK(buf.st_mode)) {
	  myIsSymLink = true;
	  struct stat st;
	  if (stat(myFilePath.local8Bit(), &st) == 0)
	      myIsDir = S_ISDIR(st.st_mode) != 0;
	  else
	      myName = ""; // indicate, that the link is broken
	} else
	    myIsSymLink = false;
	myDate = dateTime(buf.st_mtime);
	mySize = buf.st_size;
	myIsFile = !myIsDir;
	
	/* // old, slow reading
	struct passwd *pw = getpwuid(buf.st_uid);
	struct group * ge = getgrgid( buf.st_gid );
	if (pw)
	    myOwner = pw->pw_name;
	else
	    myOwner = i18n("unknown");
	if (ge)
	    myGroup = ge->gr_name;
	else
	    myGroup = i18n("unknown");
	*/

	PasswdMapper::Iterator it = passwdMap->find(buf.st_uid);
	if (it != passwdMap->end())
	  myOwner = it.data();
	else
	  myOwner = i18n("unknown");
	GroupMapper::Iterator it2 = groupMap->find(buf.st_gid);
	if (it2 != groupMap->end())
	  myGroup = it2.data();
	else
	  myGroup = i18n("unknown");
	
	myPermissions = buf.st_mode;
	parsePermissions(myPermissions);
	
    } else {
	// default
	debug(QString("the file does not exist %1%2").arg(dir)
	      .arg(name).ascii());
	myName.insert(0, "?");
	myBaseURL.insert(0, "?");
	myFilePath.insert(0, "?");
	mySize = 0;
	myIsFile = false;
	myIsDir = false;
	myPermissions = 0;
	parsePermissions(myPermissions);
    }

}

KFileInfo::KFileInfo(const KFileInfo &i)
{
    *this = i;
}

// KFileInfo::~KFileInfo()
// {
//     debug("~KFileInfo");
// }

KFileInfo &KFileInfo::operator=(const KFileInfo &i)
{
    myName= i.myName;
    myBaseURL= i.myBaseURL;
    myAccess = i.myAccess;
    myDate= i.myDate;
    myOwner= i.myOwner;
    myGroup= i.myGroup;
    myIsDir = i.myIsDir;
    myIsFile = i.myIsFile;
    myIsSymLink = i.myIsSymLink;
    myPermissions= i.myPermissions;
    mySize= i.mySize;
    myIsReadable = i.myIsReadable;
    myFilePath = myBaseURL + myName;
    return *this;
}

bool KFileInfo::isReadable() const
{
    return (::access((myBaseURL + myName).ascii(), R_OK | (myIsDir ? X_OK : 0)) == 0) ;
}

void KFileInfo::parsePermissions(const char *perms)
{
    myPermissions = 0;
    char p[11] = {0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0,
		  0};

    strncpy(p, perms, sizeof(p));

    myIsDir = (bool)(p[0] == 'd');
    myIsSymLink = (bool)(p[0] == 'l');
    myIsFile = !myIsDir;

    if(p[1] == 'r')
      myPermissions |= QFileInfo::ReadUser;

    if(p[2] == 'w')
      myPermissions |= QFileInfo::WriteUser;

    if(p[3] == 'x')
      myPermissions |= QFileInfo::ExeUser;

    if(p[4] == 'r')
      myPermissions |= QFileInfo::ReadGroup;

    if(p[5] == 'w')
      myPermissions |= QFileInfo::WriteGroup;

    if(p[6] == 'x')
      myPermissions |= QFileInfo::ExeGroup;

    if(p[7] == 'r')
      myPermissions |= QFileInfo::ReadOther;

    if(p[8] == 'w')
      myPermissions |= QFileInfo::WriteOther;

    if(p[9] == 'x')
      myPermissions |= QFileInfo::ExeOther;
}

void KFileInfo::parsePermissions(uint perm)
{
    char p[] = "----------";

    if (myIsDir)
	p[0]='d';
    else if (myIsSymLink)
	p[0]='l';

    if (perm & QFileInfo::ReadUser)
	p[1]='r';
    if (perm & QFileInfo::WriteUser)
	p[2]='w';
    if (perm & QFileInfo::ExeUser)
	p[3]='x';

    if (perm & QFileInfo::ReadGroup)
	p[4]='r';
    if (perm & QFileInfo::WriteGroup)
	p[5]='w';
    if (perm & QFileInfo::ExeGroup)
	p[6]='x';

    if (perm & QFileInfo::ReadOther)
	p[7]='r';
    if (perm & QFileInfo::WriteOther)
	p[8]='w';
    if (perm & QFileInfo::ExeOther)
	p[9]='x';

    myAccess = p;
}

/* the following will go into KLocale after Beta4!!! */
static QString *months[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

QString KFileInfo::dateTime(time_t _time) {

    if (!months[0]) {
	months[ 0] = new QString(i18n("Jan"));
	months[ 1] = new QString(i18n("Feb"));
	months[ 2] = new QString(i18n("Mar"));
	months[ 3] = new QString(i18n("Apr"));
	months[ 4] = new QString(i18n("May"));
	months[ 5] = new QString(i18n("Jun"));
	months[ 6] = new QString(i18n("Jul"));
	months[ 7] = new QString(i18n("Aug"));
	months[ 8] = new QString(i18n("Sep"));
	months[ 9] = new QString(i18n("Oct"));
	months[10] = new QString(i18n("Nov"));
	months[11] = new QString(i18n("Dec"));
    }

    QDateTime t;
    time_t now = time(0);
    t.setTime_t(_time);

    QString sTime;
    if (_time > now || now - _time >= 365 * 24 * 60 * 60)
	sTime.sprintf(" %04d", t.date().year());
    else
	sTime.sprintf("%02d:%02d", t.time().hour(), t.time().minute());

    QString text = QString("%1 %2 %3").
      arg(months[t.date().month() - 1]->left(3), 3).
      arg(t.date().day(), 2).arg(sTime);

   /* hmm... supposed to be in KLocale after Beta4, huh? :D
    QDateTime t;
    t.setTime_t(_time);
    return KGlobal::locale()->formatDateTime( t );
  */
    return text;
}


void KFileInfo::readUserInfo()
{
  struct passwd *pass;
  while ( (pass = getpwent()) != 0L ) {
    passwdMap->insert(pass->pw_uid, qstrdup(pass->pw_name));
  }
  delete pass;
  endpwent();

  struct group *gr;
  while( (gr = getgrent()) != 0L ) {
    groupMap->insert(gr->gr_gid, qstrdup(gr->gr_name));
  }
  delete gr;
  endgrent();
}


void KFileInfo::cleanup()
{
  if ( passwdMap ) {
    PasswdMapper::Iterator it;
    for( it = passwdMap->begin(); it != passwdMap->end(); ++it )
      delete it.data();

    delete passwdMap;
    passwdMap = 0L;
  }

  if ( groupMap ) {
    GroupMapper::Iterator it2;
    for( it2 = groupMap->begin(); it2 != groupMap->end(); ++it2 )
      delete it2.data();

    delete groupMap;
    groupMap = 0L;
  }
}
