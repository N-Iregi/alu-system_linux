CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
SRC = main.c syscall.c params.c memory.c flags.c stat.c

.PHONY: all clean

all: strace_0 strace_1 strace_2 strace_3 strace_4 strace_5 strace_6 strace_7 strace_8

strace_0: $(SRC)
	$(CC) $(CFLAGS) -DLEVEL=0 $(SRC) -o strace_0

strace_1: $(SRC)
	$(CC) $(CFLAGS) -DLEVEL=1 $(SRC) -o strace_1

strace_2: $(SRC)
	$(CC) $(CFLAGS) -DLEVEL=2 $(SRC) -o strace_2

strace_3: $(SRC)
	$(CC) $(CFLAGS) -DLEVEL=3 $(SRC) -o strace_3

strace_4: $(SRC)
	$(CC) $(CFLAGS) -DLEVEL=4 $(SRC) -o strace_4

strace_5: $(SRC)
	$(CC) $(CFLAGS) -DLEVEL=5 $(SRC) -o strace_5

strace_6: $(SRC)
	$(CC) $(CFLAGS) -DLEVEL=6 $(SRC) -o strace_6

strace_7: $(SRC)
	$(CC) $(CFLAGS) -DLEVEL=7 $(SRC) -o strace_7

strace_8: $(SRC)
	$(CC) $(CFLAGS) -DLEVEL=8 $(SRC) -o strace_8

clean:
	rm -f strace_0 strace_1 strace_2 strace_3 strace_4 strace_5 strace_6 strace_7 strace_8
