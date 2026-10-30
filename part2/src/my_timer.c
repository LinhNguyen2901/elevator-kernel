
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/timekeeping.h>
#include <linux/mutex.h>

MODULE_AUTHOR("cop4610t");
MODULE_DESCRIPTION("Elevator kernel module");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

#define ENTRY_NAME "timer"
#define PERMS 0444
#define PARENT NULL
#define BUF_LEN 100

static struct proc_dir_entry* proc_entry;
static char msg[BUF_LEN];
static int procfs_buf_len;

static struct timespec64 previous_time;
static int first_read = 1;
static DEFINE_MUTEX(timer_lock);

static ssize_t procfile_read(struct file* file, char __user* ubuf, size_t count, loff_t* ppos) {
    struct timespec64 current_time;
    long long seconds;
    long nanoseconds;

    if (*ppos > 0)
        return 0;

    mutex_lock(&timer_lock);

    // Get the current time
    ktime_get_real_ts64(&current_time);

    procfs_buf_len = scnprintf(msg, BUF_LEN,
        "current time: %lld.%09ld\n",
        (long long)current_time.tv_sec,
        current_time.tv_nsec);

    // Calculate elapsed time since the previous read
    if (!first_read) {
        seconds = current_time.tv_sec - previous_time.tv_sec;
        nanoseconds = current_time.tv_nsec - previous_time.tv_nsec;
        if (nanoseconds < 0) {
            seconds--;
            nanoseconds += 1000000000L;
        }
        procfs_buf_len += scnprintf(
            msg + procfs_buf_len,
            BUF_LEN - procfs_buf_len,
            "elapsed time: %lld.%09ld\n",
            seconds, nanoseconds);
    }

    // Copy data from kernel space to user space
    if (count < procfs_buf_len) {
        mutex_unlock(&timer_lock);
        return 0;
    }

    if (copy_to_user(ubuf, msg, procfs_buf_len)) {
        mutex_unlock(&timer_lock);
        return -EFAULT;
    }

    *ppos = procfs_buf_len;

    // Save the timestamp for the next read
    previous_time = current_time;
    first_read = 0;
    mutex_unlock(&timer_lock);
    return procfs_buf_len;
}

static const struct proc_ops procfile_fops = {
    .proc_read = procfile_read,
};

static int __init custom_init(void) {
    proc_entry = proc_create(ENTRY_NAME, PERMS, PARENT, &procfile_fops);
    if (proc_entry == NULL)
        return -ENOMEM;
    return 0;
}

static void __exit custom_exit(void) {
    proc_remove(proc_entry);
}

module_init(custom_init);
module_exit(custom_exit);
