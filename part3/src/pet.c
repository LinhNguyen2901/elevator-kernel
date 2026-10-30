#include <linux/list.h>
#include <linux/slab.h>
#include "pet.h"
#include "elevator.h"

struct pet *pet_create(int dest, int type){
    struct pet *p = kmalloc(sizeof(struct pet), GFP_KERNEL);
    if (!p) {
        return NULL;
    }
    p->destination_floor = dest;
    p->type = TYPE_CHAR[type];
    p->weight = TYPE_WEIGHT[type];
    return p;
}

void free_pet_list(struct list_head *head) {
    struct list_head *temp, *dummy;
    struct pet *item;

    list_for_each_safe(temp, dummy, head) {
        item = list_entry(temp, struct pet, list);
        list_del(temp);
        kfree(item);
    }
}

struct floor *floors_create(int num_floors) {
    struct floor *f_array = kzalloc(sizeof(struct floor) * num_floors, GFP_KERNEL);
    if (!f_array) {
        return NULL;
    }

    for (int i = 0; i < num_floors; i++) {
        f_array[i].number = i;
        f_array[i].size = 0;
        INIT_LIST_HEAD(&f_array[i].list);
    }

    return f_array;
}

void floors_destroy(struct floor *f_array, int num_floors) {
    if (!f_array) return;
    for (int i = 0; i < num_floors; i++) {
        free_pet_list(&f_array[i].list);
    }
    kfree(f_array);
}

int get_waiting(struct floor *f_array){
    int out = 0;
    for (int i = 0; i < NUM_FLOOR; i++) {
        out += f_array[i].size;
    }
    return out;
}

void add_pet_to_floor(struct pet* p, struct floor* f){
    f->size += 1;
    list_add_tail(&p->list, &f->list);
}

struct pet *pop_pet_from_floor(struct floor* f) {
    struct pet *item;
    if(list_empty(&f->list)){
        return NULL;
    }

    item = list_first_entry(&f->list, struct pet, list);
    list_del(&item->list);
    f->size -= 1;

    return item;
}

int format_floor(int current_floor, struct floor *f, char *buf, int *offset) {
    struct list_head *temp; 
    struct pet *item;
    
    *offset += sprintf(buf + *offset, "[%c] Floor %d: %d ", (current_floor == f->number) ? '*' : ' ', f->number+1, f->size);

    list_for_each(temp, &f->list) {
        item = list_entry(temp, struct pet, list);
        *offset += sprintf(buf + *offset, "%c%d ", item->type, item->destination_floor+1);
    } 
    *offset += sprintf(buf + *offset, "\n");
    
    return *offset;
}

int has_request_above(struct floor *floors, int f){
    for (int i = f + 1; i < NUM_FLOOR; ++i){
        if (floors[i].size > 0)
            return true;
    }
    return false;
}

int has_request_below(struct floor *floors, int f){
    for (int i = f - 1; i >= 0; --i){
        if (floors[i].size > 0)
            return true;
    }
    return false;
}
