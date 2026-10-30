#ifndef PET_H
#define PET_H
#define NUM_FLOOR 6

// type:
// C - Chihuahua: 3 lbs
// P - Pug: 14 lbs
// H - Pughuahua: 10 lbs
// D - Dachshund: 16 lbs
static const char TYPE_CHAR[] = {'C', 'P', 'H', 'D'};
static const int TYPE_WEIGHT[] = {3, 14, 10, 16};

struct pet {
    char type;
    int destination_floor;
    int weight;
    struct list_head list;
};

struct floor {
    struct list_head list;
    int size;
    int number; 
};
extern struct floor *floors;

struct pet *pet_create(int dest, int type);
void free_pet_list(struct list_head *head);

struct floor *floors_create(int num_floors);
void floors_destroy(struct floor *f_array, int num_floors);
int get_waiting(struct floor *f_array);

void add_pet_to_floor(struct pet* p, struct floor* f);
struct pet *pop_pet_from_floor(struct floor* f);
int format_floor(int current_floor, struct floor *f, char *buf, int *offset);

int has_request_above(struct floor *floors, int f);
int has_request_below(struct floor *floors, int f);

#endif