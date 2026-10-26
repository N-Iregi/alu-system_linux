#ifndef ELF_TOOLS_H
#define ELF_TOOLS_H

#include <elf.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

/* Constants missing from the elf.h of older systems */
#ifndef EM_AARCH64
#define EM_AARCH64 183
#endif
#ifndef EM_RISCV
#define EM_RISCV 243
#endif
#ifndef EM_SPARC32PLUS
#define EM_SPARC32PLUS 18
#endif
#ifndef EM_SPARCV9
#define EM_SPARCV9 43
#endif
#ifndef EM_X86_64
#define EM_X86_64 62
#endif
#ifndef STT_GNU_IFUNC
#define STT_GNU_IFUNC 10
#endif
#ifndef STB_GNU_UNIQUE
#define STB_GNU_UNIQUE 10
#endif
#ifndef SHT_SYMTAB_SHNDX
#define SHT_SYMTAB_SHNDX 18
#endif
#ifndef SHN_XINDEX
#define SHN_XINDEX 0xffff
#endif

#define ELF_OK 0
#define ELF_NOFILE 1
#define ELF_BADFMT 2

/**
 * struct sec_s - normalized section header (any class, any endianness)
 * @name: section name
 * @name_off: offset of the name in the section name string table
 * @type: section type
 * @link: section link
 * @info: section info
 * @flags: section flags
 * @addr: virtual address of the section
 * @offset: file offset of the section
 * @size: size of the section in bytes
 * @entsize: size of one entry, if the section is a table
 */
typedef struct sec_s
{
	const char *name;
	uint32_t name_off;
	uint32_t type;
	uint32_t link;
	uint32_t info;
	uint64_t flags;
	uint64_t addr;
	uint64_t offset;
	uint64_t size;
	uint64_t entsize;
} sec_t;

/**
 * struct elf_s - an ELF file loaded in memory
 * @data: raw file content (one extra NUL byte at the end)
 * @size: size of the file
 * @is64: 1 if ELFCLASS64
 * @big: 1 if big endian
 * @type: e_type
 * @machine: e_machine
 * @phnum: e_phnum
 * @shentsize: e_shentsize
 * @shstrndx: index of the section name string table
 * @nsec: number of sections
 * @entry: e_entry
 * @shoff: e_shoff
 * @sec: normalized section headers
 */
typedef struct elf_s
{
	unsigned char *data;
	size_t size;
	int is64;
	int big;
	uint16_t type;
	uint16_t machine;
	uint16_t phnum;
	uint16_t shentsize;
	uint32_t shstrndx;
	size_t nsec;
	uint64_t entry;
	uint64_t shoff;
	sec_t *sec;
} elf_t;

/* Read field f of struct T (Ehdr, Shdr, Sym) located at offset p */
#define FLD(e, T, f) \
	((e)->is64 ? offsetof(Elf64_##T, f) : offsetof(Elf32_##T, f))
#define LEN(e, T, f) \
	((e)->is64 ? sizeof(((Elf64_##T *)0)->f) : \
	sizeof(((Elf32_##T *)0)->f))
#define RD(e, T, p, f) rd((e), (p) + FLD(e, T, f), LEN(e, T, f))

/* elf_io.c */
uint64_t rd(const elf_t *e, size_t off, int n);
unsigned char *read_file(const char *path, size_t *size);
void elf_free(elf_t *e);
void elf_error(const char *prog, const char *path, int rc);

/* elf_load.c */
int elf_open(elf_t *e, const char *path);

/* nm_sym.c */
char sym_char(const elf_t *e, uint32_t shndx, unsigned char info);
void nm_print(const elf_t *e);

/* objdump_info.c */
const char *arch_name(const elf_t *e);
const char *fmt_name(const elf_t *e);
unsigned int bfd_flags(const elf_t *e);
void print_header(const elf_t *e, const char *path);

/* objdump_dump.c */
void dump_sections(const elf_t *e);

#endif /* ELF_TOOLS_H */
