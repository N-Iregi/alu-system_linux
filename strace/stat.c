#include "strace.h"

/**
 * mode_name - gives the S_IF* name of a file type
 * @m: st_mode
 * Return: the name
 */
static const char *mode_name(unsigned int m)
{
	switch (m & S_IFMT)
	{
	case S_IFREG:
		return ("S_IFREG");
	case S_IFDIR:
		return ("S_IFDIR");
	case S_IFLNK:
		return ("S_IFLNK");
	case S_IFBLK:
		return ("S_IFBLK");
	case S_IFCHR:
		return ("S_IFCHR");
	case S_IFIFO:
		return ("S_IFIFO");
	case S_IFSOCK:
		return ("S_IFSOCK");
	default:
		return ("S_IF?");
	}
}

/**
 * print_stat - prints st_mode and st_size of a struct stat
 * @pid: child pid
 * @addr: address of the struct in the child
 * @ret: syscall return value (struct is only valid on success)
 */
void print_stat(pid_t pid, unsigned long addr, long ret)
{
	struct stat st;

	if (ret < 0 || read_mem(pid, addr, &st, sizeof(st)) < 0)
	{
		printf("%#lx", addr);
		return;
	}
	printf("{st_mode=%s|0%o, st_size=%ld, ...}", mode_name(st.st_mode),
	       (unsigned int)(st.st_mode & 07777), (long)st.st_size);
}
