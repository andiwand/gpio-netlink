// TODO: receive commands per netlink to register irq
// TODO: parameter to add multiple pins

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/time.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>

#include <linux/netlink.h>
#include <net/netlink.h>
#include <net/net_namespace.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define irq_to_gpio(x)              ((x) - gpio_to_irq(0))

#define GPIO_NAME                   NULL
#define GPIO_ID                     NULL
#define NETLINK_PROTO               NETLINK_USERSOCK
#define DEFAULT_DEBUG               0
#define DEFAULT_PIN                 2
#define DEFAULT_PID                 -1
#define DEFAULT_NLGROUP             31
#define DEFAULT_IRQF                IRQF_TRIGGER_RISING

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andreas Stefl");
MODULE_DESCRIPTION("sends gpio interrupts per netlink");

struct message {
    struct timespec ts;
    int pin;
    int value;
};

static int debug = DEFAULT_DEBUG;
static int pin = DEFAULT_PIN;
static int pid = DEFAULT_PID;
static int nlgroup = DEFAULT_NLGROUP;
static int irqf = DEFAULT_IRQF;
static short int irq = 0;
static struct sock* nlsock = NULL;

module_param(debug, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
module_param(pin, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
module_param(pid, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
module_param(nlgroup, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
module_param(irqf, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(debug, "debug flag (default=" STR(DEFAULT_DEBUG) "; 1 enables debugging)");
MODULE_PARM_DESC(pin, "gpio pin number (default=" STR(DEFAULT_PIN) ")");
MODULE_PARM_DESC(pid, "process id for netlink unicast (default=" STR(DEFAULT_PID) "; -1 enables multicast)");
MODULE_PARM_DESC(nlgroup, "netlink group (default=" STR(DEFAULT_NLGROUP) ")");
MODULE_PARM_DESC(irqf, "interrupt flags (default=" STR(DEFAULT_IRQF) "; see linux/interrupt.h IRQF_TRIGGER_*)");

static void nlsock_send(void* msg, unsigned int size);

static irqreturn_t irq_handler(int irq, void *dev_id, struct pt_regs *regs) {
    struct message msg;
    
    getnstimeofday(&msg.ts);
    msg.pin = irq_to_gpio(irq);
    msg.value = gpio_get_value(msg.pin);
    
    nlsock_send(&msg, sizeof(msg));
    if (debug) {
        printk(KERN_NOTICE "interrupt pin=%d value=%d time=%ld.%ld\n", msg.pin, msg.value, msg.ts.tv_sec, msg.ts.tv_nsec);
    }
    
    return IRQ_HANDLED;
}

static void int_config(void) {
    if (gpio_request(pin, GPIO_NAME)) {
        printk("gpio request faied\n");
        return;
    }
    
    irq = gpio_to_irq(pin);
    if (irq < 0) {
        printk("gpio to irq mapping failed\n");
        return;
    }
    
    if (debug) {
        printk(KERN_NOTICE "mapped irq: %d\n", irq);
    }
    
    if (request_irq(irq, (irq_handler_t) irq_handler, irqf, GPIO_NAME, GPIO_ID)) {
        printk("irq request failed\n");
        return;
    }
}

static void int_release(void) {
    free_irq(irq, GPIO_ID);
    gpio_free(pin);
}

static void nlsock_send(void* msg, unsigned int size) {
    struct sk_buff* skb_out;
    struct nlmsghdr* nlh;
    int res;
    
    skb_out = nlmsg_new(size, GFP_KERNEL);
    if (skb_out == NULL) {
        printk(KERN_ERR "failed to allocate new skb\n");
        return;
    }
    
    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, size, 0);
    strncpy(nlmsg_data(nlh), msg, size);
    
    if (pid == -1) {
        res = nlmsg_multicast(nlsock, skb_out, 0, nlgroup, GFP_KERNEL);
    } else {
        res = nlmsg_unicast(nlsock, skb_out, pid);
    }
    if (res < 0) {
        if (debug) {
            printk(KERN_INFO "error while sending to user (maybe nobody is listening on the netlink?)\n");
        }
        return;
    }
}

static void nlsock_config(void) {
    nlsock = netlink_kernel_create(&init_net, NETLINK_PROTO, NULL);
    if (nlsock == NULL) {
        pr_err("failed to create netlink\n");
        return;
    }
}

static void nlsock_release(void) {
    netlink_kernel_release(nlsock);
}

static int init(void) {
    if (debug) {
        printk(KERN_NOTICE "started\n");
    }
    
    nlsock_config();
    int_config();
    
    return 0;
}

static void cleanup(void) {
    int_release();
    nlsock_release();
    
    if (debug) {
        printk(KERN_NOTICE "ended\n");
    }
}


module_init(init);
module_exit(cleanup);

