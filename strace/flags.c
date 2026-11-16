#include "strace.h"

/**
 * print_flags - prints the names of the bits set in v, joined by '|'
 * @v: value (low 32 bits used)
 * @t: flag table
 * @n: table size
 * @zero: text to print when v is 0 (or NULL for nothing)
 * @sep: 1 if something was already printed (so start with '|')
 */
void print_flags(unsigned long v, const flag_t *t, size_t n,
		 const char *zero, int sep)
{
	size_t i;

	v &= 0xffffffffUL;
	if (!v && zero)
		printf("%s", zero);
	for (i = 0; i < n; i++)
	{
		if (t[i].v && (v & t[i].v) == t[i].v)
		{
			printf("%s%s", sep ? "|" : "", t[i].name);
			sep = 1;
			v &= ~t[i].v;
		}
	}
	if (v)
		printf("%s%#lx", sep ? "|" : "", v);
}

/**
 * print_open - prints the flags argument of open
 * @v: flags
 */
void print_open(unsigned long v)
{
	static const char * const mode[] = {
		"O_RDONLY", "O_WRONLY", "O_RDWR", "O_ACCMODE"
	};
	static const flag_t fl[] = {
		{0100, "O_CREAT"}, {0200, "O_EXCL"}, {0400, "O_NOCTTY"},
		{01000, "O_TRUNC"}, {02000, "O_APPEND"},
		{04000, "O_NONBLOCK"}, {010000, "O_DSYNC"},
		{020000, "O_ASYNC"}, {040000, "O_DIRECT"},
		{0100000, "O_LARGEFILE"}, {0200000, "O_DIRECTORY"},
		{0400000, "O_NOFOLLOW"}, {01000000, "O_NOATIME"},
		{02000000, "O_CLOEXEC"}, {010000000, "O_PATH"}
	};

	printf("%s", mode[v & 3]);
	print_flags(v & ~3UL, fl, NELEM(fl), NULL, 1);
}

/**
 * print_mmap - prints the addr, prot and flags arguments of mmap
 * @i: parameter index
 * @v: raw value
 * Return: 1 if handled, 0 otherwise
 */
int print_mmap(size_t i, unsigned long v)
{
	static const flag_t prot[] = {
		{1, "PROT_READ"}, {2, "PROT_WRITE"}, {4, "PROT_EXEC"}
	};
	static const flag_t map[] = {
		{0x01, "MAP_SHARED"}, {0x02, "MAP_PRIVATE"},
		{0x10, "MAP_FIXED"}, {0x20, "MAP_ANONYMOUS"},
		{0x100, "MAP_GROWSDOWN"}, {0x800, "MAP_DENYWRITE"},
		{0x1000, "MAP_EXECUTABLE"}, {0x2000, "MAP_LOCKED"},
		{0x4000, "MAP_NORESERVE"}, {0x8000, "MAP_POPULATE"},
		{0x10000, "MAP_NONBLOCK"}, {0x20000, "MAP_STACK"},
		{0x40000, "MAP_HUGETLB"}
	};

	if (i == 0 && !v)
		printf("NULL");
	else if (i == 2)
		print_flags(v, prot, NELEM(prot), "PROT_NONE", 0);
	else if (i == 3)
		print_flags(v, map, NELEM(map), "0", 0);
	else
		return (0);
	return (1);
}

/**
 * print_flags_param - prints macro-interpreted parameters
 * @nr: syscall number
 * @i: parameter index
 * @v: raw value
 * Return: 1 if the parameter was printed here, 0 otherwise
 */
int print_flags_param(unsigned long nr, size_t i, unsigned long v)
{
	static const flag_t acc[] = {
		{4, "R_OK"}, {2, "W_OK"}, {1, "X_OK"}
	};

	if (nr == 2 && i == 1)
		print_open(v);
	else if (nr == 21 && i == 1)
		print_flags(v, acc, NELEM(acc), "F_OK", 0);
	else if (nr == 9)
		return (print_mmap(i, v));
	else
		return (0);
	return (1);
}
