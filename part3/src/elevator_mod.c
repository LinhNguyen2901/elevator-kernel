#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

#include "elevator.h"
#include "pet.h"

MODULE_AUTHOR("cop4610t");
MODULE_DESCRIPTION("Elevator kernel module");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

#define ENTRY_NAME "elevator"
#define PERMS 0444
#define PARENT NULL
#define BUF_LEN 10000

int start_elevator(void);                                                           // starts the elevator to pick up and drop off passengers
int issue_request(int start_floor, int destination_floor, int type);                // add passengers requests to specific floors
int stop_elevator(void);                                                            // stops the elevator

static struct proc_dir_entry* proc_entry;

struct elevator *ele = NULL;
struct floor *floors;

extern int (*STUB_start_elevator)(void);
extern int (*STUB_issue_request)(int, int, int);
extern int (*STUB_stop_elevator)(void);

static ssize_t elevator_read(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
    char *buf;
    int len = 0;
    ssize_t ret;

    buf = kmalloc(BUF_LEN, GFP_KERNEL);
    if (!buf)
        return -ENOMEM;

    mutex_lock(&ele->lock);
    
    len = sprintf(buf, "Elevator state: %s\n", STATE_NAME[ele->state]);
    len += sprintf(buf + len, "Current floor: %d\n", ele->current_floor + 1);
    len += sprintf(buf + len, "Current load: %d lbs\n", ele->current_weight);
    len += sprintf(buf + len, "Elevator status: ");
    format_passengers(ele, buf, &len);
    len += sprintf(buf + len, "\n\n");

    for (int i = NUM_FLOOR - 1; i >= 0; i--)
        format_floor(ele->current_floor, &floors[i], buf, &len);

    len += sprintf(buf + len, "\nNumber of pets: %d\n", ele->pet_count);
    len += sprintf(buf + len, "Number of pets waiting: %d\n", get_waiting(floors));
    len += sprintf(buf + len, "Number of pets serviced: %d\n", ele->serviced);

    mutex_unlock(&ele->lock);

    ret = simple_read_from_buffer(ubuf, count, ppos, buf, len);
    kfree(buf);
    return ret;
}

static const struct proc_ops procfile_fops = {
    .proc_read = elevator_read,
};

int issue_request(int start_floor, int destination_floor, int type) {
    struct pet *p;

    // invalid
    if (start_floor < 1 || start_floor > NUM_FLOOR ||
        destination_floor < 1 || destination_floor > NUM_FLOOR ||
        type < 0 || type > 3)
        return 1;

    p = pet_create(destination_floor - 1, type);
    if (!p)
        return -ENOMEM;

    mutex_lock(&ele->lock);
    add_pet_to_floor(p, &floors[start_floor - 1]);
    mutex_unlock(&ele->lock);
    return 0;
}

static int __init elevator_init(void)
{
    ele = elevator_create();
    if (!ele) {
        return -ENOMEM;
    }

    floors = floors_create(NUM_FLOOR);
    if (!floors) {
        elevator_destroy(ele);
        return -ENOMEM;
    }

    proc_entry = proc_create(ENTRY_NAME, PERMS, PARENT, &procfile_fops);
    if (!proc_entry) {
        floors_destroy(floors, NUM_FLOOR);
        elevator_destroy(ele);
        return -ENOMEM;
    }

    STUB_start_elevator = elevator_thread_start;
    STUB_issue_request  = issue_request;
    STUB_stop_elevator  = elevator_thread_stop;
    return 0;
}

static void __exit elevator_exit(void)
{
    STUB_start_elevator = NULL;
    STUB_issue_request  = NULL;
    STUB_stop_elevator  = NULL;

    elevator_thread_kill();
    proc_remove(proc_entry);
    elevator_free_all_pets();

    floors_destroy(floors, NUM_FLOOR);
    elevator_destroy(ele);
}

module_init(elevator_init);
module_exit(elevator_exit);
