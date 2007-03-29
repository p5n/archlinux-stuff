/*  pckt_make.c
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#include <string.h>
#include <unistd.h>
#include "echat.h"
#include "pckt_type.h"

char *make_head(void) {
    static char head[HEADER_LEN];
    register int n;

    if (config->packet_qchat) *head = '\0';
    else {
        if (config->packet_xor) *head = TYPE_VYPRESSXOR;
        else
            *head = TYPE_VYPRESS;
        for (n = 1; n < HEADER_LEN; n++)
            head[n] = 'a' + ((char) rand() & 0x0f);
    }
    return (head);
}

void make_whohere(void) {
    snprintf(packet, MSG_SIZE, "%s%c%s%c%s", 
        make_head(), MSG_WHOHERE,
        config->nick, MSG_SPLIT, PACKET_EOF);
    sock_bsend();
}

void make_imhere(void) {
    snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%u%u%s",
        make_head(), MSG_IMHERE,
        status->nick, MSG_SPLIT,
        config->nick, MSG_SPLIT,
        config->mode, config->active, PACKET_EOF);
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
}

void make_chat(void) {
    snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s%c%s",
        make_head(), MSG_CHAT,
        status->room->name, MSG_SPLIT,
        config->nick, MSG_SPLIT,
        buf, MSG_SPLIT, PACKET_EOF);
    sock_bsend();
    status_ok(MAKE_SCHAT);
}

void make_nick(void) {
    if (config->packet_qchat) {
        snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s",
            make_head(), MSG_NICK,
            status->nick, MSG_SPLIT,
            config->nick, MSG_SPLIT, PACKET_EOF);
    } else {
        snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%u%s",
            make_head(), MSG_NICK,
            status->nick, MSG_SPLIT,
            config->nick, MSG_SPLIT,
            config->gender, PACKET_EOF);
    }
    sock_bsend();
    status_ok(MAKE_SNICK);
}

void make_join(void) {
    if (config->packet_qchat) {
        snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%u%s",
            make_head(), MSG_JOIN,
            config->nick, MSG_SPLIT,
            status->channel, MSG_SPLIT,
            config->mode, PACKET_EOF);
    } else {
        snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%u%u%s",
            make_head(), MSG_JOIN,
            config->nick, MSG_SPLIT,
            status->channel, MSG_SPLIT,
            config->mode, config->gender, PACKET_EOF);
    }
    sock_bsend();
}

void make_leave(void) {
    if (config->packet_qchat) {
        snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s",
            make_head(), MSG_LEAVE,
            config->nick, MSG_SPLIT,
            status->channel, MSG_SPLIT, PACKET_EOF);
    } else {
        snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%u%s",
            make_head(), MSG_LEAVE,
            config->nick, MSG_SPLIT,
            status->channel, MSG_SPLIT,
            config->gender, PACKET_EOF);
    }
    sock_bsend();
}

void make_mesg(void) {
    snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s%c%s",
        make_head(), MSG_MESG,
        config->nick, MSG_SPLIT,
        status->nick, MSG_SPLIT,
        buf, MSG_SPLIT, PACKET_EOF);
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
    status_ok(MAKE_SMESG);
}

void make_mass(void) {
    snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s%c%s",
        make_head(), MSG_MASS,
        config->nick, MSG_SPLIT,
        status->nick, MSG_SPLIT,
        buf, MSG_SPLIT, PACKET_EOF);
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
    status_ok(MAKE_SMASS);
}

void make_mesgack(void) {
    if (config->packet_qchat) {
        snprintf(packet, MSG_SIZE, "%s%c%u%s%c%s%c%s",
            make_head(), MSG_MESGACK,
            config->mode,
            status->nick, MSG_SPLIT,
            config->nick, MSG_SPLIT, PACKET_EOF);
    } else {
        snprintf(packet, MSG_SIZE, "%s%c%u%s%c%s%c%u%s%s%s%c%s",
            make_head(), MSG_MESGACK,
            config->mode,
            status->nick, MSG_SPLIT,
            config->nick, MSG_SPLIT,
            config->gender,
            (config->mode == MOD_DND) ? config->answer_dnd : "",
            (config->mode == MOD_AWAY) ? config->answer_away : "",
            (config->mode == MOD_OFF) ? config->answer_off : "",
            MSG_SPLIT, PACKET_EOF);
    }
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
}

void make_exec(void) {
    snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s%c%s%c%s",
        make_head(), MSG_EXEC,
        config->nick, MSG_SPLIT,
        status->nick, MSG_SPLIT,
        buf, MSG_SPLIT,
        "", MSG_SPLIT, PACKET_EOF);        /* FIXME: no passwords */
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
}

void make_execack(void) {
    snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s%c%s",
        make_head(), MSG_EXECACK,
        status->nick, MSG_SPLIT,
        config->nick, MSG_SPLIT,
        "Get out!!!", MSG_SPLIT, PACKET_EOF);
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
}

void make_me(void) {
    snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s%c%s",
        make_head(), MSG_ME,
        status->room->name, MSG_SPLIT,
        config->nick, MSG_SPLIT,
        buf, MSG_SPLIT, PACKET_EOF);
    sock_bsend();
    status_ok(MAKE_SME);
}

void make_newtopic(void) {
    if (config->packet_qchat) {
        snprintf(packet, MSG_SIZE, "%s%c%s%c%s",
            make_head(), MSG_NEWTOPIC,
            status->chnl[0].topic, MSG_SPLIT, PACKET_EOF);
    } else {
        snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s",
            make_head(), MSG_NEWTOPIC,
            status->room->name, MSG_SPLIT,
            status->room->topic, MSG_SPLIT, PACKET_EOF);
    }
    sock_bsend();
    status_ok(MAKE_STOPIC);
}

void make_topic(void) {
    if (!strncmp(config->nick, status->nick, NICK_MAXLEN)) return;
    if (config->packet_qchat) {
        snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s",
            make_head(), MSG_TOPIC,
            status->nick, MSG_SPLIT,
            status->chnl[0].topic, MSG_SPLIT, PACKET_EOF);
    } else {
        snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s%c%s",
            make_head(), MSG_TOPIC,
            status->nick, MSG_SPLIT,
            status->channel, MSG_SPLIT,
            status->chnl[chnl_getnum()].topic, MSG_SPLIT, PACKET_EOF);
    }
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
}

void make_mode(void) {
    if (config->packet_qchat) {
        snprintf(packet, MSG_SIZE, "%s%c%s%c%u%s",
            make_head(), MSG_MODE,
            config->nick, MSG_SPLIT,
            config->mode, PACKET_EOF);
    } else {
        snprintf(packet, MSG_SIZE, "%s%c%s%c%u%u%s%s%s%c%s",
            make_head(), MSG_MODE,
            config->nick, MSG_SPLIT,
            config->mode, config->gender,
            (config->mode == MOD_DND) ? config->answer_dnd : "",
            (config->mode == MOD_AWAY) ? config->answer_away : "",
            (config->mode == MOD_OFF) ? config->answer_off : "",
            MSG_SPLIT, PACKET_EOF);
    }
    sock_bsend();
    status_ok(MAKE_SMODE);
}

void make_info(void) {
    snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s",
        make_head(), MSG_INFO,
        status->nick, MSG_SPLIT,
        config->nick, MSG_SPLIT, PACKET_EOF);
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
}

void make_inform(void) {
    char host[STR_SIZE];
    char addr[ADDR_SIZE];
    char chnlslist[STR_SIZE];
    register int n;

    if (config->packet_qchat) {
        gethostname(host, STR_SIZE);
        for (n = 0; n < status->users; n++)
            if (!strncmp(status->user[n].nick, config->nick, NICK_MAXLEN)) break;
        if (n == status->users) *addr = '\0';
        else strncpy(addr, status->user[n].addr, ADDR_SIZE);
        strncpy(chnlslist, status->chnl[0].name, CHANNEL_MAXLEN);
        for (n = 1; n < status->channels; n++)
            if (status->chnl[n].name[0] == CHR_CHNL)
                strncat(chnlslist, status->chnl[n].name, CHANNEL_MAXLEN);
        snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s%c%s%c%s%c0K%c%s#%c%s",
            make_head(), MSG_INFORM,
            status->nick, MSG_SPLIT,
            config->nick, MSG_SPLIT,
            host, MSG_SPLIT,
            config->nick, MSG_SPLIT,
            addr, MSG_SPLIT, MSG_SPLIT,
            chnlslist, MSG_SPLIT, PACKET_EOF);
    } else {
        gethostname(host, STR_SIZE);
        for (n = 0; n < status->users; n++)
            if (!strncmp(status->user[n].nick, config->nick, NICK_MAXLEN)) break;
        if (n == status->users) *addr = '\0';
        else strncpy(addr, status->user[n].addr, ADDR_SIZE);
        strncpy(chnlslist, status->chnl[0].name, CHANNEL_MAXLEN);
        for (n = 1; n < status->channels; n++)
            if (status->chnl[n].name[0] == CHR_CHNL)
                strncat(chnlslist, status->chnl[n].name, CHANNEL_MAXLEN);
        snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s%c%s%c%s%c%s#%c%s%s%s%c%s",
            make_head(), MSG_INFORM,
            status->nick, MSG_SPLIT,
            config->nick, MSG_SPLIT,
            host, MSG_SPLIT,
            config->nick, MSG_SPLIT,
            addr, MSG_SPLIT,
            chnlslist, MSG_SPLIT,
            (config->mode == MOD_DND) ? config->answer_dnd : "",
            (config->mode == MOD_AWAY) ? config->answer_away : "",
            (config->mode == MOD_OFF) ? config->answer_off : "",
            MSG_SPLIT, PACKET_EOF);
    }
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
    status_ok(MAKE_SINFO);
}

void make_beepbeep(void) {
    snprintf(packet, MSG_SIZE, "%s%c%u%s%c%s%c%s",
        make_head(), MSG_BEEP, MSG_BEEPBEEP,
        status->nick, MSG_SPLIT,
        config->nick, MSG_SPLIT, PACKET_EOF);
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
    status_ok(MAKE_SBEEP);
}

void make_beepack(void) {
    if (config->packet_qchat) {
        snprintf(packet, MSG_SIZE, "%s%c%u%s%c%s%c%s",
            make_head(), MSG_BEEP, MSG_BEEPACK,
            status->nick, MSG_SPLIT,
            config->nick, MSG_SPLIT, PACKET_EOF);
    } else {
        snprintf(packet, MSG_SIZE, "%s%c%u%s%c%s%c%u%s",
            make_head(), MSG_BEEP, MSG_BEEPACK,
            status->nick, MSG_SPLIT,
            config->nick, MSG_SPLIT, config->gender, PACKET_EOF);
    }
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
}

void make_beepreq(void) {                /* FIXME: dont implemented */
    snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s%c%s",
        make_head(), MSG_BEEPREQ,
        config->nick, MSG_SPLIT,
        buf, MSG_SPLIT,
        status->room->name, MSG_SPLIT, PACKET_EOF);
    sock_bsend();
}

void make_privatjoin(void) {
    if (config->packet_qchat) {
        snprintf(packet, MSG_SIZE, "%s%c%u%s%c%s%c%s",
            make_head(), MSG_PRIVAT, MSG_PRIVATJOIN,
            config->nick, MSG_SPLIT,
            status->channel, MSG_SPLIT, PACKET_EOF);
    } else {
        snprintf(packet, MSG_SIZE, "%s%c%u%s%c%s%c%u%s",
            make_head(), MSG_PRIVAT, MSG_PRIVATJOIN,
            config->nick, MSG_SPLIT,
            status->channel, MSG_SPLIT,
            config->gender, PACKET_EOF);
    }
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
    status_ok(MAKE_SPRIVJOIN);
}

void make_privatleave(void) {
    if (config->packet_qchat) {
        snprintf(packet, MSG_SIZE, "%s%c%u%s%c%s%c%s",
            make_head(), MSG_PRIVAT, MSG_PRIVATLEAVE,
            config->nick, MSG_SPLIT,
            status->channel, MSG_SPLIT, PACKET_EOF);
    } else {
        snprintf(packet, MSG_SIZE, "%s%c%u%s%c%s%c%u%s",
            make_head(), MSG_PRIVAT, MSG_PRIVATLEAVE,
            config->nick, MSG_SPLIT,
            status->channel, MSG_SPLIT,
            config->gender, PACKET_EOF);
    }
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
    status_ok(MAKE_SPRIVLEAVE);
}

void make_privatchat(void) {
    snprintf(packet, MSG_SIZE, "%s%c%u%s%c%s%c%s%c%s",
        make_head(), MSG_PRIVAT, MSG_PRIVATCHAT,
        config->nick, MSG_SPLIT,
        status->channel, MSG_SPLIT,
        buf, MSG_SPLIT, PACKET_EOF);
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
    status_ok(MAKE_SPRIVCHAT);
}

void make_privatme(void) {
    snprintf(packet, MSG_SIZE, "%s%c%u%s%c%s%c%s%c%s",
        make_head(), MSG_PRIVAT, MSG_PRIVATME,
        config->nick, MSG_SPLIT,
        status->channel, MSG_SPLIT,
        buf, MSG_SPLIT, PACKET_EOF);
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
    status_ok(MAKE_SPRIVME);
}

void make_here(void) {
    snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s",
        make_head(), MSG_HERE,
        config->nick, MSG_SPLIT,
        status->channel, MSG_SPLIT, PACKET_EOF);
    sock_bsend();
}

void make_wehere(void) {
    snprintf(packet, MSG_SIZE, "%s%c%s%c%s%c%s%c%u%s",
        make_head(), MSG_WEHERE,
        status->nick, MSG_SPLIT,
        status->channel, MSG_SPLIT,
        config->nick, MSG_SPLIT, config->active, PACKET_EOF);
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
    status_ok(MAKE_SHERE);
}

void make_active(void) {
    snprintf(packet, MSG_SIZE, "%s%c%s%c%u%s",
        make_head(), MSG_ACTIVE,
        config->nick, MSG_SPLIT, config->active, PACKET_EOF);
    sock_bsend();
    status_ok(MAKE_SACTIVE);
}

void make_userchnls(void) {
    snprintf(packet, MSG_SIZE, "%s%c%s%c%s",
        make_head(), MSG_USERCHNLS,
        config->nick, MSG_SPLIT, PACKET_EOF);
    sock_bsend();
}

void make_userchnlslist(void) {
    char chnlslist[STR_SIZE];
    register int n;

    strncpy(chnlslist, status->chnl[0].name, CHANNEL_MAXLEN);
    for (n = 1; n < status->channels; n++)
        if (status->chnl[n].name[0] == CHR_CHNL)
            strncat(chnlslist, status->chnl[n].name, CHANNEL_MAXLEN);
    snprintf(packet, MSG_SIZE, "%s%c%s%c%s#%c%s",
        make_head(), MSG_USERCHNLSLIST,
        status->nick, MSG_SPLIT,
        chnlslist, MSG_SPLIT, PACKET_EOF);
    if (config->packet_bcast) sock_bsend();
    else sock_usend();
}
