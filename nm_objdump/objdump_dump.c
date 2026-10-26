#include <stdio.h>
#include "elf_tools.h"

/**
 * shown - tell whether "objdump -s" displays a section
 * @e: loaded ELF file
 * @i: index of the section
 * @sym: index of the .symtab section, 0 if there is none
 *
 * Return: 1 if the section is displayed, 0 otherwise
 */
static int shown(const elf_t *e, size_t i, size_t sym)
{
	const sec_t *s = &e->sec[i];

	if (s->type == SHT_NULL || s->type == SHT_NOBITS || s->size == 0)
		return (0);
	if (s->type == SHT_SYMTAB || s->type == SHT_SYMTAB_SHNDX)
		return (0);
	if (i == e->shstrndx || (sym && i == e->sec[sym].link))
		return (0);
	if ((s->type == SHT_REL || s->type == SHT_RELA) && sym &&
	    s->link == sym && s->info > 0 && s->info < e->nsec &&
	    e->sec[s->info].type != SHT_REL && e->sec[s->info].type != SHT_RELA)
		return (0);
	return (s->offset < e->size);
}

/**
 * line - print one line (up to 16 bytes) of a section dump
 * @addr: address of the first byte
 * @w: number of hex digits of the address
 * @p: bytes to print
 * @n: number of bytes to print
 */
static void line(uint64_t addr, int w, const unsigned char *p, size_t n)
{
	size_t j;

	printf(" %0*" PRIx64 " ", w, addr);
	for (j = 0; j < 16; j++)
	{
		if (j < n)
			printf("%02x", p[j]);
		else
			printf("  ");
		if ((j & 3) == 3)
			printf(" ");
	}
	printf(" ");
	for (j = 0; j < 16; j++)
	{
		if (j >= n)
			putchar(' ');
		else
			putchar(p[j] >= 32 && p[j] < 127 ? p[j] : '.');
	}
	putchar('\n');
}

/**
 * dump_one - dump the content of one section
 * @e: loaded ELF file
 * @s: section to dump
 */
static void dump_one(const elf_t *e, const sec_t *s)
{
	uint64_t len = s->size, off;
	int w = 4;

	if (len > e->size - s->offset)
		len = e->size - s->offset;
	while (w < 16 && ((s->addr + len) >> (4 * w)) != 0)
		w++;
	printf("Contents of section %s:\n", s->name);
	for (off = 0; off < len; off += 16)
		line(s->addr + off, w, e->data + s->offset + off,
		     len - off < 16 ? len - off : 16);
}

/**
 * dump_sections - print what "objdump -s" prints for a file
 * @e: loaded ELF file
 */
void dump_sections(const elf_t *e)
{
	size_t i, sym = 0;

	for (i = 0; i < e->nsec && !sym; i++)
		if (e->sec[i].type == SHT_SYMTAB)
			sym = i;
	for (i = 0; i < e->nsec; i++)
		if (shown(e, i, sym))
			dump_one(e, &e->sec[i]);
}
