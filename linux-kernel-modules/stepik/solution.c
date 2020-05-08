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

#define KBUF_SIZE (10 * (PAGE_SIZE))
#define MODULE_DEFNAME "solution_node"

static DEFINE_SPINLOCK(ses_id_lock);

static dev_t devno_first;
static int devcount = 1;
static int my_major = 240; /* static major for experimenting */
static int my_minor = 0;
static struct cdev *devnode_cdev;

static int curr_available_ses_id = 0;

struct devnode_session {
	int ses_id;
	int avail; /* common count bytes into buffer */
	char *kbuf;
};

static struct devnode_session *devnode_alloc_session(void)
{
	struct devnode_session *ret;

	ret = kmalloc(sizeof(struct devnode_session), GFP_KERNEL);
	if (!ret)
		return NULL;

	spin_lock(&ses_id_lock);
	ret->ses_id = curr_available_ses_id;
	++curr_available_ses_id;
	spin_unlock(&ses_id_lock);

	ret->kbuf = kmalloc(sizeof(char) * KBUF_SIZE, GFP_KERNEL);
	if (!ret->kbuf) {
		kfree(ret);
		ret = NULL;
	} else {
		memset(ret->kbuf, 0, KBUF_SIZE);
		ret->kbuf[0] = ret->ses_id + '0';
		ret->avail = 1;
	}

	return ret;
}

static void devnode_free_session(struct devnode_session *s)
{
	kfree(s->kbuf);
	kfree(s);

	if (curr_available_ses_id != 0) {
		spin_lock(&ses_id_lock);
		--curr_available_ses_id;
		spin_unlock(&ses_id_lock);
	}
}

static ssize_t sol_read(struct file *file, char __user *buf, size_t lbuf,
			     loff_t *ppos)
{
	int retval;
	struct devnode_session *s = file->private_data;

	retval = simple_read_from_buffer(buf, lbuf, ppos, s->kbuf, s->avail);
	printk(KERN_DEBUG "kernel_mooc: %s: sid = %d: lbuf = %ld, "
			"ppos = %llu, retval = %d\n",
			__func__, s->ses_id, lbuf, *ppos, retval);

	return retval;
}

static ssize_t sol_write(struct file *file, const char __user *buf,
			      size_t lbuf, loff_t *ppos)
{
	int retval;
	struct devnode_session *s = file->private_data;
	int new_pos;

	retval = simple_write_to_buffer(s->kbuf, KBUF_SIZE, ppos, buf, lbuf);
	new_pos = (int)*ppos;

	if (new_pos > s->avail)
		s->avail = new_pos;

	printk(KERN_DEBUG "kernel_mooc: %s: sid = %d: ppos = %llu, "
			"retval = %d, avail = %d, kbuf = %s\n",
			__func__, s->ses_id, *ppos, retval,
			s->avail, s->kbuf);

	return retval;
}

static int sol_open(struct inode *inode, struct file *file)
{
	struct devnode_session *s;

	s = devnode_alloc_session();
	if (!s) {
		printk(KERN_ERR "%s: devnode_alloc_session() failed\n", __func__);
		return -1;
	}

	printk(KERN_DEBUG "kernel_mooc: %s: sid = %d\n", __func__, s->ses_id);
	file->private_data = s;
	file->f_pos = 0;

	return 0;
}

static int sol_release(struct inode *inode, struct file *file)
{
	struct devnode_session *s = file->private_data;

	printk(KERN_DEBUG "kernel_mooc: %s: sid = %d\n", __func__, s->ses_id);

	if (s)
		devnode_free_session(s);

	return 0;
}

static loff_t sol_seek(struct file *file, loff_t offset, int orig)
{
	loff_t testpos;
	struct devnode_session *s = file->private_data;

	switch (orig) {
	case SEEK_SET:
		testpos = offset;
		break;
	case SEEK_CUR:
		testpos = file->f_pos + offset;
		break;
	case SEEK_END:
		testpos = s->avail + offset;
		break;
	default:
		return -EINVAL;
	}

	testpos = testpos < KBUF_SIZE ? testpos : KBUF_SIZE;
	testpos = testpos >= 0 ? testpos : 0;

	file->f_pos = testpos;
	printk(KERN_DEBUG "kernel_mooc: %s: sid = %d: seek(off=%d, cmd=%d), file->f_pos = %d",
			__func__, s->ses_id, (int)offset, orig, (int)file->f_pos);

	return testpos;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = sol_open,
	.release = sol_release,
	.read = sol_read,
	.write = sol_write,
	.llseek = sol_seek,
};

static int __init solution_init(void)
{
	int retval;

	devno_first = MKDEV(my_major, my_minor);
	register_chrdev_region(devno_first, devcount, MODULE_DEFNAME);

	spin_lock_init(&ses_id_lock);

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
