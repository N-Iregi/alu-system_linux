#include <stdio.h>
#include <pthread.h>
#include "multithreading.h"

/**
 * thread_entry - entry point of a new thread, prints a string
 * @arg: address of the string to print
 *
 * Return: never returns, the thread exits
 */
void *thread_entry(void *arg)
{
	printf("%s\n", (char *)arg);
	pthread_exit(NULL);
}
