/*  echat.h
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#ifndef _ECHAT_H
#define _ECHAT_H 1

#include <stdlib.h>
#include <stdio.h>

#ifdef  EN
# include "mesg_en.h"
#endif  /* EN */
#ifdef  RU
# include "mesg_ru.h"
#endif  /* RU */
#ifdef  TU
# include "mesg_tu.h"
#endif  /* TU */

#define NAME            "eChat"
#define VERSION         "0.02"
#define AUTHOR          "SeaD"
#define MAIL            "sead@mail.ru"

#define CNF_ECHATRC     "echatrc"

#define BUF_SIZE        1024
#define MSG_SIZE        1000    /* udp broadcast limit */
#define STR_SIZE        800     /* STR_SIZE <= MSG_SIZE */

#define PATH_SIZE       256     /* must be enough */
#define HIST_SIZE       50
#define SCRN_SIZE       500
#define LINE_SIZE       80      /* LINE_SIZE >= COLS - users_width */
#define PACK_SIZE       20
#define ADDR_SIZE       15      /* strlen("255.255.255.255") == 15 */

#define USERS_MAX       32      /* may be increased */
#define CHANNELS_MAX    8
#define ADDRS_MAX       8
#define NICK_MAXLEN     20      /* recomended NICK_MAXLEN <= 20 */
#define CHANNEL_MAXLEN  NICK_MAXLEN

#define LOCALHOST       "127.0.0.1" /* loopback interface address */
#define ALLHOSTS        "224.0.0.1" /* igmp all hosts address */
#define PORT            (unsigned short) 8167 /* Vypress Chat & quickChat */
#ifdef  TCP
# define CLIENTS_MAX    8       /* tcp connections */
#endif  /* TCP */
#define TYPE_VYPRESSXOR 'Z'     /* XOR packets */
#define CHR_XOR         (unsigned char) 0 /* XOR-byte */
#define TYPE_VYPRESS    'X'     /* Vypress Chat packets */
#define HEADER_LEN      10      /* packet header length */
#define PACKET_EOF      "\x1f\x7f" /* end of packet */
#define MSG_SPLIT       '\0'    /* strings splitter */

#define CHR_CHNL        '#'     /* channel first letter */
#define CHR_NICK        ':'     /* nick last letter */

#define MOD_NORMAL      0
#define MOD_DND         1
#define MOD_AWAY        2
#define MOD_OFF         3

#define COL_SYSTEM      10      /* system text */
#define COL_ERROR       11      /* error text */
#define COL_MESG        12      /* message text */
#define COL_MY          13      /* my text */
#define COL_CHAT        14      /* others text */

struct config_t {
    char file[PATH_SIZE+2];
    char nick[NICK_MAXLEN+2];
    int gender;
    char channel[STR_SIZE+2];
    int mode;
    char active;
    char answer_dnd[STR_SIZE+2];
    char answer_away[STR_SIZE+2];
    char answer_off[STR_SIZE+2];
    char me_action[STR_SIZE+2];
    char favorite[STR_SIZE+2];
    char ignore[STR_SIZE+2];
    char ban[STR_SIZE+2];
    char address[STR_SIZE+2];
    unsigned short port;
    char packet_bcast;
    char packet_route;
    char packet_qchat;
    unsigned char packet_xor;
    char head_nick[STR_SIZE+2];
    char head_top[STR_SIZE+2];
    char head_users[STR_SIZE+2];
    char ps[STR_SIZE+2];
    char style;
    char ascii;
    char status_str;
    char seconds;
    int users_width;
    int users_timeout;
    int scroll_step;
#ifdef  CHARSET
    char charset;
#endif  /* CHARSET */
    char beep_system;
    char beep_chat;
    char beep_join;
    char beep_mesg;
    char beep_beep;
    char my_only;
    char verbose;
    char server;
#ifdef  SHELL
    char shell[STR_SIZE+2];
#endif  /* SHELL */
    char log_main[PATH_SIZE+2];
    char log_mesg[PATH_SIZE+2];
    char log_priv[PATH_SIZE+2];
    int color_bkgr;
    int color_border;
    int color_head;
    int color_status;
    int color_system;
    int color_error;
    int color_mesg;
    int color_my;
    int color_chat;
};

struct user_t {
    char nick[NICK_MAXLEN+2];
    char gender;
    int mode;
    char active;
    char favorite;
    char ignore;
    char addr[ADDR_SIZE+1];
};

struct chnl_t {
    char name[CHANNEL_MAXLEN+2];
    char topic[STR_SIZE+2];
    int scrn;
    int scrn_top;
    char screen[SCRN_SIZE][LINE_SIZE+2];
    int scrn_color[SCRN_SIZE];
    char addr[ADDR_SIZE+1];
};

struct status_t {
    char nick[NICK_MAXLEN+2];
    char gender;
    char channel[CHANNEL_MAXLEN+2];
    int mode;
    char active;
    int addrs;
    char addr[ADDRS_MAX][ADDR_SIZE+1];
    char address[ADDR_SIZE+1];
    char favorite[USERS_MAX][NICK_MAXLEN+2];
    char ignore[USERS_MAX][NICK_MAXLEN+2];
    char ban[USERS_MAX][ADDR_SIZE+1];
    unsigned int users_timeout;
    int user_sort;
    int users;
    struct user_t *talk;
    struct user_t user[USERS_MAX];
    int strn;
    int strn_end;
    int strn_cur;
    int strn_width;
    int scrn_width;
    int channels;
    struct chnl_t *room;
    struct chnl_t *room_str;
    struct chnl_t chnl[CHANNELS_MAX+1];
    int hist;
    char history[HIST_SIZE][STR_SIZE+2];
    char packet_buf[PACK_SIZE][MSG_SIZE+2];
#ifdef  TCP
    int clients;
#endif  /* TCP */
};

/*  echat.c
 */
extern char *buf;
extern char *packet;
extern char *message;
extern struct config_t *config;
extern struct status_t *status;

#ifdef  DEBUG
extern FILE *debug_file;
#endif  /* DEBUG */

void write_log(char *logfile);
void chat_exit(int reason);

/*  chat_chnl.c
 */
void chnl_parse(void);
int chnl_getnum(void);
void chnl_refresh(void);
void chnl_next(void);
void chnl_join(void);
void chnl_leave(void);
void chnl_init(void);
void chnl_kill(void);
void chat_join(void);
void chat_rejoin(void);
void chat_leave(void);
void chat_nick(void);

/*  chat_cmnd.c
 */
void cmnd_load(void);
void cmnd_active(void);
void cmnd_parse(void);

/*  chat_conf.c
 */
void conf_default(void);
int conf_read(void);
int conf_write(void);

#ifdef  CHARSET
/*  chat_cset.c
 */
void cset_inout(void);
void cset_outin(void);
void cset_string(void);
#endif  /* CHARSET */

/*  chat_parm.c
 */
void parm_getopt(int argc, char *argv[]);

/*  chat_time.c
 */
char *time_get(void);
void time_users(void);

/*  chat_user.c
 */
void user_parse(void);
void user_refresh(void);
int user_getnum(void);
void user_delfavorite(void);
void user_addfavorite(void);
void user_parsfavorite(void);
void user_delignore(void);
void user_addignore(void);
void user_parsignore(void);
void user_delban(void);
void user_addban(void);
void user_parsban(void);
int user_setfavorite(void);
int user_favorite(void);
int user_setignore(void);
int user_ignore(void);
void user_add(void);
void user_del(void);
void user_rescan(void);
void user_nick(void);
void user_mode(void);
void user_active(void);

/*  curs_strn.c
 */
void string_show(void);
void screen_show(void);
void read_str(void);
void write_str(char *name, int color);
void write_tcp(char *name);

/*  curs_term.c
 */
void term_init(void);
void term_kill(void);
void window_init(void);
void refresh_users(void);
void refresh_room(void);
void refresh_text(void);
void refresh_input(void);
void refresh_read(void);
void window_refresh(void);
void status_ok(char *status);
void status_error(char *status);
void beep_system(void);
void beep_chat(void);
void beep_join(void);
void beep_mesg(void);
void beep_beep(void);

/*  pckt_addr.c
 */
void addr_del(void);
void addr_parse(void);
int addr_netcmp(char *addr1, char *addr2);
void addr_init(void);

/*  pckt_make.c
 */
void make_whohere(void);
void make_imhere(void);
void make_chat(void);
void make_nick(void);
void make_join(void);
void make_leave(void);
void make_mesg(void);
void make_mass(void);
void make_mesgack(void);
void make_execack(void);
void make_exec(void);
void make_me(void);
void make_newtopic(void);
void make_topic(void);
void make_mode(void);
void make_info(void);
void make_inform(void);
void make_beepbeep(void);
void make_beepack(void);
void make_privatjoin(void);
void make_privatleave(void);
void make_privatchat(void);
void make_privatme(void);
void make_here(void);
void make_wehere(void);
void make_active(void);
void make_userchnlslist(void);

/*  pckt_pars.c
 */
void pars_message(void);

/*  sock_tcp.c
 */
#ifdef  TCP
void sock_tcpinit(void);
void sock_tcpkill(void);
void sock_tcprecv(void);
void sock_tcpsend(void);
#endif  /* TCP */

/*  sock_udp.c
 */
void sock_init(void);
void sock_kill(void);
void sock_recv(void);
void sock_bsend(void);
void sock_usend(void);

#endif  /* _ECHAT_H */
