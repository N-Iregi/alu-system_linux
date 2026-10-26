CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
COMMON = elf_io.c elf_load.c
NM_SRC = $(COMMON) nm_sym.c hnm.c
OBJ_SRC = $(COMMON) objdump_info.c objdump_dump.c hobjdump.c
HDR = elf_tools.h

.PHONY: all clean

all: hnm hobjdump

hnm: $(NM_SRC) $(HDR)
	$(CC) $(CFLAGS) $(NM_SRC) -o hnm

hobjdump: $(OBJ_SRC) $(HDR)
	$(CC) $(CFLAGS) $(OBJ_SRC) -o hobjdump

clean:
	$(RM) hnm hobjdump
