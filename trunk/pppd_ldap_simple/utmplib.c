/*******************************************************************
* utmplib.c - functions to process UTMP-like file "pppd_utmp"
*
* LDAP plugin for pppd. Performs PAP authentication and sets
* pppd parameters using LDAP as a backend.
*
* Copyright (c) Nordcomp LTD, Syktyvkar, Russian Federation
* Initial version written by Grigoriy Sitkarev <sitkarew@nordcomp.ru>
*
* This plugin may be distributed according to the terms of the GNU
* General Public License, version 2 or any later version.
*
********************************************************************/
/*
* 2004/05/30: initial version of utmplib procedures
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_BUF	1024

#include "ppp_utmp.h"
#include "pppd.h"


/*
* FUNCTION: off_t utmp_seek(int fd, char *line)
* PURPOSE:	searches file associated with fd until line
*			line entry is no found or returns error
* ARGUMENTS: fd		- data file descriptor
			 line	- name of the terminal line (without "/dev/")
*
* RETURNS:	-1 	in case of error
*			 off_t record offset position in case of success
*/

off_t
utmp_seek(int fd, char *line) {

	struct ppp_utmp utmp;

	if (lseek(fd, SEEK_SET, 0) != -1){
		while (read_n(fd, &utmp, sizeof(struct ppp_utmp))) {
			if (strncmp(utmp.line, line, MAXLINELEN-1) == 0)
			return ((lseek(fd, 0, SEEK_CUR)) - sizeof(struct ppp_utmp));
		}
	}
	return -1;
}

/*
* FUNCTION: int utmp_count(int fd, char *login)
* PURPOSE:	counts ACTIVE entries for login name "login"
* ARGUMENTS: fd		- data file descriptor
*			 login	- login name to count entries
*
*/

int
utmp_count(int fd, char *login) {

	int count = 0;
	struct ppp_utmp utmp;

	if (lseek(fd, SEEK_SET, 0) != -1){
		while (read_n(fd, &utmp, sizeof(struct ppp_utmp))) {
			if(strncasecmp(utmp.login, login,
			MAXNAMELEN-1) == 0 &&	(utmp.state == ACTIVE))
			++count;
		}
		return count;
	}
	return -1;
}



/*
* FUNCTION: write_n(int, void *, size_t)
* PURPOSE: Writes exactly size_t bytes or returns error
* ARGUMENTS: standard write() arguments
*
* RETURNS:	-1 in case of error
*			 1 in case of success
*/

int write_n(int fd, void *buf, size_t size) {

	size_t rc; /* keeps results */

	while (1) {
		rc = write(fd, buf, size);
		if (rc == -1) {
			switch (errno) {
				case EINTR:
						continue;
				case EIO:
						fprintf(stderr,"Hardware failure: %s\n",strerror(errno));
						return -1;
				case ENOSPC:
						fprintf(stderr,"Device if full: %s\n",strerror(errno));
						return -1;
				default:
						fprintf(stderr,"Write failed: %s\n",strerror(errno));
						return -1;
			}
		}

		if (rc < size) continue;
		return 1;
	}
}

/*
* FUNCTION: read_n(int, void *, size_t)
* PURPOSE: reads exactly size_t bytes or returns error
* ARGUMENTS: standard write() arguments
*
* RETURNS:	-1 in case of error
*			 1 in case of success
*			 0 in case of EOF (end of file)
*/

int read_n(int fd, void *buf, size_t size) {

	size_t rc; /* keeps results */

	while(1) {
		rc = read(fd, buf, size);
		if (rc == -1) {
			switch (rc) {
			case EINTR:
					continue;
			case EIO:
					fprintf(stderr,"Hardware failure: %s\n",strerror(errno));
					return -1;
			default:
					fprintf(stderr,"Read failed: %s\n",strerror(errno));
					return -1;
			}
		}

		if (rc == 0) return 0;

		if (rc < size) {
			size -= rc;
			buf = (char*)buf + rc;
		}

		return 1;
	}
}

