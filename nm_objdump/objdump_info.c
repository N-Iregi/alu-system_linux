#include <stdio.h>
#include "elf_tools.h"

/**
 * arch_name - BFD architecture name of the file
 * @e: loaded ELF file
 *
 * Return: the architecture name
 */
const char *arch_name(const elf_t *e)
{
	switch (e->machine)
	{
	case EM_386:
		return ("i386");
	case EM_X86_64:
		return ("i386:x86-64");
	case EM_ARM:
		return ("arm");
	case EM_AARCH64:
		return ("aarch64");
	case EM_SPARC:
		return ("sparc");
	case EM_SPARC32PLUS:
		return ("sparc:v8plus");
	case EM_SPARCV9:
		return ("sparc:v9");
	case EM_PPC:
		return ("powerpc:common");
	case EM_PPC64:
		return ("powerpc:common64");
	case EM_MIPS:
		return (e->is64 ? "mips:8000" : "mips:3000");
	case EM_S390:
		return (e->is64 ? "s390:64-bit" : "s390:31-bit");
	case EM_RISCV:
		return (e->is64 ? "riscv:rv64" : "riscv:rv32");
	default:
		return ("UNKNOWN!");
	}
}

/**
 * fmt_name - BFD target name of the file
 * @e: loaded ELF file
 *
 * Return: the target name, e.g. "elf64-x86-64"
 */
const char *fmt_name(const elf_t *e)
{
	static char buf[40];
	const char *w = e->is64 ? "64" : "32";
	const char *en = e->big ? "big" : "little";

	switch (e->machine)
	{
	case EM_386:
		return ("elf32-i386");
	case EM_X86_64:
		return ("elf64-x86-64");
	case EM_ARM:
		sprintf(buf, "elf32-%sarm", en);
		break;
	case EM_AARCH64:
		sprintf(buf, "elf64-%saarch64", en);
		break;
	case EM_SPARC:
	case EM_SPARC32PLUS:
	case EM_SPARCV9:
		sprintf(buf, "elf%s-sparc", w);
		break;
	case EM_PPC:
		return ("elf32-powerpc");
	case EM_PPC64:
		return (e->big ? "elf64-powerpc" : "elf64-powerpcle");
	case EM_MIPS:
		sprintf(buf, "elf%s-trad%smips", w, en);
		break;
	default:
		sprintf(buf, "elf%s-%s", w, en);
	}
	return (buf);
}

/**
 * bfd_flags - compute the BFD flags shown by "objdump -f"
 * @e: loaded ELF file
 *
 * Return: the flags
 */
unsigned int bfd_flags(const elf_t *e)
{
	unsigned int f = 0;
	size_t i, sym = 0;

	for (i = 0; i < e->nsec; i++)
	{
		if (e->sec[i].type == SHT_SYMTAB || e->sec[i].type == SHT_DYNSYM)
			f |= 0x10;
		if (e->sec[i].type == SHT_SYMTAB && !sym)
			sym = i;
	}
	for (i = 0; sym && i < e->nsec; i++)
	{
		if ((e->sec[i].type == SHT_REL || e->sec[i].type == SHT_RELA) &&
		    e->sec[i].link == sym && e->sec[i].info != 0)
			f |= 0x01;
	}
	if (e->type == ET_EXEC)
		f |= 0x02;
	else if (e->type == ET_DYN)
		f |= 0x40;
	if ((e->type == ET_EXEC || e->type == ET_DYN) && e->phnum > 0)
		f |= 0x100;
	return (f);
}

/**
 * print_header - print what "objdump -f" prints for a file
 * @e: loaded ELF file
 * @path: name of the file
 */
void print_header(const elf_t *e, const char *path)
{
	static const char * const names[] = {
		"HAS_RELOC", "EXEC_P", "HAS_LINENO", "HAS_DEBUG", "HAS_SYMS",
		"HAS_LOCALS", "DYNAMIC", "WP_TEXT", "D_PAGED"
	};
	unsigned int f = bfd_flags(e);
	int i, first = 1;

	printf("\n%s:     file format %s\n", path, fmt_name(e));
	printf("architecture: %s, flags 0x%08x:\n", arch_name(e), f);
	for (i = 0; i < 9; i++)
	{
		if (!(f & (1u << i)))
			continue;
		printf("%s%s", first ? "" : ", ", names[i]);
		first = 0;
	}
	printf("\nstart address 0x%0*" PRIx64 "\n\n", e->is64 ? 16 : 8, e->entry);
}
