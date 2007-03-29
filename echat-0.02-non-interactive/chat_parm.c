/*  chat_parm.c
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#include <string.h>
#include <unistd.h>
#include "echat.h"

void parm_help(void) {
    fprintf(stdout, "available options:\n");
    fprintf(stdout, "    -n <nick>       nick\n");
    fprintf(stdout, "    -g <0|1>        gender\n");
    fprintf(stdout, "    -c <channel>    channel\n");
    fprintf(stdout, "    -m <0|1|2|3>    mode\n");
    fprintf(stdout, "    -i <0|1>        activity\n");
    fprintf(stdout, "    -a <address>    address\n");
    fprintf(stdout, "    -p <port>       port\n");
    fprintf(stdout, "    -l <filename>   logfiles\n");
    fprintf(stdout, "    -s              server\n");
    fprintf(stdout, "    -v              verbose\n");
    fprintf(stdout, "    -d              show defines\n");
    fprintf(stdout, "    -h              this help\n");
}

void echat_defines(void) {
#ifdef  LINUX
    strncat(message, "-DLINUX ", STR_SIZE);
#endif  /* LINUX */
#ifdef  FREEBSD
    strncat(message, "-DFREEBSD ", STR_SIZE);
#endif  /* FREEBSD */
#ifdef  EN
    strncat(message, "-DEN ", STR_SIZE);
#endif  /* EN */
#ifdef  RU
    strncat(message, "-DRU ", STR_SIZE);
#endif  /* RU */
#ifdef  TU
    strncat(message, "-DTU ", STR_SIZE);
#endif  /* TU */
#ifdef  CHARSET
    strncat(message, "-DCHARSET ", STR_SIZE);
#endif  /* CHARSET */
#ifdef  PORTREUSE
    strncat(message, "-DPORTREUSE ", STR_SIZE);
#endif  /* PORTREUSE */
#ifdef  TCP
    strncat(message, "-DTCP ", STR_SIZE);
#endif  /* TCP */
#ifdef  SHELL
    strncat(message, "-DSHELL ", STR_SIZE);
#endif  /* SHELL */
    strncat(message, "\n", STR_SIZE);
    fprintf(stdout, message);
}

void parm_getopt(int argc, char *argv[]) {
    extern char *optarg;
    extern int optind;
    int option;

    while ((option = getopt(argc, argv, "n:g:c:m:i:a:p:l:svdh")) != EOF) {
#ifdef  DEBUG
        fprintf(debug_file, "configparm: Read option __%c__, value __%s__\n", option, optarg);
        fflush(debug_file);
#endif  /* DEBUG */
        switch (option) {
            case 'n':                /* nick */
                strncpy(config->nick, optarg, NICK_MAXLEN);
                break;
            case 'g':                /* gender */
                config->gender = atoi(optarg);
                break;
            case 'c':                /* channel */
                strncpy(config->channel, optarg, STR_SIZE);
                break;
            case 'm':                /* mode */
                config->mode = atoi(optarg);
                break;
            case 'i':                /* activity */
                config->active = atoi(optarg);
                break;
            case 'a':                /* address */
                strncpy(config->address, optarg, ADDR_SIZE);
                break;
            case 'p':                /* port */
                config->port = atoi(optarg);
                break;
            case 'l':                /* logfile */
                strncpy(config->log_main, optarg, PATH_SIZE);
                strncpy(config->log_priv, optarg, PATH_SIZE);
                strncpy(config->log_mesg, optarg, PATH_SIZE);
                break;
            case 's':                /* server */
                config->server = 1;
                break;
            case 'v':                /* verbose */
                config->verbose = 1;
                break;
            case 'd':                /* show defines */
                echat_defines();
                exit(EXIT_SUCCESS);
            case 'h':                /* help */
                parm_help();
                exit(EXIT_SUCCESS);
            case ':':
            case '?':
            default:
#ifdef  DEBUG
                fprintf(debug_file, "configparm: INVALID option __%c__, value __%s__\n", option, optarg);
                fflush(debug_file);
#endif  /* DEBUG */
                fprintf(stdout, "usage: %s [-ngcmiapl | -s | -v | -d | -h]\n", argv[0]);
                exit(EXIT_SUCCESS);
        }
    }
}
