/* This file is part of the KDE libraries
   Copyright (C) 2000 David Faure <faure@kde.org>

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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <assert.h>

#include <qcstring.h>
#include <qdir.h>

#include "ktar.h"

template class QDict<KTarEntry>;

////////////////////////////////////////////////////////////////////////
/////////////////////////// KTarBase ///////////////////////////////////
////////////////////////////////////////////////////////////////////////

KTarBase::KTarBase()
{
  m_open = false;
  m_dir = 0;
}

KTarBase::~KTarBase()
{
  if ( m_open )
    close();
  delete m_dir;
}

bool KTarBase::open( int mode )
{
  if ( m_open )
    close();

  m_mode = mode;
  m_open = true;
  m_dirList.clear();

  if ( mode == IO_ReadOnly )
  {
/* What is this good for ? (David)
   Isn't m_dir internal anyway ?

    // Find infos about the tar file itself
    struct stat buf;
    stat( m_filename, &buf );

    struct passwd* pw =  getpwuid( buf.st_uid );
    struct group* grp = getgrgid( buf.st_gid );

    m_dir = new KTarDirectory( this, "/", (int)buf.st_mode, (int)buf.st_mtime, pw->pw_name , grp->gr_name );
*/
    m_dir = new KTarDirectory( this, "/", 0, 0, "", "", "" );

    // read dir infos
    char buffer[ 0x200 ];
    bool ende = false;
    do
    {
      // Read header
      int n = read( buffer, 0x200 );
      if ( n == 0x200 && buffer[0] != 0 )
      {
	QString name( buffer );

        // If filename is longer than 100 (0x64) chars, then tar uses ././@LongLink (David)
        if ( name == "././@LongLink" )
        {
          // in this case, here's what happens (according to od -cx !)
          // 1) the filename is stored in the next 512b buffer
          n = read( buffer, 0x200 );
          if ( n == 0x200 && buffer[0] != 0 )
          {
            name = buffer;
            // 2) read yet another 512b buffer, for permissions, time, size ...
            n = read( buffer, 0x200 );
            if (!( n == 0x200 && buffer[0] != 0 ))
              break;
          }
          else
            break;
        }

	bool isdir = false;
	QString nm;

	if ( name.right(1) == "/" )
	{
	  isdir = true;
	  name = name.left( name.length() - 1 );
	}

	int pos = name.findRev( '/' );
	if ( pos == -1 )
	  nm = name;
	else
	  nm = name.mid( pos + 1 );

	// read access
	buffer[ 0x6a ] = 0;
	char *dummy;
	const char* p = buffer + 0x64;
	while( *p == ' ' ) ++p;
	int access = (int)strtol( p, &dummy, 8 );

	// read user and group
	QString user( buffer + 0x109 );
	QString group( buffer + 0x129 );
	QString symlink(buffer + 0x9d );

	// read time
	buffer[ 0x93 ] = 0;
	p = buffer + 0x88;
	while( *p == ' ' ) ++p;
	int time = (int)strtol( p, &dummy, 8 );

	KTarEntry* e;
	if ( isdir )
	  e = new KTarDirectory( this, nm, access, time, user, group, symlink );
	else
	{
	  // read size
	  buffer[ 0x87 ] = 0;
	  char *dummy;
	  const char* p = buffer + 0x7c;
	  while( *p == ' ' ) ++p;
	  int size = (int)strtol( p, &dummy, 8 );

	  int rest = size % 0x200;

	  // Read content
	  QByteArray arr( size );
          if ( size )
          {
            assert( arr.data() );
            int n = read( arr.data(), size );
            if ( n != size )
              arr.resize( n );

            // Skip align bytes
            if ( rest )
            {
              //gzseek( m_f, 0x200 - rest, SEEK_CUR );
              QByteArray dummy( 0x200 - rest );
              assert( dummy.data() );
              read( dummy.data(), 0x200 - rest );
            }
          }

	  e = new KTarFile( this, nm, access, time, user, group, symlink,
			    position(), size, arr );
	}

	if ( pos == -1 )
	  m_dir->addEntry( e );
	else
	{
          // In some tar files we can find dir/./file => call cleanDirPath
          QString path = QDir::cleanDirPath( name.left( pos ) );
          // Ensure container directory exists, create otherwise
          KTarDirectory * d = findOrCreate( path );
	  d->addEntry( e );
	}
      }
      else
	ende = true;
    } while( !ende );
  }

  return true;
}


KTarDirectory * KTarBase::findOrCreate( const QString & path )
{
  if ( path == "" || path == "/" ) // root dir => found
    return m_dir;
  // Important note : for tar files containing absolute paths
  // (i.e. beginning with "/"), this means the leading "/" will
  // be removed (no KDirectory for it), which is exactly the way
  // the "tar" program works (though it displays a warning about it)
  // See also KTarDirectory::entry().

  // Already created ? => found
  KTarEntry* d = m_dir->entry( path );
  if ( d && d->isDirectory() )
    return (KTarDirectory *) d;

  // Otherwise go up and try again
  int pos = path.findRev( '/' );
  KTarDirectory * parent;
  QString dirname;
  if ( pos == -1 ) // no more slash => create in root dir
  {
    parent =  m_dir;
    dirname = path;
  }
  else
  {
    QString left = path.left( pos );
    dirname = path.mid( pos + 1 );
    parent = findOrCreate( left ); // recursive call... until we find an existing dir.
  }

  //debug("found parent %s adding %s to ensure %s", parent->name().latin1(), dirname.latin1(), path.latin1());
  // Found -> add the missing piece
  KTarDirectory * e = new KTarDirectory( this, dirname, m_dir->permissions(),
					 m_dir->date(), m_dir->user(),
					 m_dir->group(), m_dir->symlink() );
  parent->addEntry( e );
  return e; // now a directory to <path> exists
}

void KTarBase::close()
{
  if ( !m_open )
    return;

  m_dirList.clear();

  delete m_dir;
  m_dir = 0;
  m_open = false;
}

const KTarDirectory* KTarBase::directory() const
{
  if ( !isOpened() )
    qWarning( "KTarBase::directory: You must open the tar file before reading it\n");

  return m_dir;
}


void KTarBase::writeDir( const QString& name, const QString& user, const QString& group )
{
  if ( !isOpened() )
  {
    qWarning( "KTarBase::writeDir: You must open the tar file before writing to it\n");
    return;
  }

  if ( m_mode != IO_WriteOnly )
  {
    qWarning( "KTarBase::writeDir: You must open the tar file for writing\n");
    return;
  }

  // In some tar files we can find dir/./ => call cleanDirPath
  QString dirName ( QDir::cleanDirPath( name ) );

  // Need trailing '/'
  if ( dirName.right(1) != "/" )
    dirName += "/";

  char buffer[ 0x201 ];
  memset( buffer, 0, 0x200 );

  // If more than 100 chars, we need to use the LongLink trick
  if ( dirName.length() > 99 )
  {
    strcpy( buffer, "././@LongLink" );
    fillBuffer( buffer, "     0", dirName.length()+1, 'L', user.ascii(), group.ascii() );
    write( buffer, 0x200 );
    memset( buffer, 0, 0x200 );
    strcpy( buffer, dirName.ascii() );
    // write long name
    write( buffer, 0x200 );
    // not even needed to reclear the buffer, tar doesn't do it
  }
  else
    // Write name
    strcpy( buffer, dirName.ascii() );

  fillBuffer( buffer, " 40755", 0, 0x35, user.ascii(), group.ascii());

  // Write header
  write( buffer, 0x200 );

  m_dirList.append( dirName ); // contains trailing slash
}

void KTarBase::writeFile( const QString& name, const QString& user, const QString& group, uint size, const char* data )
{
  if ( !isOpened() )
  {
    qWarning( "KTarBase::writeFile: You must open the tar file before writing to it\n");
    return;
  }

  if ( m_mode != IO_WriteOnly )
  {
    qWarning( "KTarBase::writeFile: You must open the tar file for writing\n");
    return;
  }

  // In some tar files we can find dir/./file => call cleanDirPath
  QString fileName ( QDir::cleanDirPath( name ) );

  /*
  // Create toplevel dirs
  // Commented out by David since it's not necessary, and if anybody thinks it is,
  // he needs to implement a findOrCreate equivalent in writeDir.
  // But as KTar and the "tar" program both handle tar files without
  // dir entries, there's really no need for that
  QString tmp ( fileName );
  int i = tmp.findRev( '/' );
  if ( i != -1 )
  {
    QString d = tmp.left( i + 1 ); // contains trailing slash
    if ( !m_dirList.contains( d ) )
    {
      tmp = tmp.mid( i + 1 );
      writeDir( d, user, group ); // WARNING : this one doesn't create its toplevel dirs
    }
  }
  */

  char buffer[ 0x201 ];
  memset( buffer, 0, 0x200 );

  // If more than 100 chars, we need to use the LongLink trick
  if ( fileName.length() > 99 )
  {
    strcpy( buffer, "././@LongLink" );
    fillBuffer( buffer, "     0", fileName.length()+1, 'L', user.ascii(), group.ascii() );
    write( buffer, 0x200 );

    memset( buffer, 0, 0x200 );
    strcpy( buffer, fileName.ascii() );
    // write long name
    write( buffer, 0x200 );
    // not even needed to reclear the buffer, tar doesn't do it
  }
  else
    // Write name
    strcpy( buffer, fileName.ascii() );

  fillBuffer( buffer, "100644", size, 0x30, user.ascii(), group.ascii() );

  // Write header
  write( buffer, 0x200 );

  // Write data
  write( data, size );

  // Write alignment
  int rest = size % 0x200;
  if ( rest )
  {
    for( uint i = 0; i < 0x200; ++i )
      buffer[i] = 0;
    write( buffer, 0x200 - rest );
  }
}

/*** Some help from the tar sources
struct posix_header
{                               byte offset
  char name[100];               *   0 *     0x0
  char mode[8];                 * 100 *     0x64
  char uid[8];                  * 108 *     0x6c
  char gid[8];                  * 116 *     0x74
  char size[12];                * 124 *     0x7c
  char mtime[12];               * 136 *     0x88
  char chksum[8];               * 148 *     0x94
  char typeflag;                * 156 *     0x9c
  char linkname[100];           * 157 *     0x9d
  char magic[6];                * 257 *     0x101
  char version[2];              * 263 *     0x107
  char uname[32];               * 265 *     0x109
  char gname[32];               * 297 *     0x129
  char devmajor[8];             * 329 *     0x149
  char devminor[8];             * 337 *     ...
  char prefix[155];             * 345 *
                                * 500 *
};
*/

void KTarBase::fillBuffer( char * buffer,
    const char * mode, int size, char typeflag, const char * uname, const char * gname )
{
  // mode (as in stat())
  assert( strlen(mode) == 6 );
  strcpy( buffer+0x64, mode );
  buffer[ 0x6a ] = ' ';
  buffer[ 0x6b ] = '\0';

  // dummy uid
  strcpy( buffer + 0x6c, "   765 ");
  // dummy gid
  strcpy( buffer + 0x74, "   144 ");

  // size
  QString s;
  s.setNum( size, 8 );
  s = s.rightJustify( 11, ' ' );
  strcpy( buffer + 0x7c, s.latin1() );
  buffer[ 0x87 ] = ' '; // space-terminate (no null after)

  // Dummy time
  s.setNum( time( 0 ), 8 );
  s = s.rightJustify( 11, ' ' );
  strcpy( buffer + 0x88, s.latin1() );
  buffer[ 0x93 ] = ' '; // space-terminate (no null after)

  // spaces, replaced by the check sum later
  buffer[ 0x94 ] = 0x20;
  buffer[ 0x95 ] = 0x20;
  buffer[ 0x96 ] = 0x20;
  buffer[ 0x97 ] = 0x20;
  buffer[ 0x98 ] = 0x20;
  buffer[ 0x99 ] = 0x20;

  /* From the tar sources :
     Fill in the checksum field.  It's formatted differently from the
     other fields: it has [6] digits, a null, then a space -- rather than
     digits, a space, then a null. */

  buffer[ 0x9a ] = '\0';
  buffer[ 0x9b ] = ' ';

  // type flag (dir, file, link)
  buffer[ 0x9c ] = typeflag;

 // magic + version
  strcpy( buffer + 0x101, "ustar");
  strcpy( buffer + 0x107, "00" );

  // user
  strcpy( buffer + 0x109, uname );
  // group
  strcpy( buffer + 0x129, gname );

  // Header check sum
  int check = 32;
  for( uint j = 0; j < 0x200; ++j )
    check += buffer[j];
  s.setNum( check, 8 );
  s = s.rightJustify( 7, ' ' );
  strcpy( buffer + 0x94, s.latin1() );
}

////////////////////////////////////////////////////////////////////////
/////////////////////////// KTarGz ///////////////////////////////////
////////////////////////////////////////////////////////////////////////


KTarGz::KTarGz( const QString& filename )
{
  m_filename = filename;
}

KTarGz::~KTarGz()
{
}


bool KTarGz::open( int mode )
{
  const char* m;
  if ( mode == IO_ReadOnly )
    m = "rb";
  else if ( mode == IO_WriteOnly )
    m = "wb";
  else
  {
    qWarning("KTarBase::open: You can only pass IO_ReadOnly or IO_WriteOnly as mode\n");
    return false;
  }

  m_f = gzopen( m_filename.ascii(), m );
  if ( !m_f )
    return false;

  return KTarBase::open( mode );
}

int KTarGz::read( char * buffer, int len )
{
  return gzread( m_f, buffer, len );
}

void KTarGz::write( const char * buffer, int len )
{
  gzwrite( m_f, (char *)buffer, len );
}

int KTarGz::position()
{
  return (int)gztell( m_f );
}

void KTarGz::close()
{
  KTarBase::close();
  gzclose( m_f );
}


////////////////////////////////////////////////////////////////////////
/////////////////////////// KTarData ///////////////////////////////////
////////////////////////////////////////////////////////////////////////


KTarData::KTarData( QDataStream * str )
{
  m_str = str;
}

KTarData::~KTarData()
{
}

bool KTarData::open( int mode )
{
  return KTarBase::open( mode );
}

int KTarData::read( char * buffer, int len )
{
  return m_str->device()->readBlock( buffer, len );
}

void KTarData::write( const char * buffer, int len )
{
  m_str->device()->writeBlock( buffer, len );
}

int KTarData::position()
{
  return m_str->device()->at();
}

////////////////////////////////////////////////////////////////////////
/////////////////////////// KTarEntry //////////////////////////////////
////////////////////////////////////////////////////////////////////////


KTarEntry::KTarEntry( KTarBase* t, const QString& name, int access, int date,
		      const QString& user, const QString& group, const
		      QString& symlink)
{
  m_name = name;
  m_access = access;
  m_date = date;
  m_user = user;
  m_group = group;
  m_symlink = symlink;
  m_tar = t;

}

QDateTime KTarEntry::datetime() const
{
  QDateTime d;
  d.setTime_t( m_date );
  return d;
}

////////////////////////////////////////////////////////////////////////
/////////////////////////// KTarFile ///////////////////////////////////
////////////////////////////////////////////////////////////////////////

KTarFile::KTarFile( KTarBase* t, const QString& name, int access, int date,
		    const QString& user, const QString& group,
		    const QString & symlink,
		    int pos, int size, const QByteArray& data )
  : KTarEntry( t, name, access, date, user, group, symlink ), m_data( data )
{
  m_pos = pos;
  m_size = size;
}

int KTarFile::position() const
{
  return m_pos;
}

int KTarFile::size() const
{
  return m_size;
}

QByteArray KTarFile::data() const
{
  // return ((KTarFile*)this)->tar()->data( m_pos, m_size );
  return m_data;
}

////////////////////////////////////////////////////////////////////////
//////////////////////// KTarDirectory /////////////////////////////////
////////////////////////////////////////////////////////////////////////


KTarDirectory::KTarDirectory( KTarBase* t, const QString& name, int access,
			      int date,
			      const QString& user, const QString& group,
			      const QString &symlink)
  : KTarEntry( t, name, access, date, user, group, symlink )
{
  m_entries.setAutoDelete( true );
}

QStringList KTarDirectory::entries() const
{
  QStringList l;

  QDictIterator<KTarEntry> it( m_entries );
  for( ; it.current(); ++it )
    l.append( it.currentKey() );

  return l;
}

KTarEntry* KTarDirectory::entry( QString name )
  // not "const QString & name" since we want a local copy
  // (to remove leading slash if any)
{
  int pos = name.find( '/' );
  if ( pos == 0 ) // ouch absolute path (see also KTarBase::findOrCreate)
  {
    name = name.mid( 1 ); // remove leading slash
    pos = name.find( '/' ); // look again
  }
  if ( pos != -1 )
  {
    QString left = name.left( pos );
    QString right = name.mid( pos + 1 );

    KTarEntry* e = m_entries[ left ];
    if ( !e || !e->isDirectory() )
      return 0;
    return ((KTarDirectory*)e)->entry( right );
  }

  return m_entries[ name ];
}

const KTarEntry* KTarDirectory::entry( QString name ) const
{
  return ((KTarDirectory*)this)->entry( name );
}

void KTarDirectory::addEntry( KTarEntry* entry )
{
  m_entries.insert( entry->name(), entry );
}
