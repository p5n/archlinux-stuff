##  Makefile
##
##  copyright (c) 2000-2003 SeaD
##  see GPL for copying info
##

NAME=echat-noi
VERSION=0.02
PREFIX=/usr
LIBS=-lncurses
STATIC=
CC=gcc
CFLAGS=-Wall -g -O2
OBJS=chat_chnl.o chat_cmnd.o chat_conf.o chat_cset.o chat_parm.o chat_time.o chat_user.o curs_strn.o curs_term.o echat.o pckt_addr.o pckt_make.o pckt_pars.o sock_tcp.o sock_udp.o

DEFINES=-DFREEBSD -DEN -DCHARSET

.c.o:
	@echo Compiling $*.c
	@$(CC) -c $(CFLAGS) $(DEFINES) $<

all:	start $(NAME)

start:
	@echo "Making $(NAME) $(VERSION)"
	@echo "LIBS = $(LIBS)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "DEFINES = $(DEFINES)"

$(NAME): $(OBJS)
	@echo Linking $@
	@$(CC) -o $@ $(OBJS) $(LIBS) $(STATIC)

clean:
	@echo Cleaning OBJS
	@rm -f $(OBJS)

cleanall: clean
	@echo Removing $(NAME)
	@rm -f $(NAME)

install: all
	mkdir -p -m 755 $(PREFIX)/bin
	install -s -m 7755 $(NAME) $(PREFIX)/bin/$(NAME)

chat_chnl.o: chat_chnl.c echat.h
chat_cmnd.o: chat_cmnd.c chat_cmnd.h echat.h
chat_conf.o: chat_conf.c chat_conf.h echat.h
chat_cset.o: chat_cset.c echat.h
chat_parm.o: chat_parm.c echat.h
chat_time.o: chat_time.c echat.h
chat_user.o: chat_user.c echat.h
curs_strn.o: curs_strn.c curs_term.h echat.h
curs_term.o: curs_term.c curs_term.h echat.h
echat.o: echat.c echat.h
pckt_addr.o: pckt_addr.c echat.h
pckt_make.o: pckt_make.c pckt_type.h echat.h
pckt_pars.o: pckt_pars.c pckt_type.h echat.h
sock_tcp.o: sock_tcp.c echat.h
sock_udp.o: sock_udp.c echat.h
