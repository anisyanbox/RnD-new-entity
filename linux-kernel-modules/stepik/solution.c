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
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define MODULE_DEFNAME "solution_node1"
#define MODULE_NAME_MAX_LEN 64

static char in_node_name[MODULE_NAME_MAX_LEN];
module_param_string(node_name, in_node_name, MODULE_NAME_MAX_LEN, 0664);

static dev_t devno;
static int my_major = 0;
static int my_minor = 0;
static struct cdev *devnode_cdev;
static struct class *devnode_class;
static struct device *devnode_device;

static ssize_t solution_read(struct file *file, char __user *buf, size_t lbuf,
			     loff_t *ppos)
{
	static char show[20];
	int showlen;

	showlen = sprintf(show, "%d\n", my_major);

	return simple_read_from_buffer(buf, lbuf, ppos, show, showlen);
}

static ssize_t solution_write(struct file *file, const char __user *buf,
			      size_t lbuf, loff_t *ppos)
{
	return lbuf;
}

static int solution_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int solution_release(struct inode *inode, struct file *file)
{
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = solution_open,
	.release = solution_release,
	.read = solution_read,
	.write = solution_write,
};

static int __init solution_init(void)
{
	int retval = 0;
	char *node_name;

	if (!in_node_name[0])
		node_name = MODULE_DEFNAME;
	else
		node_name = in_node_name;

	retval = alloc_chrdev_region(&devno, my_minor, 1, node_name);
	if (retval < 0)
		return retval;

	my_major = MAJOR(devno);
	printk(KERN_INFO "dynamic major is %d\n", my_major);

	/* allocate memory */
	devnode_cdev = cdev_alloc();
	if (!devnode_cdev)
		return -ENOMEM;

	/* assign fops to cdev */
	cdev_init(devnode_cdev, &fops);

	/* assign kernel object to common device table */
	retval = cdev_add(devnode_cdev, devno, 1);
	if (retval)
		goto init_err;

	devnode_class = class_create(THIS_MODULE, node_name);
	if (!devnode_class)
		goto init_err;

	devnode_device = device_create(devnode_class, NULL, devno,
			NULL, "%s", node_name);
	if (!devnode_device)
		goto init_err;

	printk(KERN_INFO "device was created with name: %s\n", node_name);
	retval = 0;
	goto init_ok;

init_err:
	if (devnode_class)
		class_destroy(devnode_class);

	if (devnode_cdev)
		cdev_del(devnode_cdev);

init_ok:
	return retval;
}

static void __exit solution_exit(void)
{
	if (devnode_device && devnode_class)
		device_destroy(devnode_class, devno);

	if (devnode_class)
		class_destroy(devnode_class);

	if (devnode_cdev)
		cdev_del(devnode_cdev);

	unregister_chrdev_region(devno, 1);
}

module_init(solution_init);
module_exit(solution_exit);

MODULE_DESCRIPTION("Learn Linux Kernel with stepik.org");
MODULE_AUTHOR("Aleksandr Anisimov <anisimov.alexander.s@gmail.com>");
MODULE_LICENSE("GPL v2");