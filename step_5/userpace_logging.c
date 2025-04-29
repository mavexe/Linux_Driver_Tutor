#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched.h>
#include <linux/cred.h>

#define PROC_NAME "app_monitor"

static struct proc_dir_entry *proc_entry; // ptr на PROC_NAME файл  https://elixir.bootlin.com/linux/v4.2/source/fs/proc/internal.h#L31

static int proc_show(struct seq_file *m, void *v) 
{
  struct task_struct *task; // структура процесса ядра 
  seq_printf(m, "Userspace apps\n");
  seq_printf(m, "PID\t Name\t User\n");

  for_each_process(task) { // макрос ядра https://elixir.bootlin.com/linux/v4.4.139/source/include/linux/sched.h#L2682
  if (task->mm) { // если текущий task использует память->не поток (maybe task->flag!=PF_KTHREAD)
      seq_printf(m, "%d\t%s\t%d\n", 
                 task->pid, //идентификатор 
                 task->comm, //
		 __kuid_val(task_uid(task)));
        }
    }
  return 0;
}

// Открытие proc-файла
static int proc_open(struct inode *inode, struct file *file)
{
return single_open(file, proc_show, NULL); //функция ядра https://elixir.bootlin.com/linux/v4.2/source/include/linux/seq_file.h#L131
}

static const struct proc_ops fops = {  //https://elixir.bootlin.com/linux/v6.13.6/source/include/linux/proc_fs.h#L29
    .proc_open = proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init monitor_init(void)
{
    proc_entry = proc_create(PROC_NAME, 0, NULL, &fops);
    if (!proc_entry) {
      return -ENOMEM;
    }
    printk(KERN_INFO "модуль загружен\n");
    return 0;
}

static void __exit monitor_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "модуль выгружен\n");
}

module_init(monitor_init);
module_exit(monitor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Damir");

