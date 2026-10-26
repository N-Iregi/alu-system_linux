#include <stdio.h>
#include <string.h>
#include "elf_tools.h"

/**
 * sec_char - letter for a symbol defined in a regular section
 * @s: section holding the symbol
 *
 * Return: lowercase nm letter
 */
static char sec_char(const sec_t *s)
{
	if (s->flags & SHF_EXECINSTR)
		return ('t');
	if (s->flags & SHF_ALLOC)
	{
		if (s->type == SHT_NOBITS)
			return ('b');
		return ((s->flags & SHF_WRITE) ? 'd' : 'r');
	}
	if (strncmp(s->name, ".debug", 6) == 0 ||
	    strncmp(s->name, ".zdebug", 7) == 0)
		return ('N');
	return ('n');
}

/**
 * sym_char - compute the nm type letter of a symbol
 * @e: loaded ELF file
 * @shndx: section index of the symbol
 * @info: st_info of the symbol
 *
 * Return: the letter, uppercase if the symbol is not local
 */
char sym_char(const elf_t *e, uint32_t shndx, unsigned char info)
{
	int bind = info >> 4, type = info & 0xf;
	char c;

	if (shndx == SHN_COMMON)
		return ('C');
	if (shndx == SHN_UNDEF)
	{
		if (bind != STB_WEAK)
			return ('U');
		return (type == STT_OBJECT ? 'v' : 'w');
	}
	if (type == STT_GNU_IFUNC)
		return ('i');
	if (bind == STB_WEAK)
		return (type == STT_OBJECT ? 'V' : 'W');
	if (bind == STB_GNU_UNIQUE)
		return ('u');
	if (shndx == SHN_ABS)
		c = 'a';
	else if (shndx < e->nsec)
		c = sec_char(&e->sec[shndx]);
	else
		return ('?');
	if (bind != STB_LOCAL)
		c = c - 'a' + 'A';
	return (c);
}

/**
 * nm_one - print one symbol of the symbol table
 * @e: loaded ELF file
 * @p: offset of the symbol in the file
 * @str: string table of the symbol table
 */
static void nm_one(const elf_t *e, size_t p, const sec_t *str)
{
	unsigned char info = RD(e, Sym, p, st_info);
	uint32_t shndx = RD(e, Sym, p, st_shndx);
	uint32_t noff = RD(e, Sym, p, st_name);
	uint64_t val = RD(e, Sym, p, st_value);
	const char *name = "";
	int width = e->is64 ? 16 : 8;

	if ((info & 0xf) == STT_SECTION || (info & 0xf) == STT_FILE)
		return;
	if (str && str->offset < e->size && noff < e->size - str->offset)
		name = (const char *)e->data + str->offset + noff;
	if (shndx == SHN_UNDEF)
		printf("%*s", width, "");
	else
		printf("%0*" PRIx64, width, val);
	printf(" %c %s\n", sym_char(e, shndx, info), name);
}

/**
 * nm_print - print the symbol table (.symtab) as "nm -p" does
 * @e: loaded ELF file
 *
 * Return: nothing; the caller checks for a symbol table beforehand
 */
void nm_print(const elf_t *e)
{
	const sec_t *st = NULL, *str;
	size_t i, p, esz;

	for (i = 0; i < e->nsec && !st; i++)
		if (e->sec[i].type == SHT_SYMTAB)
			st = &e->sec[i];
	if (!st)
		return;
	esz = e->is64 ? sizeof(Elf64_Sym) : sizeof(Elf32_Sym);
	str = st->link < e->nsec ? &e->sec[st->link] : NULL;
	if (st->entsize >= esz)
		esz = st->entsize;
	if (st->offset > e->size || st->size > e->size - st->offset)
		return;
	for (p = 1; p * esz + esz <= st->size; p++)
		nm_one(e, st->offset + p * esz, str);
}
