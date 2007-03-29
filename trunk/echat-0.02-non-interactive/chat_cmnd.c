/*  chat_cmnd.c
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#include <string.h>
#include <unistd.h>
#include "echat.h"
#include "chat_cmnd.h"

void cmnd_next(void) {
    register int n, m;

    for (n = 0; n < STR_SIZE; n++) if (buf[n] == '\0') break;
    for (m = ++n, n = 0; m < STR_SIZE-NICK_MAXLEN; n++, m++) buf[n] = buf[m];
}

void cmnd_help(void) {
    if (!strncasecmp(buf, CMD_QUIT, CMD_MAXLEN) || !strncasecmp(buf, CMD_QUIT1, CMD_MAXLEN) || !strncasecmp(buf, CMD_QUIT2, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HQUIT, time_get(), CMD_QUIT, CMD_QUIT1, CMD_QUIT2, NAME);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_HELP, CMD_MAXLEN) || !strncasecmp(buf, CMD_HELP1, CMD_MAXLEN) || !strncasecmp(buf, CMD_HELP2, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HHELP, time_get(), CMD_HELP, CMD_HELP1, CMD_HELP2);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_VERSION, CMD_MAXLEN) || !strncasecmp(buf, CMD_VERSION1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HVERSION, time_get(), CMD_VERSION, CMD_VERSION1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_STATUS, CMD_MAXLEN) || !strncasecmp(buf, CMD_STATUS1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HSTATUS, time_get(), CMD_STATUS, CMD_STATUS1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_KEYS, CMD_MAXLEN) || !strncasecmp(buf, CMD_KEYS1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HKEYS, time_get(), CMD_KEYS, CMD_KEYS1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_VERBOSE, CMD_MAXLEN) || !strncasecmp(buf, CMD_VERBOSE1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HVERBOSE, time_get(), CMD_VERBOSE, CMD_VERBOSE1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_CLEAR, CMD_MAXLEN) || !strncasecmp(buf, CMD_CLEAR1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HCLEAR, time_get(), CMD_CLEAR, CMD_CLEAR1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_SHELL, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HSHELL, time_get(), CMD_SHELL);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_LOAD, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HLOAD, time_get(), CMD_LOAD);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_SAVE, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HSAVE, time_get(), CMD_SAVE);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }

    if (!strncasecmp(buf, CMD_ANSWER, CMD_MAXLEN) || !strncasecmp(buf, CMD_ANSWER1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HANSWER, time_get(), CMD_ANSWER, CMD_ANSWER1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_GENDER, CMD_MAXLEN) || !strncasecmp(buf, CMD_GENDER1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HGENDER, time_get(), CMD_GENDER, CMD_GENDER1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_MY, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HMY, time_get(), CMD_MY);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_IGNORE, CMD_MAXLEN) || !strncasecmp(buf, CMD_IGNORE1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HIGNORE, time_get(), CMD_IGNORE, CMD_IGNORE1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_FAVORITE, CMD_MAXLEN) || !strncasecmp(buf, CMD_FAVORITE1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HFAVORITE, time_get(), CMD_FAVORITE, CMD_FAVORITE1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_BAN, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HBAN, time_get(), CMD_BAN);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_JOIN, CMD_MAXLEN) || !strncasecmp(buf, CMD_JOIN1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HJOIN, time_get(), CMD_JOIN, CMD_JOIN1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_LEAVE, CMD_MAXLEN) || !strncasecmp(buf, CMD_LEAVE1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HLEAVE, time_get(), CMD_LEAVE, CMD_LEAVE1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }

    if (!strncasecmp(buf, CMD_NICK, CMD_MAXLEN) || !strncasecmp(buf, CMD_NICK1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HNICK, time_get(), CMD_NICK, CMD_NICK1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_MESG, CMD_MAXLEN) || !strncasecmp(buf, CMD_MESG1, CMD_MAXLEN) || !strncasecmp(buf, CMD_MESG2, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HMESG, time_get(), CMD_MESG, CMD_MESG1, CMD_MESG2);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_MASS, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HMASS, time_get(), CMD_MASS);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_EXEC, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HEXEC, time_get(), CMD_EXEC);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_ME, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HME, time_get(), CMD_ME);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_TOPIC, CMD_MAXLEN) || !strncasecmp(buf, CMD_TOPIC1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HTOPIC, time_get(), CMD_TOPIC, CMD_TOPIC1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_ADD, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HADD, time_get(), CMD_ADD);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_MODE, CMD_MAXLEN) || !strncasecmp(buf, CMD_MODE1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HMODE, time_get(), CMD_MODE, CMD_MODE1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_NORMAL, CMD_MAXLEN) || !strncasecmp(buf, CMD_NORMAL1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HNORMAL, time_get(), CMD_NORMAL, CMD_NORMAL1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_DND, CMD_MAXLEN) || !strncasecmp(buf, CMD_DND1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HDND, time_get(), CMD_DND, CMD_DND1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_AWAY, CMD_MAXLEN) || !strncasecmp(buf, CMD_AWAY1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HAWAY, time_get(), CMD_AWAY, CMD_AWAY1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_OFF, CMD_MAXLEN) || !strncasecmp(buf, CMD_OFF1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HOFF, time_get(), CMD_OFF, CMD_OFF1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_INFO, CMD_MAXLEN) || !strncasecmp(buf, CMD_INFO1, CMD_MAXLEN) || !strncasecmp(buf, CMD_INFO2, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HINFO, time_get(), CMD_INFO, CMD_INFO1, CMD_INFO2);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_BEEP, CMD_MAXLEN) || !strncasecmp(buf, CMD_BEEP1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HBEEP, time_get(), CMD_BEEP, CMD_BEEP1);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_TALK, CMD_MAXLEN) || !strncasecmp(buf, CMD_TALK1, CMD_MAXLEN) || !strncasecmp(buf, CMD_TALK2, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HTALK, time_get(), CMD_TALK, CMD_TALK1, CMD_TALK2);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_HERE, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HHERE, time_get(), CMD_HERE);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }
    if (!strncasecmp(buf, CMD_ACTIVE, CMD_MAXLEN) || !strncasecmp(buf, CMD_ACTIVE1, CMD_MAXLEN)) {
        snprintf(message, STR_SIZE, CMND_HACTIVE, time_get(), CMD_ACTIVE);
        write_str(status->room->name, COL_SYSTEM);
        return;
    }

    snprintf(message, STR_SIZE, CMND_HCOMMANDS, time_get());
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, "%s(%s,%s) %s(%s,%s) %s(%s) %s(%s) %s(%s) %s(%s) %s(%s) %s",
        CMD_QUIT, CMD_QUIT1, CMD_QUIT2, CMD_HELP, CMD_HELP1, CMD_HELP2, CMD_VERSION, CMD_VERSION1,
        CMD_STATUS, CMD_STATUS1, CMD_KEYS, CMD_KEYS1, CMD_VERBOSE, CMD_VERBOSE1, CMD_CLEAR, CMD_CLEAR1, CMD_SHELL);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, "%s %s %s(%s) %s(%s) %s %s(%s) %s(%s) %s",
        CMD_LOAD, CMD_SAVE, CMD_ANSWER, CMD_ANSWER1, CMD_GENDER, CMD_GENDER1, CMD_MY,
        CMD_FAVORITE, CMD_FAVORITE1, CMD_IGNORE, CMD_IGNORE1, CMD_BAN);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, "%s(%s) %s(%s) %s(%s) %s(%s,%s) %s %s %s %s(%s) %s",
        CMD_JOIN, CMD_JOIN1, CMD_LEAVE, CMD_LEAVE1, CMD_NICK, CMD_NICK1, CMD_MESG, CMD_MESG1, CMD_MESG2, CMD_MASS, CMD_EXEC,
        CMD_ME, CMD_TOPIC, CMD_TOPIC1, CMD_ADD);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, "%s(%s) %s(%s) %s(%s) %s(%s) %s(%s) %s(%s,%s) %s(%s)",
        CMD_MODE, CMD_MODE1, CMD_NORMAL, CMD_NORMAL1, CMD_DND, CMD_DND1, CMD_AWAY, CMD_AWAY1, CMD_OFF, CMD_OFF1,
        CMD_INFO, CMD_INFO1, CMD_INFO2, CMD_BEEP, CMD_BEEP1);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, "%s(%s,%s) %s %s\n",
        CMD_TALK, CMD_TALK1, CMD_TALK2, CMD_HERE, CMD_ACTIVE);
    write_str(status->room->name, COL_SYSTEM);

    /* SERGEJ */
    snprintf(message, STR_SIZE, "%s <count> <sleep> <user> <text>\n", CMD_NMESG);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, "%s\n", CMD_CHAN);
    write_str(status->room->name, COL_SYSTEM);
    /* SERGEJ */
}

void cmnd_keys(void) {
    snprintf(message, STR_SIZE, CMND_HKEYWORK, time_get());
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, CMND_HKEY1);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, CMND_HKEY2);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, CMND_HKEY3);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, CMND_HKEY4);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, CMND_HKEY5);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, CMND_HKEY6);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, CMND_HKEY7);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, CMND_HKEY8);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, CMND_HKEY9);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, CMND_HKEY0);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, CMND_HKEYA);
    write_str(status->room->name, COL_SYSTEM);
#ifdef  CHARSET
    snprintf(message, STR_SIZE, CMND_HKEYB);
    write_str(status->room->name, COL_SYSTEM);
#endif  /* CHARSET */
    snprintf(message, STR_SIZE, CMND_HKEYC);
    write_str(status->room->name, COL_SYSTEM);
    snprintf(message, STR_SIZE, CMND_HKEYD);
    write_str(status->room->name, COL_SYSTEM);
}

void cmnd_version(void) {
    snprintf(message, STR_SIZE, CMND_VERSION, time_get(), NAME, VERSION);
    write_str(status->room->name, COL_SYSTEM);
}

void cmnd_status(void) {
    snprintf(message, STR_SIZE, CMND_STATUS, time_get());
    write_str(status->room->name, COL_SYSTEM);
    if (config->packet_qchat) strncpy(buf, "qChat", STR_SIZE);
    else {
        if (config->packet_xor) strncpy(buf, "VyChatXOR", STR_SIZE);
        else strncpy(buf, "VyChat", STR_SIZE);
    }
    if (config->packet_bcast && config->packet_route) strncat(buf, " (bcast,route)", STR_SIZE);
    else if (config->packet_bcast) strncat(buf, " (bcast)", STR_SIZE);
    else if (config->packet_route) strncat(buf, " (route)", STR_SIZE);
    snprintf(message, STR_SIZE, CMND_STATUSA, config->address, config->port, buf);
    write_str(status->room->name, COL_SYSTEM);
#ifdef  TCP
    if (status->clients) {
        snprintf(message, STR_SIZE, CMND_STATUSL, status->clients);
        write_str(status->room->name, COL_SYSTEM);
    }
#endif  /* TCP */
}

void cmnd_verbose(void) {
    config->verbose = !config->verbose;
    if (config->verbose) snprintf(message, MSG_SIZE, CMND_VERBOSEON, time_get());
    else snprintf(message, MSG_SIZE, CMND_VERBOSEOFF, time_get());
    write_str(status->room->name, COL_SYSTEM);
}

void cmnd_clear(void) {
    register int n;

    for (n = 0; n < HIST_SIZE; n++) {
        status->history[n][0] = '\0';
        status->history[n][1] = '\0';
    }
    status->hist = status->strn = status->strn_end = status->strn_cur = 0;
    for (n = 0; n < SCRN_SIZE; n++) {
        status->room->screen[n][0] = '\0';
        status->room->screen[n][1] = '\0';
    }
    status->room->scrn = status->room->scrn_top = 0;
    snprintf(message, MSG_SIZE, CMND_CLEAR, time_get());
    status_ok(CMND_SCLEAR);
    if (config->verbose) write_str(status->room->name, COL_SYSTEM);
}

void cmnd_shell(void) {
#ifdef  SHELL
    int pid;

    if (*buf != '\0') strncpy(config->shell, buf, PATH_SIZE);
    if ((pid = fork())) {
        if (pid > 0) {
            snprintf(message, MSG_SIZE, CMND_SHELL, time_get());
            write_str(status->room->name, COL_SYSTEM);
            snprintf(message, MSG_SIZE, "%s\n", config->shell);
            write_str(status->room->name, COL_SYSTEM);
        } else {
            snprintf(message, MSG_SIZE, CMND_SHELLE, time_get());
            write_str(status->room->name, COL_ERROR);
        }
    } else execlp(config->shell, buf, NULL);
#else   /* SHELL */
    snprintf(message, STR_SIZE, CMND_SHELLD, time_get());
    write_str(status->room->name, COL_ERROR);
#endif  /* SHELL */
}

void cmnd_load(void) {
    if (*buf == '\0') snprintf(config->file, STR_SIZE, "~.%s", CNF_ECHATRC);
    else strncpy(config->file, buf, PATH_SIZE);
    if (*config->file == '~') {
        snprintf(buf, PATH_SIZE, "%s/%s", getenv("HOME"), &config->file[1]);
        strncpy(config->file, buf, PATH_SIZE);
    }
    term_kill();
    sock_kill();
    if (conf_read()) {
        term_init();
        window_init();
        snprintf(message, MSG_SIZE, CMND_LOAD, time_get());
        write_str(status->room->name, COL_SYSTEM);
        snprintf(message, MSG_SIZE, "%s\n", config->file);
        write_str(status->room->name, COL_SYSTEM);
        status_ok(CMND_SLOAD);
    } else {
        term_init();
        window_init();
        snprintf(message, MSG_SIZE, CMND_LOADF, time_get());
        write_str(status->room->name, COL_ERROR);
        status_error(CMND_SLOADF);
    }
    addr_parse();
    user_parsfavorite();
    user_parsignore();
    user_parsban();
    sock_init();
    addr_init();
    window_refresh();
    user_rescan();
}

void cmnd_save(void) {
    if (*buf == '\0') snprintf(config->file, STR_SIZE, "~.%s", CNF_ECHATRC);
    else strncpy(config->file, buf, PATH_SIZE);
    if (*config->file == '~') {
        snprintf(buf, PATH_SIZE, "%s/%s", getenv("HOME"), &config->file[1]);
        strncpy(config->file, buf, PATH_SIZE);
    }
    if (conf_write()) {
        snprintf(message, MSG_SIZE, CMND_SAVEF, time_get());
        write_str(status->room->name, COL_SYSTEM);
        snprintf(message, MSG_SIZE, "%s\n", config->file);
        write_str(status->room->name, COL_SYSTEM);
        status_ok(CMND_SSAVE);
    } else {
        snprintf(message, MSG_SIZE, CMND_SAVEF, time_get());
        write_str(status->room->name, COL_ERROR);
        status_error(CMND_SSAVEF);
    }
}

void cmnd_answer(void) {
    if (status->room->name[0] != CHR_CHNL) return;
    switch (config->mode) {
        case MOD_DND:
            if (*buf != '\0') strncpy(config->answer_dnd, buf, STR_SIZE);
            snprintf(message, MSG_SIZE, CMND_ANSDND, time_get());
            write_str(status->room->name, COL_SYSTEM);
            snprintf(message, MSG_SIZE, "%s\n", config->answer_dnd);
            write_str(status->room->name, COL_SYSTEM); break;
        case MOD_AWAY:
            if (*buf != '\0') strncpy(config->answer_away, buf, STR_SIZE);
            snprintf(message, MSG_SIZE, CMND_ANSAWAY, time_get());
            write_str(status->room->name, COL_SYSTEM);
            snprintf(message, MSG_SIZE, "%s\n", config->answer_away);
            write_str(status->room->name, COL_SYSTEM); break;
        case MOD_OFF:
            if (*buf != '\0') strncpy(config->answer_off, buf, STR_SIZE);
            snprintf(message, MSG_SIZE, CMND_ANSOFF, time_get());
            write_str(status->room->name, COL_SYSTEM);
            snprintf(message, MSG_SIZE, "%s\n", config->answer_off);
            write_str(status->room->name, COL_SYSTEM); break;
        default: break;
    }
}

void cmnd_gender(void) {
    if (config->gender) {
        config->gender = 0;
        snprintf(message, MSG_SIZE, CMND_GENDERM, time_get());
        status_ok(CMND_SGENDERM);
    } else {
        config->gender = 1;
        snprintf(message, MSG_SIZE, CMND_GENDERF, time_get());
        status_ok(CMND_SGENDERF);
    }
    if (config->verbose) write_str(status->room->name, COL_SYSTEM);
}

void cmnd_my(void) {
    if (config->my_only) {
        config->my_only = 0;
        snprintf(message, MSG_SIZE, CMND_BEEPALL, time_get());
        status_ok(CMND_SBEEPALL);
    } else {
        config->my_only = 1;
        snprintf(message, MSG_SIZE, CMND_BEEPCUR, time_get());
        status_ok(CMND_SBEEPCUR);
    }
    if (config->verbose) write_str(status->room->name, COL_SYSTEM);
}

void cmnd_favorite(void) {
    register int n;

    if ((*buf == '\0') || ((buf[0] == ' ') && (buf[1] == '\0'))) {
        snprintf(message, MSG_SIZE, CMND_FAVORITEN, time_get());
        write_str(status->room->name, COL_SYSTEM);
        if (buf[0] == ' ') for (n = 0; n < USERS_MAX; status->favorite[n++][0] = '\0');
        else {
            *message = '\0';
            for (n = 0; n < USERS_MAX; n++) {
                if (status->favorite[n][0] == '\0') break;
                strncat(message, status->favorite[n], NICK_MAXLEN);
                strncat(message, ": ", 2);
            }
            strncat(message, "\n", 1);
            write_str(status->room->name, COL_SYSTEM);
        }
    } else while (*buf != '\0') {
        user_parse();
        cmnd_next();
        for (n = 0; n < USERS_MAX; n++)
            if (!strncmp(status->favorite[n], status->nick, NICK_MAXLEN)) break;
        if (n == USERS_MAX) {
            user_addfavorite();
            snprintf(message, MSG_SIZE, CMND_FAVORITEI, time_get(), status->nick);
        } else {
            user_delfavorite();
            snprintf(message, MSG_SIZE, CMND_FAVORITEU, time_get(), status->nick);
        }
        if (config->verbose) write_str(status->room->name, COL_SYSTEM);
        status->user[user_getnum()].favorite = user_setfavorite();
        user_refresh();
    }
}

void cmnd_ignore(void) {
    register int n;

    if ((*buf == '\0') || ((buf[0] == ' ') && (buf[1] == '\0'))) {
        snprintf(message, MSG_SIZE, CMND_IGNOREN, time_get());
        write_str(status->room->name, COL_SYSTEM);
        if (buf[0] == ' ') for (n = 0; n < USERS_MAX; status->ignore[n++][0] = '\0');
        else {
            *message = '\0';
            for (n = 0; n < USERS_MAX; n++) {
                if (status->ignore[n][0] == '\0') break;
                strncat(message, status->ignore[n], NICK_MAXLEN);
                strncat(message, ": ", 2);
            }
            strncat(message, "\n", 1);
            write_str(status->room->name, COL_SYSTEM);
        }
    } else while (*buf != '\0') {
        user_parse();
        cmnd_next();
        for (n = 0; n < USERS_MAX; n++)
            if (!strncmp(status->ignore[n], status->nick, NICK_MAXLEN)) break;
        if (n == USERS_MAX) {
            user_addignore();
            snprintf(message, MSG_SIZE, CMND_IGNOREI, time_get(), status->nick);
        } else {
            user_delignore();
            snprintf(message, MSG_SIZE, CMND_IGNOREU, time_get(), status->nick);
        }
        if (config->verbose) write_str(status->room->name, COL_SYSTEM);
        status->user[user_getnum()].ignore = user_setignore();
        user_refresh();
    }
}

void cmnd_ban(void) {
    register int n;

    if ((*buf == '\0') || ((buf[0] == ' ') && (buf[1] == '\0'))) {
        snprintf(message, MSG_SIZE, CMND_BANA, time_get());
        write_str(status->room->name, COL_SYSTEM);
        if (buf[0] == ' ') for (n = 0; n < USERS_MAX; status->ban[n++][0] = '\0');
        else {
            *message = '\0';
            for (n = 0; n < USERS_MAX; n++) {
                if (status->ban[n][0] == '\0') break;
                strncat(message, status->ban[n], NICK_MAXLEN);
                strncat(message, " ", 1);
            }
            strncat(message, "\n", 1);
            write_str(status->room->name, COL_SYSTEM);
        }
    } else while (*buf != '\0') {
        user_parse();
        cmnd_next();
        if (user_getnum() < 0) {
            snprintf(message, MSG_SIZE, CMND_INICK, time_get(), status->nick);
            write_str(status->room->name, COL_ERROR);
            continue;
        }
        strncpy(status->address, status->user[user_getnum()].addr, ADDR_SIZE);
        for (n = 0; n < USERS_MAX; n++)
            if (!strncmp(status->ban[n], status->address, ADDR_SIZE)) break;
        if (n == USERS_MAX) {
            user_addban();
            snprintf(message, MSG_SIZE, CMND_BANB, time_get(), status->address);
        } else {
            user_delban();
            snprintf(message, MSG_SIZE, CMND_BANU, time_get(), status->address);
        }
        if (config->verbose) write_str(status->room->name, COL_SYSTEM);
        status->user[user_getnum()].ignore = user_setignore();
        refresh_users();
    }
}

void cmnd_chat(void) {
    if (status->room->name[0] == CHR_CHNL) make_chat();
    else {
        strncpy(status->channel, status->room->name, NICK_MAXLEN);
        strncpy(status->address, status->room->addr, ADDR_SIZE);
        make_privatchat();
        snprintf(message, MSG_SIZE, "<%s> %s\n", config->nick, buf);
        write_log(config->log_priv);
        write_str(status->channel, COL_MY);
        if (config->mode == MOD_NORMAL) beep_chat();
    }
}

void cmnd_join(void) {
    if (*buf == '\0') return;
    if ((strchr(buf, CHR_NICK) != NULL) ||
        (strchr(buf, CHR_CHNL) != NULL) ||
        (strchr(buf, CHR_RESTR1) != NULL) ||
        (strchr(buf, CHR_RESTR2) != NULL) ||
        (strchr(buf, CHR_RESTR3) != NULL) ||
        (strchr(buf, CHR_RESTR4) != NULL) ||
        (strchr(buf, CHR_RESTR5) != NULL) ||
        (strchr(buf, CHR_RESTR6) != NULL) ||
        (strchr(buf, CHR_RESTR7) != NULL) ||
        (strchr(buf, CHR_RESTR8) != NULL)) {
        snprintf(message, MSG_SIZE, CMND_ICHANNEL, time_get(), buf);
        write_str(status->room->name, COL_ERROR);
        return;
    }
    if (buf[0] != CHR_CHNL) status->channel[0] = CHR_CHNL;
    else status->channel[0] = '\0';
    status->channel[1] = '\0';
    strncat(status->channel, buf, CHANNEL_MAXLEN-1);
    make_join();
}

void cmnd_leave(void) {
    if (*buf == '\0') strncpy(status->channel, status->room->name, CHANNEL_MAXLEN);
    else strncpy(status->channel, buf, CHANNEL_MAXLEN);
    if (!strncmp(status->chnl[0].name, status->channel, CHANNEL_MAXLEN)) {
        snprintf(message, MSG_SIZE, CMND_LEAVE, time_get(), status->channel);
        write_str(status->room->name, COL_ERROR);
        return;
    }
    if (status->channel[0] != CHR_CHNL) {
        strncpy(status->nick, status->channel, NICK_MAXLEN);
        strncpy(status->address, status->user[user_getnum()].addr, ADDR_SIZE);
        make_privatleave();
        strncpy(status->nick, config->nick, NICK_MAXLEN);
        strncpy(status->address, status->user[user_getnum()].addr, ADDR_SIZE);
        make_privatleave();
    } else make_leave();
}

void cmnd_nick(void) {
    register int n;

    if (*buf == '\0') return;
    if ((strchr(buf, CHR_CHNL) != NULL) ||
        (strchr(buf, CHR_NICK) != NULL) ||
        (strchr(buf, CHR_RESTR1) != NULL) ||
        (strchr(buf, CHR_RESTR2) != NULL) ||
        (strchr(buf, CHR_RESTR3) != NULL) ||
        (strchr(buf, CHR_RESTR4) != NULL) ||
        (strchr(buf, CHR_RESTR5) != NULL) ||
        (strchr(buf, CHR_RESTR5) != NULL) ||
        (strchr(buf, CHR_RESTR6) != NULL) ||
        (strchr(buf, CHR_RESTR7) != NULL) ||
        (strchr(buf, CHR_RESTR8) != NULL)) {
        snprintf(message, MSG_SIZE, CMND_INICK, time_get(), buf);
        write_str(status->room->name, COL_ERROR);
        return;
    }
    strncpy(status->nick, buf, NICK_MAXLEN);
    for (n = 0; n < status->users; n++)
        if (!strncmp(status->user[n].nick, status->nick, NICK_MAXLEN))
    if (n != status->users) {
        snprintf(message, MSG_SIZE, CMND_NICK, time_get(), status->nick);
        write_str(status->room->name, COL_ERROR);
        return;
    }
    strncpy(status->nick, config->nick, NICK_MAXLEN);
    strncpy(config->nick, buf, NICK_MAXLEN);
    make_nick();
}

void cmnd_mesg(void) {
    if (*buf == '\0') return;
    user_parse();
    cmnd_next();
    strncpy(status->address, status->user[user_getnum()].addr, ADDR_SIZE);
    make_mesg();
    snprintf(message, MSG_SIZE, CMND_MESG, time_get(), status->nick);
    write_log(config->log_mesg);
    snprintf(message, MSG_SIZE, "%s\n", buf);
    write_log(config->log_mesg);
}

void cmnd_nmesg(void) {
    int i, n, s;
    char *saved_buf = buf;

    if (*buf == '\0') return;
    n = get_num_send();
    s = get_num_send();
    user_parse();
    cmnd_next();
    strncpy(status->address, status->user[user_getnum()].addr, ADDR_SIZE);
/* SERGEJ */ 
    for(i=0;i<n;i++)
    {    
        make_mesg();
        usleep(s*1000);
    }

    buf = saved_buf;

    snprintf(message, MSG_SIZE, CMND_MESG, time_get(), status->nick);
    write_log(config->log_mesg);
    snprintf(message, MSG_SIZE, "%s\n", buf);
    write_log(config->log_mesg);
}

void cmnd_mass(void) {
    register int n;

    if (*buf == '\0') return;
    for (n = 0; n < status->users; n++) {
        strncpy(status->nick, status->user[n].nick, NICK_MAXLEN);
        strncpy(status->address, status->user[n].addr, ADDR_SIZE);
        if (strncmp(config->nick, status->nick, NICK_MAXLEN)) make_mass();
    }
    snprintf(message, MSG_SIZE, CMND_MASS, time_get());
    write_log(config->log_mesg);
    snprintf(message, MSG_SIZE, "%s\n", buf);
    write_log(config->log_mesg);
}

void cmnd_exec(void) {
    if (*buf == '\0') return;
    user_parse();
    cmnd_next();
    strncpy(status->address, status->user[user_getnum()].addr, ADDR_SIZE);
    make_exec();
}

void cmnd_me(void) {
    if (*buf == '\0') strncpy(buf, config->me_action, STR_SIZE);
    if (status->room->name[0] == CHR_CHNL) make_me();
    else {
        strncpy(status->channel, status->room->name, CHANNEL_MAXLEN);
        strncpy(status->address, status->room->addr, ADDR_SIZE);
        make_privatme();
        snprintf(message, MSG_SIZE, " * %s %s\n", config->nick, buf);
        write_log(config->log_priv);
        write_str(status->channel, COL_MY);
        if (config->mode == MOD_NORMAL) beep_chat();
    }
}

void cmnd_topic(void) {
    int len = STR_SIZE - (strlen(config->nick)+4);

    if (status->room->name[0] != CHR_CHNL) return;
    if (config->packet_qchat) {
        if (*buf == '\0') {
            snprintf(message, MSG_SIZE, CMND_QTOPIC, time_get());
            write_str(status->room->name, COL_SYSTEM);
            snprintf(message, MSG_SIZE, "%s\n", status->chnl[0].topic);
            write_str(status->room->name, COL_SYSTEM);
        } else if ((buf[0] == ' ') && (buf[1] == '\0')) {
            status->chnl[0].topic[0] = '\0';
            make_newtopic();
        } else {
            snprintf(status->chnl[0].topic, len, "%s", buf);
/* SERGEJ
            strncat(status->chnl[0].topic, " (", 2);
            strncat(status->chnl[0].topic, config->nick, NICK_MAXLEN);
            strncat(status->chnl[0].topic, ")", 1);
SERGEJ */
            make_newtopic();
        }
    } else {
        if (*buf == '\0') {
            snprintf(message, MSG_SIZE, CMND_TOPIC, time_get(), status->room->name);
            write_str(status->room->name, COL_SYSTEM);
            snprintf(message, MSG_SIZE, "%s\n", status->room->topic);
            write_str(status->room->name, COL_SYSTEM);
        } else if ((buf[0] == ' ') && (buf[1] == '\0')) {
            status->room->topic[0] = '\0';
            make_newtopic();
        } else {
            snprintf(status->room->topic, len, "%s", buf);
/* SERGEJ
            strncat(status->room->topic, " (", 2);
            strncat(status->room->topic, config->nick, NICK_MAXLEN);
            strncat(status->room->topic, ")", 1);
SERGEJ */
            make_newtopic();
        }
    }
}

void cmnd_add(void) {
    int len;

    if (*status->room->name != CHR_CHNL) return;
    if (config->packet_qchat) {
        if ((len = STR_SIZE - (strlen(status->chnl[0].topic)+strlen(config->nick)+4)) < 0) return;
        if (*buf == '\0') {
            strncat(status->chnl[0].topic, " (", 2);
            strncat(status->chnl[0].topic, config->nick, NICK_MAXLEN);
            strncat(status->chnl[0].topic, ")", 1);
            make_newtopic();
        } else {
            strncat(status->chnl[0].topic, " ", 1);
            strncat(status->chnl[0].topic, buf, len);
            strncat(status->chnl[0].topic, " (", 2);
            strncat(status->chnl[0].topic, config->nick, NICK_MAXLEN);
            strncat(status->chnl[0].topic, ")", 1);
            make_newtopic();
        }
    } else {
        if ((len = STR_SIZE - (strlen(status->room->topic)+strlen(config->nick)+4)) < 0) return;
        if (*buf == '\0') {
            strncat(status->room->topic, " (", 2);
            strncat(status->room->topic, config->nick, NICK_MAXLEN);
            strncat(status->room->topic, ")", 1);
            make_newtopic();
        } else {
            strncat(status->room->topic, " ", 1);
            strncat(status->room->topic, buf, len);
            strncat(status->room->topic, " (", 2);
            strncat(status->room->topic, config->nick, NICK_MAXLEN);
            strncat(status->room->topic, ")", 1);
            make_newtopic();
        }
    }
}

void cmnd_mode(void) {
    if ((*buf == '\0') ||
        (((config->mode = atoi(buf)) != MOD_DND) &&
        (config->mode != MOD_AWAY) &&
        (config->mode != MOD_OFF))) config->mode = 0;
    make_mode();
}

void cmnd_normal(void) {
    config->mode = MOD_NORMAL;
    make_mode();
}

void cmnd_dnd(void) {
    if ((buf[0] == ' ') && (buf[1] == '\0')) config->answer_dnd[0] = '\0';
    else if (*buf != '\0') strncpy(config->answer_dnd, buf, STR_SIZE);
    config->mode = MOD_DND;
    make_mode();
}

void cmnd_away(void) {
    if ((buf[0] == ' ') && (buf[1] == '\0')) config->answer_away[0] = '\0';
    else if (*buf != '\0') strncpy(config->answer_away, buf, STR_SIZE);
    config->mode = MOD_AWAY;
    make_mode();
}

void cmnd_off(void) {
    if ((buf[0] == ' ') && (buf[1] == '\0')) config->answer_off[0] = '\0';
    else if (*buf != '\0') strncpy(config->answer_off, buf, STR_SIZE);
    config->mode = MOD_OFF;
    make_mode();
}

void cmnd_info(void) {
    if (*buf == '\0') {
        strncpy(status->nick, config->nick, NICK_MAXLEN);
        make_info();
    } else {
        while (*buf != '\0') {
            user_parse();
            cmnd_next();
            strncpy(status->address, status->user[user_getnum()].addr, ADDR_SIZE);
            make_info();
        }
    }
}

void cmnd_beep(void) {
    if (*buf == '\0') {
        strncpy(status->nick, config->nick, NICK_MAXLEN);
        make_beepbeep();
    } else {
        while (*buf != '\0') {
            user_parse();
            cmnd_next();
            strncpy(status->address, status->user[user_getnum()].addr, ADDR_SIZE);
            make_beepbeep();
        }
    }
}

void cmnd_talk(void) {
    if (*buf == '\0') return;
    user_parse();
    strncpy(status->channel, status->nick, NICK_MAXLEN);
    strncpy(status->address, status->user[user_getnum()].addr, ADDR_SIZE);
    make_privatjoin();
    strncpy(status->nick, config->nick, NICK_MAXLEN);
    strncpy(status->address, status->user[user_getnum()].addr, ADDR_SIZE);
    make_privatjoin();
}

void cmnd_here(void) {
    strncpy(status->channel, status->room->name, CHANNEL_MAXLEN);
    make_here();
}

void cmnd_active(void) {
    if (config->active) config->active = 0;
    else config->active = 1;
    make_active();
}

extern int g_show_channel;

void cmnd_parse(void) {
    register int n;

    if (*buf != CHR_CMND) { cmnd_chat(); return; }
    *buf = '\0'; cmnd_next();
    for (n = 0; n < STR_SIZE; n++) if (buf[n] == ' ') { buf[n] = '\0'; break; }
    if (!strncasecmp(buf, CMD_QUIT, CMD_MAXLEN) || !strncasecmp(buf, CMD_QUIT1, CMD_MAXLEN) || !strncasecmp(buf, CMD_QUIT2, CMD_MAXLEN)) chat_exit(0);
    if (!strncasecmp(buf, CMD_HELP, CMD_MAXLEN) || !strncasecmp(buf, CMD_HELP1, CMD_MAXLEN) || !strncasecmp(buf, CMD_HELP2, CMD_MAXLEN)) {
        cmnd_next(); cmnd_help(); return;
    }
    if (!strncasecmp(buf, CMD_VERSION, CMD_MAXLEN) || !strncasecmp(buf, CMD_VERSION1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_version(); return;
    }
    if (!strncasecmp(buf, CMD_STATUS, CMD_MAXLEN) || !strncasecmp(buf, CMD_STATUS1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_status(); return;
    }
    if (!strncasecmp(buf, CMD_KEYS, CMD_MAXLEN) || !strncasecmp(buf, CMD_KEYS1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_keys(); return;
    }
    if (!strncasecmp(buf, CMD_VERBOSE, CMD_MAXLEN) || !strncasecmp(buf, CMD_VERBOSE1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_verbose(); return;
    }
    if (!strncasecmp(buf, CMD_CLEAR, CMD_MAXLEN) || !strncasecmp(buf, CMD_CLEAR1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_clear(); return;
    }
    if (!strncasecmp(buf, CMD_SHELL, CMD_MAXLEN)) {
        cmnd_next(); cmnd_shell(); return;
    }
    if (!strncasecmp(buf, CMD_LOAD, CMD_MAXLEN)) {
        cmnd_next(); cmnd_load(); return;
    }
    if (!strncasecmp(buf, CMD_SAVE, CMD_MAXLEN)) {
        cmnd_next(); cmnd_save(); return;
    }

    if (!strncasecmp(buf, CMD_ANSWER, CMD_MAXLEN) || !strncasecmp(buf, CMD_ANSWER1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_answer(); return;
    }
    if (!strncasecmp(buf, CMD_GENDER, CMD_MAXLEN) || !strncasecmp(buf, CMD_GENDER1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_gender(); return;
    }
    if (!strncasecmp(buf, CMD_MY, CMD_MAXLEN)) {
        cmnd_next(); cmnd_my(); return;
    }
    if (!strncasecmp(buf, CMD_FAVORITE, CMD_MAXLEN) || !strncasecmp(buf, CMD_FAVORITE1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_favorite(); return;
    }
    if (!strncasecmp(buf, CMD_IGNORE, CMD_MAXLEN) || !strncasecmp(buf, CMD_IGNORE1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_ignore(); return;
    }
    if (!strncasecmp(buf, CMD_BAN, CMD_MAXLEN)) {
        cmnd_next(); cmnd_ban(); return;
    }
    if (!strncasecmp(buf, CMD_JOIN, CMD_MAXLEN) || !strncasecmp(buf, CMD_JOIN1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_join(); return;
    }
    if (!strncasecmp(buf, CMD_LEAVE, CMD_MAXLEN) || !strncasecmp(buf, CMD_LEAVE1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_leave(); return;
    }

    if (!strncasecmp(buf, CMD_NICK, CMD_MAXLEN) || !strncasecmp(buf, CMD_NICK1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_nick(); return;
    }
    if (!strncasecmp(buf, CMD_MESG, CMD_MAXLEN) || !strncasecmp(buf, CMD_MESG1, CMD_MAXLEN) || !strncasecmp(buf, CMD_MESG2, CMD_MAXLEN)) {
        cmnd_next(); cmnd_mesg(); return;
    }
/* SERGEJ >>> */
    if (!strncasecmp(buf, "nmsg", CMD_MAXLEN))
    {
      cmnd_next();
      cmnd_nmesg();
      return;
    }
    if (!strncasecmp(buf, "chan", CMD_MAXLEN))
    {
      cmnd_next();
      g_show_channel = !g_show_channel;
      snprintf(message, MSG_SIZE, g_show_channel ? "Channels on" : "Channels off");
      write_str(status->room->name, COL_SYSTEM);
      return;
    }
/* <<< SERGEJ */
    if (!strncasecmp(buf, CMD_MASS, CMD_MAXLEN)) {
        cmnd_next(); cmnd_mass(); return;
    }
    if (!strncasecmp(buf, CMD_EXEC, CMD_MAXLEN)) {
        cmnd_next(); cmnd_exec(); return;
    }
    if (!strncasecmp(buf, CMD_ME, CMD_MAXLEN)) {
        cmnd_next(); cmnd_me(); return;
    }
    if (!strncasecmp(buf, CMD_TOPIC, CMD_MAXLEN) || !strncasecmp(buf, CMD_TOPIC1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_topic(); return;
    }
    if (!strncasecmp(buf, CMD_ADD, CMD_MAXLEN)) {
        cmnd_next(); cmnd_add(); return;
    }
    if (!strncasecmp(buf, CMD_MODE, CMD_MAXLEN) || !strncasecmp(buf, CMD_MODE1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_mode(); return;
    }
    if (!strncasecmp(buf, CMD_NORMAL, CMD_MAXLEN) || !strncasecmp(buf, CMD_NORMAL1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_normal(); return;
    }
    if (!strncasecmp(buf, CMD_DND, CMD_MAXLEN) || !strncasecmp(buf, CMD_DND1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_dnd(); return;
    }
    if (!strncasecmp(buf, CMD_AWAY, CMD_MAXLEN) || !strncasecmp(buf, CMD_AWAY1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_away(); return;
    }
    if (!strncasecmp(buf, CMD_OFF, CMD_MAXLEN) || !strncasecmp(buf, CMD_OFF1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_off(); return;
    }
    if (!strncasecmp(buf, CMD_INFO, CMD_MAXLEN) || !strncasecmp(buf, CMD_INFO1, CMD_MAXLEN) || !strncasecmp(buf, CMD_INFO2, CMD_MAXLEN)) {
        cmnd_next(); cmnd_info(); return;
    }
    if (!strncasecmp(buf, CMD_BEEP, CMD_MAXLEN) || !strncasecmp(buf, CMD_BEEP1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_beep(); return;
    }
    if (!strncasecmp(buf, CMD_TALK, CMD_MAXLEN) || !strncasecmp(buf, CMD_TALK1, CMD_MAXLEN) || !strncasecmp(buf, CMD_TALK2, CMD_MAXLEN)) {
        cmnd_next(); cmnd_talk(); return;
    }
    if (!strncasecmp(buf, CMD_HERE, CMD_MAXLEN)) {
        cmnd_next(); cmnd_here(); return;
    }
    if (!strncasecmp(buf, CMD_ACTIVE, CMD_MAXLEN) || !strncasecmp(buf, CMD_ACTIVE1, CMD_MAXLEN)) {
        cmnd_next(); cmnd_active(); return;
    }

    if (config->status_str) status_error(CMND_SUNKNOWN);
    else {
        snprintf(message, STR_SIZE, CMND_UNKNOWN, time_get(), buf);
        write_str(status->room->name, COL_ERROR);
    }
}
