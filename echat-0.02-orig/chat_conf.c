/*  chat_conf.c
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#include <string.h>
#include <unistd.h>
#include "echat.h"
#include "chat_conf.h"

#ifdef  DEBUG
# include <errno.h>
#endif  /* DEBUG */

char *option, *value;
FILE *config_file;

void conf_default(void) {
    strncpy(config->nick, DEF_NICK, NICK_MAXLEN);
    config->gender = DEF_GENDER;
    strncpy(config->channel, DEF_CHANNEL, STR_SIZE);
    config->mode = DEF_MODE;
    config->active = DEF_ACTIVE;
    strncpy(config->answer_dnd, DEF_ANSWER_DND, STR_SIZE);
    strncpy(config->answer_away, DEF_ANSWER_AWAY, STR_SIZE);
    strncpy(config->answer_off, DEF_ANSWER_OFF, STR_SIZE);
    strncpy(config->me_action, DEF_ME_ACTION, STR_SIZE);
    strncpy(config->favorite, DEF_FAVORITE, STR_SIZE);
    strncpy(config->ignore, DEF_IGNORE, STR_SIZE);
    strncpy(config->ban, DEF_BAN, STR_SIZE);
    strncpy(config->address, DEF_ADDRESS, STR_SIZE);
    config->port = DEF_PORT;
    config->packet_bcast = DEF_PACKET_BCAST;
/*      FIXME: getpeername
    config->packet_route = DEF_PACKET_ROUTE;
*/
    config->packet_qchat = DEF_PACKET_QCHAT;
    config->packet_xor = DEF_PACKET_XOR;
    strncpy(config->head_nick, DEF_HEAD_NICK, STR_SIZE);
    strncpy(config->head_top, DEF_HEAD_TOP, STR_SIZE);
    strncpy(config->head_users, DEF_HEAD_USERS, STR_SIZE);
    strncpy(config->ps, DEF_PS, STR_SIZE);
    config->style = DEF_STYLE;
    config->ascii = DEF_ASCII;
    config->status_str = DEF_STATUS_STR;
    config->seconds = DEF_SECONDS;
    config->users_width = DEF_USERS_WIDTH;
    config->users_timeout = DEF_USERS_TIMEOUT;
    config->scroll_step = DEF_SCROLL_STEP;
#ifdef  CHARSET
    config->charset = DEF_CHARSET;
#endif  /* CHARSET */
    config->beep_system = DEF_BEEP_SYSTEM;
    config->beep_chat = DEF_BEEP_CHAT;
    config->beep_join = DEF_BEEP_JOIN;
    config->beep_mesg = DEF_BEEP_MESG;
    config->beep_beep = DEF_BEEP_BEEP;
    config->my_only = DEF_MY_ONLY;
    config->verbose = DEF_VERBOSE;
    config->server = DEF_SERVER;
#ifdef  SHELL
    strncpy(config->shell, DEF_SHELL, STR_SIZE);
#endif  /* SHELL */
    strncpy(config->log_main, DEF_LOGMAIN, PATH_SIZE);
    strncpy(config->log_mesg, DEF_LOGMESG, PATH_SIZE);
    strncpy(config->log_priv, DEF_LOGPRIV, PATH_SIZE);
    config->color_bkgr = DEF_COL_BKGR;
    config->color_border = DEF_COL_BORDER;
    config->color_head = DEF_COL_HEAD;
    config->color_status = DEF_COL_STATUS;
    config->color_system = DEF_COL_SYSTEM;
    config->color_error = DEF_COL_ERROR;
    config->color_mesg = DEF_COL_MESG;
    config->color_my = DEF_COL_MY;
    config->color_chat = DEF_COL_CHAT;
#ifdef  DEBUG
    fprintf(debug_file, "config: Default config initialized.\n");
    fflush(debug_file);
#endif  /* DEBUG */
}

int conf_getopt(void) {
    register int n;

    if (fgets(buf, STR_SIZE, config_file) == NULL) return 0;

    for (option = value = buf, n = 0; n < STR_SIZE; n++) {
        if ((buf[n] == '\n') || (buf[n] == CNF_COMMENT1) || (buf[n] == CNF_COMMENT2)) {
            *option = '\0';
            *value = '\0';
            return 1;
        }
        if ((buf[n] != ' ') && (buf[n] != '\t')) {
            option = &buf[n];
            break;
        }
    }
    for (; n < STR_SIZE; n++) {
        if ((buf[n] == '\n') || (buf[n] == CNF_COMMENT1) || (buf[n] == CNF_COMMENT2)) {
            buf[n] = '\0';
            value = &buf[n];
            return 1;
        }
        if ((buf[n] == ' ') || (buf[n] == '\t') || (buf[n] == '=')) {
            buf[n] = '\0';
            break;
        }
    }
    for (n++; n < STR_SIZE; n++) {
        if ((buf[n] == '\n') || (buf[n] == CNF_COMMENT1) || (buf[n] == CNF_COMMENT2)) {
            buf[n] = '\0';
            value = &buf[n];
            return 1;
        }
        if ((buf[n] != ' ') && (buf[n] != '\t') && (buf[n] != '=')) {
            value = &buf[n];
            break;
        }
    }
    if (buf[n] == '\"') {
        for (n++, value++; n < STR_SIZE; n++)
            if ((buf[n] == '\"') || (buf[n] == '\n')) {
                buf[n] = '\0';
                break;
            }
    } else {
        for (; n < STR_SIZE; n++)
            if ((buf[n] == '\n') || (buf[n] == ' ') || (buf[n] == '\t') ||
                (buf[n] == CNF_COMMENT1) || (buf[n] == CNF_COMMENT2)) {
                buf[n] = '\0';
                break;
            }
    }
    return 1;
}

int conf_atoibool(void) {
    if ((*value == '\0') ||
        !strncmp(value, "1", 2) ||
        !strncmp(value, "on", 3) ||
        !strncmp(value, "yes", 4) ||
        !strncmp(value, "true", 5)) return 1;
    else return 0;
}

int conf_read(void) {
#ifdef  DEBUG
    fprintf(debug_file, "configfile: Open %s\n", config->file);
    fflush(debug_file);
#endif  /* DEBUG */
    if (!(config_file = fopen(config->file, "r"))) {
#ifdef  DEBUG
        fprintf(debug_file, "configfile: %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
        return 0;
    }
    while (conf_getopt()) {
        if (*option == '\0') continue;
#ifdef  DEBUG
        fprintf(debug_file, "configfile: Read option __%s__, value __%s__\n", option, value);
        fflush(debug_file);
#endif  /* DEBUG */
        if (!strncasecmp(option, "nick", OPTION_MAXLEN)) {
            strncpy(config->nick, value, NICK_MAXLEN);
            continue;
        }
        if (!strncasecmp(option, "gender", OPTION_MAXLEN)) {
            config->gender = atoi(value);
            continue;
        }
        if (!strncasecmp(option, "channel", OPTION_MAXLEN)) {
            strncpy(config->channel, value, STR_SIZE);
            continue;
        }
        if (!strncasecmp(option, "mode", OPTION_MAXLEN)) {
            config->mode = atoi(value);
            continue;
        }
        if (!strncasecmp(option, "active", OPTION_MAXLEN)) {
            config->active = conf_atoibool();
            continue;
        }
        if (!strncasecmp(option, "answer_dnd", OPTION_MAXLEN)) {
            strncpy(config->answer_dnd, value, STR_SIZE);
            continue;
        }
        if (!strncasecmp(option, "answer_away", OPTION_MAXLEN)) {
            strncpy(config->answer_away, value, STR_SIZE);
            continue;
        }
        if (!strncasecmp(option, "answer_off", OPTION_MAXLEN)) {
            strncpy(config->answer_off, value, STR_SIZE);
            continue;
        }
        if (!strncasecmp(option, "me_action", OPTION_MAXLEN)) {
            strncpy(config->me_action, value, STR_SIZE);
            continue;
        }
        if (!strncasecmp(option, "favorite", OPTION_MAXLEN)) {
            strncpy(config->favorite, value, STR_SIZE);
            continue;
        }
        if (!strncasecmp(option, "ignore", OPTION_MAXLEN)) {
            strncpy(config->ignore, value, STR_SIZE);
            continue;
        }
        if (!strncasecmp(option, "ban", OPTION_MAXLEN)) {
            strncpy(config->ban, value, STR_SIZE);
            continue;
        }
        if (!strncasecmp(option, "address", OPTION_MAXLEN)) {
            strncpy(config->address, value, STR_SIZE);
            continue;
        }
        if (!strncasecmp(option, "port", OPTION_MAXLEN)) {
            config->port = (unsigned short) atoi(value);
            continue;
        }
        if (!strncasecmp(option, "packet_bcast", OPTION_MAXLEN)) {
            config->packet_bcast = conf_atoibool();
            continue;
        }
/*        FIXME: getpeername
        if (!strncasecmp(option, "packet_route", OPTION_MAXLEN)) {
            config->packet_route = conf_atoibool();
            continue;
        }
*/
        if (!strncasecmp(option, "packet_qchat", OPTION_MAXLEN)) {
            config->packet_qchat = conf_atoibool();
            continue;
        }
        if (!strncasecmp(option, "packet_xor", OPTION_MAXLEN)) {
            config->packet_xor = (unsigned char) atoi(value);
            continue;
        }
        if (!strncasecmp(option, "head_nick", OPTION_MAXLEN)) {
            strncpy(config->head_nick, value, VALUE_MAXLEN);
            continue;
        }
        if (!strncasecmp(option, "head_top", OPTION_MAXLEN)) {
            strncpy(config->head_top, value, VALUE_MAXLEN);
            continue;
        }
        if (!strncasecmp(option, "head_users", OPTION_MAXLEN)) {
            strncpy(config->head_users, value, VALUE_MAXLEN);
            continue;
        }
        if (!strncasecmp(option, "ps", OPTION_MAXLEN)) {
            strncpy(config->ps, value, VALUE_MAXLEN);
            continue;
        }
        if (!strncasecmp(option, "style", OPTION_MAXLEN)) {
            config->style = conf_atoibool();
            continue;
        }
        if (!strncasecmp(option, "ascii", OPTION_MAXLEN)) {
            config->ascii = conf_atoibool();
            continue;
        }
        if (!strncasecmp(option, "status_str", OPTION_MAXLEN)) {
            config->status_str = conf_atoibool();
            continue;
        }
        if (!strncasecmp(option, "seconds", OPTION_MAXLEN)) {
            config->seconds = conf_atoibool();
            continue;
        }
        if (!strncasecmp(option, "users_width", OPTION_MAXLEN)) {
            config->users_width = atoi(value);
            continue;
        }
        if (!strncasecmp(option, "users_timeout", OPTION_MAXLEN)) {
            config->users_timeout = atoi(value);
            continue;
        }
        if (!strncasecmp(option, "scroll_step", OPTION_MAXLEN)) {
            config->scroll_step = atoi(value);
            continue;
        }
#ifdef  CHARSET
        if (!strncasecmp(option, "charset", OPTION_MAXLEN)) {
            config->charset = conf_atoibool();
            continue;
        }
#endif  /* CHARSET */
        if (!strncasecmp(option, "beep_system", OPTION_MAXLEN)) {
            config->beep_system = conf_atoibool();
            continue;
        }
        if (!strncasecmp(option, "beep_chat", OPTION_MAXLEN)) {
            config->beep_chat = conf_atoibool();
            continue;
        }
        if (!strncasecmp(option, "beep_join", OPTION_MAXLEN)) {
            config->beep_join = conf_atoibool();
            continue;
        }
        if (!strncasecmp(option, "beep_mesg", OPTION_MAXLEN)) {
            config->beep_mesg = conf_atoibool();
            continue;
        }
        if (!strncasecmp(option, "beep_beep", OPTION_MAXLEN)) {
            config->beep_beep = conf_atoibool();
            continue;
        }
        if (!strncasecmp(option, "my_only", OPTION_MAXLEN)) {
            config->my_only = conf_atoibool();
            continue;
        }
        if (!strncasecmp(option, "verbose", OPTION_MAXLEN)) {
            config->verbose = conf_atoibool();
            continue;
        }
        if (!strncasecmp(option, "server", OPTION_MAXLEN)) {
            config->server = conf_atoibool();
            continue;
        }
#ifdef  SHELL
        if (!strncasecmp(option, "shell", OPTION_MAXLEN)) {
            strncpy(config->shell, value, STR_SIZE);
            continue;
        }
#endif  /* SHELL */
        if (!strncasecmp(option, "log_main", OPTION_MAXLEN)) {
            strncpy(config->log_main, value, PATH_SIZE);
            continue;
        }
        if (!strncasecmp(option, "log_message", OPTION_MAXLEN)) {
            strncpy(config->log_mesg, value, PATH_SIZE);
            continue;
        }
        if (!strncasecmp(option, "log_private", OPTION_MAXLEN)) {
            strncpy(config->log_priv, value, PATH_SIZE);
            continue;
        }
        if (!strncasecmp(option, "color_bkgr", OPTION_MAXLEN)) {
            config->color_bkgr = atoi(value);
            continue;
        }
        if (!strncasecmp(option, "color_border", OPTION_MAXLEN)) {
            config->color_border = atoi(value);
            continue;
        }
        if (!strncasecmp(option, "color_head", OPTION_MAXLEN)) {
            config->color_head = atoi(value);
            continue;
        }
        if (!strncasecmp(option, "color_status", OPTION_MAXLEN)) {
            config->color_status = atoi(value);
            continue;
        }
        if (!strncasecmp(option, "color_system", OPTION_MAXLEN)) {
            config->color_system = atoi(value);
            continue;
        }
        if (!strncasecmp(option, "color_error", OPTION_MAXLEN)) {
            config->color_error = atoi(value);
            continue;
        }
        if (!strncasecmp(option, "color_mesg", OPTION_MAXLEN)) {
            config->color_mesg = atoi(value);
            continue;
        }
        if (!strncasecmp(option, "color_my", OPTION_MAXLEN)) {
            config->color_my = atoi(value);
            continue;
        }
        if (!strncasecmp(option, "color_chat", OPTION_MAXLEN)) {
            config->color_chat = atoi(value);
            continue;
        }
#ifdef  DEBUG
        fprintf(debug_file, "configfile: INVALID option __%s__, value __%s__\n", option, value);
        fflush(debug_file);
#endif  /* DEBUG */
    }
#ifdef  DEBUG
    fprintf(debug_file, "configfile: Close %s\n", CNF_ECHATRC);
    fflush(debug_file);
#endif  /* DEBUG */
    if (fclose(config_file)) {
#ifdef  DEBUG
        fprintf(debug_file, "configfile: %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
    }
    return 1;
}

int conf_write(void) {
#ifdef  DEBUG
    fprintf(debug_file, "configfile: Open %s\n", config->file);
    fflush(debug_file);
#endif  /* DEBUG */
    if (!(config_file = fopen(config->file, "w"))) {
#ifdef  DEBUG
        fprintf(debug_file, "configfile: %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
        return 0;
    }
    fprintf(config_file, "nick \"%s\"\n", config->nick);
    if (config->gender && (config->gender != DEF_GENDER))
        fprintf(config_file, "gender\n");
    fprintf(config_file, "channel \"%s\"\n", config->channel);
    fprintf(config_file, "mode = %u\n", config->mode);
    if (config->active && (config->active != DEF_ACTIVE))
        fprintf(config_file, "active\n");
    fprintf(config_file, "answer_dnd \"%s\"\n", config->answer_dnd);
    fprintf(config_file, "answer_away \"%s\"\n", config->answer_away);
    fprintf(config_file, "answer_off \"%s\"\n", config->answer_off);
    fprintf(config_file, "me_action \"%s\"\n", config->me_action);
    fprintf(config_file, "favorite \"%s\"\n", config->favorite);
    fprintf(config_file, "ignore \"%s\"\n", config->ignore);
    fprintf(config_file, "ban \"%s\"\n", config->ban);
    fprintf(config_file, "address \"%s\"\n", config->address);
    fprintf(config_file, "port = %u\n", config->port);
    if (config->packet_bcast && (config->packet_bcast != DEF_PACKET_BCAST))
        fprintf(config_file, "packet_bcast\n");
/*        FIXME: getpeername
    if (config->packet_route && (config->packet_route != DEF_PACKET_ROUTE))
        fprintf(config_file, "packet_route\n");
*/
    if (config->packet_qchat && (config->packet_qchat != DEF_PACKET_QCHAT))
        fprintf(config_file, "packet_qchat\n");
    if (config->packet_xor != DEF_PACKET_XOR)
        fprintf(config_file, "packet_xor = %u\n", config->packet_xor);
    if (strncmp(config->head_nick, DEF_HEAD_NICK, STR_SIZE))
        fprintf(config_file, "head_nick \"%s\"\n", config->head_nick);
    if (strncmp(config->head_top, DEF_HEAD_TOP, STR_SIZE))
        fprintf(config_file, "head_top \"%s\"\n", config->head_top);
    if (strncmp(config->head_users, DEF_HEAD_USERS, STR_SIZE))
        fprintf(config_file, "head_users \"%s\"\n", config->head_users);
    if (strncmp(config->ps, DEF_PS, STR_SIZE))
        fprintf(config_file, "ps \"%s\"\n", config->ps);
    if (config->style && (config->style != DEF_STYLE))
        fprintf(config_file, "style\n");
    if (config->ascii && (config->ascii != DEF_ASCII))
        fprintf(config_file, "ascii\n");
    if (config->status_str && (config->status_str != DEF_STATUS_STR))
        fprintf(config_file, "status_str\n");
    if (config->seconds && (config->seconds != DEF_SECONDS))
        fprintf(config_file, "seconds\n");
    fprintf(config_file, "users_width = %u\n", config->users_width);
    fprintf(config_file, "users_timeout = %u\n", config->users_timeout);
    fprintf(config_file, "scroll_step = %u\n", config->scroll_step);
#ifdef  CHARSET
    if (config->charset && (config->charset != DEF_CHARSET))
        fprintf(config_file, "charset\n");
#endif  /* CHARSET */
    if (config->beep_system && (config->beep_system != DEF_BEEP_SYSTEM))
        fprintf(config_file, "beep_system\n");
    if (config->beep_chat && (config->beep_chat != DEF_BEEP_CHAT))
        fprintf(config_file, "beep_chat\n");
    if (config->beep_join && (config->beep_join != DEF_BEEP_JOIN))
        fprintf(config_file, "beep_join\n");
    if (config->beep_mesg && (config->beep_mesg != DEF_BEEP_MESG))
        fprintf(config_file, "beep_mesg\n");
    if (config->beep_beep && (config->beep_beep != DEF_BEEP_BEEP))
        fprintf(config_file, "beep_beep\n");
    if (config->my_only && (config->my_only != DEF_MY_ONLY))
        fprintf(config_file, "my_only\n");
    if (config->verbose && (config->verbose != DEF_VERBOSE))
        fprintf(config_file, "verbose\n");
    if (config->server && (config->server != DEF_SERVER))
        fprintf(config_file, "server\n");
#ifdef  SHELL
    if (strncmp(config->shell, DEF_SHELL, STR_SIZE))
        fprintf(config_file, "shell \"%s\"\n", config->shell);
#endif  /* SHELL */
    if (strncmp(config->log_main, DEF_LOGMAIN, PATH_SIZE))
        fprintf(config_file, "log_main \"%s\"\n", config->log_main);
    if (strncmp(config->log_mesg, DEF_LOGMESG, PATH_SIZE))
        fprintf(config_file, "log_message \"%s\"\n", config->log_mesg);
    if (strncmp(config->log_priv, DEF_LOGPRIV, PATH_SIZE))
        fprintf(config_file, "log_private \"%s\"\n", config->log_priv);
    fprintf(config_file, "color_bkgr = %u\n", config->color_bkgr);
    fprintf(config_file, "color_border = %u\n", config->color_border);
    fprintf(config_file, "color_head = %u\n", config->color_head);
    fprintf(config_file, "color_status = %u\n", config->color_status);
    fprintf(config_file, "color_system = %u\n", config->color_system);
    fprintf(config_file, "color_error = %u\n", config->color_error);
    fprintf(config_file, "color_mesg = %u\n", config->color_mesg);
    fprintf(config_file, "color_my = %u\n", config->color_my);
    fprintf(config_file, "color_chat = %u\n", config->color_chat);
#ifdef  DEBUG
    fprintf(debug_file, "configfile: Close %s\n", CNF_ECHATRC);
    fflush(debug_file);
#endif  /* DEBUG */
    if (fclose(config_file)) {
#ifdef  DEBUG
        fprintf(debug_file, "configfile: %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
    }
    return 1;
}
