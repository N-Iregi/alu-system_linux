#include <stdio.h>
#include "todo_api.h"

/**
 * print_pairs - prints key/value pairs
 * @label: label to print before each pair
 * @arr: pairs
 * @n: number of pairs
 */
static void print_pairs(const char *label, const pair_t *arr, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		printf("%s: \"%s\" -> \"%s\"\n", label, arr[i].key,
		       arr[i].value);
}

/**
 * print_parsed - prints the parsed request, depending on LEVEL
 * @req: parsed request
 */
void print_parsed(const request_t *req)
{
	if (LEVEL == 0)
		printf("Method: %s\nPath: %s\nVersion: %s\n", req->method,
		       req->path, req->version);
	else if (LEVEL == 1)
	{
		printf("Path: %s\n", req->path);
		print_pairs("Query", req->queries, req->nb_queries);
	}
	else if (LEVEL == 2)
		print_pairs("Header", req->headers, req->nb_headers);
	else if (LEVEL == 3)
	{
		printf("Path: %s\n", req->path);
		print_pairs("Body param", req->body, req->nb_body);
	}
}
