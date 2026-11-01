#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/list.h>
#include "elevator.h"
#include "pet.h"

#define LOAD_DELAY_MS 1000
#define MOVE_DELAY_MS 2000
static struct task_struct *elevator_task = NULL;

struct elevator *elevator_create(void) {
    struct elevator *e = kzalloc(sizeof(struct elevator), GFP_KERNEL);
    if (!e) 
        return NULL;
    e->current_floor = 0;
    e->current_weight = 0;
    e->pet_count = 0;
    e->state = OFFLINE;
    e->serviced = 0;
    INIT_LIST_HEAD(&e->passengers);
    mutex_init(&e->lock);
    
    return e;
}

void elevator_destroy(struct elevator *e) {
    if (e) {
        kfree(e);
    }
}

int format_passengers(struct elevator *e, char *buf, int *offset) {
    struct list_head *temp; 
    struct pet *item;
    list_for_each(temp, &e->passengers) {
        item = list_entry(temp, struct pet, list);
        *offset += sprintf(buf + *offset, "%c%d ", item->type, item->destination_floor+1);
    } 
    return *offset;
}

static int elevator_thread(void *data)
{
    enum State prev_state = OFFLINE;
    while (!kthread_should_stop()) {
        enum Action action = NONE;
        if (ele->state != OFFLINE && mutex_lock_interruptible(&ele->lock) == 0) {
            // stopping and no pets left on board -> go OFFLINE
            if (ele->stopping && ele->pet_count == 0) {
                ele->state = OFFLINE;
                ele->stopping = 0;
                mutex_unlock(&ele->lock);
            }
            switch (ele->state){
                case LOADING:
                    elevator_unload(ele, &floors[ele->current_floor]);
                    ele->state = elevator_scan(ele, prev_state);
                    if (!ele->stopping)
                        elevator_load(ele, &floors[ele->current_floor], ele->state);
                    if (ele->state == UP || ele->state == DOWN)
                        action = MOVE;
                break;
                case IDLE:
                    if (!ele->stopping && has_request_here(&floors[ele->current_floor], ele->state)) {
                        prev_state = ele->state;
                        ele->state = LOADING;
                        action = LOAD;
                    } else {
                        ele->state = elevator_scan(ele, ele->state);
                        if (ele->state == UP || ele->state == DOWN) 
                            action = MOVE;
                    }
                break;
                case UP: case DOWN:
                    if (has_destination_here(&ele->passengers, ele->current_floor) || (!ele->stopping && has_request_here(&floors[ele->current_floor], ele->state))) {
                        prev_state = ele->state;
                        ele->state = LOADING;
                        action = LOAD;
                    }
                    else action = MOVE;
                break;
                case OFFLINE: mutex_unlock(&ele->lock); continue;
            }
            mutex_unlock(&ele->lock);
        }
        else 
            msleep(1000);
        if (action == LOAD) msleep(LOAD_DELAY_MS);
        else if (action == MOVE){
            msleep(MOVE_DELAY_MS);
            if (mutex_lock_interruptible(&ele->lock) == 0) {
                if (ele->state == UP || ele->state == DOWN) {
                    elevator_move_floor(ele, ele->state);
                }
                mutex_unlock(&ele->lock);
            }
        }
    }
    return 0;
}

int elevator_thread_start(void) {
    mutex_lock(&ele->lock);
    if (ele->state != OFFLINE) {
        mutex_unlock(&ele->lock);
        return 1;
    }
    ele->state = IDLE;
    mutex_unlock(&ele->lock);

    if (!elevator_task) {
        elevator_task = kthread_run(elevator_thread, NULL, "elevator_thread");
        if (IS_ERR(elevator_task)) {
            elevator_task = NULL;
            return -ENOMEM;
        }
    }
    return 0;
}

int elevator_thread_stop(void) {
    mutex_lock(&ele->lock);
    if (ele->stopping) {
        mutex_unlock(&ele->lock);
        return 1;
    }
    if (ele->state != OFFLINE)
        ele->stopping = 1;
    mutex_unlock(&ele->lock);
    return 0;
}

void elevator_move_floor(struct elevator *ele, enum State cur){
    if (cur == UP && ele->current_floor < NUM_FLOOR - 1) 
        ele->current_floor += 1;
    else if (cur == DOWN && ele->current_floor > 0) {
        ele->current_floor -= 1;
    }
}

void elevator_unload(struct elevator *ele, struct floor *f){
    struct list_head * temp; 
    struct list_head * dummy; 
    struct pet * item;
    list_for_each_safe(temp, dummy, &ele->passengers) {
        item = list_entry(temp, struct pet, list);
        if (item->destination_floor == f->number) {
            ele->current_weight -= item->weight;
            ele->pet_count--;
            ele->serviced++;
            list_del(temp);
            kfree(item);
        }
    }
}

void elevator_load(struct elevator *ele, struct floor *f, enum State cur){
    struct list_head * temp;
    struct list_head * dummy; struct pet * item;
    list_for_each_safe(temp, dummy, &f->list) {
        if (MAX_WEIGHT - ele->current_weight < 3) break;
        item = list_entry(temp, struct pet, list);
        if ((ele->current_floor == NUM_FLOOR - 1 || ele->current_floor == 0 ||
            (cur == UP && item->destination_floor > ele->current_floor) ||
            (cur == DOWN && item->destination_floor < ele->current_floor) || cur == IDLE) &&
            ele->current_weight + item->weight <= MAX_WEIGHT && ele->pet_count < MAX_PASSENGERS) {
            
            list_del(temp);
            list_add_tail(&item->list, &ele->passengers);
            ele->current_weight += item->weight;
            ele->pet_count++;
            f->size--;
        }
    }
}

int has_destination_above(struct list_head *pets, int f){
    struct list_head * temp; struct pet * item;
    list_for_each(temp, pets) {
        item = list_entry(temp, struct pet, list);
        if (item->destination_floor > f)
            return true;
    }
    return false;
}

int has_destination_here(struct list_head *pets, int f){
    struct list_head * temp; struct pet * item;
    list_for_each(temp, pets) {
        item = list_entry(temp, struct pet, list);
        if (item->destination_floor == f)
            return true;
    }
    return false;
}

int has_destination_below(struct list_head *pets, int f){
    struct list_head * temp; struct pet * item;
    list_for_each(temp, pets) {
        item = list_entry(temp, struct pet, list);
        if (item->destination_floor < f)
            return true;
    }
    return false;
}

int has_request_here(struct floor *floor, enum State cur){
    struct list_head * temp;
    struct list_head * dummy; struct pet * item;
    list_for_each_safe(temp, dummy, &floor->list) {
        if (MAX_WEIGHT - ele->current_weight < 3) break;
        item = list_entry(temp, struct pet, list);
        if ((ele->current_floor == NUM_FLOOR - 1 || ele->current_floor == 0 ||
            (cur == UP && item->destination_floor > ele->current_floor) ||
            (cur == DOWN && item->destination_floor < ele->current_floor) || cur == IDLE) &&
            ele->current_weight + item->weight <= MAX_WEIGHT && ele->pet_count < MAX_PASSENGERS) {
            return true;
        }
    }
    return false;
}

int find_nearest_request(struct floor *floors, int current_floor) {
    int distance = 0;
    while (current_floor + distance < NUM_FLOOR || current_floor - distance >= 0) {
        int up = current_floor + distance;
        int down = current_floor - distance;

        if (up < NUM_FLOOR && !list_empty(&floors[up].list))
            return up;
        if (down >= 0 && !list_empty(&floors[down].list))
            return down;
        
        distance++;
    }
    
    return -1;
}

enum State elevator_scan(struct elevator *ele, enum State cur){
    if (cur == UP) {
        if (has_request_above(floors, ele->current_floor) || has_destination_above(&ele->passengers, ele->current_floor)) {
            return UP;
        } else if (has_request_below(floors, ele->current_floor) || has_destination_below(&ele->passengers, ele->current_floor)) {
            return DOWN;
        } else {
            return IDLE;
        }
    }
    
    if (cur == DOWN) {
        if (has_request_below(floors, ele->current_floor) || has_destination_below(&ele->passengers, ele->current_floor)) {
            return DOWN;
        } else if (has_request_above(floors, ele->current_floor) || has_destination_above(&ele->passengers, ele->current_floor)) {
            return UP;
        } else {
            return IDLE;
        }
    }
    
    if (cur == IDLE) {
        if (has_destination_above(&ele->passengers, ele->current_floor))
            return UP;
        if (has_destination_below(&ele->passengers, ele->current_floor))
            return DOWN;

        int target = find_nearest_request(floors, ele->current_floor);
        struct pet *first;

        // nobody waiting -> stay IDLE
        if (target < 0)
            return IDLE;
        if (target > ele->current_floor) return UP;
        if (target < ele->current_floor) return DOWN;

        // someone waiting on THIS floor -> go the way the first pet wants
        first = list_first_entry(&floors[target].list, struct pet, list);
        return (first->destination_floor > ele->current_floor) ? UP : DOWN;
    }
    
    return IDLE;
}


void elevator_thread_kill(void) {
    if (elevator_task) {
        kthread_stop(elevator_task);
        elevator_task = NULL;
    }
}

void elevator_free_all_pets(void) {
    int i;

    mutex_lock(&ele->lock);
    for (i = 0; i < NUM_FLOOR; i++) {
        free_pet_list(&floors[i].list);
        floors[i].size = 0;
    }
    free_pet_list(&ele->passengers);
    ele->pet_count = 0;
    ele->current_weight = 0;
    mutex_unlock(&ele->lock);
}