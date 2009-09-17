# Makefile for PPPD_LDAP plugin
#

#DEBUG=y
TLS=y

PLUGIN=pppd_ldap_simple.so
TOOLS=ppp_list ppp_toggle
DESTINATION=/usr/lib/ppp/
CC=gcc
LD=ld
CFLAGS=-I../.. -I../../../include -O2 -I/usr/include/pppd -fPIC
LDFLAGS=-lldap -lc

CFLAGS += -DDEBUG=1

ifdef TLS
CFLAGS += -DOPT_WITH_TLS=1
endif

all : $(PLUGIN) $(TOOLS)

ppp_list: utmplib.o
	$(CC) $(CFLAGS) ppp_list.c -o ppp_list utmplib.o $(LDFLAGS)

ppp_toggle:	utmplib.o
	$(CC) $(CFLAGS) ppp_toggle.c -o ppp_toggle utmplib.o $(LDFLAGS)

pppd_ldap_simple.so: main.o utmplib.o
	$(LD) -shared -o pppd_ldap_simple.so utmplib.o main.o $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

utmplib.o: utmplib.c
	$(CC) $(CFLAGS) -c -o utmplib.o utmplib.c

clean :
	rm -rf $(PLUGIN) $(TOOLS) *.o *.so *~
