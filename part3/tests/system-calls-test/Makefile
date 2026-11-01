obj-m += syscheck.o

KDIR := /lib/modules/$(shell uname -r)/build

.PHONY: all clean run

all: syscheck.ko test-syscalls

run: test-syscalls
	./test-syscalls

test-syscalls: test-syscalls.c test-syscalls.h
	$(CC) -Wall -Wextra -o $@ test-syscalls.c

syscheck.ko: syscheck.c
	$(MAKE) -C $(KDIR) M=$(CURDIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(CURDIR) clean
	rm -f test-syscalls test_syscalls