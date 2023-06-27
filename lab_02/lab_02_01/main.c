#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/path.h>
#include <linux/fs_struct.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ALEX");

static int __init module_my(void)
{
    struct task_struct *task = &init_task;
    do 
    {
        printk(KERN_INFO "+ name %s, pid-%d, parent: %s, ppid-%d, state- %d, flags- %d, prio- %d, policy- %s, root- %s, pwd- %s\n", 
        task->comm, task->pid, task->parent->comm, task->parent->pid,
        task->__state, task->flags, task->prio, task->policy,
        task->fs->root.dentry->d_name.name, task->fs->pwd.dentry->d_name.name);

    } while ((task = next_task(task)) != &init_task);
    printk(KERN_INFO "+ name %s, pid-%d, parent: %s, ppid-%d, state- %d, flags- %d, prio- %d, policy- %s, root- %s, pwd- %s\n", 
        current->comm, current->pid, current->parent->comm, current->parent->pid,
        current->__state, current->flags, current->prio, current->policy,
        current->fs->root.dentry->d_name.name, current->fs->pwd.dentry->d_name.name);
    return 0;
}

static void __exit module_myexit(void)
{
}

module_init(module_my);
module_exit(module_myexit);
