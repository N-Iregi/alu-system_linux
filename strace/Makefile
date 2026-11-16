CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
SRC = main.c syscall.c params.c memory.c flags.c stat.c
HDR = strace.h syscalls.h
BINS = strace_0 strace_1 strace_2 strace_3 strace_4 strace_5 \
	strace_6 strace_7 strace_8

.PHONY: all clean

all: $(BINS)

strace_%: $(SRC) $(HDR)
	$(CC) $(CFLAGS) -DLEVEL=$* $(SRC) -o $@

clean:
	rm -f $(BINS)
