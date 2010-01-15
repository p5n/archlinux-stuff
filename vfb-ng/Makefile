ifneq ($(KERNELRELEASE),)
obj-m := vfb-ng.o
vfb-ng-objs := vfb.o
else
KDIR        := /lib/modules/$(shell uname -r)/build
PWD         := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

install:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules_install
endif

clean:
	rm -rf *.o *.ko .*.cmd *.mod.c *~ Module.symvers modules.order .tmp_versions

