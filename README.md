# gpio-netlink
Kernel module to notify gpio interrupts with netlink

## Compilation
```
CCPREFIX=... KERNEL_SRC=... make
```

## Usage
```
$ modinfo gpio-netlink.kofilename:       gpio-netlink.ko
description:    sends gpio interrupts per netlink
author:         Andreas Stefl
license:        GPL
srcversion:     2A165851184B232A0CAEDBD
depends:        
vermagic:       4.6.7-rt11 preempt mod_unload modversions ARMv6 
parm:           debug:debug flag (default=0; 1 enables debugging) (int)
parm:           pin:gpio pin number (default=2) (int)
parm:           pid:process id for netlink unicast (default=-1; -1 enables multicast) (int)
parm:           nlgroup:netlink group (default=31) (int)
parm:           irqf:interrupt flags (default=0x00000001; see linux/interrupt.h IRQF_TRIGGER_*) (int)
```
