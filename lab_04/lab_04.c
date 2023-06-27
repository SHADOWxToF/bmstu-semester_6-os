// #include <linux/module.h>
// #include <linux/kernel.h>
// #include <linux/proc_fs.h>
// #include <linux/init.h>
// #include <linux/path.h>
// #include <linux/fs_struct.h>
// #include <asm/uaccess.h>
// #include <linux/moduleparam.h>
// #include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("ALEX");
#define BUFSIZE PAGE_SIZE
// #define BUFSIZE 100
#define HELPBUF 100

static struct proc_dir_entry *proc_file;
static char *cookie_pot;
static char *read_char;
static char *write_char;
static struct proc_dir_entry *my_dir;
static struct proc_dir_entry *my_symlink;


static ssize_t mywrite(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos) 
{
    if (cookie_pot + BUFSIZE < write_char + count)
    {
        printk( KERN_INFO "buffer is filled");
        return -ENOSPC;
    }
    if (copy_from_user(write_char, ubuf, count))
        return -EFAULT;
    write_char[count - 1] = 0;
	printk( KERN_INFO "copy data from user: %s", write_char);
    write_char += count;
	return count;
}

static ssize_t myread(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) 
{
    if (*ppos > 0)
        return 0;
    if (write_char == cookie_pot)
        return 0;
    if (read_char == write_char)
        read_char = cookie_pot;
    char help_buffer[HELPBUF];
    memset(help_buffer, 0, HELPBUF);
    int len = snprintf(help_buffer, HELPBUF, "%s\n", read_char);
    if (copy_to_user(ubuf, help_buffer, len))
        return -EFAULT;
    *ppos += len;
	printk( KERN_INFO "copy data to user: %s", help_buffer);
    read_char += len;
	return len;
}

static int myopen(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "file is opened");
    return 0;
}

static int myrelease(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "file is released");
    return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
#define HAVE_PROC_OPS
#endif

#ifdef HAVE_PROC_OPS
static struct proc_ops myops = 
{
	.proc_read = myread,
	.proc_write = mywrite,
    .proc_open = myopen,
    .proc_release = myrelease,
};
#else
static struct file_operations myops =
{
    .read = myread,
    .write = mywrite,
    .open = myopen,
    .release = myrelease,
};
#endif

static int __init myinit(void)
{
    if (!(cookie_pot = vmalloc(BUFSIZE)))
        return -ENOMEM;
    if (!(proc_file = proc_create("fortune", 0666, NULL, &myops)))
    {
        vfree(cookie_pot);
        return -ENOMEM;
    }
    if (!(my_dir = proc_mkdir("myprocdir", NULL)))
    {
        vfree(cookie_pot);
        remove_proc_entry("fortune", NULL);
        return -ENOMEM;
    }
    if (!(my_symlink = proc_symlink("myprocsymlink", my_dir, "/proc/myprocdir")))
    {
        vfree(cookie_pot);
        remove_proc_entry("fortune", NULL);
        remove_proc_entry("myprocdir", NULL);
        return -ENOMEM;
    }
    write_char = cookie_pot;
    read_char = cookie_pot;
    memset(cookie_pot, 0, BUFSIZE);
    return 0;
}

static void __exit myexit(void)
{
    vfree(cookie_pot);
    remove_proc_entry("myprocsymlink", NULL);
    remove_proc_entry("myprocdir", NULL);
	remove_proc_entry("fortune", NULL);
}

module_init(myinit);
module_exit(myexit);