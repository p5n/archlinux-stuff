/*  curs_strn.c
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#include <string.h>
#include <curses.h>
#include "echat.h"
#include "curs_term.h"

void string_add(void) {
    register int n;

#ifdef  DEBUG
    fprintf(debug_file, "read_str(%s): %s\n", status->room->name, string);
    fflush(debug_file);
#endif  /* DEBUG */
    status->hist = 0;
    if (!strncmp(status->history[1], string, STR_SIZE)) return;
    for (n = HIST_SIZE-1; n > 1; n--)
        strncpy(status->history[n], status->history[n-1], STR_SIZE);
    strncpy(status->history[1], string, status->strn_end+1);
}

void string_show(void) {
    werase(read_win);
    wmove(read_win, 0, status->strn_cur-status->strn);
    waddnstr(read_win, buf, (status->strn_width-1)-(status->strn_cur-status->strn));
    wmove(read_win, 0, 0);
    waddnstr(read_win, string+status->strn, status->strn_cur-status->strn);
    refresh_read();
}

void string_up(void) {
    if (status->hist == HIST_SIZE-1) return;
    if (status->history[status->hist+1][0] == '\0') return;
    if (status->hist == 0) {
        strncpy(status->history[0], string, status->strn_cur+1);
        strncat(status->history[0], buf, status->strn_end-status->strn_cur+1);
    }
    strncpy(string, status->history[++status->hist], STR_SIZE);
    status->strn_end = status->strn_cur = strlen(string);
    if (status->strn_end > (status->strn_width-1))
        status->strn = status->strn_end - (status->strn_width-1);
    else status->strn = 0;
    *buf = '\0';
}

void string_down(void) {
    if (status->hist == 0) return;
    strncpy(string, status->history[--status->hist], STR_SIZE);
    status->strn_end = status->strn_cur = strlen(string);
    if (status->strn_end > (status->strn_width-1))
        status->strn = status->strn_end - (status->strn_width-1);
    else status->strn = 0;
    *buf = '\0';
}

void string_left(void) {
    register int n;

    if (status->strn_cur == 0) return;
    if ((status->strn_end > (status->strn_width-1)) &&
        ((status->strn_cur-8) > 0) &&
        (status->strn == (status->strn_cur-8)))
        status->strn--;
    for (n = (status->strn_end-status->strn_cur+1); n > 0; n--) { buf[n] = buf[n-1]; }
    *buf = string[--status->strn_cur];
    string[status->strn_cur] = '\0';
}

void string_right(void) {
    register int n;

    if (status->strn_cur == status->strn_end) return;
    if ((status->strn_end > (status->strn_width-1)) &&
        (status->strn_end > (status->strn_cur+8)) &&
        (status->strn == (status->strn_cur-(status->strn_width-1)+8)))
        status->strn++;
    string[status->strn_cur++] = *buf;
    string[status->strn_cur] = '\0';
    for (n = 0; n < (status->strn_end-status->strn_cur+1); n++) { buf[n] = buf[n+1]; }
}

void string_bs(void) {
    if (status->strn_cur == 0) return;
    string[--status->strn_cur] = '\0';
    status->strn_end--;
    if ((status->strn_end > (status->strn_width-1)) &&
        ((status->strn_cur-7) > 0) &&
        (status->strn == (status->strn_cur-7))) status->strn--;
}

void string_del(void) {
    register int n;

    if (status->strn_cur == status->strn_end) return;
    for (n = 0; n < ((status->strn_end-status->strn_cur)+1); n++) buf[n] = buf[n+1];
    status->strn_end--;
}

void string_home(void) {
    strncat(string, buf, status->strn_end-status->strn_cur+1);
    strncpy(buf, string, status->strn_end+1);
    *string = '\0';
    status->strn_end = strlen(buf);
    status->strn = status->strn_cur = 0;
}

void string_end(void) {
    strncat(string, buf, status->strn_end-status->strn_cur+1);
    *buf = '\0';
    status->strn_end = status->strn_cur = strlen(string);
    if (status->strn_cur > (status->strn_width-1))
        status->strn = status->strn_cur - (status->strn_width-1);
}

void screen_add(int color) {                /* FIXME: word wrapping */
    char *str = message;
    int size;
    register int n;

    if (message[0] == '\n' && message[1] == '\0') return;
    if (message[n=(strlen(message)-1)] == '\n') message[n] = '\0';
#ifdef  DEBUG
    fprintf(debug_file, "write_str(%s): %s\n", status->room_str->name, message);
    fflush(debug_file);
#endif  /* DEBUG */
    size = n / (status->scrn_width-1);
    if ((n % (status->scrn_width-1))) size++;
    for (n = SCRN_SIZE-size; n >= size; n--) {
        strncpy(status->room_str->screen[n], status->room_str->screen[n-size], status->scrn_width);
        status->room_str->scrn_color[n] = status->room_str->scrn_color[n-size];
    }
    for (n = size - 1; n >= 0; n--) {
        strncpy(status->room_str->screen[n], str, status->scrn_width-1);
        status->room_str->scrn_color[n] = color;
        if (status->room_str->scrn_top < ((SCRN_SIZE-1)-(LINES-2))) status->room_str->scrn_top++;
        if ((str += (status->scrn_width-1)) > (message+MSG_SIZE)) break;
    }
}

void screen_show(void) {
    register int n;

    for (n = LINES; n > 0; n--) {
        wattrset(text_win, COLOR_PAIR(status->room->scrn_color[status->room->scrn+n]) | A_BOLD);
        waddnstr(text_win, status->room->screen[status->room->scrn+n], status->scrn_width-1);
        if (status->room->screen[status->room->scrn+n][status->scrn_width-2] == '\0') waddch(text_win, '\n');
    }
    wattrset(text_win, COLOR_PAIR(status->room->scrn_color[status->room->scrn]) | A_BOLD);
    waddnstr(text_win, status->room->screen[status->room->scrn], status->scrn_width-2);
    refresh_text();
    refresh_read();
}

void screen_up(void) {
    status->room->scrn += config->scroll_step;
    if ((status->room->scrn+config->scroll_step) > status->room->scrn_top) status->room->scrn = status->room->scrn_top;
    screen_show();
}

void screen_down(void) {
    status->room->scrn -= config->scroll_step;
    if ((status->room->scrn-config->scroll_step) < 0) status->room->scrn = 0;
    screen_show();
}

void screen_top(void) {
    if ((status->room->scrn = status->room->scrn_top) > LINES-2)
        status->room->scrn = status->room->scrn_top - (LINES-2);
    screen_show();
}

void screen_bot(void) {
    status->room->scrn = 0;
    screen_show();
}

void read_str(void) {
    static char slash = 0;
    int ch;

    switch ((ch = wgetch(read_win))) {
        case ERR: return;
        case CTL_D:
            chat_exit(0);
        case CTL_A:
            cmnd_active(); return;
        case CTL_O:
        case TAB:
            chnl_next(); return;
        case KEY_F(1):
            status->room = &status->chnl[0]; chnl_refresh(); return;
        case KEY_F(2):
            if (status->channels > 1) status->room = &status->chnl[1]; chnl_refresh(); return;
        case KEY_F(3):
            if (status->channels > 2) status->room = &status->chnl[2]; chnl_refresh(); return;
        case KEY_F(4):
            if (status->channels > 3) status->room = &status->chnl[3]; chnl_refresh(); return;
        case CTL_K:
            status->user_sort = !status->user_sort; user_refresh(); return;
        case KEY_F(5):
        case CTL_L:
        case KEY_IC:
            user_rescan(); return;
        case CTL_R:
            term_kill(); term_init(); window_init(); window_refresh(); return;
        case CTL_T:
            screen_top(); break;
        case CTL_B:
            screen_bot(); break;
        case CTL_P:
        case KEY_PPAGE:
            screen_up(); break;
        case CTL_N:
        case KEY_NPAGE:
            screen_down(); break;
#ifdef  CHARSET
        case CTL_E:
            cset_string(); break;
#endif  /* CHARSET */
        case CTL_U: return;        /* FIXME: undo dont implemented */
        case KEY_UP:
            string_up(); break;
        case KEY_DOWN:
            string_down(); break;
        case KEY_LEFT:
            string_left(); break;
        case KEY_RIGHT:
            string_right(); break;
        case KEY_HOME:
            string_home(); break;
        case KEY_END:
            string_end(); break;
        case KEY_DC:
            string_del(); break;
        case KEY_BACKSPACE:
            string_bs(); break;
        case ENTER:                /* FIXME: backslash escaping */
            if (slash) { slash--; break; }
            screen_bot();
            if (status->strn_end == 0) { return; }
            strncat(string, buf, status->strn_end-status->strn_cur+1);
            strncpy(buf, string, status->strn_end+1);
            string_add();
            buf[status->strn_end+1] = '\0';
            cmnd_parse();
            status->strn = status->strn_end = status->strn_cur = 0;
            *string = *buf = '\0';
            break;
        case '\\':                /* FIXME: backslash escaping */
/*            if (!slash) { slash++; break; }
            slash--; */
        case '1': case '2': case '3':
        case '4': case '5': case '6':
        case '7': case '8': case '9':
            if (status->strn_cur && ((ch-'1') >= 0) && ((ch-'1') < status->users)) {
                if ((string[status->strn_cur-1] == CHR_DROP) &&
                    (status->strn_end < (STR_SIZE-(NICK_MAXLEN+2)))) {
                    string[--status->strn_cur] = '\0';
                    strncat(string, status->user[ch-'1'].nick, NICK_MAXLEN+1);
                    strncat(string, ": ", 2);
                    status->strn_cur = strlen(string);
                    status->strn_end = status->strn_cur + strlen(buf);
                    if (status->strn_cur > status->strn+(status->strn_width-1))
                        status->strn = status->strn_cur - (status->strn_width-1);
                    status->hist = 0;
                    break;
                }
                if ((string[status->strn_cur-1] == CHR_MESG) &&
                    (status->strn_end < (STR_SIZE-(NICK_MAXLEN+6))) &&
                    (status->strn_cur == 1)) {
                    snprintf(string, STR_SIZE, "/%s %s ", CMD_MESG1, status->user[ch-'1'].nick);
                    status->strn_cur = strlen(string);
                    status->strn_end = status->strn_cur + strlen(buf);
                    if (status->strn_cur > status->strn+(status->strn_width-1))
                        status->strn = status->strn_cur - (status->strn_width-1);
                    status->hist = 0;
                    break;
                }
            }
        default:
            if (status->strn_end == STR_SIZE) return;
            string[status->strn_cur++] = ch;
            string[status->strn_cur] = '\0';
            status->strn_end++;
            if (status->strn_cur > status->strn+(status->strn_width-1))
                status->strn++;
    }
    string_show();
}

void write_str(char *name, int color) {
    strncpy(status->channel, name, CHANNEL_MAXLEN);
    status->room_str = &status->chnl[chnl_getnum()];
    screen_add(color);
    if (!strncmp(name, status->room->name, CHANNEL_MAXLEN)) screen_show();
}

#ifdef  TCP
void write_tcp(char *name) {
    if (strncmp(status->chnl[0].name, name, CHANNEL_MAXLEN)) return;
    sock_tcpsend();
}
#endif  /* TCP */
