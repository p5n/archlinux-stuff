#!/usr/bin/make
#
# Copyright (c) 2003-2004 by Salvador E. Tropea.
# Covered by the GPL license.
#
ifeq ($(prefix),)
  prefix=g:/dj
endif
ifeq ($(INSTALL),)
  INSTALL=install
endif
EXE_EXT=.exe
libdir=$(prefix)/lib
OS=DOS
MAINTAINER_MODE=1

export

.PHONY:  maintainance static-lib rhtv-config$(EXE_EXT) internac install-static install-internac  install-headers install-config clean \
	intl-dummy examples

all:  maintainance static-lib rhtv-config$(EXE_EXT) internac intl-dummy

maintainance:
	$(MAKE) -C makes -f maintain.mak

static-lib:
	$(MAKE) -C makes -f librhtv.mkf

internac:
	$(MAKE) -C intl


intl-dummy:
	$(MAKE) -C intl/dummy
	cp intl/dummy/libtvfintl.a makes


examples:
	$(MAKE) -C examples

rhtv-config$(EXE_EXT): rhtv-config.c include/tv/configtv.h
	gcc -o rhtv-config$(EXE_EXT) -Iinclude rhtv-config.c

install-headers:
	$(INSTALL) -d -m 0755 $(prefix)/include/rhtvision
	rm -f $(prefix)/include/rhtvision/*.h
	$(INSTALL) -m 0644 include/*.h $(prefix)/include/rhtvision
	$(INSTALL) -d -m 0755 $(prefix)/include/rhtvision/tv
	$(INSTALL) -m 0644 include/tv/*.h $(prefix)/include/rhtvision/tv
	$(INSTALL) -d -m 0755 $(prefix)/include/rhtvision/tv/dos
	$(INSTALL) -m 0644 include/tv/dos/*.h $(prefix)/include/rhtvision/tv/dos
	$(INSTALL) -d -m 0755 $(prefix)/include/rhtvision/cl
	$(INSTALL) -m 0644 include/cl/*.h $(prefix)/include/rhtvision/cl


install-static: static-lib
	$(INSTALL) -d -m 0755 $(libdir)
	$(INSTALL) -m 0644 makes/librhtv.a $(libdir)

install-internac:
	$(MAKE) -C intl install


install-intl-dummy: intl-dummy
	$(INSTALL) -d -m 0755 $(libdir)
	$(INSTALL) -m 0644 intl/dummy/libtvfintl.a $(libdir)


install-config:
	$(INSTALL) -d -m 0755 $(prefix)/bin
	$(INSTALL) -m 0755 rhtv-config$(EXE_EXT) $(prefix)/bin


install: install-static install-internac  install-headers install-intl-dummy install-config

clean:
	rm -f makes/librhtv.so*
	rm -f makes/obj/*.o
	rm -f makes/obj/*.lo
	rm -f makes/librhtv.a
	rm -f compat/obj/*.o
	rm -f compat/obj/*.lo
	rm -f intl/dummy/*.o
	rm -f intl/dummy/*.lo
	rm -f intl/dummy/*.a
	-$(MAKE) -C examples clean
	rm -f configure.cache
	rm -f rhtv-config$(EXE_EXT)


#
# For compatibility with automake:
# needed to 'make dist' of tiger
#
distdir: clean
	@cp -pR * $(distdir)

