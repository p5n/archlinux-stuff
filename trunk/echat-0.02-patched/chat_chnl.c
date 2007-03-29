/*  chat_chnl.c
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#include <string.h>
#include "echat.h"

void chnl_parse(void) {
    register int n;

    strncpy(status->channel, config->channel, CHANNEL_MAXLEN);
    for (n = 1; n < CHANNEL_MAXLEN; n++)
        if (status->channel[n] == CHR_CHNL) { status->channel[n] = '\0'; break; }
}

void chnl_deny(void) {
    snprintf(message, MSG_SIZE, CHNL_OUT, time_get(), status->channel);
    write_str(status->room->name, COL_ERROR);
}

void chnl_refresh(void) {
    refresh_room();
    screen_show();
    refresh_input();
    string_show();
}

void chnl_next(void) {
    register int n;

    for (n = 0; n < status->channels;)
        if (status->room == &status->chnl[n++]) break;
    if (n == status->channels) status->room = &status->chnl[0];
    else status->room = &status->chnl[n];
    chnl_refresh();
}

int chnl_getnum(void) {
    register int n;

    for (n = 0; n < status->channels; n++)
        if (!strncmp(status->chnl[n].name, status->channel, CHANNEL_MAXLEN)) return(n);
    return(0);
}

void chnl_init(void) {
    char str[CHANNEL_MAXLEN];
    register int n, m, l;

    for (n = l = 0; (n < CHANNELS_MAX) && (l < STR_SIZE-CHANNEL_MAXLEN); n++, l++)
        for (m = 0; m < CHANNEL_MAXLEN; m++, l++)
            if (((str[m] = config->channel[l]) == '\0') || (config->channel[l+1] == CHR_CHNL)) {
                str[m+1] = '\0';
                strncpy(status->channel, str, CHANNEL_MAXLEN);
                make_join();
                if (str[m] == '\0') return;
                break;
            }
}

void chnl_kill(void) {
    strncpy(status->channel, status->chnl[0].name, CHANNEL_MAXLEN);
    make_leave();
}

void chnl_add(void) {
    strncpy(status->chnl[status->channels].name, status->channel, CHANNEL_MAXLEN);
    status->channels++;
}

void chnl_del(void) {
    register int n;

    if ((n = chnl_getnum()) < status->channels)
        for (n++; n < status->channels; n++)
            memcpy(&status->chnl[n-1], &status->chnl[n], sizeof(struct chnl_t));
    status->channels--;
    memset(&status->chnl[status->channels], 0, sizeof(struct chnl_t));
    status->room = &status->chnl[0];
}

void chnl_join(void) {
    if (!chnl_getnum()) {
        if (strncmp(status->chnl[0].name, status->channel, CHANNEL_MAXLEN)) {
            if (status->channels == CHANNELS_MAX) { chnl_deny(); return; }
            if (!strncmp(config->nick, status->nick, NICK_MAXLEN)) {
                chnl_add();
                make_here();
            }
        } else { make_imhere(); user_add(); }
/*        make_topic(); */
    }
    snprintf(message, MSG_SIZE, CHNL_JOIN, time_get(), status->nick, status->channel);
#ifdef  TCP
    write_tcp(status->channel);
#endif  /* TCP */
    if (config->verbose) {
        write_log(config->log_main);
/* SERGEJ:        write_str(status->channel, COL_SYSTEM);*/
    }
    if (((!strncmp(status->chnl[0].name, status->channel, CHANNEL_MAXLEN) &&
        strncmp(config->nick, status->nick, NICK_MAXLEN)) ||
        (!config->my_only || !strncmp(status->room->name, status->channel, CHANNEL_MAXLEN))) &&
        (config->mode == MOD_NORMAL)) beep_join();
    refresh_room();
    screen_show();
    refresh_input();
    string_show();
}

void chnl_leave(void) {
    if (!strncmp(status->chnl[0].name, status->channel, CHANNEL_MAXLEN)) user_del();
    snprintf(message, MSG_SIZE, CHNL_LEAVE, time_get(), status->nick,  status->channel);
#ifdef  TCP
    write_tcp(status->channel);
#endif  /* TCP */
    if (config->verbose) {
        write_log(config->log_main);
/* SERGEJ:        write_str(status->channel, COL_SYSTEM);*/
    }
    if (strncmp(status->chnl[0].name, status->channel, CHANNEL_MAXLEN) &&
            !strncmp(config->nick, status->nick, NICK_MAXLEN)) chnl_del();
    if (((!strncmp(status->chnl[0].name, status->channel, CHANNEL_MAXLEN) &&
        strncmp(config->nick, status->nick, NICK_MAXLEN)) ||
        (!config->my_only || !strncmp(status->room->name, status->channel, CHANNEL_MAXLEN))) &&
        (config->mode == MOD_NORMAL)) beep_join();
    refresh_room();
    screen_show();
    refresh_input();
    string_show();
}

void chat_join(void) {
    if (!chnl_getnum()) {
        if (status->channels == CHANNELS_MAX) { chnl_deny(); return; }
        chnl_add();
        if (strncmp(config->nick, status->nick, NICK_MAXLEN)) {
            strncpy(status->nick, config->nick, NICK_MAXLEN);
            strncpy(status->address, status->user[user_getnum()].addr, ADDR_SIZE);
            make_privatjoin();
            strncpy(status->nick, status->channel, NICK_MAXLEN);
            strncpy(status->address, status->user[user_getnum()].addr, ADDR_SIZE);
            make_privatjoin();
            strncpy(status->chnl[chnl_getnum()].addr, status->address, ADDR_SIZE);
        } else {
            strncpy(status->nick, status->channel, NICK_MAXLEN);
            strncpy(status->chnl[chnl_getnum()].addr, status->user[user_getnum()].addr, ADDR_SIZE);
            strncpy(status->nick, config->nick, NICK_MAXLEN);
        }
    }
    snprintf(message, MSG_SIZE, CHNL_JOINP, time_get(), status->nick);
    write_log(config->log_priv);
/* SERGEJ:    write_str(status->channel, COL_SYSTEM);*/
    if ((config->mode == MOD_NORMAL) || (config->mode == MOD_DND)) beep_join();
    refresh_room();
    screen_show();
    refresh_input();
    string_show();
}

void chat_rejoin(void) {
    if (!strncmp(config->nick, status->nick, NICK_MAXLEN) || chnl_getnum()) return;
    if (status->channels == CHANNELS_MAX) { chnl_deny(); return; }
    chnl_add();
    strncpy(status->nick, status->channel, NICK_MAXLEN);
    strncpy(status->address, status->user[user_getnum()].addr, ADDR_SIZE);
    strncpy(status->chnl[chnl_getnum()].addr, status->address, ADDR_SIZE);
    if ((config->mode == MOD_NORMAL) || (config->mode == MOD_DND)) beep_join();
    refresh_room();
    screen_show();
    refresh_input();
    string_show();
}

void chat_leave(void) {
    snprintf(message, MSG_SIZE, CHNL_LEAVEP, time_get(), status->nick);
    write_log(config->log_priv);
    write_str(status->channel, COL_SYSTEM);
    snprintf(message, MSG_SIZE, "\n");
    write_log(config->log_priv);
    if (chnl_getnum() && !strncmp(config->nick, status->nick, NICK_MAXLEN))
        chnl_del();
    if ((config->mode == MOD_NORMAL) || (config->mode == MOD_DND)) beep_join();
    refresh_room();
    screen_show();
    refresh_input();
    string_show();
}

void chat_nick(void) {
    if (!chnl_getnum()) return;
    strncpy(status->chnl[chnl_getnum()].name, status->nick, NICK_MAXLEN);
    refresh_input();
    string_show();
}
