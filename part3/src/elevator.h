#ifndef ELEVATOR_H
#define ELEVATOR_H
#define MAX_WEIGHT 50
#define MAX_PASSENGERS 5

#include <linux/mutex.h>
#include "pet.h"

enum State {OFFLINE, IDLE, LOADING, UP, DOWN};
static const char* const STATE_NAME[] = {
    [OFFLINE] = "OFFLINE",
    [IDLE] = "IDLE",
    [LOADING] = "LOADING",
    [UP] = "UP",
    [DOWN] = "DOWN"
};

struct elevator {
    int current_floor;
    int current_weight;
    int pet_count;
    enum State state;
    struct list_head passengers;
    struct mutex lock;
    int serviced;
    int stopping;
};
extern struct elevator *ele;

struct elevator *elevator_create(void);
void elevator_destroy(struct elevator *e);

int elevator_thread_start(void);
int elevator_thread_stop(void);

int add_request(int start, int dest, int type);

void elevator_move_floor(struct elevator *ele, enum State cur);
void elevator_unload(struct elevator *ele, struct floor *f);
void elevator_load(struct elevator *ele, struct floor *f, enum State cur);
enum State elevator_scan(struct elevator *ele, enum State cur);

int has_destination_above(struct list_head *pets, int f);
int has_destination_below(struct list_head *pets, int f);
int find_nearest_request(struct floor *floors, int current_floor);

void elevator_thread_kill(void);
void elevator_free_all_pets(void);

#endif