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
}

void term_kill(void) {
}

void window_init(void) {
}

void refresh_users(void) {
}

void refresh_room(void) {
}

void refresh_percent(void) {
}

void refresh_text(void) {
}

void refresh_input(void) {
}

void refresh_read(void) {
}

void window_refresh(void) {
}

void status_ok(char *str) {
}

void status_error(char *str) {
}

void beep_system(void) {
}

void beep_chat(void) {
}

void beep_join(void) {
}

void beep_mesg(void) {
}

void beep_beep(void) {
}
