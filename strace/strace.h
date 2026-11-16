#ifndef STRACE_H
#define STRACE_H

#ifndef LEVEL
#define LEVEL 8
#endif

#define STR_MAX 4096
#define NELEM(a) (sizeof(a) / sizeof((a)[0]))

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/user.h>
#include <sys/wait.h>
#include "syscalls.h"

/**
 * struct flag_s - a flag value and its name
 * @v: flag value
 * @name: flag name
 */
typedef struct flag_s
{
	unsigned long v;
	const char *name;
} flag_t;

const syscall_t *find_syscall(size_t nr);
unsigned long get_arg(struct user_regs_struct *r, size_t i);
void print_entry(pid_t pid, struct user_regs_struct *r);
void print_ret(const syscall_t *sc, struct user_regs_struct *r);
int is_deferred(struct user_regs_struct *r);

void print_scalar(type_t t, unsigned long v);
void print_buf_param(pid_t pid, struct user_regs_struct *r);
void print_param(pid_t pid, const syscall_t *sc, size_t i,
		 struct user_regs_struct *r);

int read_byte(pid_t pid, unsigned long addr, unsigned char *out);
int read_mem(pid_t pid, unsigned long addr, void *buf, size_t len);
void print_char(unsigned char c);
void print_str(pid_t pid, unsigned long addr, long len);

void print_flags(unsigned long v, const flag_t *t, size_t n,
		 const char *zero, int sep);
void print_open(unsigned long v);
int print_mmap(size_t i, unsigned long v);
int print_flags_param(unsigned long nr, size_t i, unsigned long v);

void print_stat(pid_t pid, unsigned long addr, long ret);

#endif /* STRACE_H */
