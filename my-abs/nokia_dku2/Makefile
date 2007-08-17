##########################################################################
#                                                                        #
#  This program is free software; you can redistribute it and/or modify  #
#  it under the terms of the GNU General Public License as published by  #
#  the Free Software Foundation; either version 2 of the License, or     #
#  (at your option) any later version.                                   #
#                                                                        #
#  Changelog                                                             #
#    2005-02-01  <tengel@sonic.net>  Initial version                     #
#                                                                        #
##########################################################################

kernel_ver=$(shell uname -r)

MODULE_NAME   := nokia_dku2
MODULE_ROOT   := /lib/modules/$(kernel_ver)/kernel/drivers/usb/serial

KERN_SOURCE   := /lib/modules/$(kernel_ver)/source
PWD           := $(shell pwd)
EXTRA_CFLAGS  := -I/lib/modules/$(kernel_ver)/source/drivers/usb/serial

obj-m         := $(MODULE_NAME).o
MODULE_OBJECT := $(MODULE_NAME).ko

all: module

module:
	@$(MAKE) -C $(KERN_SOURCE) SUBDIRS=$(PWD) modules

clean:
	@rm -f .*.{cmd,flags}
	@rm -f $(MODULE_NAME).{o,ko,mod.{o,c}} built-in.o $(VERSION_HEADER)
	@rm -fr .tmp_versions

install:
	install -D -m 644 $(MODULE_OBJECT) $(DESTDIR)/$(MODULE_ROOT)/$(MODULE_OBJECT)
#	/sbin/depmod -a
