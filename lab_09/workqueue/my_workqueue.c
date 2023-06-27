#include <asm/io.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/workqueue.h>

#include "ascii_names.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kalashkov Pavel");


struct my_work
{
    struct work_struct work;
    unsigned long data;
};

static struct workqueue_struct *workqueue;

static struct my_work *work1;
static struct work_struct *work2;

int irq_num = 1;

void fwork1(struct work_struct *work) 
{
    struct my_work *my_work = (struct my_work *)work;
    int data = my_work->data;

    printk(KERN_INFO "work1 handler\n");

    if (data < 84)
        printk(KERN_DEBUG "key=%s", ascii_names[data]);
}

void fwork2(struct work_struct *work) 
{
    printk(KERN_INFO "work2 handler\n");
    msleep(10);
    printk(KERN_INFO "work2 slept %lu jiffies\n", msecs_to_jiffies(10));
}

irqreturn_t irq_handler(int irq, void *dev) 
{

    if (irq == irq_num) 
    {
        int code = inb(0x60);
        work1->data = code;
        queue_work(workqueue, (struct work_struct *)work1);
        queue_work(workqueue, work2);
        return IRQ_HANDLED;
    }
    return IRQ_NONE;
}

static int __init my_init(void)
{
    printk(KERN_DEBUG "beginning of module\n");
    int res = 0;
    if ((res = request_irq(irq_num, &irq_handler, IRQF_SHARED, "my_irq_handler", &irq_handler)))
    {
        printk(KERN_ERR "request irq error");
        return res;
    }

    workqueue = alloc_workqueue("workqueue", __WQ_LEGACY | WQ_MEM_RECLAIM, 1);
    if (workqueue == NULL) 
    {
      printk(KERN_ERR "alloc_workqueue error\n");
      free_irq(irq_num, irq_handler);
      return -1;
    }

    work1 = kmalloc(sizeof(struct my_work), GFP_KERNEL);
    if (work1 == NULL) 
    {
      printk(KERN_ERR "alloc work1 error\n");
      destroy_workqueue(workqueue);
      free_irq(irq_num, irq_handler);
      return -1;
    }

    work2 = kmalloc(sizeof(struct work_struct), GFP_KERNEL);
    if (work2 == NULL) 
    {
      printk(KERN_ERR "alloc work2 error\n");
      destroy_workqueue(workqueue);
      kfree(work1);
      free_irq(irq_num, irq_handler);
      return -1;
    }

    INIT_WORK((struct work_struct *) work1, fwork1);
    INIT_WORK(work2, fwork2);

    return 0;
}

static void __exit my_exit(void)
{
  flush_workqueue(workqueue);
  destroy_workqueue(workqueue);
  synchronize_irq(irq_num);
  free_irq(irq_num, irq_handler);
  kfree(work1);
  kfree(work2);
  printk(KERN_DEBUG "end of module");
}

module_init(my_init);
module_exit(my_exit);
