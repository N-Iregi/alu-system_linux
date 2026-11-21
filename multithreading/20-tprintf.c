#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include "multithreading.h"

static pthread_mutex_t print_lock;

/**
 * lock_init - initializes the mutex before main
 */
__attribute__((constructor)) static void lock_init(void)
{
	pthread_mutex_init(&print_lock, NULL);
}

/**
 * lock_destroy - destroys the mutex after main
 */
__attribute__((destructor)) static void lock_destroy(void)
{
	pthread_mutex_destroy(&print_lock);
}

/**
 * tprintf - thread-safe printf preceded by the calling thread ID
 * @format: format string
 *
 * Return: number of characters printed
 */
int tprintf(char const *format, ...)
{
	va_list ap;
	int n;

	pthread_mutex_lock(&print_lock);
	n = printf("[%lu] ", (unsigned long)pthread_self());
	va_start(ap, format);
	n += vprintf(format, ap);
	va_end(ap);
	pthread_mutex_unlock(&print_lock);
	return (n);
}
