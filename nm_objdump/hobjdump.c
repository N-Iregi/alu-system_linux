#include <stdio.h>
#include "elf_tools.h"

/**
 * objdump_file - process one file
 * @prog: name of the running program
 * @path: file to process
 *
 * Return: 0 on success, 1 on failure
 */
static int objdump_file(const char *prog, const char *path)
{
	elf_t e;
	int rc = elf_open(&e, path);

	if (rc != ELF_OK)
	{
		elf_error(prog, path, rc);
		return (1);
	}
	print_header(&e, path);
	dump_sections(&e);
	elf_free(&e);
	return (0);
}

/**
 * main - reproduce "objdump -sf"
 * @ac: argument count
 * @av: argument vector
 *
 * Return: 0 if every file was processed, 1 otherwise
 */
int main(int ac, char **av)
{
	int i, ret = 0;

	if (ac < 2)
		return (objdump_file(av[0], "a.out"));
	for (i = 1; i < ac; i++)
		ret |= objdump_file(av[0], av[i]);
	return (ret);
}
