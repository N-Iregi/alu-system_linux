#include <stdlib.h>
#include <string.h>
#include "todo_api.h"

static todo_t *head;
static todo_t *tail;
static size_t next_id;

/**
 * dup_str - duplicates a string
 * @s: string
 *
 * Return: the copy, or NULL on failure
 */
static char *dup_str(const char *s)
{
	char *d = malloc(strlen(s) + 1);

	if (d)
		strcpy(d, s);
	return (d);
}

/**
 * todo_head - gets the first todo
 *
 * Return: head of the list
 */
todo_t *todo_head(void)
{
	return (head);
}

/**
 * todo_add - appends a todo (ids are never reused)
 * @title: title
 * @desc: description
 *
 * Return: the new todo, or NULL on failure
 */
todo_t *todo_add(const char *title, const char *desc)
{
	todo_t *t = malloc(sizeof(*t));

	if (!t)
		return (NULL);
	t->title = dup_str(title);
	t->description = dup_str(desc);
	if (!t->title || !t->description)
	{
		free(t->title);
		free(t->description);
		free(t);
		return (NULL);
	}
	t->id = next_id++;
	t->next = NULL;
	if (tail)
		tail->next = t;
	else
		head = t;
	tail = t;
	return (t);
}

/**
 * todo_find - finds a todo by id
 * @id: id
 *
 * Return: the todo, or NULL
 */
todo_t *todo_find(size_t id)
{
	todo_t *t;

	for (t = head; t; t = t->next)
		if (t->id == id)
			return (t);
	return (NULL);
}

/**
 * todo_delete - deletes a todo by id
 * @id: id
 *
 * Return: 1 if deleted, 0 if not found
 */
int todo_delete(size_t id)
{
	todo_t *cur = head, *prev = NULL;

	while (cur && cur->id != id)
	{
		prev = cur;
		cur = cur->next;
	}
	if (!cur)
		return (0);
	if (prev)
		prev->next = cur->next;
	else
		head = cur->next;
	if (cur == tail)
		tail = prev;
	free(cur->title);
	free(cur->description);
	free(cur);
	return (1);
}
