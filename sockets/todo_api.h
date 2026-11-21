#ifndef TODO_API_H
#define TODO_API_H

#include <stddef.h>

#ifndef LEVEL
#define LEVEL 7
#endif

#define PORT 8080
#define BUF_SIZE 65536
#define MAX_PAIRS 32

/**
 * struct pair_s - a key/value pair (pointers into the request buffer)
 * @key: key
 * @value: value
 */
typedef struct pair_s
{
	char *key;
	char *value;
} pair_t;

/**
 * struct request_s - a parsed HTTP request
 * @method: HTTP method
 * @path: request path (without query string)
 * @version: HTTP version
 * @queries: query pairs
 * @nb_queries: number of query pairs
 * @headers: header pairs
 * @nb_headers: number of headers
 * @body_raw: raw body
 * @body: body parameter pairs
 * @nb_body: number of body parameters
 */
typedef struct request_s
{
	char *method;
	char *path;
	char *version;
	pair_t queries[MAX_PAIRS];
	size_t nb_queries;
	pair_t headers[MAX_PAIRS];
	size_t nb_headers;
	char *body_raw;
	pair_t body[MAX_PAIRS];
	size_t nb_body;
} request_t;

/**
 * struct todo_s - a todo item (singly linked list node)
 * @id: id
 * @title: title
 * @description: description
 * @next: next todo
 */
typedef struct todo_s
{
	size_t id;
	char *title;
	char *description;
	struct todo_s *next;
} todo_t;

size_t content_length(const char *buf);
int parse_request(char *buf, request_t *req);
const char *find_pair(const pair_t *arr, size_t n, const char *key,
		      int nocase);

void print_parsed(const request_t *req);

void reply(int client, const request_t *req, int code, const char *reason,
	   const char *body);
void route(int client, const request_t *req);

todo_t *todo_add(const char *title, const char *desc);
todo_t *todo_find(size_t id);
int todo_delete(size_t id);
todo_t *todo_head(void);

char *todo_to_json(const todo_t *t);
char *todos_to_json(void);

#endif /* TODO_API_H */
