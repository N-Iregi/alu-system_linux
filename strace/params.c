#include "strace.h"

/**
 * print_scalar - prints a non-pointer parameter according to its type
 * @t: parameter type
 * @v: raw register value
 */
void print_scalar(type_t t, unsigned long v)
{
	if (LEVEL < 5)
		t = VOID_P;
	switch (t)
	{
	case INT:
	case PID_T:
		printf("%d", (int)v);
		break;
	case LONG:
	case SSIZE_T:
		printf("%ld", (long)v);
		break;
	case UNSIGNED_INT:
	case UINT32_T:
		printf("%u", (unsigned int)v);
		break;
	case SIZE_T:
	case U64:
	case UNSIGNED_LONG:
		printf("%lu", v);
		break;
	default:
		printf("%#lx", v);
	}
}

/**
 * print_buf_param - prints the buffer argument of read or write
 * @pid: child pid
 * @r: registers
 */
void print_buf_param(pid_t pid, struct user_regs_struct *r)
{
	unsigned long addr = get_arg(r, 1);
	long len = (long)get_arg(r, 2);

	if (r->orig_rax == 0)
	{
		if ((long)r->rax < 0)
		{
			printf("%#lx", addr);
			return;
		}
		if ((long)r->rax < len)
			len = (long)r->rax;
	}
	print_str(pid, addr, len);
}

/**
 * print_param - prints one parameter
 * @pid: child pid
 * @sc: syscall entry
 * @i: parameter index
 * @r: registers
 */
void print_param(pid_t pid, const syscall_t *sc, size_t i,
		 struct user_regs_struct *r)
{
	type_t t = sc->params[i];
	unsigned long v = get_arg(r, i);

	if (LEVEL >= 6 && print_flags_param(sc->nr, i, v))
		return;
	if (LEVEL >= 4 && t == CHAR_P)
		print_str(pid, v, -1);
	else if (LEVEL >= 7 && sc->nr <= 1 && i == 1)
		print_buf_param(pid, r);
	else if (LEVEL >= 8 && t == STRUCT_STAT_P)
		print_stat(pid, v, (long)r->rax);
	else
		print_scalar(t, v);
}
