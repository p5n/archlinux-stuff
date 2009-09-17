/* ppp_utmp.h
*
* Data definitions
*
*/

#include <pppd.h>

/* line state integers */

#define	IDLE		0
#define	ACTIVE		1
#define UTMP		"/var/run/pppd_utmp"

#define MAXLINELEN 32
#define MAXIFLEN	8
#define MAXANUMLEN 24

struct ppp_utmp {

 char		login[MAXNAMELEN];/* login name */
 char		line[MAXLINELEN]; /* terminal line name */
 char		ifname[MAXIFLEN]; /* ppp interface name */
 char		cpn[MAXANUMLEN];  /* A-side number */
 u_int32_t	ip_address; /* peer's IP address */
 time_t		time;		/* last changed */
 int		state;		/* line state */

};

/* Function definitions */

int
write_n(int fd, void *buf, size_t size);

int
read_n(int fd, void *buf, size_t size);

off_t
utmp_seek(int fd, char *line);

int
utmp_count(int fd, char *login);
