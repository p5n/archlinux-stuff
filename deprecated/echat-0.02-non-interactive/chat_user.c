/*  chat_user.c
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#include <string.h>

#include <ctype.h>

#include "echat.h"

#define SORT_LEN        8


/* SERGEJ */
int get_num_send(void) {
    int n, m;
    char nb[8];

    n = m =0;
    while(buf[n] == ' ') n++;
    while(isdigit(buf[n]))
    {
        nb[m++] = buf[n++];
    }
    while(buf[n] == ' ') n++;
    buf += n;
    sscanf(nb, "%d", &n);

    return n;
}

/* SERGEJ */
void xreplace_nick(char *xnick)
{
    int i;
    int len = strlen(xnick);

    for(i=0;i<len;i++)
    {
        if(xnick[i]=='#')
        {
            xnick[i] = ' ';
        }
    }
}
/* SERGEJ */

void user_parse(void) {
    register int n, m;

    for (m = 0; m < status->users; m++) {
        for (n = 0; n < NICK_MAXLEN; n++)
            if (buf[n] != status->user[m].nick[n]) break;
        if ((buf[n] == CHR_NICK) && (n == strlen(status->user[m].nick))) {
            strncpy(status->nick, status->user[m].nick, NICK_MAXLEN);
            for (buf[n+1] = '\0'; n < STR_SIZE-n; n++) buf[n] = buf[n+1];
/* SERGEJ */
xreplace_nick(status->nick);
/* SERGEJ */
            return;
        }
        if (((buf[n] == ' ') || (buf[n] == '\0')) && (n == strlen(status->user[m].nick))) {
            strncpy(status->nick, status->user[m].nick, NICK_MAXLEN);
            buf[n] = '\0';
/* SERGEJ */
xreplace_nick(status->nick);
/* SERGEJ */
            return;
        }
        continue;
    }
    for (n = 0; n < NICK_MAXLEN; n++)
        if ((buf[n] == ' ') || (buf[n] == '\0')) { buf[n] = '\0'; break; }
    strncpy(status->nick, buf, NICK_MAXLEN);
/* SERGEJ */
xreplace_nick(status->nick);
/* SERGEJ */
}

void user_swap(struct user_t *user1, struct user_t *user2) {
    struct user_t swp;

    memcpy(&swp, user1, sizeof(struct user_t));
    memcpy(user1, user2, sizeof(struct user_t));
    memcpy(user2, &swp, sizeof(struct user_t));
}

void user_sort(void) {
    int fvr = 0;
    register int n, m, l;

    for (n = 0; n < status->users; n++)
        if (status->user[n].favorite) fvr++;
    for (m = 0; m < fvr; m++)
        for (n = fvr; n < status->users; n++)
            if (status->user[m].favorite) break;
            else if (status->user[n].favorite) {
                user_swap(&status->user[m], &status->user[n]); break;
            }
    for (l = SORT_LEN; l >= 0; l--)
        for (m = fvr; m < status->users; m++)
            for (n = fvr; n < status->users; n++)
                if (status->user_sort) {
                    if (status->user[m].nick[l] == '\0') break;
                    if ((unsigned char) status->user[m].nick[l] > (unsigned char) status->user[n].nick[l])
                        user_swap(&status->user[m], &status->user[n]);
                } else {
                    if (status->user[n].nick[l] == '\0') break;
                    if ((unsigned char) status->user[n].nick[l] > (unsigned char) status->user[m].nick[l])
                        user_swap(&status->user[m], &status->user[n]);
                }
}

void user_refresh(void) {
    user_sort();
    refresh_users();
    refresh_read();
}

int user_getnum(void) {
    register int n;

    for (n = 0; n < status->users; n++)
        if (!strncmp(status->user[n].nick, status->nick, NICK_MAXLEN)) return(n);
    for (n = 0; n < status->users; n++)
        if (!strncmp(status->user[n].nick, config->nick, NICK_MAXLEN)) return(n);
    return(-1);
}

void user_delfavorite(void) {
    register int n;

    for (n = 0; n < USERS_MAX-1; n++)
        if (!strncmp(status->favorite[n], status->nick, NICK_MAXLEN)) break;
    for (; n < USERS_MAX-1; n++)
        strncpy(status->favorite[n], status->favorite[n+1], NICK_MAXLEN);
    status->favorite[n][0] = '\0';
}

void user_addfavorite(void) {
    register int n;

    for (n = USERS_MAX-1; n > 0; n--)
        strncpy(status->favorite[n], status->favorite[n-1], NICK_MAXLEN);
    strncpy(status->favorite[0], status->nick, NICK_MAXLEN);
}

void user_parsfavorite(void) {                /* FIXME: /load command */
    char str[NICK_MAXLEN];
    register int n, m;

    for (n = 0; n < strlen(config->favorite); n++)
        for (m = 0; m < NICK_MAXLEN; m++, n++)
            if (((str[m] = config->favorite[n]) == CHR_NICK) || (str[m] == '\0')) {
                str[m] = '\0';
                strncpy(status->nick, str, NICK_MAXLEN);
                user_addfavorite();
                break;
            }
}

void user_delignore(void) {
    register int n;

    for (n = 0; n < USERS_MAX-1; n++)
        if (!strncmp(status->ignore[n], status->nick, NICK_MAXLEN)) break;
    for (; n < USERS_MAX-1; n++)
        strncpy(status->ignore[n], status->ignore[n+1], NICK_MAXLEN);
    status->ignore[n][0] = '\0';
}

void user_addignore(void) {
    register int n;

    for (n = USERS_MAX-1; n > 0; n--)
        strncpy(status->ignore[n], status->ignore[n-1], NICK_MAXLEN);
    strncpy(status->ignore[0], status->nick, NICK_MAXLEN);
}

void user_parsignore(void) {                /* FIXME: /load command */
    char str[NICK_MAXLEN];
    register int n, m;

    for (n = 0; n < strlen(config->ignore); n++)
        for (m = 0; m < NICK_MAXLEN; m++, n++)
            if (((str[m] = config->ignore[n]) == CHR_NICK) || (str[m] == '\0')) {
                str[m] = '\0';
                strncpy(status->nick, str, NICK_MAXLEN);
                user_addignore();
                break;
            }
}

void user_delban(void) {
    register int n;

    for (n = 0; n < USERS_MAX-1; n++)
        if (!strncmp(status->ban[n], status->address, ADDR_SIZE)) break;
    for (; n < USERS_MAX-1; n++)
        strncpy(status->ban[n], status->ban[n+1], ADDR_SIZE);
    status->ban[n][0] = '\0';
}

void user_addban(void) {
    register int n;

    for (n = USERS_MAX-1; n > 0; n--)
        strncpy(status->ban[n], status->ban[n-1], ADDR_SIZE);
    strncpy(status->ban[0], status->address, ADDR_SIZE);
}

void user_parsban(void) {                /* FIXME: /load command */
    char str[ADDR_SIZE+1];
    register int n, m;

    for (n = 0; n < strlen(config->ban); n++)
        for (m = 0; m < ADDR_SIZE+1; m++, n++)
            if (((str[m] = config->ban[n]) == ' ') || (str[m] == '\0')) {
                str[m] = '\0';
                strncpy(status->address, str, ADDR_SIZE);
                user_addban();
                break;
            }
}

int user_setban(void) {
    register int n;

    strncpy(status->address, status->user[user_getnum()].addr, ADDR_SIZE);
    for (n = 0; n < USERS_MAX; n++)
        if (!strncmp(status->ban[n], status->address, ADDR_SIZE)) return 1;
    return 0;
}

int user_setfavorite(void) {
    register int n;

    for (n = 0; n < USERS_MAX; n++)
        if (!strncmp(status->favorite[n], status->nick, NICK_MAXLEN)) return 1;
    return 0;
}

int user_favorite(void) {
    if (user_getnum() < 0) return 0;
    return status->user[user_getnum()].favorite;
}

int user_setignore(void) {
    register int n;

    if (user_setban()) return 1;
    for (n = 0; n < USERS_MAX; n++)
        if (!strncmp(status->ignore[n], status->nick, NICK_MAXLEN)) return 1;
    return 0;
}

int user_ignore(void) {
    if (user_getnum() < 0) return 0;
    return status->user[user_getnum()].ignore;
}

void user_add(void) {                /* FIXME: nick collisions */
    register int n;

    for (n = 0; n < status->users; n++)
        if (!strncmp(status->user[n].nick, status->nick, NICK_MAXLEN)) break;
    if ((n == status->users) && (n < USERS_MAX-1)) status->users++;
    strncpy(status->user[n].nick, status->nick, NICK_MAXLEN);
    strncpy(status->user[n].addr, status->address, ADDR_SIZE);
    status->user[n].mode = status->mode;
    status->user[n].active = status->active;
    status->user[n].favorite = user_setfavorite();
    status->user[n].ignore = user_setignore();
    user_refresh();
}

void user_del(void) {
    register int n;

    if ((n = user_getnum()) < 0) return;
    status->mode = status->user[n].mode;
    status->active = status->user[n].active;
    for (; n < status->users; n++)
        memcpy(&status->user[n], &status->user[n+1], sizeof(struct user_t));
    status->users--;
    user_refresh();
}

void user_rescan(void) {
    for (; status->users > 0;) {
        strncpy(status->nick, status->user[status->users-1].nick, NICK_MAXLEN);
        user_del();
    }
    make_whohere();
}

void user_nick(void) {
    register int n;

    for (n = 0; n < status->users; n++) {
        if (!strncmp(status->user[n].nick, status->channel, NICK_MAXLEN)) {
            strncpy(status->user[n].nick, status->nick, NICK_MAXLEN);
            status->user[n].favorite = user_setfavorite();
            status->user[n].ignore = user_setignore();
            break;
        }
    }
    user_refresh();
}

void user_mode(void) {
    status->user[user_getnum()].mode = status->mode;
    refresh_room();
    screen_show();
    user_refresh();
}

void user_active(void) {
    status->user[user_getnum()].active = status->active;
    user_refresh();
}
