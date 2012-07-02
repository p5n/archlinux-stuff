#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include "ulogd/ulogd.h"
#include "ulogd/conffile.h"

#define DEBUGP(x, args...)	fprintf(stderr, x, ## args)

static FILE *of = NULL;

struct intr_id
{
	char* name;
	unsigned int id;
};

#define INTR_IDS 	35
static struct intr_id intr_ids[INTR_IDS] = {
	{ "oob.time.sec", 0 },
	{ "oob.prefix", 0 },
	{ "oob.in", 0 },
	{ "oob.out", 0 },
	{ "raw.mac", 0 },
	{ "ip.saddr", 0 },
	{ "ip.daddr", 0 },
	{ "ip.totlen", 0 },
	{ "ip.tos", 0 },
	{ "ip.ttl", 0 },
	{ "ip.id", 0 }, /* 10 */
	{ "ip.fragoff", 0 },
	{ "ip.protocol", 0 },
	{ "tcp.sport", 0 },
	{ "tcp.dport", 0 },
	{ "tcp.seq", 0 },
	{ "tcp.ackseq", 0 },
	{ "tcp.window", 0 },
	{ "tcp.urg", 0 },
	{ "tcp.ack", 0 },
	{ "tcp.psh", 0 }, /* 20 */
	{ "tcp.rst", 0 },
	{ "tcp.syn", 0 },
	{ "tcp.fin", 0 },
	{ "tcp.urgp", 0 },
	{ "udp.sport", 0 },
	{ "udp.dport", 0 },
	{ "udp.len", 0 },
	{ "icmp.type", 0 },
	{ "icmp.code", 0 },
	{ "icmp.echoid", 0 }, /* 30 */
	{ "icmp.echoseq", 0 },
	{ "icmp.gateway", 0 },
	{ "icmp.fragmtu", 0 },
	{ "ahesp.spi", 0 },
};

/* get all key id's for the keys we are intrested in */
static int get_ids(void)
{
  int i;
  struct intr_id *cur_id;

  for (i = 0; i < INTR_IDS; i++)
  {
    cur_id = &intr_ids[i];
    cur_id->id = keyh_getid(cur_id->name);
    if (!cur_id->id)
    {
      ulogd_log(ULOGD_ERROR,
                "Cannot resolve keyhash id for %s\n",
                cur_id->name);
      return 1;
    }
  }
  return 0;
}

#define GET_VALUE(x)	ulogd_keyh[intr_ids[x].id].interp->result[ulogd_keyh[intr_ids[x].id].offset].value

static struct config_entry sq_ce = {
	.key = "file",
	.type = CONFIG_TYPE_STRING,
	.options = CONFIG_OPT_NONE,
	.u = { .string = "/var/log/sq.log" }
};

static int _sq_output(ulog_iret_t *result)
{
  char src[16];
  char dst[16];
  int len = GET_VALUE(7).ui16;
  int proto = GET_VALUE(12).ui8;
  int spt = -1;
  int dpt = -1;
  char *prefix = (char *) GET_VALUE(1).ptr;

  snprintf(src, 16, "%03hhu.%03hhu.%03hhu.%03hhu",
           (GET_VALUE(5).ui32 >> 24) & 255,
           (GET_VALUE(5).ui32 >> 16) & 255,
           (GET_VALUE(5).ui32 >>  8) & 255,
           (GET_VALUE(5).ui32 >>  0) & 255);
  snprintf(dst, 16, "%03hhu.%03hhu.%03hhu.%03hhu",
           (GET_VALUE(6).ui32 >> 24) & 255,
           (GET_VALUE(6).ui32 >> 16) & 255,
           (GET_VALUE(6).ui32 >>  8) & 255,
           (GET_VALUE(6).ui32 >>  0) & 255);

  switch (proto)
  {
    case IPPROTO_TCP:
      spt = GET_VALUE(13).ui16;
      dpt = GET_VALUE(14).ui16;
      break;

    case IPPROTO_UDP:
      spt = GET_VALUE(25).ui16;
      dpt = GET_VALUE(26).ui16;
      break;

    default:
      break;
  }

  fprintf(of, "%s %s %s %d %d %d %d\n",
          prefix,
          src, dst,
          len, proto,
          spt, dpt);

  return 0;
}

static void sighup_handler_print(int signal)
{
  switch (signal)
  {
    case SIGHUP:
      if(of != NULL)
      {
        fclose(of);
      }

      config_parse_file("SQ", &sq_ce);

      of = fopen(sq_ce.u.string, "a");
      if (!of)
      {
        ulogd_log(ULOGD_FATAL, "can't open PKTLOG: %s\n",
                  strerror(errno));
        exit(2);
      }

      break;
    default:
      break;
  }
}

static int sq_init(void)
{
  /* parse config */
  config_parse_file("SQ", &sq_ce);

  if (get_ids())
  {
    ulogd_log(ULOGD_ERROR, "can't resolve all keyhash id's\n");
  }

  of = fopen(sq_ce.u.string, "a");
  if (!of)
  {
		ulogd_log(ULOGD_FATAL, "can't open PKTLOG: %s\n",
              strerror(errno));
		exit(2);
	}
}

static void sq_fini(void)
{
  if(of != NULL)
  {
    fclose(of);
  }
}

static ulog_output_t sq_op = {
	.name = "sq",
	.output = &_sq_output,
	.signal = sighup_handler_print,
	.init = &sq_init,
	.fini = &sq_fini,
};

void _init(void)
{
  /* register output callback */
  register_output(&sq_op);
}
