all: ulogd_SQ.so

ulogd_SQ.so: ulogd_SQ.c
	gcc -fPIC -c -o ulogd_SQ.o ulogd_SQ.c -I.
	ld -fPIC -o ulogd_SQ.so ulogd_SQ.o -lc -shared

clean:
	rm -f ulogd_SQ.so *.o

install: all
	install -D -m 755 ulogd_SQ.so $(DESTDIR)/usr/lib/ulogd/ulogd_SQ.so
