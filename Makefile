MODULE=gpio-netlink

#CCPREFIX=/home/andreas/Downloads/rpi-build-tools/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-
#KERNEL_SRC=/home/andreas/Downloads/rpi1-kernel-4.6.7-rt-src-built

obj-m += ${MODULE}.o

all: clean compile

compile:
	make ARCH=arm CROSS_COMPILE=${CCPREFIX} -C ${KERNEL_SRC} M=$(PWD) modules

clean:
	make -C ${KERNEL_SRC} M=$(PWD) clean
