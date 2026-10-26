# nm_objdump

Re-implementation of two GNU binutils commands for ELF files
(32/64-bit, little/big endian, any ELF type).

- `hnm [objfile ...]`      -> same output as `nm -p`
- `hobjdump [objfile ...]` -> same output as `objdump -sf`

## Build

    make            # builds hnm and hobjdump

or, without make:

    gcc -Wall -Werror -Wextra -pedantic elf_io.c elf_load.c nm_sym.c hnm.c -o hnm
    gcc -Wall -Werror -Wextra -pedantic elf_io.c elf_load.c objdump_info.c objdump_dump.c hobjdump.c -o hobjdump

## Files

- elf_tools.h: shared types and prototypes
- elf_io.c: endian-aware reader, file loading, error messages
- elf_load.c: ELF header and section header parsing
- nm_sym.c, hnm.c: symbol listing
- objdump_info.c, objdump_dump.c, hobjdump.c: header info and hex dump
