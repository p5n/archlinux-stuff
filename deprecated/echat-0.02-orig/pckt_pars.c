/*  pckt_pars.c
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#include <string.h>
#include "echat.h"
#include "pckt_type.h"

int pars_head(void) {
    char ch;
    register int n, m;

    if (config->packet_qchat)
        for (ch = packet[0], m = 1, n = 0; m < MSG_SIZE;) packet[n++] = packet[m++];
    else {
        if (config->packet_xor) {
            if (*packet != TYPE_VYPRESSXOR) {
#ifdef  DEBUG
                fprintf(debug_file, "error: Invalid packet type %c\n", *packet);
                fflush(debug_file);
#endif  /* DEBUG */
                return -1;
            }
        } else {
            if (*packet != TYPE_VYPRESS) {
#ifdef  DEBUG
                fprintf(debug_file, "error: Invalid packet type %c\n", *packet);
                fflush(debug_file);
#endif  /* DEBUG */
                return -1;
            }
        }
        ch = packet[HEADER_LEN];
        for (m = HEADER_LEN+1, n = 0; m < MSG_SIZE;) packet[n++] = packet[m++];
    }
    return ((int) ch);
}

int pars_number(void) {
    char ch;
    register int n;

    ch = *packet - '0';
    for (n = 0; n < MSG_SIZE; n++) packet[n] = packet[n+1];
    return ((int) ch);
}

char *pars_word(void) {
    register int n, m;

    for (n = 0; n < MSG_SIZE; n++) if (packet[n] == MSG_SPLIT) break;
    for (m = ++n, n = 0; m < MSG_SIZE; n++, m++) packet[n] = packet[m];
    return (packet);
}

void pars_whohere(void) {
    strncpy(status->nick, packet, NICK_MAXLEN);
    make_imhere();
    status_ok(PARS_SHERE);
}

void pars_imhere(void) {
    if (strncmp(config->nick, packet, NICK_MAXLEN)) return;
    strncpy(status->nick, pars_word(), NICK_MAXLEN);
    pars_word();
    status->mode = pars_number();
    status->active = pars_number();
    user_add();
}

void pars_chat(void) {
    strncpy(status->channel, packet, CHANNEL_MAXLEN);
    strncpy(status->nick, pars_word(), NICK_MAXLEN);
    if (user_ignore()) return;
    if (strncmp(status->chnl[0].name, status->channel, CHANNEL_MAXLEN) &&
        !chnl_getnum()) return;
    snprintf(message, MSG_SIZE, "<%s> %s\n", status->nick, pars_word());
#ifdef  TCP
    write_tcp(status->channel);
#endif  /* TCP */
    write_log(config->log_main);
    if (!strncmp(config->nick, status->nick, NICK_MAXLEN))
        write_str(status->channel, COL_MY);
    else
        write_str(status->channel, COL_CHAT);
    if ((!config->my_only || !strncmp(status->room->name, status->channel, CHANNEL_MAXLEN)) &&
        (config->mode == MOD_NORMAL)) beep_chat();
}

void pars_nick(void) {
    strncpy(status->channel, packet, NICK_MAXLEN);
    strncpy(status->nick, pars_word(), NICK_MAXLEN);
    if (!config->packet_qchat) {
        pars_word(); status->gender = pars_number();
    }
    user_nick();
    chat_nick();
    if (user_ignore()) return;
    snprintf(message, MSG_SIZE, PARS_NICK, time_get(), status->channel, status->nick);
#ifdef  TCP
    write_tcp(status->chnl[0].name);
#endif  /* TCP */
    if (!user_ignore() && config->verbose) {
        write_log(config->log_main);
        write_str(status->chnl[0].name, COL_SYSTEM);
    }
    refresh_room();
    screen_show();
    refresh_input();
    string_show();
    if ((!config->my_only || !strncmp(status->room->name, status->channel, CHANNEL_MAXLEN)) &&
        (config->mode == MOD_NORMAL)) beep_system();
}

void pars_join(void) {
    strncpy(status->nick, packet, NICK_MAXLEN);
    strncpy(status->channel, pars_word(), CHANNEL_MAXLEN);
    pars_word(); status->mode = pars_number();
    if (!config->packet_qchat)
        status->gender = pars_number();
    chnl_join();
}

void pars_leave(void) {
    strncpy(status->nick, packet, NICK_MAXLEN);
    strncpy(status->channel, pars_word(), CHANNEL_MAXLEN);
    if (!config->packet_qchat) {
        pars_word(); status->gender = pars_number();
    }
    chnl_leave();
}

void pars_mesg(void) {
    register int n;

    strncpy(status->nick, packet, NICK_MAXLEN);
    if (user_ignore()) return;
    if (strncmp(config->nick, pars_word(), NICK_MAXLEN)) return;
    pars_word();
    for (n = 0; packet[n] != MSG_SPLIT; n++) if (packet[n] == '\r') packet[n] = ' ';
    snprintf(message, MSG_SIZE, PARS_MESG, time_get(), status->nick);
    write_log(config->log_mesg);
    write_str(status->room->name, COL_MESG);
    snprintf(message, MSG_SIZE, "%s\n", packet);
    write_log(config->log_mesg);
    write_str(status->room->name, COL_MESG);
    if (config->mode != MOD_OFF) make_mesgack();
    if ((config->mode == MOD_NORMAL) || (config->mode == MOD_DND) ||
        ((config->mode == MOD_AWAY) && user_favorite())) beep_mesg();
}

void pars_mass(void) {
    register int n;

    if (config->mode == MOD_OFF) return;
    strncpy(status->nick, packet, NICK_MAXLEN);
    if (user_ignore()) return;
    if (strncmp(config->nick, pars_word(), NICK_MAXLEN)) return;
    pars_word();
    for (n = 0; packet[n] != MSG_SPLIT; n++) if (packet[n] == '\r') packet[n] = ' ';
    snprintf(message, MSG_SIZE, PARS_MASS, time_get(), status->nick);
    write_log(config->log_mesg);
    write_str(status->chnl[0].name, COL_MESG);
    snprintf(message, MSG_SIZE, "%s\n", packet);
    write_log(config->log_mesg);
    write_str(status->chnl[0].name, COL_MESG);
    make_mesgack();
    if (config->mode == MOD_NORMAL) beep_mesg();
}

void pars_mesgack(void) {
    status->mode = pars_number();
    if (strncmp(config->nick, packet, NICK_MAXLEN)) return;
    strncpy(status->nick, pars_word(), NICK_MAXLEN);
    if (config->packet_qchat) {
        snprintf(message, MSG_SIZE, PARS_MESGACK, time_get(), status->nick);
        write_log(config->log_mesg);
        write_str(status->room->name, COL_MESG);
    } else {
        pars_word(); status->gender = pars_number();
        if (*packet == '\0') {
            snprintf(message, MSG_SIZE, PARS_MESGACK, time_get(), status->nick);
            write_log(config->log_mesg);
            write_str(status->room->name, COL_MESG);
        } else {
            snprintf(message, MSG_SIZE, PARS_MESGANS, time_get(), status->nick);
            write_log(config->log_mesg);
            write_str(status->room->name, COL_MESG);
            snprintf(message, MSG_SIZE, "%s\n", packet);
            write_log(config->log_mesg);
            write_str(status->room->name, COL_MESG);
        }
    }
}

void pars_exec(void) {
    strncpy(status->nick, packet, NICK_MAXLEN);
    if (user_ignore()) return;
    if (strncmp(config->nick, pars_word(), NICK_MAXLEN)) return;
    snprintf(message, MSG_SIZE, PARS_EXEC, time_get(), status->nick);
    write_log(config->log_main);
    write_str(status->room->name, COL_ERROR);
    snprintf(message, MSG_SIZE, "%s\n", pars_word());
    write_log(config->log_main);
    write_str(status->room->name, COL_ERROR);
    if (config->mode != MOD_OFF) make_execack();
}

void pars_execack(void) {
    if (strncmp(config->nick, packet, NICK_MAXLEN)) return;
    strncpy(status->nick, pars_word(), NICK_MAXLEN);
    snprintf(message, MSG_SIZE, PARS_EXECACK, time_get(), status->nick);
    write_log(config->log_main);
    write_str(status->chnl[0].name, COL_SYSTEM);
    snprintf(message, MSG_SIZE, "%s\n", pars_word());
    write_log(config->log_main);
    write_str(status->chnl[0].name, COL_SYSTEM);
}

void pars_me(void) {
    strncpy(status->channel, packet, CHANNEL_MAXLEN);
    strncpy(status->nick, pars_word(), NICK_MAXLEN);
    if (user_ignore()) return;
    if (strncmp(status->chnl[0].name, status->channel, CHANNEL_MAXLEN) &&
        !chnl_getnum()) return;
    snprintf(message, MSG_SIZE, " * %s %s\n", status->nick, pars_word());
#ifdef  TCP
    write_tcp(status->channel);
#endif  /* TCP */
    write_log(config->log_main);
    if (!strncmp(config->nick, status->nick, NICK_MAXLEN))
        write_str(status->channel, COL_MY);
    else
        write_str(status->channel, COL_CHAT);
    if ((!config->my_only || !strncmp(status->room->name, status->channel, CHANNEL_MAXLEN)) &&
        (config->mode == MOD_NORMAL)) beep_chat();
}

void pars_newtopic(void) {
    if (config->packet_qchat) {
        strncpy(status->chnl[0].topic, packet, STR_SIZE);
        snprintf(message, MSG_SIZE, PARS_QTOPIC, time_get());
#ifdef  TCP
        write_tcp(status->room->name);
#endif  /* TCP */
        if (config->verbose) {
            write_log(config->log_main);
            write_str(status->room->name, COL_SYSTEM);
        }
        snprintf(message, MSG_SIZE, "%s\n", status->chnl[0].topic);
#ifdef  TCP
        write_tcp(status->room->name);
#endif  /* TCP */
        if (config->verbose) {
            write_log(config->log_main);
            write_str(status->room->name, COL_SYSTEM);
        }
    } else {
        strncpy(status->channel, packet, CHANNEL_MAXLEN);
        if (strncmp(status->chnl[0].name, status->channel, CHANNEL_MAXLEN) &&
            !chnl_getnum()) return;
        strncpy(status->chnl[chnl_getnum()].topic, pars_word(), STR_SIZE);
        snprintf(message, MSG_SIZE, PARS_TOPIC, time_get(), status->channel);
#ifdef  TCP
        write_tcp(status->channel);
#endif  /* TCP */
        if (config->verbose) {
            write_log(config->log_main);
            write_str(status->channel, COL_SYSTEM);
        }
        snprintf(message, MSG_SIZE, "%s\n", status->chnl[chnl_getnum()].topic);
#ifdef  TCP
        write_tcp(status->channel);
#endif  /* TCP */
        if (config->verbose) {
            write_log(config->log_main);
            write_str(status->channel, COL_SYSTEM);
        }
    }
    refresh_room();
    screen_show();
    refresh_input();
    string_show();
    if ((!config->my_only || !strncmp(status->room->name, status->channel, CHANNEL_MAXLEN)) &&
        (config->mode == MOD_NORMAL)) beep_system();
}

void pars_topic(void) {
    if (strncmp(config->nick, packet, NICK_MAXLEN)) return;
    if (config->packet_qchat) {
        if (!strncmp(status->chnl[0].topic, pars_word(), STR_SIZE)) return;
        strncpy(status->chnl[0].topic, packet, STR_SIZE);
        snprintf(message, MSG_SIZE, PARS_QTOPICC, time_get());
#ifdef  TCP
        write_tcp(status->room->name);
#endif  /* TCP */
        write_log(config->log_main);
        write_str(status->room->name, COL_SYSTEM);
        snprintf(message, MSG_SIZE, "%s\n", status->chnl[0].topic);
#ifdef  TCP
        write_tcp(status->room->name);
#endif  /* TCP */
        write_log(config->log_main);
        write_str(status->room->name, COL_SYSTEM);
    } else {
        strncpy(status->channel, pars_word(), CHANNEL_MAXLEN);
        if (!strncmp(status->room->topic, pars_word(), STR_SIZE)) return;
        strncpy(status->chnl[chnl_getnum()].topic, packet, STR_SIZE);
        snprintf(message, MSG_SIZE, PARS_TOPICC, time_get(), status->channel);
#ifdef  TCP
        write_tcp(status->channel);
#endif  /* TCP */
        write_log(config->log_main);
        write_str(status->channel, COL_SYSTEM);
        snprintf(message, MSG_SIZE, "%s\n", status->chnl[chnl_getnum()].topic);
#ifdef  TCP
        write_tcp(status->channel);
#endif  /* TCP */
        write_log(config->log_main);
        write_str(status->channel, COL_SYSTEM);
    }
    refresh_room();
    screen_show();
    refresh_input();
    string_show();
}

void pars_mode(void) {
    strncpy(status->nick, packet, NICK_MAXLEN);
    snprintf(message, MSG_SIZE, PARS_MODE, time_get(), status->nick);
    pars_word();
    switch ((status->mode = pars_number())) {
        case MOD_NORMAL: strncat(message, PARS_MODENORMAL, STR_SIZE); break;
        case MOD_DND: strncat(message, PARS_MODEDND, STR_SIZE); break;
        case MOD_AWAY: strncat(message, PARS_MODEAWAY, STR_SIZE); break;
        case MOD_OFF: strncat(message, PARS_MODEOFF, STR_SIZE); break;
    }
#ifdef  TCP
    write_tcp(status->chnl[0].name);
#endif  /* TCP */
    if (!user_ignore() && config->verbose) {
        write_log(config->log_main);
        write_str(status->chnl[0].name, COL_SYSTEM);
    }
    if (!config->packet_qchat) {
        status->gender = pars_number();
        if (status->mode != MOD_NORMAL) {
            snprintf(message, MSG_SIZE, "%s\n", packet);
#ifdef  TCP
            write_tcp(status->chnl[0].name);
#endif  /* TCP */
            if (!user_ignore() && config->verbose) {
                write_log(config->log_main);
                write_str(status->chnl[0].name, COL_SYSTEM);
            }
        }
    }
    user_mode();
    if (!user_ignore() && (!config->my_only || !strncmp(status->room->name, status->channel, CHANNEL_MAXLEN)) &&
        (config->mode == MOD_NORMAL)) beep_system();
}

void pars_info(void) {
    if (strncmp(config->nick, packet, NICK_MAXLEN)) return;
    strncpy(status->nick, pars_word(), NICK_MAXLEN);
    if (user_ignore()) return;
    if (config->verbose) {
        snprintf(message, MSG_SIZE, PARS_INFO, time_get(), status->nick);
        write_log(config->log_main);
        write_str(status->chnl[0].name, COL_SYSTEM);
        if ((!config->my_only || !strncmp(status->room->name, status->channel, CHANNEL_MAXLEN)) &&
            (config->mode == MOD_NORMAL)) beep_system();
    }
    make_inform();
}

void pars_inform(void) {
    char host[STR_SIZE];
    char user[STR_SIZE];
    char addr[STR_SIZE];
    char chnlslist[STR_SIZE];
    char answer[STR_SIZE];

    if (config->packet_qchat) {
        if (strncmp(config->nick, packet, NICK_MAXLEN)) return;
        snprintf(message, MSG_SIZE, PARS_INFORM, time_get(), pars_word());
        write_log(config->log_main);
        write_str(status->room->name, COL_SYSTEM);
        strncpy(host, pars_word(), STR_SIZE);
        strncpy(user, pars_word(), STR_SIZE);
        strncpy(answer, pars_word(), STR_SIZE);
        strncpy(addr, pars_word(), STR_SIZE);
        strncpy(chnlslist, pars_word(), STR_SIZE);
        snprintf(message, MSG_SIZE, PARS_QINFOUH, user, host, addr, answer);
        write_log(config->log_main);
        write_str(status->room->name, COL_SYSTEM);
        chnlslist[strlen(chnlslist)-1] = '\0';
        *answer = '\0';
    } else {
        if (strncmp(config->nick, packet, NICK_MAXLEN)) return;
        snprintf(message, MSG_SIZE, PARS_INFORM, time_get(), pars_word());
        write_log(config->log_main);
        write_str(status->room->name, COL_SYSTEM);
        strncpy(host, pars_word(), STR_SIZE);
        strncpy(user, pars_word(), STR_SIZE);
        strncpy(addr, pars_word(), STR_SIZE);
        strncpy(chnlslist, pars_word(), STR_SIZE);
        strncpy(answer, pars_word(), STR_SIZE);
        snprintf(message, MSG_SIZE, PARS_INFOUH, user, host, addr);
        write_log(config->log_main);
        write_str(status->room->name, COL_SYSTEM);
        chnlslist[strlen(chnlslist)-1] = '\0';
    }
    if (strncmp(status->chnl[0].name, chnlslist, CHANNEL_MAXLEN)) {
        snprintf(message, MSG_SIZE, PARS_INFOCH, chnlslist);
        write_log(config->log_main);
        write_str(status->room->name, COL_SYSTEM);
    }
    if (*answer != '\0') {
        snprintf(message, MSG_SIZE, "%s\n", answer);
        write_log(config->log_main);
        write_str(status->room->name, COL_SYSTEM);
    }
}

void pars_beep(void) {
    switch (pars_number()) {
        case MSG_BEEPBEEP:
            if (strncmp(config->nick, packet, NICK_MAXLEN)) break;
            strncpy(status->nick, pars_word(), NICK_MAXLEN);
            if (user_ignore()) break;
            snprintf(message, MSG_SIZE, PARS_BEEP, time_get(), status->nick);
            if (config->verbose) {
                write_log(config->log_main);
                write_str(status->room->name, COL_MESG);
            }
            if ((config->mode == MOD_NORMAL) || (config->mode == MOD_DND) ||
                    ((config->mode == MOD_AWAY) && user_favorite())) {
                make_beepack();
                beep_beep();
            }
            break;
        case MSG_BEEPACK:
            if (strncmp(config->nick, packet, NICK_MAXLEN)) break;
            if (!config->packet_qchat) {
                strncpy(status->nick, pars_word(), NICK_MAXLEN); 
                status->gender = pars_number();
            }
            snprintf(message, MSG_SIZE, PARS_BEEPACK, time_get(), status->nick);
            write_log(config->log_main);
            write_str(status->room->name, COL_MESG);
            break;
        default:
#ifdef  DEBUG
            fprintf(debug_file, "error: Unknown beep type.\n");
            fflush(debug_file);
#endif  /* DEBUG */
            break;
    }
}

void pars_beepreq(void) {        /* FIXME: who needs this??? */
    status_ok(PARS_SBEEPREQ);
}

void pars_privat(void) {
    int privat;

    privat = pars_number();
    strncpy(status->nick, packet, NICK_MAXLEN);
    if (user_ignore()) return;
    strncpy(status->channel, packet, NICK_MAXLEN);
    if (!strncmp(config->nick, status->nick, NICK_MAXLEN))
        strncpy(status->channel, pars_word(), NICK_MAXLEN);
    else if (strncmp(config->nick, pars_word(), NICK_MAXLEN)) return;
    switch (privat) {
        case MSG_PRIVATJOIN:
            if (!config->packet_qchat) {
                pars_word(); status->gender = pars_number();
            }
            chat_join();
            break;
        case MSG_PRIVATLEAVE:
            if (!config->packet_qchat) {
                pars_word(); status->gender = pars_number();
            }
            chat_leave();
            break;
        case MSG_PRIVATCHAT:
            chat_rejoin();
            snprintf(message, MSG_SIZE, "<%s> %s\n", status->nick, pars_word());
            write_log(config->log_priv);
            write_str(status->channel, COL_CHAT);
            if ((!config->my_only || !strncmp(status->room->name, status->channel, CHANNEL_MAXLEN)) &&
                (config->mode == MOD_NORMAL)) beep_chat();
            break;
        case MSG_PRIVATME:
            chat_rejoin();
            snprintf(message, MSG_SIZE, " * %s %s\n", status->nick, pars_word());
            write_log(config->log_priv);
            write_str(status->channel, COL_CHAT);
            if ((!config->my_only || !strncmp(status->room->name, status->channel, CHANNEL_MAXLEN)) &&
                (config->mode == MOD_NORMAL)) beep_chat();
            break;
        default:
#ifdef  DEBUG
            fprintf(debug_file, "error: Unknown private type.\n");
            fflush(debug_file);
#endif  /* DEBUG */
            break;
    }
}

void pars_here(void) {
    strncpy(status->nick, packet, NICK_MAXLEN);
    strncpy(status->channel, pars_word(), CHANNEL_MAXLEN);
    if (strncmp(status->chnl[0].name, status->channel, CHANNEL_MAXLEN) &&
        !chnl_getnum()) return;
    if (config->mode != MOD_OFF) make_wehere();
}

void pars_wehere(void) {
    if (strncmp(config->nick, packet, NICK_MAXLEN)) return;
    strncpy(status->channel, pars_word(), CHANNEL_MAXLEN);
    strncpy(status->nick, pars_word(), NICK_MAXLEN);
    pars_word();
    snprintf(message, MSG_SIZE, PARS_HERE, time_get(), status->nick, (pars_number() ? PARS_HEREACT : PARS_HERENAC));
    write_log(config->log_main);
    write_str(status->channel, COL_SYSTEM);
}

void pars_active(void) {
    strncpy(status->nick, packet, NICK_MAXLEN);
    pars_word();
    status->active = pars_number();
    user_active();
}

void pars_userchnls(void) {
    strncpy(status->nick, packet, NICK_MAXLEN);
    if (config->mode != MOD_OFF) make_userchnlslist();
}

void pars_userchnlslist(void) {
    if (strncmp(config->nick, packet, NICK_MAXLEN)) return;
    strncpy(config->channel, pars_word(), CHANNEL_MAXLEN);
    chnl_parse();
}

void pars_message(void) {
    switch (pars_head()) {
        case MSG_WHOHERE: pars_whohere(); break;
        case MSG_IMHERE: pars_imhere(); break;
        case MSG_CHAT: pars_chat(); break;
        case MSG_NICK: pars_nick(); break;
        case MSG_JOIN: pars_join(); break;
        case MSG_LEAVE: pars_leave(); break;
        case MSG_MESG: pars_mesg(); break;
        case MSG_MASS: pars_mass(); break;
        case MSG_MESGACK: pars_mesgack(); break;
        case MSG_EXEC: pars_exec(); break;
        case MSG_EXECACK: pars_execack(); break;
        case MSG_ME: pars_me(); break;
        case MSG_NEWTOPIC: pars_newtopic(); break;
        case MSG_TOPIC: pars_topic(); break;
        case MSG_MODE: pars_mode(); break;
        case MSG_INFO: pars_info(); break;
        case MSG_INFORM: pars_inform(); break;
        case MSG_BEEP: pars_beep(); break;
        case MSG_BEEPREQ: pars_beepreq(); break;
        case MSG_PRIVAT: pars_privat(); break;
        case MSG_HERE: pars_here(); break;
        case MSG_WEHERE: pars_wehere(); break;
        case MSG_ACTIVE: pars_active(); break;
        case MSG_USERCHNLS: pars_userchnls(); break;
        case MSG_USERCHNLSLIST: pars_userchnlslist(); break;
#ifdef  DEBUG
        default:
            fprintf(debug_file, "error: Unknown message type.\n");
            fflush(debug_file);
#endif  /* DEBUG */
    }
}
