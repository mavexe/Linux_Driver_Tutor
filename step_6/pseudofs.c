#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/module.h>

#include <linux/string.h>

static struct kobject *my_kobj;
static int sysfs_val = 0;

// Функция чтения значения
static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", sysfs_val);
}

// Функция записи значения
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr,
                          const char *buf, size_t count)
{
    int ret = kstrtoint(buf, 10, &sysfs_val);
    if (ret < 0)
        return ret;
    return count;
}

// Определение атрибута (используем __ATTR вместо __ATTRIBUTE)
static struct kobj_attribute value_attr = __ATTR(value, 0664, value_show, value_store);

// Инициализация модуля
static int __init module_fs_init(void)
{
    int ret;
    
    // Создаем kobject в /sys/kernel/
    my_kobj = kobject_create_and_add("module", kernel_kobj);
    if (!my_kobj) {
        pr_err("Failed to create kobject\n");
        return -ENOMEM;
    }
    
    // Создаем файл value
    ret = sysfs_create_file(my_kobj, &value_attr.attr);
    if (ret) {
        pr_err("Failed to create sysfs file\n");
        kobject_put(my_kobj);
        return ret;
    }
    
    pr_info("Module loaded\n");
    return 0;
}

// Выгрузка модуля
static void __exit module_fs_exit(void)
{
    kobject_put(my_kobj);
    pr_info("Module unloaded\n");
}

module_init(module_fs_init);
module_exit(module_fs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Damir");
MODULE_DESCRIPTION("Simple sysfs example module");
