DRIVERNAME := wch
all:	modules 

install: modules
	mkdir -p /lib/modules/$(shell uname -r)/kernel/drivers/char/
	cp -f ./$(DRIVERNAME).ko /lib/modules/$(shell uname -r)/kernel/drivers/char/
	mkdir -p /lib/modules/$(shell uname -r)/misc/
	cp -f ./$(DRIVERNAME).ko /lib/modules/$(shell uname -r)/misc/
	depmod -a
	
	
ifneq ($(KERNELRELEASE),)

obj-m += $(DRIVERNAME).o
$(DRIVERNAME)-y := wch_main.o wch_devtable.o wch_serial.o
else
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)

modules:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean

endif
