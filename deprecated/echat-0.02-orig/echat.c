/*  echat.c
 *
 *  copyright (c) 2000-2003 SeaD
 *  see GPL for copying info
 *
 */

#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "echat.h"

#ifdef  FREEBSD
#include <sys/stat.h>
#endif  /* FREEBSD */

#ifdef  DEBUG
# include <errno.h>
#endif  /* DEBUG */

char *buf = NULL;
char *packet = NULL;
char *message = NULL;
struct config_t *config = NULL;
struct status_t *status = NULL;

#ifdef  DEBUG
FILE *debug_file = NULL;
#endif  /* DEBUG */

int SIGNAL = 0;

void write_log(char *log_file) {
    FILE *fp = NULL;

    if ((fp = fopen(log_file, "a"))) {
        fprintf(fp, "%s", message);
        fclose(fp);
        return;
    } else {
#ifdef  DEBUG
        fprintf(debug_file, "logfile: %s\n", strerror(errno));
        fflush(debug_file);
#endif  /* DEBUG */
        return;
    }
}

void chat_exit(int reason) {
    if (reason) {
        snprintf(message, STR_SIZE, ECHAT_SIGNAL, time_get(), reason);
        write_log(config->log_main);
        write_str(status->room->name, COL_ERROR);
    } else {
        chnl_kill();
        snprintf(message, STR_SIZE, ECHAT_BYE, time_get(), config->nick);
        write_log(config->log_main);
        write_str(status->room->name, COL_SYSTEM);
    }
    term_kill();
    sock_kill();
    snprintf(message, STR_SIZE, "\n");
    write_log(config->log_main);

    free(status);
    free(config);
    free(message);
    free(packet);
    free(buf);

#ifdef  DEBUG
    fprintf(debug_file, "eChat: Exit on reason %d\n", reason);
    fflush(debug_file);
#endif  /* DEBUG */
#ifdef  DEBUG
    fprintf(stderr, "eChat: debugfile: Closing %s\n", DEBUG);
    if (fclose(debug_file))
        fprintf(stderr, "echat: debugfile: %s\n", strerror(errno));
#endif  /* DEBUG */
    exit(EXIT_SUCCESS);
}

void chat_signal(int signal) {
    SIGNAL = signal;
#ifdef  DEBUG
    fprintf(debug_file, "SIGNAL received %d\n", signal);
#endif  /* DEBUG */
}

void stat_init(void) {
    chnl_parse();
    status->room = &status->chnl[0];
    strncpy(status->chnl[0].name, status->channel, CHANNEL_MAXLEN);
    status->channels++;

    if (*config->log_main == '~') {
        snprintf(buf, PATH_SIZE, "%s/%s", getenv("HOME"), &config->log_main[1]);
        strncpy(config->log_main, buf, PATH_SIZE);
    }
    if (*config->log_mesg == '~') {
        snprintf(buf, PATH_SIZE, "%s/%s", getenv("HOME"), &config->log_mesg[1]);
        strncpy(config->log_mesg, buf, PATH_SIZE);
    }
    if (*config->log_priv == '~') {
        snprintf(buf, PATH_SIZE, "%s/%s", getenv("HOME"), &config->log_priv[1]);
        strncpy(config->log_priv, buf, PATH_SIZE);
    }
    *buf = '\0';
}

int main(int argc, char *argv[]) {

    fprintf(stdout, "%s v%s by %s <%s>\n\n", NAME, VERSION, AUTHOR, MAIL);

#ifdef  DEBUG
    fprintf(stderr, "eChat: debugfile: Opening %s\n", DEBUG);
    if (!(debug_file = fopen(DEBUG, "a")))
        fprintf(stderr, "echat: debugfile: %s\n", strerror(errno));
#endif  /* DEBUG */
#ifdef  DEBUG
    fprintf(debug_file, "eChat: Start %s v%s\n", NAME, VERSION);
    fflush(debug_file);
#endif  /* DEBUG */

    if ((buf = (char *) malloc(BUF_SIZE)) == NULL) {
#ifdef  DEBUG
        fprintf(debug_file, "malloc(): %s\n", strerror(errno));
#endif  /* DEBUG */
        exit(EXIT_FAILURE);
    }
    if ((packet = (char *) malloc(MSG_SIZE)) == NULL) {
#ifdef  DEBUG
        fprintf(debug_file, "malloc(): %s\n", strerror(errno));
#endif  /* DEBUG */
        exit(EXIT_FAILURE);
    }
    if ((message = (char *) malloc(MSG_SIZE)) == NULL) {
#ifdef  DEBUG
        fprintf(debug_file, "malloc(): %s\n", strerror(errno));
#endif  /* DEBUG */
        exit(EXIT_FAILURE);
    }
    if ((config = (struct config_t *) malloc(sizeof(struct config_t))) == NULL) {
#ifdef  DEBUG
        fprintf(debug_file, "malloc(): %s\n", strerror(errno));
#endif  /* DEBUG */
        exit(EXIT_FAILURE);
    }
    if ((status = (struct status_t *) malloc(sizeof(struct status_t))) == NULL) {
#ifdef  DEBUG
        fprintf(debug_file, "malloc(): %s\n", strerror(errno));
#endif  /* DEBUG */
        exit(EXIT_FAILURE);
    }
    memset(config, 0, sizeof(struct config_t));
    memset(status, 0, sizeof(struct status_t));
    srand((unsigned int) getpid());

    conf_default();
    snprintf(config->file, STR_SIZE, "%s/.%s", getenv("HOME"), CNF_ECHATRC);
    if (!conf_read()) {
#ifdef  DEBUG
        fprintf(debug_file, "configfile: Failed load %s\n", config->file);
        fflush(debug_file);
#endif  /* DEBUG */
    }
    snprintf(config->file, STR_SIZE, "./.%s", CNF_ECHATRC);
    if (!conf_read()) {
#ifdef  DEBUG
        fprintf(debug_file, "configfile: Failed load %s\n", config->file);
        fflush(debug_file);
#endif  /* DEBUG */
    }
    snprintf(config->file, STR_SIZE, "/etc/%s", CNF_ECHATRC);
    if (!conf_read()) {
#ifdef  DEBUG
        fprintf(debug_file, "configfile: Failed load %s\n", config->file);
        fflush(debug_file);
#endif  /* DEBUG */
    }
    parm_getopt(argc, argv);

    addr_parse();
    user_parsfavorite();
    user_parsignore();
    user_parsban();
    sock_init();
    addr_init();
    term_init();
    stat_init();
    window_init();
    window_refresh();

    if (signal(SIGINT, chat_exit) == SIG_ERR) {
#ifdef  DEBUG
        fprintf(debug_file, "signal(SIGINT): %s\n", strerror(errno));
#endif  /* DEBUG */
        chat_exit(-1);
    }
    if (signal(SIGTERM, chat_exit) == SIG_ERR) {
#ifdef  DEBUG
        fprintf(debug_file, "signal(SIGTERM): %s\n", strerror(errno));
#endif  /* DEBUG */
        chat_exit(-1);
    }
    if (signal(SIGQUIT, SIG_IGN) == SIG_ERR) {
#ifdef  DEBUG
        fprintf(debug_file, "signal(SIGQUIT): %s\n", strerror(errno));
#endif  /* DEBUG */
        chat_exit(-1);
    }
    if (signal(SIGHUP, chat_signal) == SIG_ERR) {
#ifdef  DEBUG
        fprintf(debug_file, "signal(SIGHUP): %s\n", strerror(errno));
#endif  /* DEBUG */
        chat_exit(-1);
    }
    if (signal(SIGWINCH, chat_signal) == SIG_ERR) {
#ifdef  DEBUG
        fprintf(debug_file, "signal(SIGWINCH): %s\n", strerror(errno));
#endif  /* DEBUG */
        chat_exit(-1);
    }

    snprintf(message, STR_SIZE, ECHAT_WELCOME, time_get(), NAME, VERSION, config->nick);
    write_log(config->log_main);
    write_str(status->room->name, COL_SYSTEM);

    chnl_init();

    while (1) {
        if (SIGNAL == SIGHUP) {
            *buf = 0; cmnd_load(); SIGNAL = 0;
        }
        if (SIGNAL == SIGWINCH) {
            term_kill(); term_init(); window_init(); window_refresh(); SIGNAL = 0;
        }
        sock_recv();
#ifdef  TCP
        sock_tcprecv();
#endif  /* TCP */
        read_str();
        time_users();
    }
}
