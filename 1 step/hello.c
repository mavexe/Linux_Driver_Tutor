#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void)
{
    printk(KERN_INFO "hello - hello, damir\n");
    return 0;
}

static void __exit my_exit(void)
{
    printk(KERN_INFO "hello - goodbye damir\n");
}

module_init(my_init);
module_exit(my_exit);  // ← Исправлено!

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Damir");
MODULE_DESCRIPTION("Simple hello world driver");
