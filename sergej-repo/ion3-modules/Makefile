TOPDIR=..
include $(TOPDIR)/build/system-inc.mk

######################################

INCLUDES += $(X11_INCLUDES) $(LIBTU_INCLUDES) $(LIBEXTL_INCLUDES) -I..
CFLAGS += $(XOPEN_SOURCE) $(C99_SOURCE)

SOURCES=mod_xkb.c

MAKE_EXPORTS=mod_xkb

MODULE=mod_xkb

######################################

include $(TOPDIR)/build/rules.mk

######################################

_install: module_install

######################################

.PHONY: tags
tags:
	exuberant-ctags -R . $(TOPDIR)
