#include <linux/fs.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/seq_file.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <asm/io.h>

#include "ascii_names.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ALEX");

static int schedule_time;

static struct tasklet_struct *tasklet;
static char tasklet_data[]="tasklet_handler was called";


int irq_num = 1;

static void tasklet_handler(unsigned long data)
{
    printk(KERN_DEBUG "tasklet handler");
    printk(KERN_DEBUG "tasklet was scheduled in %lluusecs", schedule_time);
    if (data < 84)
        printk(KERN_DEBUG "key=%s", ascii_names[data]);
    printk(KERN_DEBUG "tasklet was handled in %lluusecs", ktime_get());
}



static irqreturn_t irq_handler(int num, void *pointer)
{
    if (num == irq_num)
    {
        schedule_time = ktime_get();
        int code = inb(0x60);
        tasklet->data = code;
        tasklet_schedule(tasklet);
        return IRQ_HANDLED;
    }
    return IRQ_NONE;

}


static int __init my_init(void)
{

    int res = 0;
    if ((res = request_irq(irq_num, irq_handler, IRQF_SHARED, "my_irq_handler", &irq_handler)))
    {
        printk(KERN_ERR "request irq error");
        return res;
    }

    tasklet = kmalloc(sizeof(struct tasklet_struct), GFP_KERNEL);
    if (!tasklet)
    {
        free_irq(irq_num, irq_handler);
        printk(KERN_ERR "kmalloc error");
        return -1;
    }

    tasklet_init(tasklet, tasklet_handler, (unsigned long) tasklet_data);
    printk(KERN_DEBUG "beginning of module");
    return res;
}

static void __exit my_exit(void)
{
    free_irq(irq_num, irq_handler);
    tasklet_kill(tasklet);
    printk(KERN_DEBUG "end of module");
}

module_init(my_init);
module_exit(my_exit);