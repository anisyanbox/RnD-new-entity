MODULE_NALE=solution

obj-m += $(MODULE_NALE).o

all:
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	sudo make -C /lib/modules/$(shell uname -r)/build  M=$(shell pwd) clean

load:
	sudo dmesg -c >> /dev/null
	sudo insmod $(MODULE_NALE).ko
	dmesg

unload:
	sudo dmesg -c >> /dev/null
	sudo rmmod $(MODULE_NALE).ko
	dmesg