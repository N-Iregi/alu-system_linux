#include "strace.h"

/**
 * find_syscall - looks up a syscall by number
 * @nr: syscall number
 * Return: table entry or NULL
 */
const syscall_t *find_syscall(size_t nr)
{
	size_t i;

	for (i = 0; i < NELEM(syscalls_64_g); i++)
		if (syscalls_64_g[i].nr == nr)
			return (&syscalls_64_g[i]);
	return (NULL);
}

/**
 * get_arg - gets the i-th syscall argument from the registers
 * @r: registers
 * @i: argument index (0 to 5)
 * Return: the argument
 */
unsigned long get_arg(struct user_regs_struct *r, size_t i)
{
	unsigned long a[6];

	a[0] = r->rdi;
	a[1] = r->rsi;
	a[2] = r->rdx;
	a[3] = r->r10;
	a[4] = r->r8;
	a[5] = r->r9;
	return (a[i]);
}

/**
 * is_deferred - tells if the line must be printed at syscall exit
 * @r: registers
 * Return: 1 for read and stat-like calls (buffers filled by the kernel)
 */
int is_deferred(struct user_regs_struct *r)
{
	const syscall_t *sc = find_syscall(r->orig_rax);
	size_t i;

	if (!sc)
		return (0);
	if (LEVEL >= 7 && sc->nr == 0)
		return (1);
	if (LEVEL < 8)
		return (0);
	for (i = 0; i < sc->nb_params; i++)
		if (sc->params[i] == STRUCT_STAT_P)
			return (1);
	return (0);
}

/**
 * print_entry - prints the start of a syscall line
 * @pid: child pid
 * @r: registers
 */
void print_entry(pid_t pid, struct user_regs_struct *r)
{
	const syscall_t *sc = find_syscall(r->orig_rax);
	size_t i;

	if (LEVEL == 0)
	{
		printf("%llu\n", r->orig_rax);
		fflush(stdout);
		return;
	}
	if (!sc)
		printf("syscall_%llu", r->orig_rax);
	else
		printf("%s", sc->name);
	if (LEVEL >= 3)
		printf("(");
	for (i = 0; sc && LEVEL >= 3 && i < sc->nb_params; i++)
	{
		if (sc->params[i] == VOID)
			break;
		if (i)
			printf(", ");
		if (sc->params[i] == VARARGS)
		{
			printf("...");
			break;
		}
		print_param(pid, sc, i, r);
	}
	fflush(stdout);
}

/**
 * print_ret - prints the end of a syscall line
 * @sc: syscall entry (may be NULL)
 * @r: registers at syscall exit
 */
void print_ret(const syscall_t *sc, struct user_regs_struct *r)
{
	long v = (long)r->rax;
	type_t t = (sc && LEVEL >= 5) ? sc->ret : VOID_P;

	if (LEVEL == 0)
		return;
	if (LEVEL == 1)
		printf("\n");
	else
		printf("%s", LEVEL == 2 ? " = " : ") = ");
	if (LEVEL == 1)
	{
		fflush(stdout);
		return;
	}
	switch (t)
	{
	case INT:
	case PID_T:
		printf("%d\n", (int)v);
		break;
	case LONG:
	case SSIZE_T:
		printf("%ld\n", v);
		break;
	case UNSIGNED_INT:
	case UINT32_T:
		printf("%u\n", (unsigned int)v);
		break;
	case SIZE_T:
	case U64:
	case UNSIGNED_LONG:
		printf("%lu\n", (unsigned long)v);
		break;
	default:
		printf("%#lx\n", (unsigned long)v);
	}
	fflush(stdout);
}
