/*  curs_term.c
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#include <string.h>
#include <curses.h>
#include "echat.h"
#include "curs_term.h"

#ifdef  DEBUG
# include <errno.h>
#endif  /* DEBUG */

WINDOW *room_win = NULL;
WINDOW *users_win = NULL;
WINDOW *input_win = NULL;
WINDOW *stat_win = NULL;
WINDOW *text_win = NULL;
WINDOW *read_win = NULL;

char *string = NULL;

void term_init(void) {
    if ((string = (char *) malloc(STR_SIZE+2)) == NULL) {
#ifdef  DEBUG
        fprintf(debug_file, "malloc(): %s\n", strerror(errno));
#endif  /* DEBUG */
        exit(EXIT_FAILURE);
    }
    *string = *buf = '\0';
    if (!*config->head_nick) *config->head_nick = ' ';
    if (!*config->head_top) *config->head_top = ' ';
    if (!*config->head_users) *config->head_users = ' ';
    config->head_users[config->users_width-3] = '\0';
    initscr();
    curs_set(1);
    cbreak();
    noecho();
    nl();
    start_color();
    init_pair(COL_BORDER, config->color_border, config->color_bkgr);
    init_pair(COL_HEAD, config->color_head, config->color_bkgr);
    init_pair(COL_STATUS, config->color_status, config->color_bkgr);
    init_pair(COL_SYSTEM, config->color_system, config->color_bkgr);
    init_pair(COL_ERROR, config->color_error, config->color_bkgr);
    init_pair(COL_MESG, config->color_mesg, config->color_bkgr);
    init_pair(COL_MY, config->color_my, config->color_bkgr);
    init_pair(COL_CHAT, config->color_chat, config->color_bkgr);
#ifdef  DEBUG
    fprintf(debug_file, "ncurses: Initialization complete.\n");
    fflush(debug_file);
#endif  /* DEBUG */
}

void term_kill(void) {
    status_ok(TERM_SQUIT);
    endwin();
    free(string);
#ifdef  DEBUG
    fprintf(debug_file, "ncurses: Shutdown complete.\n");
    fflush(debug_file);
#endif  /* DEBUG */
}

void window_init(void) {
    if (config->style) {
        if ((room_win = newwin(LINES-4, COLS-(config->users_width+2), 0, 0)) == NULL) {
#ifdef  DEBUG
            fprintf(debug_file, "ncurses: room_win = newwin(): %s.\n", strerror(errno));
#endif  /* DEBUG */
            exit(EXIT_FAILURE);
        }
    } else {
        if ((room_win = newwin((config->status_str ? LINES-3 : LINES-2), COLS-config->users_width, 0, 0)) == NULL) {
#ifdef  DEBUG
            fprintf(debug_file, "ncurses: room_win = newwin(): %s.\n", strerror(errno));
#endif  /* DEBUG */
            exit(EXIT_FAILURE);
        }
    }

    if (config->style) {
        if ((input_win = newwin(3, COLS, LINES-4, 0)) == NULL) {
#ifdef  DEBUG
            fprintf(debug_file, "ncurses: input_win = newwin(): %s.\n", strerror(errno));
#endif  /* DEBUG */
            exit(EXIT_FAILURE);
        }
    } else {
        if ((input_win = newwin(2, COLS, (config->status_str ? LINES-3 : LINES-2), 0)) == NULL) {
#ifdef  DEBUG
            fprintf(debug_file, "ncurses: input_win = newwin(): %s.\n", strerror(errno));
#endif  /* DEBUG */
            exit(EXIT_FAILURE);
        }
    }

    if ((config->users_width < 10) || (config->users_width > 22)) config->users_width = 12;
    if (config->style) {
        if ((users_win = newwin(LINES-4, config->users_width+2, 0, COLS-(config->users_width+2))) == NULL) {
#ifdef  DEBUG
            fprintf(debug_file, "ncurses: users_win = newwin(): %s.\n", strerror(errno));
#endif  /* DEBUG */
            exit(EXIT_FAILURE);
        }
    } else {
        if ((users_win = newwin((config->status_str ? LINES-3 : LINES-2), config->users_width, 0, COLS-config->users_width)) == NULL) {
#ifdef  DEBUG
            fprintf(debug_file, "ncurses: users_win = newwin(): %s.\n", strerror(errno));
#endif  /* DEBUG */
            exit(EXIT_FAILURE);
        }
    }

    if (config->style) {
        if ((text_win = subwin(room_win, LINES-6, COLS-(config->users_width+4), 1, 1)) == NULL) {
#ifdef  DEBUG
            fprintf(debug_file, "ncurses: text_win = subwin(room_win): %s.\n", strerror(errno));
#endif  /* DEBUG */
            exit(EXIT_FAILURE);
        }
        if ((status->scrn_width = COLS - (config->users_width+4)) > LINE_SIZE)
            status->scrn_width = LINE_SIZE;
        wsetscrreg(text_win, 0, LINES-5);
    } else {
        if ((text_win = subwin(room_win, (config->status_str ? LINES-4 : LINES-3), COLS-(config->users_width+1), 1, 0)) == NULL) {
#ifdef  DEBUG
            fprintf(debug_file, "ncurses: text_win = subwin(room_win): %s.\n", strerror(errno));
#endif  /* DEBUG */
            exit(EXIT_FAILURE);
        }
        if ((status->scrn_width = COLS - config->users_width) > LINE_SIZE)
            status->scrn_width = LINE_SIZE;
        wsetscrreg(text_win, 0, (config->status_str ? LINES-5 : LINES-4));
    }
    wbkgdset(text_win, ' ' | COLOR_PAIR(COL_BORDER));
    wattrset(text_win, COLOR_PAIR(COL_SYSTEM)/* SERGEJ: | A_BOLD */);
    werase(text_win);
    leaveok(text_win, TRUE);
    scrollok(text_win, TRUE);

    if (config->style) {
        if ((read_win = subwin(input_win, 1, status->strn_width=COLS-(strlen(config->ps)+4), LINES-3, strlen(config->ps)+2)) == NULL) {
#ifdef  DEBUG
            fprintf(debug_file, "ncurses: read_win = subwin(input_win): %s.\n", strerror(errno));
#endif  /* DEBUG */
            exit(EXIT_FAILURE);
        }
    } else {
        if ((read_win = subwin(input_win, 1, status->strn_width=COLS-strlen(config->ps), (config->status_str ? LINES-2 : LINES-1), strlen(config->ps))) == NULL) {
#ifdef  DEBUG
            fprintf(debug_file, "ncurses: read_win = subwin(input_win): %s.\n", strerror(errno));
#endif  /* DEBUG */
            exit(EXIT_FAILURE);
        }
    }
    keypad(read_win, TRUE);
    leaveok(read_win, FALSE);
    scrollok(read_win, TRUE);
    wtimeout(read_win, READ_TIMEOUT);
    wbkgdset(read_win, ' ' | COLOR_PAIR(COL_MY));
    wattrset(read_win, COLOR_PAIR(COL_MY)/* SERGEJ: | A_BOLD */);
    werase(read_win);

    if (config->style || config->status_str) {
        if ((stat_win = newwin(1, COLS, LINES-1, 0)) == NULL) {
#ifdef  DEBUG
            fprintf(debug_file, "ncurses: stat_win = newwin(): %s.\n", strerror(errno));
#endif  /* DEBUG */
            exit(EXIT_FAILURE);
        }
        snprintf(message, STR_SIZE, "%s v%s.", NAME, VERSION);
        status_ok(message);
    }
}

void refresh_users(void) {
    register int n;

    wbkgdset(users_win, ' ' | COLOR_PAIR(COL_BORDER));
    werase(users_win);
    wattrset(users_win, COLOR_PAIR(COL_BORDER));
    if (config->style) {
        if (config->ascii) wborder(users_win, '|', '|', '-', '-', '+', '+', '+', '+');
        else wborder(users_win, 0, 0, 0, 0, 0, 0, 0, 0);
        wmove(users_win, 0, 2);
    } else {
        if (config->ascii) wborder(users_win, ' ', ' ', '-', ' ', '-', '-', ' ', ' ');
        else wborder(users_win, ' ', ' ', ACS_HLINE, ' ', ACS_HLINE, ACS_HLINE, ' ', ' ');
        wmove(users_win, 0, 0);
    }
    wprintw(users_win, config->head_users);
    if (status->users < 2) wattrset(users_win, COLOR_PAIR(COL_HEAD));
    else wattrset(users_win, COLOR_PAIR(COL_HEAD)/* SERGEJ: | A_BOLD */);
    wprintw(users_win, "%d ", status->users);
    for (n = 0; (n < (config->status_str ? LINES-4 : LINES-3)) && (n < status->users); n++) {
        wmove(users_win, (config->style ? n+2 : n+1), (config->style ? 1 : 0));
        if (status->user[n].active) wattrset(users_win, COLOR_PAIR(COL_SYSTEM)/* SERGEJ: | A_BOLD */);
        else wattrset(users_win, COLOR_PAIR(COL_SYSTEM));
        if (status->user[n].ignore)
            wattrset(users_win, COLOR_PAIR(COL_ERROR)/* SERGEJ: | A_BOLD */);
        if (status->user[n].mode == MOD_NORMAL) waddch(users_win, CHR_NORMAL);
        if (status->user[n].mode == MOD_DND) waddch(users_win, CHR_DND);
        if (status->user[n].mode == MOD_AWAY) waddch(users_win, CHR_AWAY);
        if (status->user[n].mode == MOD_OFF) waddch(users_win, CHR_OFF);
        wattrset(users_win, COLOR_PAIR(COL_CHAT)/* SERGEJ: | A_BOLD */);
/* SERGEJ */ validate_string(status->user[n].nick);
        if (strlen(status->user[n].nick) < config->users_width) {
            wmove(users_win, (config->style ? n+2 : n+1), (config->style ? 2 : 1));
            waddnstr(users_win, status->user[n].nick, config->users_width);
        } else {
            wmove(users_win, (config->style ? n+2 : n+1), (config->style ? 2 : 1));
            waddnstr(users_win, status->user[n].nick, config->users_width-2);
            wattrset(users_win, COLOR_PAIR(COL_SYSTEM)/* SERGEJ: | A_BOLD */);
            waddch(users_win, CHR_BIGNICK);
            wattrset(users_win, COLOR_PAIR(COL_CHAT)/* SERGEJ: | A_BOLD */);
        }
/* SERGEJ */
        if(n<9)
        {
            waddch(users_win, '.');
            waddch(users_win, '0'+1+n);
        }
/* SERGEJ */
    }
    wrefresh(users_win);
}

void refresh_room(void) {
    int len = strlen(config->nick);

    wattrset(room_win, COLOR_PAIR(COL_BORDER));
    if (config->style) {
        if (config->ascii) wborder(room_win, '|', '|', '-', '-', '+', '+', '+', '+');
        else wborder(room_win, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
        if (config->ascii) wborder(room_win, ' ', '|', '-', ' ', '-', '-', ' ', '|');
        else wborder(room_win, ' ', ACS_VLINE, ACS_HLINE, ' ', ACS_HLINE, ACS_HLINE, ' ', ACS_VLINE);
    }
    wmove(room_win, 0, 2);
    wprintw(room_win, config->head_nick);
    if (config->mode == MOD_NORMAL) wattrset(room_win, COLOR_PAIR(COL_HEAD)/* SERGEJ: | A_BOLD */);
    else wattrset(room_win, COLOR_PAIR(COL_HEAD));
    waddnstr(room_win, config->nick, NICK_LEN);
    waddch(room_win, ' ');
    if (*status->room->name == CHR_CHNL) {
        if (len > NICK_LEN) {
            wmove(room_win, 0, strlen(config->head_nick) +4+NICK_LEN);
            wattrset(room_win, COLOR_PAIR(COL_HEAD));
            wprintw(room_win, config->head_top);
            wattrset(room_win, COLOR_PAIR(COL_HEAD)/* SERGEJ: | A_BOLD */);
/* SERGEJ */ validate_string(status->chnl[0].topic);
            if (config->packet_qchat)
                waddnstr(room_win, status->chnl[0].topic,
                    (COLS-(config->users_width+NICK_LEN)) - (config->style ?
                    (strlen(config->head_nick)+strlen(config->head_top)+9) :
                    (strlen(config->head_nick)+strlen(config->head_top)+6)));
            else
                waddnstr(room_win, status->room->topic,
                    (COLS-(config->users_width+NICK_LEN)) - (config->style ?
                    (strlen(config->head_nick)+strlen(config->head_top)+9) :
                    (strlen(config->head_nick)+strlen(config->head_top)+6)));
            waddch(room_win, ' ');
        } else {
            wmove(room_win, 0, strlen(config->head_nick) + 4+len);
            wattrset(room_win, COLOR_PAIR(COL_HEAD));
/* SERGEJ */ validate_string(config->head_top);
/* SERGEJ */ validate_string(status->chnl[0].topic);
            wprintw(room_win, config->head_top);
            wattrset(room_win, COLOR_PAIR(COL_HEAD)/* SERGEJ: | A_BOLD */);
            if (config->packet_qchat)
                waddnstr(room_win, status->chnl[0].topic,
                    (COLS-(config->users_width+len)) - (config->style ?
                    (strlen(config->head_nick)+strlen(config->head_top)+9) :
                    (strlen(config->head_nick)+strlen(config->head_top)+6)));
            else
                waddnstr(room_win, status->room->topic,
                    (COLS-(config->users_width+len)) - (config->style ?
                    (strlen(config->head_nick)+strlen(config->head_top)+9) :
                    (strlen(config->head_nick)+strlen(config->head_top)+6)));
            waddch(room_win, ' ');
        }
    }
    wrefresh(room_win);
}

void refresh_percent(void) {
    wmove(room_win, 1, status->scrn_width+1);
    wattrset(room_win, COLOR_PAIR(COL_HEAD)/* SERGEJ: | A_BOLD */);
    waddch(room_win, CHR_PERCENT);
    wrefresh(room_win);
}

void refresh_text(void) {
    touchwin(room_win);
    wrefresh(text_win);
}

void refresh_input(void) {
    register int n;

    wbkgdset(input_win, ' ' | COLOR_PAIR(COL_BORDER));
    werase(input_win);
    wattrset(input_win, COLOR_PAIR(COL_BORDER));
    if (config->style) {
        if (config->ascii) wborder(input_win, '|', '|', '-', '-', '+', '+', '+', '+');
        else wborder(input_win, 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
        if (config->ascii) wborder(input_win, ' ', ' ', '-', ' ', '-', '-', ' ', ' ');
        else wborder(input_win, ' ', ' ', ACS_HLINE, ' ', ACS_HLINE, ACS_HLINE, ' ', ' ');
    }
    wmove(input_win, 0, 2);
    for (n = 0; n < status->channels; n++) {
        waddch(input_win, ' ');
        if (n) {
            wattrset(input_win, COLOR_PAIR(COL_HEAD));
            waddch(input_win, ACS_HLINE);
            waddch(input_win, ' ');
        }
        if (!strncmp(status->chnl[n].name, status->room->name, CHANNEL_MAXLEN)) {
            wattrset(input_win, COLOR_PAIR(COL_HEAD)/* SERGEJ: | A_BOLD */);
            waddnstr(input_win, status->chnl[n].name, CHANNEL_MAXLEN);
        } else {
            wattrset(input_win, COLOR_PAIR(COL_HEAD));
            waddnstr(input_win, status->chnl[n].name, 8);
        }
    }
    waddch(input_win, ' ');
    wattrset(input_win, COLOR_PAIR(COL_MY)/* SERGEJ: | A_BOLD */);
    wmove(input_win, 1, (config->style ? 2 : 0));
    waddnstr(input_win, config->ps, STR_SIZE);
    wrefresh(input_win);
}

void refresh_read(void) {
    touchwin(input_win);
    wrefresh(read_win);
}

void window_refresh(void) {
    refresh_users();
    refresh_room();
    screen_show();
    refresh_input();
    string_show();
}

void status_ok(char *str) {
    if (!config->verbose || (!config->style && !config->status_str)) return;
    wbkgdset(stat_win, ' ' | COLOR_PAIR(COL_STATUS));
    werase(stat_win);
    wattrset(stat_win, COLOR_PAIR(COL_STATUS));
    wmove(stat_win, 0, 0);
    waddnstr(stat_win, str, STR_SIZE);
    wrefresh(stat_win);
    refresh_read();
}

void status_error(char *str) {
    if (!config->style && !config->status_str) return;
    wbkgdset(stat_win, ' ' | COLOR_PAIR(COL_ERROR));
    werase(stat_win);
    wattrset(stat_win, COLOR_PAIR(COL_ERROR)/* SERGEJ: | A_BOLD */);
    wmove(stat_win, 0, 0);
    waddnstr(stat_win, str, STR_SIZE);
    wrefresh(stat_win);
    refresh_read();
}

void beep_system(void) {
#ifdef  LINUX
    if (config->beep_system) {
        fprintf(stdout, "%s%s%u%s%s%s%u%s\a",
            ESC, SND_TONE, SND_SYST_TONE, SND_END, ESC, SND_DUR, SND_SYST_DUR, SND_END);
        fflush(stdout);
    }
#endif  /* LINUX */
#ifdef  DEBUG
    fprintf(debug_file, "ncurses: beep(system)\n");
    fflush(debug_file);
#endif  /* DEBUG */
}

void beep_chat(void) {
#ifdef  LINUX
    if (config->beep_chat) {
        fprintf(stdout, "%s%s%u%s%s%s%u%s\a",
            ESC, SND_TONE, SND_CHAT_TONE, SND_END, ESC, SND_DUR, SND_CHAT_DUR, SND_END);
        fflush(stdout);
    }
#endif  /* LINUX */
#ifdef  DEBUG
    fprintf(debug_file, "ncurses: beep(chat)\n");
    fflush(debug_file);
#endif  /* DEBUG */
}

void beep_join(void) {
#ifdef  LINUX
    if (config->beep_join) {
        fprintf(stdout, "%s%s%u%s%s%s%u%s\a",
            ESC, SND_TONE, SND_JOIN_TONE, SND_END, ESC, SND_DUR, SND_JOIN_DUR, SND_END);
        fflush(stdout);
    }
#endif  /* LINUX */
#ifdef  DEBUG
    fprintf(debug_file, "ncurses: beep(join)\n");
    fflush(debug_file);
#endif  /* DEBUG */
}

void beep_mesg(void) {
    if (config->beep_mesg) {
#ifdef  LINUX
        fprintf(stdout, "%s%s%u%s%s%s%u%s\a",
            ESC, SND_TONE, SND_MESG_TONE, SND_END, ESC, SND_DUR, SND_MESG_DUR, SND_END);
        fflush(stdout);
#endif  /* LINUX */
#ifdef  FREEBSD
        fprintf(stdout, "\a");
        fflush(stdout);
#endif  /* FREEBSD */
    }
#ifdef  DEBUG
    fprintf(debug_file, "ncurses: beep(mesg)\n");
    fflush(debug_file);
#endif  /* DEBUG */
}

void beep_beep(void) {
    if (config->beep_beep) {
#ifdef  LINUX
        fprintf(stdout, "%s%s%u%s%s%s%u%s\a",
            ESC, SND_TONE, SND_BEEP_TONE, SND_END, ESC, SND_DUR, SND_BEEP_DUR, SND_END);
        fflush(stdout);
#endif  /* LINUX */
#ifdef  FREEBSD
        fprintf(stdout, "\a");
        fflush(stdout);
#endif  /* FREEBSD */
    }
#ifdef  DEBUG
    fprintf(debug_file, "ncurses: beep(beep)\n");
    fflush(debug_file);
#endif  /* DEBUG */
}
