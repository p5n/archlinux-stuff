/* main.h
*
*  LDAP plugin for pppd
*
*/

#define MAX_BUF	1024
#define SEARCH_TIMEOUT 20
/* main.h - contains defenitions for pppd_ldap plugin */

/* Keeps interpreted data recieved from LDAP */

#include <chap-new.h>

struct ldap_data {
	int			maxconnect; /* maximum connect time in sec */
	int			maxoctets; /* maximum number of octets, reserved */
	int			maxoctets_dir; /* limit direction, reserved */
	int			idle_time_limit; /* connection idle timeout in sec */
	int			mru; /* Maximum recieve unit, reserved  */
	u_int32_t	addr; /* peer's IP address in network format */
	bool		access_ok; /* 1 if username/password pair correct */
	bool		address_set; /* 1 if addr contains value */
	bool		rebind; /* set to 1, reserved */
};

/* plugin main functions */

static int
ldap_pap_auth(char *user, char *password, char **msgp,
	struct wordlist **paddrs, struct wordlist **popts);

static void
ldap_ip_choose(u_int32_t *addrp);

static int
ldap_address_allowed(u_int32_t addr);

static int
ldap_pap_check(void);

static int
ldap_setoptions(LDAP *ld, LDAPMessage *mesg,
	struct ldap_data *ldap_data);

static void
ldap_ip_down(void *opaque, int arg);

static void
ldap_ip_up(void *opaque, int arg);

static int
ldap_activate_utmp(struct ldap_data *ldap_data,
	char *devnam, char *ppp_devname, char *user);

static int
ldap_deactivate_utmp(char *devnam);

static int ldap_chap_verify(char *name, char *ourname, int id,
		 struct chap_digest_type *digest,
		 unsigned char *challenge, unsigned char *response,
		 char *message, int message_space);
