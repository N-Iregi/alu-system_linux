#include "strace.h"

/**
 * read_byte - reads one byte from the child (aligned word access)
 * @pid: child pid
 * @addr: address in the child
 * @out: where to store the byte
 * Return: 0 on success, -1 on error
 */
int read_byte(pid_t pid, unsigned long addr, unsigned char *out)
{
	long w;

	errno = 0;
	w = ptrace(PTRACE_PEEKDATA, pid, addr & ~7UL, NULL);
	if (w == -1 && errno)
		return (-1);
	*out = (unsigned char)(w >> ((addr & 7) * 8));
	return (0);
}

/**
 * read_mem - reads a block of memory from the child
 * @pid: child pid
 * @addr: address in the child
 * @buf: destination
 * @len: number of bytes
 * Return: 0 on success, -1 on error
 */
int read_mem(pid_t pid, unsigned long addr, void *buf, size_t len)
{
	size_t i, n;
	long w;

	for (i = 0; i < len; i += sizeof(w))
	{
		errno = 0;
		w = ptrace(PTRACE_PEEKDATA, pid, addr + i, NULL);
		if (w == -1 && errno)
			return (-1);
		n = (len - i < sizeof(w)) ? len - i : sizeof(w);
		memcpy((char *)buf + i, &w, n);
	}
	return (0);
}

/**
 * print_char - prints one character, escaped if needed
 * @c: the character
 */
void print_char(unsigned char c)
{
	if (c >= 7 && c <= 13)
		printf("\\%c", "abtnvfr"[c - 7]);
	else if (c == '"' || c == '\\')
		printf("\\%c", c);
	else if (c >= 32 && c < 127)
		printf("%c", c);
	else
		printf("\\%o", c);
}

/**
 * print_str - prints a string or buffer from the child
 * @pid: child pid
 * @addr: address in the child
 * @len: length, or -1 for a NUL-terminated string
 */
void print_str(pid_t pid, unsigned long addr, long len)
{
	unsigned char c;
	long i;
	int dots = 0;

	if (!addr)
	{
		printf("NULL");
		return;
	}
	if (len != 0 && read_byte(pid, addr, &c) < 0)
	{
		printf("%#lx", addr);
		return;
	}
	printf("\"");
	for (i = 0; len < 0 ? i < STR_MAX : i < len; i++)
	{
		if (len > 32 && i == 32)
		{
			dots = 1;
			break;
		}
		if (read_byte(pid, addr + i, &c) < 0 || (len < 0 && !c))
			break;
		print_char(c);
	}
	printf("\"%s", dots ? "..." : "");
}
