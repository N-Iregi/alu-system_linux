#include <stdlib.h>
#include <string.h>
#include "todo_api.h"

/**
 * parse_id - parses a positive integer
 * @s: string
 * @id: parsed value
 *
 * Return: 1 if valid, 0 otherwise
 */
static int parse_id(const char *s, size_t *id)
{
	char *end;

	if (*s < '0' || *s > '9')
		return (0);
	*id = strtoul(s, &end, 10);
	return (*end == '\0');
}

/**
 * post_todo - POST /todos
 * @client: client socket
 * @req: request
 */
static void post_todo(int client, const request_t *req)
{
	const char *title, *desc;
	todo_t *t;
	char *json = NULL;

	if (!find_pair(req->headers, req->nb_headers, "Content-Length", 1))
	{
		reply(client, req, 411, "Length Required", NULL);
		return;
	}
	title = find_pair(req->body, req->nb_body, "title", 0);
	desc = find_pair(req->body, req->nb_body, "description", 0);
	if (!title || !desc)
	{
		reply(client, req, 422, "Unprocessable Entity", NULL);
		return;
	}
	t = todo_add(title, desc);
	if (t)
		json = todo_to_json(t);
	if (!json)
	{
		reply(client, req, 500, "Internal Server Error", NULL);
		return;
	}
	reply(client, req, 201, "Created", json);
	free(json);
}

/**
 * get_todos - GET /todos and GET /todos?id=N
 * @client: client socket
 * @req: request
 */
static void get_todos(int client, const request_t *req)
{
	const char *idstr = find_pair(req->queries, req->nb_queries, "id", 0);
	size_t id;
	todo_t *t;
	char *json;

	if (LEVEL >= 6 && idstr)
	{
		t = parse_id(idstr, &id) ? todo_find(id) : NULL;
		json = t ? todo_to_json(t) : NULL;
		if (!json)
			reply(client, req, 404, "Not Found", NULL);
		else
			reply(client, req, 200, "OK", json);
		free(json);
		return;
	}
	json = todos_to_json();
	if (!json)
	{
		reply(client, req, 500, "Internal Server Error", NULL);
		return;
	}
	reply(client, req, 200, "OK", json);
	free(json);
}

/**
 * delete_todo - DELETE /todos?id=N
 * @client: client socket
 * @req: request
 */
static void delete_todo(int client, const request_t *req)
{
	const char *idstr = find_pair(req->queries, req->nb_queries, "id", 0);
	size_t id;

	if (idstr && parse_id(idstr, &id) && todo_delete(id))
		reply(client, req, 204, "No Content", NULL);
	else
		reply(client, req, 404, "Not Found", NULL);
}

/**
 * route - dispatches a request according to its method and path
 * @client: client socket
 * @req: request
 */
void route(int client, const request_t *req)
{
	int is_todos = strcmp(req->path, "/todos") == 0;

	if (is_todos && strcmp(req->method, "POST") == 0)
		post_todo(client, req);
	else if (is_todos && LEVEL >= 5 && strcmp(req->method, "GET") == 0)
		get_todos(client, req);
	else if (is_todos && LEVEL >= 7 &&
		 strcmp(req->method, "DELETE") == 0)
		delete_todo(client, req);
	else
		reply(client, req, 404, "Not Found", NULL);
}
