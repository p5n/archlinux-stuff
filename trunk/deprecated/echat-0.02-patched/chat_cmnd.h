/*  chat_cmnd.h
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#ifndef _CHAT_CMND_H
#define _CHAT_CMND_H 1

#define CHR_CMND        '/'

#define CHR_RESTR1      '\"'
#define CHR_RESTR2      '\\'
#define CHR_RESTR3      '/'
#define CHR_RESTR4      '|'
#define CHR_RESTR5      '>'
#define CHR_RESTR6      '<'
#define CHR_RESTR7      '?'
#define CHR_RESTR8      '*'

#define CMD_MAXLEN      32

#define CMD_NICK        "nick"
#define CMD_NICK1       "n"
#define CMD_GENDER      "gender"
#define CMD_GENDER1     "g"
#define CMD_INFO        "info"
#define CMD_INFO1       "i"
#define CMD_INFO2       "whois"
#define CMD_HERE        "here"
#define CMD_ME          "me"
#define CMD_BEEP        "beep"
#define CMD_BEEP1       "b"
#define CMD_EXEC        "exec"
#define CMD_FAVORITE    "favorite"
#define CMD_FAVORITE1   "favor"
#define CMD_IGNORE      "ignore"
#define CMD_IGNORE1     "ign"
#define CMD_BAN         "ban"

#define CMD_JOIN        "join"
#define CMD_JOIN1       "j"
#define CMD_LEAVE       "leave"
#define CMD_LEAVE1      "l"
#define CMD_TALK        "talk"
#define CMD_TALK1       "t"
#define CMD_TALK2       "chat"
#define CMD_MESG        "message"
#define CMD_MESG1       "mesg"
#define CMD_MESG2       "m"
#define CMD_MASS        "mass"
#define CMD_TOPIC       "topic"
#define CMD_TOPIC1      "top"
#define CMD_ADD         "add"
#define CMD_MY          "my"

/* SERGEJ */
#define CMD_NMESG       "nmesg"
#define CMD_CHAN        "chan"
/* SERGEJ */

#define CMD_ACTIVE      "active"
#define CMD_ACTIVE1     "act"
#define CMD_ANSWER      "answer"
#define CMD_ANSWER1     "a"
#define CMD_MODE        "mode"
#define CMD_MODE1       "mod"
#define CMD_NORMAL      "normal"
#define CMD_NORMAL1     "o"
#define CMD_DND         "dnd"
#define CMD_DND1        "dd"
#define CMD_AWAY        "away"
#define CMD_AWAY1       "aw"
#define CMD_OFF         "off"
#define CMD_OFF1        "of"

#define CMD_LOAD        "load"
#define CMD_SAVE        "save"
#define CMD_SHELL       "!"
#define CMD_CLEAR       "clear"
#define CMD_CLEAR1      "c"
#define CMD_VERBOSE     "verbose"
#define CMD_VERBOSE1    "verb"
#define CMD_KEYS        "keys"
#define CMD_KEYS1       "k"
#define CMD_STATUS      "status"
#define CMD_STATUS1     "s"
#define CMD_VERSION     "version"
#define CMD_VERSION1    "v"
#define CMD_HELP        "help"
#define CMD_HELP1       "h"
#define CMD_HELP2       "?"
#define CMD_QUIT        "quit"
#define CMD_QUIT1       "q"
#define CMD_QUIT2       "exit"

#endif  /* _CHAT_CMND_H */
