#include <linux/module.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/errno.h> 
#include <linux/device.h> 
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/init.h>

static ssize_t my_sys_show(struct kobject *kobj, struct kobj_attribute *attr,
			   char *buf)
{
	static int read_op_cnt = 0;

	++read_op_cnt;

	return sprintf(buf, "%d\n", read_op_cnt);
}

static ssize_t my_sys_store(struct kobject *kobj, struct kobj_attribute *attr,
			    const char *buf, size_t count)
{
	return count;
}

static struct kobj_attribute my_sys_attribute = 
	__ATTR(my_sys, 0664, my_sys_show, my_sys_store);

/*
 * Create a group of attributes so that we can create and destroy them all
 * at once.
 */
static struct attribute *attrs[] = {
	&my_sys_attribute.attr,
	NULL, /* need to NULL terminate the list of attributes */
};

/*
 * All this attributes will be expotred in your kobj dir.
 *
 * kobj will be created after kobject_create_and_add() call
 * in sysfs
 */
static const struct attribute_group my_kobject_groups = {
	.attrs = attrs,
};

static struct kobject *my_kobject;

static int __init solution_init(void)
{
	int retval;

	/*
	 * This function creates a kobject structure dynamically and registers it
	 * with sysfs with 'name'. When you are finished with this structure, call
	 * kobject_put() and the structure will be dynamically freed when
	 * it is no longer being used.
	 *
	 * For this kobj kernel uses default ktype with release function:
	 * https://elixir.bootlin.com/linux/latest/source/lib/kobject.c#L750
	 */
	my_kobject = kobject_create_and_add("my_kobject", kernel_kobj);
	if (!my_kobject)
		return -ENOMEM;

	retval = sysfs_create_group(my_kobject, &my_kobject_groups);
	if (retval)
		kobject_put(my_kobject);

	return retval;
}

static void __exit solution_exit(void)
{
	kobject_put(my_kobject);	
}

module_init(solution_init);
module_exit(solution_exit);

MODULE_DESCRIPTION("Learn Linux Kernel with stepik.org");
MODULE_AUTHOR("Aleksandr Anisimov <anisimov.alexander.s@gmail.com>");
MODULE_LICENSE("GPL v2");