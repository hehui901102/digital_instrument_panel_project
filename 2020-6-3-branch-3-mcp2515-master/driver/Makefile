ifeq ($(KERNELRELEASE),)

# 按需修改内核代码位置
KERNELDIR ?= /home/hehui/work/linux-3.4.2-JZ2440V3-master/
PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules* Module*

.PHONY: modules modules_install clean

else
    obj-m := mcp2515.o
endif

