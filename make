#!/bin/bash

CCPREFIX="/home/andreas/Downloads/rpi-build-tools/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-"

KERNEL_SRC="/home/andreas/Downloads/rpi1-kernel-4.6.7-src-built"
make CCPREFIX=$CCPREFIX KERNEL_SRC=$KERNEL_SRC
mv gpio-netlink.ko ../gpio-netlink_rpi1-4.6.7.ko
make CCPREFIX=$CCPREFIX KERNEL_SRC=$KERNEL_SRC clean

KERNEL_SRC="/home/andreas/Downloads/rpi1-kernel-4.6.7-rt-src-built"
make CCPREFIX=$CCPREFIX KERNEL_SRC=$KERNEL_SRC
mv gpio-netlink.ko ../gpio-netlink_rpi1-4.6.7-rt.ko
make CCPREFIX=$CCPREFIX KERNEL_SRC=$KERNEL_SRC clean

KERNEL_SRC="/home/andreas/Downloads/rpi2-kernel-4.6.7-src-built"
make CCPREFIX=$CCPREFIX KERNEL_SRC=$KERNEL_SRC
mv gpio-netlink.ko ../gpio-netlink_rpi2-4.6.7.ko
make CCPREFIX=$CCPREFIX KERNEL_SRC=$KERNEL_SRC clean

KERNEL_SRC="/home/andreas/Downloads/rpi2-kernel-4.6.7-rt-src-built"
make CCPREFIX=$CCPREFIX KERNEL_SRC=$KERNEL_SRC
mv gpio-netlink.ko ../gpio-netlink_rpi2-4.6.7-rt.ko
make CCPREFIX=$CCPREFIX KERNEL_SRC=$KERNEL_SRC clean
