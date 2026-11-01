CC ?= gcc
CFLAGS ?= -Wall -Wextra
STRACE ?= strace

.PHONY: all run trace clean

all: empty part1

empty: empty.c
	$(CC) $(CFLAGS) -o $@ $<

part1: part1.c
	$(CC) $(CFLAGS) -o $@ $<

empty.trace: empty
	$(STRACE) -o $@ ./empty

part1.trace: part1
	$(STRACE) -o $@ ./part1

trace: empty.trace part1.trace

run: part1
	./part1

clean:
	rm -f empty part1
