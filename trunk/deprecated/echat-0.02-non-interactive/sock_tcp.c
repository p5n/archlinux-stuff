/*  sock_tcp.c
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#ifdef  TCP

#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include "echat.h"

#ifdef  DEBUG
# include <errno.h>
#endif  /* DEBUG */

#define BACKLOG         5
#define POLL_TIMEOUT    0.01

struct sockaddr_in *satl;
int sfdtl, sfdta = -1;

void sock_tcpinit(void) {

    if ((satl = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in))) == NULL) {
#ifdef  DEBUG
        fprintf(debug_file, "malloc(): %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
        return;
    }

    memset(satl, 0, sizeof(struct sockaddr_in));
    satl->sin_family = AF_INET;
    satl->sin_port = htons(config->port);
/* SERGEJ: */
    satl->sin_addr.s_addr = INADDR_ANY;
/*    satl->sin_addr.s_addr = inet_addr("192.168.0.1"); */

    if ((sfdtl = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
#ifdef  DEBUG
        fprintf(debug_file, "socket(): %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
        return;
    }

    if (bind(sfdtl, (struct sockaddr *) satl, sizeof(struct sockaddr)) < 0) {
#ifdef  DEBUG
        fprintf(debug_file, "bind(): %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
        return;
    }

    if (listen(sfdtl, BACKLOG) < 0) {
#ifdef  DEBUG
        fprintf(debug_file, "listen(): %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
        return;
    }
}

void sock_tcpkill(void) {
    close(sfdtl);
    free(satl);
}

void sock_tcpdrop(void) {
    status->clients--;
    close(sfdta);
    sfdta = -1;
}

void sock_tcpconn(void) {
    if (status->clients++ == (CLIENTS_MAX-1)) {
        snprintf(message, MSG_SIZE, SOCK_TOUT);
        send(sfdta, message, strlen(message), 0);
        sock_tcpdrop();
    }
    snprintf(message, MSG_SIZE, SOCK_TWELCOME);
    send(sfdta, message, strlen(message), 0);
    snprintf(message, STR_SIZE, SOCK_TCONN, time_get());
    write_log(config->log_main);
    write_str(status->room->name, COL_SYSTEM);
    if (config->mode == MOD_NORMAL) beep_system();
}

void sock_tcprecv(void) {
    struct pollfd usfd;
    int s = sizeof(struct sockaddr_in);

    if (sfdta < 0) {
        usfd.fd = sfdtl;
        usfd.events = POLLIN | POLLPRI;
        while (poll(&usfd, 1, POLL_TIMEOUT) > 0) {
            if ((sfdta = accept(sfdtl, (struct sockaddr *) satl, &s)) < 0) {
#ifdef  DEBUG
                fprintf(debug_file, "accept(): %s\n", strerror(errno));
                fflush(debug_file);
#endif  /* DEBUG */
                return;
            }
            sock_tcpconn();
        }
    }
}

void sock_tcpsend(void) {
    if (sfdta > 0)
#ifdef  LINUX
        if (send(sfdta, message, strlen(message), MSG_NOSIGNAL) < 0)
#endif  /* LINUX */
        if (send(sfdta, message, strlen(message), 0) < 0)
            sock_tcpdrop();
}

#endif  /* TCP */
