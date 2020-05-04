obj-m += uid_changer.o

MODULE_NAME=uid_changer

ARCH=$(shell uname -m)
BUILD_DIR=/lib/modules/$(shell uname -r)/build

all:
	sudo make -C $(BUILD_DIR) M=$(shell pwd) \
		ARCH=$(ARCH)  modules
clean:
	sudo make -C $(BUILD_DIR) M=$(shell pwd)  \
			ARCH=$(ARCH) clean

load:
	sudo dmesg -c > /dev/null
	sudo insmod uid_changer.ko
	dmesg

unload:
	sudo dmesg -c > /dev/null
	sudo rmmod uid_changer.ko
	dmesg

