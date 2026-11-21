#include <stdlib.h>
#include "multithreading.h"

/**
 * add_factor - appends a factor to the list
 * @list: list of factors
 * @f: factor
 */
static void add_factor(list_t *list, unsigned long f)
{
	unsigned long *p = malloc(sizeof(*p));

	if (!p)
		return;
	*p = f;
	list_add(list, p);
}

/**
 * prime_factors - factorizes a number into prime factors
 * @s: string representation of the number
 *
 * Return: list of prime factors (unsigned long), or NULL on failure
 */
list_t *prime_factors(char const *s)
{
	list_t *list;
	unsigned long n, d, step = 2;

	list = malloc(sizeof(*list));
	if (!list)
		return (NULL);
	if (!list_init(list))
	{
		free(list);
		return (NULL);
	}
	n = strtoul(s, NULL, 10);
	for (; n > 1 && n % 2 == 0; n /= 2)
		add_factor(list, 2);
	for (; n > 1 && n % 3 == 0; n /= 3)
		add_factor(list, 3);
	for (d = 5; d <= 4294967295UL && d * d <= n; d += step)
	{
		step = 6 - step;
		while (n % d == 0)
		{
			add_factor(list, d);
			n /= d;
		}
	}
	if (n > 1)
		add_factor(list, n);
	return (list);
}
