/* ppp_toggle.c - toggle ACTIVE/IDLE PPP session tool
*
*  Deactivate/Activate session in UTMP-like file "pppd_utmp"
*
* Copyright (c) Nordcomp LTD, Syktyvkar, Russian Federation
* Initial version written by Grigoriy Sitkarev <sitkarew@nordcomp.ru>
*
* This plugin may be distributed according to the terms of the GNU
* General Public License, version 2 or any later version.
*
* TODO: Lot's of things... Sorting, displaying, etc.
*
*********************************************************************/
/*
*
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF		256

#include "ppp_utmp.h"

#define DEACTIVATE		0
#define ACTIVATE		1

static char rcsid[] = "$Id: ppp_toggle.c, v 0.12a 2004/05/31 21:34:25 sitkarev Exp$";
static char usage[] = "Usage: ppp_toggle [options] LINE\n\tOptions: "
					  "-d\tdeactivate\n"
					  "\t\t -a\tactivate\n";

int main(argc,argv)
int argc; char *argv[];
{

	int rc; /* return values */
	int fd; /* UTMP file descriptor */
	int action;
	struct ppp_utmp entry;
	char addr[BUF];
	char time[BUF];

	if (argc < 2) {
	fprintf(stdout,"%s\n", usage);
	return -1;
	}

	if(strncmp(argv[1],"-d",2) == 0) action = DEACTIVATE;
	if(strncmp(argv[1],"-a",2) == 0) action = ACTIVATE;

	if ((fd = open(UTMP,O_RDWR,0600)) == -1)
	{
		fprintf(stderr,"%s: %s\n",argv[0], strerror(errno));
		return -1;
	}

	if (lockf(fd, F_LOCK, 0) == -1) {
		fprintf(stderr,"Can not lock utmp file: %s\n",strerror(errno));
		return -1;
	};

		while(read_n(fd, &entry, sizeof(struct ppp_utmp))) {
			if (strncmp(argv[2], entry.line, MAXLINELEN-1) == 0) {

					fprintf(stderr,"FOUND: %s\n",entry.line);
					switch (action) {

						case DEACTIVATE:
								entry.state = IDLE;
								break;

						case ACTIVATE:
								entry.state = ACTIVE;
								break;

						default:
								fprintf(stdout,"%s\n", usage);
								return -1;
					}

					lseek(fd, -sizeof(struct ppp_utmp), SEEK_CUR);
					if (write_n(fd, &entry, sizeof(struct ppp_utmp)) == -1) {
						fprintf(stderr,"Can not change entry state: %s\n",
								strerror(errno));
						return -1;
					}
			}
		}

	lseek(fd, 0, SEEK_SET);
	lockf(fd, F_ULOCK, 0);

return 1;
}

