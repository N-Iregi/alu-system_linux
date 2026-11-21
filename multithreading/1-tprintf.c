#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include "multithreading.h"

/**
 * tprintf - printf preceded by the calling thread ID
 * @format: format string
 *
 * Return: number of characters printed
 */
int tprintf(char const *format, ...)
{
	va_list ap;
	int n;

	n = printf("[%lu] ", (unsigned long)pthread_self());
	va_start(ap, format);
	n += vprintf(format, ap);
	va_end(ap);
	return (n);
}
