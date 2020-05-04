#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/cred.h>
#include <linux/uidgid.h>
#include <linux/device.h>
#include <linux/stat.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/slab.h>

#define UID_CHANGER_DEV_NAME "uid_changer"
#define UID_CHANGER_CLASS_NAME "uid_changer"
#define UID_CHARGER_MINOR 0

#define ROOT_UID 0

static dev_t dev_no;
static struct class *uid_changer_class;
static struct device *uid_changer_dev;
static struct cdev *uid_changer_cdev;

static void uid_changer_apply_new_uid_for_pid(pid_t pid, uid_t new_uid)
{
	struct task_struct *task;
	uid_t *old_uid;
	gid_t *old_gid;

	for_each_process(task) {
		if (task->pid == pid) {
			printk(KERN_INFO "change-uid-for: pid=%d | pname=%s | "
				"uid=%d | NEW UID=%d",
				task->pid, task->comm,
				task->real_cred->uid.val, new_uid);

			/*
			 * We can't directly write to this memory, because
			 * 'struct cred' has 'const' qualifier.
			 *
			 * That's why try to access with help other pointer.
			 */

			/* real UID */
			old_uid = &task->real_cred->uid.val;
			*old_uid = new_uid;

			/* real SUID */
			old_uid = &task->real_cred->suid.val;
			*old_uid = new_uid;

			/* real EUID */
			old_uid = &task->real_cred->euid.val;
			*old_uid = new_uid;

			/* real FSUID */
			old_uid = &task->real_cred->fsuid.val;
			*old_uid = new_uid;

			/* real GID */
			old_uid = &task->real_cred->gid.val;
			*old_uid = new_uid;

			/* real SGID */
			old_uid = &task->real_cred->sgid.val;
			*old_uid = new_uid;

			/* real EGID */
			old_uid = &task->real_cred->egid.val;
			*old_uid = new_uid;

			/* real FSGID */
			old_uid = &task->real_cred->fsgid.val;
			*old_uid = new_uid;

			/* effective UID */
			old_uid = &task->cred->uid.val;
			*old_uid = new_uid;

			/* effective SUID */
			old_uid = &task->cred->suid.val;
			*old_uid = new_uid;

			/* effective EUID */
			old_uid = &task->cred->euid.val;
			*old_uid = new_uid;

			/* effective FSUID */
			old_uid = &task->cred->fsuid.val;
			*old_uid = new_uid;

			/* effective GID */
			old_uid = &task->cred->gid.val;
			*old_uid = new_uid;

			/* effective SGID */
			old_uid = &task->cred->sgid.val;
			*old_uid = new_uid;

			/* effective EGID */
			old_uid = &task->cred->egid.val;
			*old_uid = new_uid;

			/* effective FSGID */
			old_uid = &task->cred->fsgid.val;
			*old_uid = new_uid;

			break;
		}
	}
}

static int uid_changer_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "%s\n", __func__);

	uid_changer_apply_new_uid_for_pid(current->pid, ROOT_UID);

	return 0;
}

static int uid_changer_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "%s\n", __func__);
	return 0;
}

static ssize_t uid_changer_read(struct file *file, char __user *buf,
	size_t lbuf, loff_t *ppos)
{
	printk(KERN_INFO "%s\n", __func__);
	return 0;
}

static ssize_t mychrdev_write(struct file *file, const char __user *buf,
	size_t lbuf, loff_t *ppos)
{
	printk(KERN_INFO "%s\n", __func__);

	*ppos += lbuf;

	return lbuf;
}

static struct file_operations uid_changer_fops = {
	.owner = THIS_MODULE,
	.open = uid_changer_open,
	.release = uid_changer_release,
	.read = uid_changer_read,
	.write = mychrdev_write,
};

static ssize_t list_all_pids_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int len = 0;
	int curr_len = 0;
	struct task_struct *task;
	char *temp_buf;

	temp_buf = kmalloc(2 * sizeof(char) * PAGE_SIZE, GFP_KERNEL);
	if (!temp_buf)
		return 0;

	for_each_process(task) {
		if (len + curr_len >= PAGE_SIZE)
			break;

		len += curr_len;
		curr_len =  sprintf(temp_buf + len, "%d|%.4s|%d\n",
			task->pid, task->comm, task->real_cred->uid.val);

	}

	memcpy(buf, temp_buf, PAGE_SIZE);
	kfree(temp_buf);

	return len;
}

static DEVICE_ATTR(list_all_pids, S_IRUSR | S_IRGRP |S_IROTH,
		list_all_pids_show, NULL);

static ssize_t substitute_uid_for_pid_store(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	int res;
	int pid_to_sub;

	res = kstrtoint(buf, 10, &pid_to_sub);
	if (res)
		return (ssize_t)res;

	uid_changer_apply_new_uid_for_pid((pid_t)pid_to_sub, ROOT_UID);

	return count;
}

static DEVICE_ATTR(substitute_uid_for_pid, S_IWUSR | S_IWGRP,
		NULL, substitute_uid_for_pid_store);

static int __init uid_changer_init(void)
{
	int res, ret = -1;
	dev_t curr_dev;

	/* Request the kernel for one device number */
	res = alloc_chrdev_region(&dev_no, UID_CHARGER_MINOR,
		1, UID_CHANGER_DEV_NAME);
	if (res < 0) {
		printk(KERN_ERR "%s: alloc_chrdev_region() error\n", __func__);
		return -1;
	}

	curr_dev = MKDEV(MAJOR(dev_no), MINOR(dev_no));

	/* Create a class: appears at /sys/class/<name> */
	uid_changer_class = class_create(THIS_MODULE, UID_CHANGER_CLASS_NAME);
	if (!uid_changer_class) {
		printk(KERN_ERR "%s: class_create() error\n", __func__);
		goto init_error;
	}

	/* Initialize and create each of the device(cdev) */
	uid_changer_cdev = cdev_alloc();
	if (!uid_changer_cdev) {
		printk(KERN_ERR "%s: cdev_alloc() error\n", __func__);
		goto init_error;
	}

	/* Assign fops to the chardev */
	cdev_init(uid_changer_cdev, &uid_changer_fops);

	/* Create a device node */
	uid_changer_dev = device_create(uid_changer_class, NULL, curr_dev, NULL,
		"%s%d", UID_CHANGER_DEV_NAME, UID_CHARGER_MINOR);
	if (!uid_changer_dev) {
		printk(KERN_ERR "%s: device_create() error\n", __func__);
		goto init_error;
	}

	res = cdev_add(uid_changer_cdev, curr_dev, 1);
	if (res < 0) {
		printk(KERN_ERR "%s: cdev_add() error\n", __func__);
		goto init_error;
	}

	/* sysfs attribute to list process's PIDs from userspace */
	res = device_create_file(uid_changer_dev, &dev_attr_list_all_pids);
	if (res) {
		printk(KERN_ERR "%s: device_create_file() error\n", __func__);
		goto init_error;
	}

	/* sysfs attribute to list process's PIDs from userspace */
	res = device_create_file(uid_changer_dev, &dev_attr_substitute_uid_for_pid);
	if (res) {
		printk(KERN_ERR "%s: device_create_file() error\n", __func__);
		device_remove_file(uid_changer_dev, &dev_attr_list_all_pids);
		goto init_error;
	}

	ret = 0;
	printk(KERN_INFO "UID changer init\n");
	goto init_ok;

init_error:
	if (uid_changer_class) {
		device_destroy(uid_changer_class, curr_dev);
		class_destroy(uid_changer_class);
	}

	if (uid_changer_cdev)
		cdev_del(uid_changer_cdev);

	unregister_chrdev_region(dev_no, 1);

init_ok:
	return ret;
}

static void __exit uid_changer_exit(void)
{
	dev_t curr_dev;

	curr_dev = MKDEV(MAJOR(dev_no), MINOR(dev_no));

	device_remove_file(uid_changer_dev, &dev_attr_substitute_uid_for_pid);
	device_remove_file(uid_changer_dev, &dev_attr_list_all_pids);

	device_destroy(uid_changer_class, curr_dev);
	class_destroy(uid_changer_class);
	cdev_del(uid_changer_cdev);

	unregister_chrdev_region(dev_no, 1);

	printk(KERN_INFO "UID changer exit\n");
}

module_init(uid_changer_init);
module_exit(uid_changer_exit);

MODULE_DESCRIPTION("Change UID of process with known PID");
MODULE_AUTHOR("Aleksandr Anisimov <a.anisimov@omprussia.ru");
MODULE_LICENSE("GPL");
