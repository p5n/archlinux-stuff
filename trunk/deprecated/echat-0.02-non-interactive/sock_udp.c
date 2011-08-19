/*  sock_udp.c
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include "echat.h"

#ifdef  DEBUG
# include <errno.h>
#endif  /* DEBUG */

#define POLL_TIMEOUT    1

struct sockaddr_in *sacl, *sacu, *sacb[ADDRS_MAX];
int sfdcl, sfdcu, sfdcb;
int len;

int packet_len(void) {
    char peof[] = PACKET_EOF;
    register int n;

    for (n = 0; n < MSG_SIZE;)
        if ((packet[n++] == peof[0]) && (packet[n] == peof[1])) return (n-1);
#ifdef  DEBUG
    fprintf(debug_file, "error: Invalid packet: %s\n", packet);
    fflush(debug_file);
#endif  /* DEBUG */
    return 0;
}

void packet_xor(void) {
    register int n;

    for (n = HEADER_LEN; n < len; packet[n++] ^= config->packet_xor);
}

int packet_buffer(void) {
    register int n;

    for (n = 0; n < PACK_SIZE; n++)
        if (!strncmp(packet, status->packet_buf[n], HEADER_LEN)) {
#ifdef  DEBUG
            fprintf(debug_file, "buffer: dupe(%15s): %s\n", status->address, packet);
            fflush(debug_file);
#endif  /* DEBUG */
            return -1;
        }
    for (n = PACK_SIZE-2; n > 0; n--)
        strncpy(status->packet_buf[n], status->packet_buf[n-1], HEADER_LEN);
    strncpy(status->packet_buf[n], packet, HEADER_LEN);
    return 0;
}

void sock_init(void) {
    int s = 1;
    register int n;

    if ((sacl = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in))) == NULL) {
#ifdef  DEBUG
        fprintf(debug_file, "malloc(): %s\n", strerror(errno));
#endif  /* DEBUG */
        exit(EXIT_FAILURE);
    }
    if ((sacu = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in))) == NULL) {
#ifdef  DEBUG
        fprintf(debug_file, "malloc(): %s\n", strerror(errno));
#endif  /* DEBUG */
        exit(EXIT_FAILURE);
    }
    for (n = 0; n < status->addrs; n++)
        if ((sacb[n] = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in))) == NULL) {
#ifdef  DEBUG
            fprintf(debug_file, "malloc(): %s\n", strerror(errno));
#endif  /* DEBUG */
            exit(EXIT_FAILURE);
        }

    memset(sacl, 0, sizeof(struct sockaddr_in));
    sacl->sin_family = AF_INET;
    sacl->sin_port = htons(config->port);
/* SERGEJ: */
    sacl->sin_addr.s_addr = INADDR_ANY;
/*    sacl->sin_addr.s_addr = inet_addr("192.168.0.1"); */

    memset(sacu, 0, sizeof(struct sockaddr_in));
    sacu->sin_family = AF_INET;
    sacu->sin_port = htons(config->port);

    for (n = 0; n < status->addrs; n++) {
        memset(sacb[n], 0, sizeof(struct sockaddr_in));
        sacb[n]->sin_family = AF_INET;
        sacb[n]->sin_port = htons(config->port);
        if (!inet_aton(status->addr[n], &sacb[n]->sin_addr)) {
#ifdef  DEBUG
            fprintf(debug_file, "eChat: Invalid address %s\n", status->address);
            fflush(debug_file);
#endif  /* DEBUG */
            fprintf(stderr, "eChat: Invalid address.\n");
            exit(EXIT_FAILURE);
        }
    }

    if ((sfdcl = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
#ifdef  DEBUG
        fprintf(debug_file, "socket(): %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
        fprintf(stderr, "eChat: Socket allocation failed.\n");
        exit(EXIT_FAILURE);
    }
#ifdef  PORTREUSE
    if (setsockopt(sfdcl, SOL_SOCKET, SO_REUSEADDR, &s, sizeof(s)) < 0) {
#ifdef  DEBUG
        fprintf(debug_file, "setsockopt(): %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
        fprintf(stderr, "eChat: Set socket option failed.\n");
        exit(EXIT_FAILURE);
    }
#endif  /* PORTREUSE */
    if (bind(sfdcl, (struct sockaddr *) sacl, sizeof(struct sockaddr)) < 0) {
#ifdef  DEBUG
        fprintf(debug_file, "bind(): %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
        fprintf(stderr, "eChat: Bind socket failed.\n");
        exit(EXIT_FAILURE);
    }

    if ((sfdcu = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
#ifdef  DEBUG
        fprintf(debug_file, "socket(): %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
        snprintf(message, STR_SIZE, SOCK_ALLOC, time_get());
        write_str(status->room->name, COL_ERROR);
        return;
    }

    if ((sfdcb = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
#ifdef  DEBUG
        fprintf(debug_file, "socket(): %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
        fprintf(stderr, "eChat: Socket allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(sfdcb, SOL_SOCKET, SO_BROADCAST, &s, sizeof(s)) < 0) {
#ifdef  DEBUG
        fprintf(debug_file, "setsockopt(): %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
        fprintf(stderr, "eChat: Set socket option failed.\n");
        exit(EXIT_FAILURE);
    }

#ifdef  TCP
    sock_tcpinit();
#endif  /* TCP */
}

void sock_kill(void) {
    register int n;

#ifdef  TCP
    sock_tcpkill();
#endif  /* TCP */
    close(sfdcl);
    close(sfdcu);
    close(sfdcb);
    free(sacl);
    free(sacu);
    for (n = 0; n < status->addrs; n++) free(sacb[n]);
}

void sock_route(void) {
    register int n;

    for (n = 0; n < status->addrs; n++) {
        if (!addr_netcmp(status->addr[n], status->address)) continue;

/*        if (!strncmp(status->address, LOCALHOST, ADDR_SIZE)) continue; */

#ifdef  DEBUG
        fprintf(debug_file, "sendto(%15s): __%s__ (%u bytes)\n",
            inet_ntoa(sacb[n]->sin_addr), packet, len);
        fflush(debug_file);
#endif  /* DEBUG */
        if (sendto(sfdcb, packet, len, 0, (struct sockaddr *) sacb[n], sizeof(struct sockaddr)) < 0) {
#ifdef  DEBUG
            fprintf(debug_file, "sendto(): %s\n", strerror(errno));
            fflush(debug_file);
#endif  /* DEBUG */
            if (config->status_str) status_error(SOCK_SROUTE);
            else {
                snprintf(message, STR_SIZE, SOCK_ROUTE, time_get(), inet_ntoa(sacb[n]->sin_addr));
                write_str(status->room->name, COL_ERROR);
            }
        }
    }
}

void sock_recv(void) {
    struct pollfd usfd;

/* SERGEJ */
	memset(packet, 0, MSG_SIZE);

    usfd.fd = sfdcl;
    usfd.events = POLLIN | POLLPRI;
    while (poll(&usfd, 1, POLL_TIMEOUT) > 0) {
        len = sizeof(struct sockaddr);
        if ((len = recvfrom(sfdcl, packet, MSG_SIZE, 0, (struct sockaddr *) sacl, &len)) < 0) {
#ifdef  DEBUG
            fprintf(debug_file, "recvfrom(): %s\n", strerror(errno));
            fflush(debug_file);
#endif  /* DEBUG */
            if (config->status_str) status_error(SOCK_SRECEIVE);
            else {
                snprintf(message, STR_SIZE, SOCK_RECV, time_get(), inet_ntoa(sacl->sin_addr));
                write_str(status->room->name, COL_ERROR);
            }
            return;
        }
        strncpy(status->address, inet_ntoa(sacl->sin_addr), ADDR_SIZE);
#ifdef  DEBUG
        fprintf(debug_file, "recvfrom(%15s): __%s__ (%u bytes)\n",
            status->address, packet, len);
        fflush(debug_file);
#endif  /* DEBUG */
        if (!config->packet_qchat)
            if (packet_buffer()) return;
        if (config->packet_route) sock_route();
        if (!config->packet_qchat)
            if (config->packet_xor) packet_xor();
/* SERGEJ: DEBUG */
#if 0
{
int m;
FILE *f=fopen("/tmp/echat-packet-dump.dat", "a");
for (m = 0; m < MSG_SIZE; m++)
{
	fprintf(f, "%c", packet[m]);
}
fprintf(f, "\n");
fclose(f);
}
#endif
/* SERGEJ */
#ifdef  CHARSET
        if (config->charset) cset_outin();
#endif  /* CHARSET */
        pars_message();
    }
}

void sock_usend(void) {
    if (!inet_aton(status->address, &sacu->sin_addr)) {
#ifdef  DEBUG
        fprintf(debug_file, "inet_aton(%15s): invalid address for %s\n",
            status->address, status->nick);
        fflush(debug_file);
#endif  /* DEBUG */
        snprintf(message, STR_SIZE, SOCK_ADDRESS, time_get(), status->nick, status->address);
        write_str(status->room->name, COL_ERROR);
        return;
    }
    if (!(len = packet_len())) return;
#ifdef  CHARSET
    if (config->charset) cset_inout();
#endif  /* CHARSET */
    if (!config->packet_qchat)
        if (config->packet_xor) packet_xor();
#ifdef  DEBUG
    fprintf(debug_file, "sendto(%15s): __%s__ (%u bytes)\n",
        status->address, packet, len);
    fflush(debug_file);
#endif  /* DEBUG */
    if (sendto(sfdcu, packet, len, 0, (struct sockaddr *) sacu, sizeof(struct sockaddr)) < 0) {
#ifdef  DEBUG
        fprintf(debug_file, "sendto(): %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
        if (config->status_str) status_error(SOCK_SUSEND);
        else {
            snprintf(message, STR_SIZE, SOCK_USEND, time_get(), status->address);
            write_str(status->room->name, COL_ERROR);
        }
    }
}

void sock_bsend(void) {
    register int n;

#ifdef  CHARSET
    if (config->charset) cset_inout();
#endif  /* CHARSET */
    if (!(len = packet_len())) return;
    if (!config->packet_qchat)
        if (config->packet_xor) packet_xor();
    for (n = 0; n < status->addrs; n++) {
#ifdef  DEBUG
        fprintf(debug_file, "sendto(%15s): __%s__ (%u bytes)\n",
            inet_ntoa(sacb[n]->sin_addr), packet, len);
        fflush(debug_file);
#endif  /* DEBUG */
        if (sendto(sfdcb, packet, len, 0, (struct sockaddr *) sacb[n], sizeof(struct sockaddr)) < 0) {
#ifdef  DEBUG
            fprintf(debug_file, "sendto(): %s\n", strerror(errno));
            fflush(debug_file);
#endif  /* DEBUG */
            if (config->status_str) status_error(SOCK_SBSEND);
            else {
                snprintf(message, STR_SIZE, SOCK_BSEND, time_get(), inet_ntoa(sacb[n]->sin_addr));
                write_str(status->room->name, COL_ERROR);
            }
        }
    }
}
