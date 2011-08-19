/*  curs_term.h
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#ifndef _CURS_TERM_H
#define _CURS_TERM_H 1

#define COL_BORDER      1       /* borders */
#define COL_HEAD        2       /* heads */
#define COL_STATUS      3       /* status */

#define NICK_LEN        12

#define READ_TIMEOUT    10

#define CHR_NORMAL      '+'
#define CHR_DND         '-'
#define CHR_AWAY        '='
#define CHR_OFF         '*'

#define CHR_BIGNICK     '>'
#define CHR_PERCENT     '*'

#define CHR_DROP        '`'
#define CHR_MESG        '/'     /* from chat_cmnd.h: CHR_CMND */

#define CMD_MESG1       "mesg"  /* from chat_cmnd.h */

#ifdef  LINUX                   /* ESC for Linux */
#define ESC             "\033"  /* escape */
#define SND_TONE        "[10;"  /* sound tone */
#define SND_DUR         "[11;"  /* sound duration */
#define SND_END         "]"
#define SND_SYST_DUR    20
#define SND_SYST_TONE   1500
#define SND_CHAT_DUR    20
#define SND_CHAT_TONE   300
#define SND_JOIN_DUR    20
#define SND_JOIN_TONE   3000
#define SND_MESG_DUR    100
#define SND_MESG_TONE   400
#define SND_BEEP_DUR    100
#define SND_BEEP_TONE   500
#endif  /* LINUX */

#define TAB             '\t'    /* Tab */
#define ENTER           '\n'    /* Enter */

#define CTL_A           '\x01'  /* ^A */
#define CTL_B           '\x02'  /* ^B */
#define CTL_D           '\x04'  /* ^D */
#define CTL_E           '\x05'  /* ^E */
#define CTL_K           '\x0B'  /* ^K */
#define CTL_L           '\x0C'  /* ^L */
#define CTL_N           '\x0E'  /* ^N */
#define CTL_O           '\x0F'  /* ^O */
#define CTL_P           '\x10'  /* ^P */
#define CTL_R           '\x12'  /* ^R */
#define CTL_T           '\x14'  /* ^T */
#define CTL_U           '\x15'  /* ^U */
#define CTL_Y           '\x19'  /* ^Y */

extern WINDOW *room_win;
extern WINDOW *users_win;
extern WINDOW *input_win;
extern WINDOW *stat_win;
extern WINDOW *text_win;
extern WINDOW *read_win;

extern char *string;

#endif  /* _CURS_TERM_H */
