    /*

    Copyright (C) 2000 Stefan Westerfeld
                       stefan@space.twc.de

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    Permission is also granted to link this program with the Qt
    library, treating Qt like a library that normally accompanies the
    operating system kernel, whether or not that is in fact the case.

    */

#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>


#include "md5.h"
#include "md5auth.h"

struct random_info {
	struct timeval tv;
	int pid;
	struct utsname un;
	char dev_urandom[16];
	char seed[MD5_COOKIE_LEN+1];
	int number;
};

static char md5_seed[MD5_COOKIE_LEN+1];
static char md5_cookie[MD5_COOKIE_LEN+1];
static int md5_init = 0;
static int md5_random_cookie_number = 0;

static char *md5_to_ascii_overwrite(char *md5)
{
	char outascii[33], *result;
	int i;

	// convert to readable text
	for(i=0;i<16;i++) {
		sprintf(&outascii[i*2],"%02x",md5[i] & 0xff);
	}
	result = strdup(outascii);

	// try to zero out traces
	bzero(md5,16);
	bzero(outascii,33);

	// well, here it is
	return result;
}

char *md5_auth_mangle(const char *cookie)
{
	char mangle[MD5_COOKIE_LEN*2+1];
	char out[MD5_COOKIE_LEN];
	strcpy(mangle,md5_cookie);
	strcat(mangle,cookie);

	MD5sum((unsigned char *)mangle,MD5_COOKIE_LEN*2,out);
	bzero(mangle,MD5_COOKIE_LEN*2);

	return md5_to_ascii_overwrite(out);
}

char *md5_auth_mkcookie()
{
	struct random_info r;
	char out[16];
	int rndfd;

	bzero(&r,sizeof(struct random_info));

	// collect some "random" system information
	gettimeofday(&r.tv,0);
	r.pid = getpid();
	uname(&r.un);

	// linux random device ; if that works, we have good random anyway, the
	// above are just helpers for the case that it doesn't
	rndfd = open("/dev/urandom",O_RDONLY);
	if(rndfd != -1)
	{
		read(rndfd,r.dev_urandom,16);
		close(rndfd);
	}

	// ensure that two cookies that are  requested very shortly after each
	// other (so that it looks like "at the same time") won't be the same
	r.number = ++md5_random_cookie_number;

	// this is some seed from a file which is updated sometimes with a
	// new "md5_auth_mkcookie()" after initialization
	strncmp(r.seed,md5_seed,MD5_COOKIE_LEN);

	// build hash value of all information
	MD5sum((unsigned char *)&r,sizeof(struct random_info),out);

	// zero out traces and return
	bzero(&r,sizeof(struct random_info));
	return md5_to_ascii_overwrite(out);
}

const char *md5_auth_cookie()
{
	assert(md5_init);
	return md5_cookie;
}

static int md5_load_cookie(const char *filename, char *cookie)
{
	int fd = open(filename,O_RDONLY);
	int i;

	if(fd != -1) {
		struct stat st;
		for(i=0;i<5;i++) {
			fstat(fd,&st);
			if(st.st_size == MD5_COOKIE_LEN) {
				lseek(fd, 0, SEEK_SET); 
				if(read(fd,cookie,MD5_COOKIE_LEN) == MD5_COOKIE_LEN)
				{
					cookie[MD5_COOKIE_LEN] = 0;
					close(fd);
					return 1;
				}
			}
			fprintf(stderr,"mcop warning: "
					"authority file has wrong size (just being written?)\n");
			sleep(1);
		}
	}
	return 0;
}

void md5_auth_init(const char *authname, const char *seedname)
{
	struct stat st;
	int fd;
	char *cookie;

	// don't care if it works - no harm is being done if it doesn't
	md5_load_cookie(seedname,md5_seed);

	fd = open(authname,O_CREAT|O_EXCL|O_WRONLY,S_IRUSR|S_IWUSR);
	if(fd != -1) {
		cookie = md5_auth_mkcookie();
		write(fd,cookie,strlen(cookie));
		bzero(cookie,strlen(cookie));
		free(cookie);
		close(fd);
	}

	if(!md5_load_cookie(authname,md5_cookie))
	{
		fprintf(stderr,
			"mcop error: authority file is corrupt (remove %s to fix that)\n",
			authname);

		exit(1);
	}

	if(seedname)
	{
		/*
		 * maxage ensures that not everybody will try to update the seed
		 * at the same time, while it will take at most 5 hours between
		 * updates (if there are any initialization calls)
		 */
		int maxage = 300 + (getpid() & 0xfff)*4;
		int lstat_result = lstat(seedname,&st);
		if(lstat_result != 0 || (time(0) - st.st_mtime) > maxage)
		{
			fd = open(seedname,O_TRUNC|O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR);
			if(fd != -1) {
				cookie = md5_auth_mkcookie();
				write(fd,cookie,strlen(cookie));
				bzero(cookie,strlen(cookie));
				free(cookie);
				close(fd);
			}
		}
	}
}
