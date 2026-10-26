#include <stdlib.h>
#include <string.h>
#include "elf_tools.h"

/**
 * parse_header - fill the elf_t fields coming from the ELF header
 * @e: loaded ELF file
 *
 * Return: 0 on success, -1 if this is not a valid ELF file
 */
static int parse_header(elf_t *e)
{
	if (e->size < EI_NIDENT || memcmp(e->data, ELFMAG, SELFMAG) != 0)
		return (-1);
	if (e->data[EI_CLASS] != ELFCLASS32 && e->data[EI_CLASS] != ELFCLASS64)
		return (-1);
	if (e->data[EI_DATA] != ELFDATA2LSB && e->data[EI_DATA] != ELFDATA2MSB)
		return (-1);
	e->is64 = e->data[EI_CLASS] == ELFCLASS64;
	e->big = e->data[EI_DATA] == ELFDATA2MSB;
	if (e->size < (e->is64 ? sizeof(Elf64_Ehdr) : sizeof(Elf32_Ehdr)))
		return (-1);
	e->type = RD(e, Ehdr, 0, e_type);
	e->machine = RD(e, Ehdr, 0, e_machine);
	e->entry = RD(e, Ehdr, 0, e_entry);
	e->shoff = RD(e, Ehdr, 0, e_shoff);
	e->phnum = RD(e, Ehdr, 0, e_phnum);
	e->shentsize = RD(e, Ehdr, 0, e_shentsize);
	e->nsec = RD(e, Ehdr, 0, e_shnum);
	e->shstrndx = RD(e, Ehdr, 0, e_shstrndx);
	return (0);
}

/**
 * parse_sec - normalize one section header
 * @e: loaded ELF file
 * @i: index of the section
 */
static void parse_sec(elf_t *e, size_t i)
{
	size_t p = e->shoff + i * e->shentsize;
	sec_t *s = &e->sec[i];

	s->name_off = RD(e, Shdr, p, sh_name);
	s->type = RD(e, Shdr, p, sh_type);
	s->flags = RD(e, Shdr, p, sh_flags);
	s->addr = RD(e, Shdr, p, sh_addr);
	s->offset = RD(e, Shdr, p, sh_offset);
	s->size = RD(e, Shdr, p, sh_size);
	s->link = RD(e, Shdr, p, sh_link);
	s->info = RD(e, Shdr, p, sh_info);
	s->entsize = RD(e, Shdr, p, sh_entsize);
	s->name = "";
}

/**
 * set_names - resolve the name of every section
 * @e: loaded ELF file
 */
static void set_names(elf_t *e)
{
	const sec_t *str;
	size_t i, lim = 0;

	if (e->shstrndx >= e->nsec)
		return;
	str = &e->sec[e->shstrndx];
	if (str->offset >= e->size)
		return;
	lim = e->size - str->offset;
	if (str->size < lim)
		lim = str->size;
	for (i = 0; i < e->nsec; i++)
	{
		if (e->sec[i].name_off < lim)
			e->sec[i].name = (const char *)e->data + str->offset +
				e->sec[i].name_off;
	}
}

/**
 * load_sections - read the section header table
 * @e: loaded ELF file
 *
 * Return: 0 on success, -1 if the table is invalid
 */
static int load_sections(elf_t *e)
{
	uint64_t n = e->nsec;
	size_t i, min = e->is64 ? sizeof(Elf64_Shdr) : sizeof(Elf32_Shdr);

	e->nsec = 0;
	if (e->shoff == 0)
		return (0);
	if (e->shentsize < min || e->shoff > e->size ||
	    e->size - e->shoff < min)
		return (-1);
	if (n == 0)
		n = RD(e, Shdr, e->shoff, sh_size);
	if (e->shstrndx == SHN_XINDEX)
		e->shstrndx = RD(e, Shdr, e->shoff, sh_link);
	if (n == 0)
		return (0);
	if (n > (e->size - e->shoff) / e->shentsize)
		return (-1);
	e->sec = calloc(n, sizeof(sec_t));
	if (!e->sec)
		return (-1);
	e->nsec = n;
	for (i = 0; i < e->nsec; i++)
		parse_sec(e, i);
	set_names(e);
	return (0);
}

/**
 * elf_open - load and parse an ELF file
 * @e: structure to fill
 * @path: path of the file
 *
 * Return: ELF_OK, ELF_NOFILE (see errno) or ELF_BADFMT
 */
int elf_open(elf_t *e, const char *path)
{
	memset(e, 0, sizeof(*e));
	e->data = read_file(path, &e->size);
	if (!e->data)
		return (ELF_NOFILE);
	if (parse_header(e) != 0 || load_sections(e) != 0)
	{
		elf_free(e);
		return (ELF_BADFMT);
	}
	return (ELF_OK);
}
