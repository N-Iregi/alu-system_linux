#include "strace.h"

/**
 * next_stop - resumes the child until the next syscall stop
 * @pid: child pid
 * Return: 1 if stopped at a syscall, 0 if the child is gone
 */
int next_stop(pid_t pid)
{
	int status, sig = 0;

	while (1)
	{
		ptrace(PTRACE_SYSCALL, pid, NULL, (void *)(long)sig);
		waitpid(pid, &status, 0);
		if (!WIFSTOPPED(status))
			return (0);
		if (WSTOPSIG(status) == (SIGTRAP | 0x80))
			return (1);
		sig = WSTOPSIG(status);
	}
}

/**
 * print_first - prints the execve line (its args can't be read later)
 * @r: registers at the post-exec stop
 * @argv: traced command and its arguments
 * @envp: environment
 */
void print_first(struct user_regs_struct *r, char **argv, char **envp)
{
	int i, n = 0;

	if (LEVEL < 4)
		print_entry(0, r);
	else
	{
		printf("execve(\"%s\", [", argv[0]);
		for (i = 0; argv[i]; i++)
			printf("%s\"%s\"", i ? ", " : "", argv[i]);
		while (envp[n])
			n++;
		printf("], [/* %d vars */]", n);
	}
	print_ret(find_syscall(r->orig_rax), r);
}

/**
 * print_unknown_ret - prints the end of the last syscall (it never returns)
 */
void print_unknown_ret(void)
{
	if (LEVEL == 1)
		printf("\n");
	else if (LEVEL == 2)
		printf(" = ?\n");
	else if (LEVEL >= 3)
		printf(") = ?\n");
	fflush(stdout);
}

/**
 * trace - main tracing loop
 * @pid: child pid
 * @argv: traced command and its arguments
 * @envp: environment
 */
void trace(pid_t pid, char **argv, char **envp)
{
	int status;
	struct user_regs_struct r;

	waitpid(pid, &status, 0);
	ptrace(PTRACE_SETOPTIONS, pid, NULL,
	       (void *)(long)PTRACE_O_TRACESYSGOOD);
	ptrace(PTRACE_GETREGS, pid, NULL, &r);
	print_first(&r, argv, envp);
	while (next_stop(pid))
	{
		ptrace(PTRACE_GETREGS, pid, NULL, &r);
		if (!is_deferred(&r))
			print_entry(pid, &r);
		if (!next_stop(pid))
		{
			print_unknown_ret();
			break;
		}
		ptrace(PTRACE_GETREGS, pid, NULL, &r);
		if (is_deferred(&r))
			print_entry(pid, &r);
		print_ret(find_syscall(r.orig_rax), &r);
	}
}

/**
 * main - executes and traces a command
 * @argc: argument count
 * @argv: argument vector
 * @envp: environment
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv, char **envp)
{
	pid_t pid;

	if (argc < 2)
		return (1);
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execve(argv[1], argv + 1, envp);
		return (1);
	}
	trace(pid, argv + 1, envp);
	return (0);
}
