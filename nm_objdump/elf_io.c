#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elf_tools.h"

/**
 * rd - read an unsigned integer from the file, honoring its endianness
 * @e: loaded ELF file
 * @off: offset in the file
 * @n: number of bytes of the integer (1, 2, 4 or 8)
 *
 * Return: the value read
 */
uint64_t rd(const elf_t *e, size_t off, int n)
{
	uint64_t v = 0;
	int i;

	for (i = 0; i < n; i++)
	{
		if (e->big)
			v = (v << 8) | e->data[off + i];
		else
			v |= (uint64_t)e->data[off + i] << (8 * i);
	}
	return (v);
}

/**
 * read_file - load a whole file in memory
 * @path: path of the file
 * @size: where to store the file size
 *
 * Return: buffer (size + 1 bytes, last one is NUL), or NULL on failure
 */
unsigned char *read_file(const char *path, size_t *size)
{
	FILE *fp;
	unsigned char *buf;
	long len;

	fp = fopen(path, "rb");
	if (!fp)
		return (NULL);
	if (fseek(fp, 0, SEEK_END) != 0)
	{
		fclose(fp);
		return (NULL);
	}
	len = ftell(fp);
	rewind(fp);
	buf = len < 0 ? NULL : malloc((size_t)len + 1);
	if (!buf || fread(buf, 1, (size_t)len, fp) != (size_t)len)
	{
		free(buf);
		fclose(fp);
		return (NULL);
	}
	fclose(fp);
	buf[len] = '\0';
	*size = (size_t)len;
	return (buf);
}

/**
 * elf_free - release the memory held by a loaded ELF file
 * @e: loaded ELF file
 */
void elf_free(elf_t *e)
{
	free(e->data);
	free(e->sec);
	memset(e, 0, sizeof(*e));
}

/**
 * elf_error - print the error matching an elf_open() failure
 * @prog: name of the running program
 * @path: file that could not be processed
 * @rc: return value of elf_open()
 */
void elf_error(const char *prog, const char *path, int rc)
{
	if (rc == ELF_BADFMT)
		fprintf(stderr, "%s: %s: file format not recognized\n",
			prog, path);
	else if (errno == ENOENT)
		fprintf(stderr, "%s: '%s': No such file\n", prog, path);
	else
		fprintf(stderr, "%s: %s: %s\n", prog, path, strerror(errno));
}
