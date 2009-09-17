/* ppp_list.c - active PPP session listing tool
*
*  Lists records and it's values from UTMP-like file "pppd_utmp"
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
* 2004/05/31: FOOLISH mistake fixed!!! Field width extended to
* display long line paths correctly.
* v 0.12 --> v 0.12a
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

#define LINEWIDTH		20
#define	LOGINWIDTH		10
#define IFACEWIDTH		6
#define ADDRESSWIDTH	15
#define	TIMEWIDTH		10

static char rcsid[] = "$Id: ppp_list.c, v 0.12a 2004/05/31 21:34:25 sitkarev Exp$";

int main(argc,argv)
int argc; char *argv[];
{

	int rc; /* return values */
	int fd; /* UTMP file descriptor */
	struct ppp_utmp entry;
	char addr[BUF];
	char time[BUF];

	if ((fd = open(UTMP,O_RDONLY,0600)) == -1)
	{
		fprintf(stderr,"%s: %s\n",argv[0], strerror(errno));
		return -1;
	}

	/* Here goes header */

	fprintf(stdout,"%-*s %-*s %-*s %-*s %*s\n",
			LOGINWIDTH, "LOGIN",
			LINEWIDTH,	"LINE",
			IFACEWIDTH,	"IFACE",
			ADDRESSWIDTH, "ADDRESS",
			TIMEWIDTH,	"TIME");

	/* Display entry values if line is active */

	while(read_n(fd, &entry, sizeof(struct ppp_utmp)))
	{
		if (entry.state == ACTIVE) {
			strftime(time,BUF,"%b %d %T",localtime(&entry.time));
			fprintf(stdout,"%-*s \%-*s %-*s %-*s %-*s\n",
				LOGINWIDTH,	entry.login,
				LINEWIDTH,	entry.line,
				IFACEWIDTH,	entry.ifname,
				ADDRESSWIDTH, inet_ntop(AF_INET, &entry.ip_address, addr, BUF),
				TIMEWIDTH,	time
			);
		}
	}

return 1;
}


