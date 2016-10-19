MODULE=gpio-netlink

obj-m += ${MODULE}.o

all: compile

compile: gpio-netlink.c
	make ARCH=arm CROSS_COMPILE=${CCPREFIX} -C ${KERNEL_SRC} M=$(PWD) modules

clean:
	make -C ${KERNEL_SRC} M=$(PWD) clean
