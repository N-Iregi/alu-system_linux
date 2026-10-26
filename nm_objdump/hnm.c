#include <stdio.h>
#include "elf_tools.h"

/**
 * has_symtab - tell whether a file has a .symtab
 * @e: loaded ELF file
 *
 * Return: 1 if it has one, 0 otherwise
 */
static int has_symtab(const elf_t *e)
{
	size_t i;

	for (i = 0; i < e->nsec; i++)
		if (e->sec[i].type == SHT_SYMTAB && e->sec[i].size > 0)
			return (1);
	return (0);
}

/**
 * nm_file - process one file
 * @prog: name of the running program
 * @path: file to process
 * @multi: 1 if several files are given, to print a header
 *
 * Return: 0 on success, 1 on failure
 */
static int nm_file(const char *prog, const char *path, int multi)
{
	elf_t e;
	int rc = elf_open(&e, path);

	if (rc != ELF_OK)
	{
		elf_error(prog, path, rc);
		return (1);
	}
	if (multi)
		printf("\n%s:\n", path);
	if (!has_symtab(&e))
	{
		fprintf(stderr, "%s: %s: no symbols\n", prog, path);
		elf_free(&e);
		return (1);
	}
	nm_print(&e);
	elf_free(&e);
	return (0);
}

/**
 * main - reproduce "nm -p"
 * @ac: argument count
 * @av: argument vector
 *
 * Return: 0 if every file was processed, 1 otherwise
 */
int main(int ac, char **av)
{
	int i, ret = 0;

	if (ac < 2)
		return (nm_file(av[0], "a.out", 0));
	for (i = 1; i < ac; i++)
		ret |= nm_file(av[0], av[i], ac > 2);
	return (ret);
}
