/*  chat_conf.h
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#ifndef _CHAT_CONF_H
#define _CHAT_CONF_H 1

#define OPTION_MAXLEN   20
#define VALUE_MAXLEN    20

#define CNF_COMMENT1    '#'
#define CNF_COMMENT2    ';'

#define DEF_NICK        "Nick"          /* your nick (recommended length <20 chars) */
#define DEF_GENDER      0               /* gender: 0 - male 1 - female */
#define DEF_CHANNEL     "#Main"         /* default: #Main */
#define DEF_MODE        MOD_NORMAL      /* see echat.h */
#define DEF_ACTIVE      0               /* activity: 0 - not active !0 - active */
#define DEF_ANSWER_DND  ""              /* dnd autoanswer message */
#define DEF_ANSWER_AWAY ""              /* away autoanswer message */
#define DEF_ANSWER_OFF  ""              /* offline autoanswer message */
#define DEF_ME_ACTION   ""              /* default /me action */
#define DEF_FAVORITE    ""              /* noone to favorite */
#define DEF_IGNORE      ""              /* noone to ignore */
#define DEF_BAN         ""              /* noone to ban */
#define DEF_ADDRESS     "255.255.255.255" /* BCAST addresses */
#define DEF_PORT        PORT            /* default port, see echat.h */
#define DEF_PACKET_BCAST 0              /* broadcast sending: 0 - off !0 - on */
#define DEF_PACKET_ROUTE 0              /* packet routing: 0 - off !0 - on */
#define DEF_PACKET_QCHAT 0              /* quickChat protocol */
#define DEF_PACKET_XOR  CHR_XOR         /* packets "encrypt" XOR-byte, see echat.h */
#define DEF_HEAD_NICK   " nick: "       /* default: " nick: " */
#define DEF_HEAD_TOP    " top: "        /* default: " top: " */
#define DEF_HEAD_USERS  " users: "      /* default: " users: " */
#define DEF_PS          "eChat> "       /* default: "eChat> " */
#define DEF_STYLE       0               /* window style: 0 or !0 */
#define DEF_ASCII       0               /* ascii graph: 0 - off !0 - on */
#define DEF_STATUS_STR  0               /* status string: 0 - off !0 - on */
#define DEF_SECONDS     0               /* show seconds: 0 - off !0 - on */
#define DEF_USERS_WIDTH 12              /* users window width */
#define DEF_USERS_TIMEOUT 300           /* users refresh timeout */
#define DEF_SCROLL_STEP 1               /* scroll step */
#ifdef  CHARSET
# define DEF_CHARSET    0               /* message charset: 0 - N/A !0 - cp1251 */
#endif  /* CHARSET */
#define DEF_BEEP_SYSTEM 0               /* beep system: 0 - off !0 - on */
#define DEF_BEEP_CHAT   0               /* beep chat: 0 - off !0 - on */
#define DEF_BEEP_JOIN   0               /* beep join: 0 - off !0 - on */
#define DEF_BEEP_MESG   0               /* beep mesg: 0 - off !0 - on */
#define DEF_BEEP_BEEP   0               /* beep beep: 0 - off !0 - on */
#define DEF_MY_ONLY     0               /* beeps: 0 - all, !0 - my only */
#define DEF_VERBOSE     0               /* verbose: 0 - off !0 - on */
#define DEF_SERVER      0               /* server mode: 0 - off !0 - on */
#ifdef  SHELL
# define DEF_SHELL      ""              /* default: no shell command */
#endif  /* SHELL */
#define DEF_LOGMAIN     ""              /* default: no main logfile */
#define DEF_LOGMESG     ""              /* default: no message logfile */
#define DEF_LOGPRIV     ""              /* default: no private logfile */

/* from curses.h :
 *      COLOR_BLACK 0   COLOR_RED 1     COLOR_GREEN 2   COLOR_YELLOW 3
 *      COLOR_BLUE 4    COLOR_MAGENTA 5 COLOR_CYAN 6    COLOR_WHITE 7
 */
#define DEF_COL_BKGR    0
#define DEF_COL_BORDER  7
#define DEF_COL_HEAD    7
#define DEF_COL_STATUS  7
#define DEF_COL_SYSTEM  6
#define DEF_COL_ERROR   1
#define DEF_COL_MESG    5
#define DEF_COL_MY      3
#define DEF_COL_CHAT    2

#endif  /* _CHAT_CONF_H */
