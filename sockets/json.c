#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "todo_api.h"

#define JSON_OVERHEAD 64

/**
 * json_write - writes one todo as JSON
 * @dst: destination (large enough)
 * @t: todo
 *
 * Return: number of characters written
 */
static int json_write(char *dst, const todo_t *t)
{
	return (sprintf(dst,
			"{\"id\":%lu,\"title\":\"%s\",\"description\":\"%s\"}",
			(unsigned long)t->id, t->title, t->description));
}

/**
 * todo_to_json - JSON representation of one todo
 * @t: todo
 *
 * Return: malloc'd string, or NULL on failure
 */
char *todo_to_json(const todo_t *t)
{
	char *s = malloc(strlen(t->title) + strlen(t->description) +
			 JSON_OVERHEAD);

	if (s)
		json_write(s, t);
	return (s);
}

/**
 * todos_to_json - JSON representation of the whole list
 *
 * Return: malloc'd string, or NULL on failure
 */
char *todos_to_json(void)
{
	todo_t *t;
	size_t size = 3;
	char *s, *p;

	for (t = todo_head(); t; t = t->next)
		size += strlen(t->title) + strlen(t->description) +
			JSON_OVERHEAD;
	s = malloc(size);
	if (!s)
		return (NULL);
	p = s;
	*p++ = '[';
	for (t = todo_head(); t; t = t->next)
	{
		if (t != todo_head())
			*p++ = ',';
		p += json_write(p, t);
	}
	*p++ = ']';
	*p = '\0';
	return (s);
}
