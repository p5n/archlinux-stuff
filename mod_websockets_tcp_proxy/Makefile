all: modules

modules: .libs/mod_websocket.so .libs/mod_websocket_tcp_proxy.so

.libs/mod_websocket.so: mod_websocket.c validate_utf8.h websocket_plugin.h
	apxs -c mod_websocket.c

.libs/mod_websocket_tcp_proxy.so: mod_websocket_tcp_proxy.c websocket_plugin.h
	apxs -c mod_websocket_tcp_proxy.c

clean:
	rm -rf .libs *.o *.slo *.la *.lo

install: modules
	install -Dm0644 .libs/mod_websocket.so $(DESTDIR)/usr/lib/httpd/modules/mod_websocket.so
	install -Dm0644 .libs/mod_websocket_tcp_proxy.so $(DESTDIR)/usr/lib/httpd/modules/mod_websocket_tcp_proxy.so
