#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/seq_file.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ALEX");
#define BUFSIZE PAGE_SIZE
// #define BUFSIZE 100
#define TMPBUF 100

static struct proc_dir_entry *proc_file;
static char *cookie_pot;
static char *read_char;
static char *write_char;

static void *my_start(struct seq_file *file, loff_t *pos)
{
    printk(KERN_INFO "++ my_start begins");
    printk(KERN_INFO "++ %lld, %p", *pos, (void *) cookie_pot);
    printk(KERN_INFO "++ my_start ends");
    if (*pos == 0)
        return cookie_pot;
    else
        return NULL;
}

static void *my_next(struct seq_file *file, void *v, loff_t *pos)
{
    printk(KERN_INFO "++ my_next begins");
    printk(KERN_INFO "++ v=%p, pos=%lld", v, *pos);
    printk(KERN_INFO "++ my_next ends");
    return NULL;
}

static void my_stop(struct seq_file *file, void *v)
{
    printk(KERN_INFO "++ my_stop begins");
    if (v)
        printk(KERN_INFO "++ v=%p", v);
    else
        printk(KERN_INFO "++ v=null");
    printk(KERN_INFO "++ my_stop ends");
}

static int my_show(struct seq_file *file, void *v)
{
    printk(KERN_INFO "++ my_show begins");
    printk(KERN_INFO "++ v=%p", v);
    if (read_char == write_char)
        read_char = cookie_pot;
    char tmpbuf[TMPBUF];
    memset(tmpbuf, 0, TMPBUF);
    int len = snprintf(tmpbuf, TMPBUF, "%s\n", read_char);
    read_char += len;
    seq_printf(file, "%s", tmpbuf);
    printk(KERN_INFO "++ my_show ends");
    return 0;
}

static struct seq_operations myseqops =
{
    .start = my_start,
    .next = my_next,
    .stop = my_stop,
    .show = my_show,
};

static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "++ file is opened");
    return seq_open(file, &myseqops);
}

static int my_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "++ file is released");
    return seq_release(inode, file);
}

static ssize_t my_write(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos) 
{
    if (cookie_pot + BUFSIZE < write_char + count)
    {
        printk( KERN_INFO "++ buffer is filled");
        return -ENOSPC;
    }
    if (copy_from_user(write_char, ubuf, count))
        return -EFAULT;
    write_char[count - 1] = 0;
	printk( KERN_INFO "++ copy data from user: %s", write_char);
    write_char += count;
	return count;
}

static ssize_t my_read(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) 
{
    printk(KERN_INFO "++ seq_read: begins");
	return seq_read(file, ubuf, count, ppos);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
#define HAVE_PROC_OPS
#endif

#ifdef HAVE_PROC_OPS
static struct proc_ops myops = 
{
    .proc_open = my_open,
    .proc_release = my_release,
	.proc_read = my_read,
	.proc_write = my_write,
    .proc_lseek = seq_lseek,
};
#else
static struct file_operations myops =
{
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write,
    .lseek = seq_lseek,
};
#endif


static int __init my_init(void)
{
    if (!(cookie_pot = vmalloc(BUFSIZE)))
        return -ENOMEM;
    if (!(proc_file = proc_create("sequence", 0626, NULL, &myops)))
    {
        vfree(cookie_pot);
        return -ENOMEM;
    }
    write_char = cookie_pot;
    read_char = cookie_pot;
    memset(cookie_pot, 0, BUFSIZE);
    return 0;
}

static void __exit my_exit(void)
{
    vfree(cookie_pot);
	remove_proc_entry("sequence", NULL);
}

module_init(my_init);
module_exit(my_exit);