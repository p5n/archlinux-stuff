/*  chat_time.c
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#include <time.h>
#include "echat.h"

char *time_get(void) {
    static char cur_time[10];
    time_t t;
    struct tm *lt;

    t = time(&t);
    lt = localtime(&t);
    if (config->seconds) snprintf(cur_time, 9, "%02d:%02d:%02d", lt->tm_hour, lt->tm_min, lt->tm_sec);
    else snprintf(cur_time, 6, "%02d:%02d", lt->tm_hour, lt->tm_min);
    return (cur_time);
}

void time_users(void) {
    time_t t;

    if ((t = (unsigned int) time(&t)) < status->users_timeout) return;
    status->users_timeout = t + config->users_timeout;
    user_rescan();
}
