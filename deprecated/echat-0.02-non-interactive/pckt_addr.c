/*  pckt_addr.c
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#include <string.h>
#include "echat.h"

void addr_add(void) {
    register int n;

    for (n = ADDRS_MAX-1; n > 0; n--)
        strncpy(status->addr[n], status->addr[n-1], ADDR_SIZE);
    strncpy(status->addr[0], status->address, ADDR_SIZE);
    if (status->addrs < ADDRS_MAX) status->addrs++;
}

void addr_del(void) {
    register int n;

    for (n = 0; n < ADDRS_MAX-1; n++)
        if (!strncmp(status->addr[n], status->address, ADDR_SIZE)) break;
    for (; n < ADDRS_MAX-1; n++)
        strncpy(status->addr[n], status->addr[n+1], ADDR_SIZE);
    status->addr[n][0] = '\0';
}

void addr_parse(void) {
    char str[ADDR_SIZE+1];
    register int n, m;

    for (n = 0; n < strlen(config->address); n++)
        for (m = 0; m < ADDR_SIZE+1; m++, n++)
            if (((str[m] = config->address[n]) == ' ') || (str[m] == '\0')) {
                str[m] = '\0';
                strncpy(status->address, str, ADDR_SIZE);
                addr_add();
                break;
            }
}

int addr_netcmp(char *addr1, char *addr2) {
    char str1[ADDR_SIZE+1], str2[ADDR_SIZE+1];
    register int n;

    for (n = strlen(addr1), strncpy(str1, addr1, ADDR_SIZE); n > 0; n--)
        if (str1[n] == '.') { str1[n] = '\0'; break; }
    for (n = strlen(addr2), strncpy(str2, addr2, ADDR_SIZE); n > 0; n--)
        if (str2[n] == '.') { str2[n] = '\0'; break; }
    return (strncmp(str1, str2, ADDR_SIZE));
}

void addr_init(void) {

}
