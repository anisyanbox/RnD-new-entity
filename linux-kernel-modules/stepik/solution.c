#include <linux/module.h>
#include <linux/moduleparam.h>
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
#include <linux/list.h>

static struct list_head *get_module_list_head(void)
{
	return THIS_MODULE->list.prev;
}

static int get_module_cnt(void)
{
	int m_cnt = 0;
	struct list_head *iterator;

	list_for_each(iterator, get_module_list_head()) {
		++m_cnt;
	}

	return m_cnt;
}

static int fill_arr_with_str(char **arr, int arr_size)
{
	int i = 0;
	struct list_head *iterator;

	list_for_each(iterator, get_module_list_head()) {
		arr[i] = (list_entry(iterator, struct module, list))->name;
		++i;

		if (i == arr_size)
			break;
	}

	return i;
}

/* change pointers */
static void my_swap(char **a, char **b)
{
	char *t = *a;

	*a = *b;
	*b = t;
}

static void sort_string_array(char **arr, int arr_size)
{
	int i, j;
	char *s1;
	char *s2;

	for (i = 0; i < arr_size; ++i) {
		for (j = i + 1; j < arr_size; ++j) {
			s1 = arr[i];
			s2 = arr[j];
			if (strcmp(s1, s2) > 0)
				my_swap(&arr[i], &arr[j]);
		}
	}
}

static ssize_t my_sys_show(struct kobject *kobj, struct kobj_attribute *attr,
			   char *buf)
{
	int i;
	int len = 0;
	int m_cnt;
	char **str_table;

	m_cnt = get_module_cnt();

	str_table = kmalloc(sizeof(char *) * m_cnt, GFP_KERNEL);
	if (!str_table)
		return -ENOMEM;

	m_cnt = fill_arr_with_str(str_table, m_cnt);

	sort_string_array(str_table, m_cnt);

	for (i = 0; i < m_cnt; ++i) {
		len += sprintf(buf + len, "%s\n", str_table[i]);

		if (len + MODULE_NAME_LEN > PAGE_SIZE)
			break;
	}

	kfree(str_table);

	return (ssize_t)len;
}

static struct kobj_attribute my_sys_attribute = 
	__ATTR(my_sys, 0444, my_sys_show, NULL);

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