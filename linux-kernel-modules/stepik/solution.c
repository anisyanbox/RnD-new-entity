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
#include <linux/spinlock.h>
#include <linux/spinlock_types.h>
#include <linux/ioctl.h>

#define IOC_MAGIC 'k'

#define SUM_LENGTH _IOWR(IOC_MAGIC, 1, char*)
#define SUM_CONTENT _IOWR(IOC_MAGIC, 2, char*)

#define MODULE_DEFNAME "solution_node"

static dev_t devno_first;
static int devcount = 1;
static int my_major = 240; /* static major for experimenting */
static int my_minor = 0;
static struct cdev *devnode_cdev;

static ssize_t sol_read(struct file *file, char __user *buf, size_t lbuf,
			loff_t *ppos)
{
	return 0;
}

static ssize_t sol_write(struct file *file, const char __user *buf,
			 size_t lbuf, loff_t *ppos)
{
	return lbuf;
}

static int sol_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int sol_release(struct inode *inode, struct file *file)
{
	return 0;
}

static long sol_unlocked_ioctl(struct file *file,
		unsigned int cmd, unsigned long arg)
{
	long cur_sum;
	static long sum_len = 0;
	static long sum_content = 0;

	switch (cmd) {
	case SUM_LENGTH:
		sum_len += strlen((char *)arg);
		return sum_len;
	case SUM_CONTENT:
		(void)kstrtol((char*)arg, 10, &cur_sum);
		sum_content += cur_sum;
		return sum_content;
	default:
		break;
	}

	return -EINVAL;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = sol_open,
	.release = sol_release,
	.read = sol_read,
	.write = sol_write,
	.unlocked_ioctl = sol_unlocked_ioctl,
};

static int __init solution_init(void)
{
	int retval;

	devno_first = MKDEV(my_major, my_minor);
	register_chrdev_region(devno_first, devcount, MODULE_DEFNAME);

	/* allocate memory */
	devnode_cdev = cdev_alloc();
	if (!devnode_cdev) {
		printk(KERN_ERR "%s: cdev_alloc() failed\n", __func__);
		return -ENOMEM;
	}

	/* assign fops to cdev */
	cdev_init(devnode_cdev, &fops);

	/* assign kernel object to common device table */
	retval = cdev_add(devnode_cdev, devno_first, 1);
	if (retval) {
		printk(KERN_ERR "%s: cdev_add() failed\n", __func__);
		cdev_del(devnode_cdev);
	}

	return 0;
}

static void __exit solution_exit(void)
{
	if (devnode_cdev)
		cdev_del(devnode_cdev);

	unregister_chrdev_region(devno_first, devcount);
}

module_init(solution_init);
module_exit(solution_exit);

MODULE_DESCRIPTION("Learn Linux Kernel with stepik.org");
MODULE_AUTHOR("Aleksandr Anisimov <anisimov.alexander.s@gmail.com>");
MODULE_LICENSE("GPL v2");