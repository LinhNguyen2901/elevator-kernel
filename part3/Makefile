
obj-m += elevator_module.o
elevator_module-objs := src/elevator.o src/elevator_mod.o src/pet.o
KDIR := /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(CURDIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean
