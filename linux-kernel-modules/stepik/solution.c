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

static int open_cnt = 0;
static int common_write_data_bytes = 0;

static dev_t first; /* first number of device node */
static unsigned int devcount = 1;
static int my_major = 240; /* static major */
static int my_minor = 0;
static struct cdev *devnode_cdev;

#define MYDEV_NAME "solution_node"

static ssize_t solution_read(struct file *file, char __user *buf, size_t lbuf,
			     loff_t *ppos)
{
	int showlen;
	char show[50];

	showlen = sprintf(show, "%d %d\n", open_cnt, common_write_data_bytes);

	return simple_read_from_buffer(buf, lbuf, ppos, show, showlen);
}

static ssize_t solution_write(struct file *file, const char __user *buf,
			      size_t lbuf, loff_t *ppos)
{
	common_write_data_bytes += lbuf;

	return lbuf;
}

static int solution_open(struct inode *inode, struct file *file)
{
	++open_cnt;

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

	first = MKDEV(my_major, my_minor);
	register_chrdev_region(first, devcount, MYDEV_NAME);

	/* allocate memory */
	devnode_cdev = cdev_alloc();
	if (!devnode_cdev)
		return -ENOMEM;

	/* assign fops to cdev */
	cdev_init(devnode_cdev, &fops);

	/*
	 * Assign kernel object to common device table
	 *
	 * use sudo mknod /dev/solution_node c 240 0 2> /dev/null
	 * to create node in devfs
	 */
	retval = cdev_add(devnode_cdev, first, devcount);
	if (retval)
		cdev_del(devnode_cdev);

	return retval;
}

static void __exit solution_exit(void)
{
	if (devnode_cdev)
		cdev_del(devnode_cdev);

	unregister_chrdev_region(first, devcount);
}

module_init(solution_init);
module_exit(solution_exit);

MODULE_DESCRIPTION("Learn Linux Kernel with stepik.org");
MODULE_AUTHOR("Aleksandr Anisimov <anisimov.alexander.s@gmail.com>");
MODULE_LICENSE("GPL v2");