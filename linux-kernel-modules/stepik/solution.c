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
#include <linux/hrtimer.h>
#include <linux/jiffies.h>
#include <linux/time64.h>
#include <linux/ktime.h>

#define MY_HOST 1

#ifndef MY_HOST
#include "checker.h"
#endif

struct solution_timer_t {
	struct hrtimer timer;
	ktime_t period;
};

#define MAX_CNT_TIMER 32

/* delays for timers in milisec */
static unsigned long delays[MAX_CNT_TIMER] = { 0 };
static int delays_arr_len = 0;
module_param_array(delays, ulong, &delays_arr_len, 0);

static struct solution_timer_t solution_timer;
static int next_timer_to_run = 0;

static enum hrtimer_restart timer_handler(struct hrtimer *hrt)
{
#ifndef MY_HOST
	check_timer();
#endif
	printk(KERN_INFO "kernel_mooc: cur-timer = %d | "
			"expired delay = %ld\n",
			next_timer_to_run, delays[next_timer_to_run]);

	++next_timer_to_run;

	if (next_timer_to_run == delays_arr_len)
		return HRTIMER_NORESTART;

	solution_timer.period = ms_to_ktime(delays[next_timer_to_run]);
	hrtimer_forward_now(hrt, solution_timer.period);

	return HRTIMER_RESTART;
}

static int __init solution_init(void)
{
	hrtimer_init(&solution_timer.timer, CLOCK_REALTIME, HRTIMER_MODE_REL);

	solution_timer.timer.function = timer_handler;
	solution_timer.period = ms_to_ktime(delays[next_timer_to_run]);

#ifndef MY_HOST
	check_timer();
#endif
	hrtimer_set_expires(&solution_timer.timer, solution_timer.period);
	hrtimer_start(&solution_timer.timer, solution_timer.period,
			HRTIMER_MODE_REL);

	return 0;
}

static void __exit solution_exit(void)
{
	hrtimer_cancel(&solution_timer.timer);
}

module_init(solution_init);
module_exit(solution_exit);

MODULE_DESCRIPTION("Learn Linux Kernel with stepik.org");
MODULE_AUTHOR("Aleksandr Anisimov <anisimov.alexander.s@gmail.com>");
MODULE_LICENSE("GPL v2");