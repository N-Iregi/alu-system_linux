#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "todo_api.h"

/**
 * content_length - finds the Content-Length value in a raw request
 * @buf: raw request
 *
 * Return: the value, or 0 if absent
 */
size_t content_length(const char *buf)
{
	const char *p = strstr(buf, "\r\n");

	while (p && strncmp(p, "\r\n\r\n", 4) != 0)
	{
		p += 2;
		if (strncasecmp(p, "Content-Length:", 15) == 0)
			return (strtoul(p + 15, NULL, 10));
		p = strstr(p, "\r\n");
	}
	return (0);
}

/**
 * parse_pairs - splits "k1=v1&k2=v2" in place
 * @s: string to split (modified)
 * @arr: destination array
 * @n: number of pairs stored
 */
static void parse_pairs(char *s, pair_t *arr, size_t *n)
{
	char *amp, *eq;

	while (s && *s && *n < MAX_PAIRS)
	{
		amp = strchr(s, '&');
		if (amp)
			*amp++ = '\0';
		eq = strchr(s, '=');
		if (eq)
			*eq++ = '\0';
		arr[*n].key = s;
		arr[*n].value = eq ? eq : "";
		(*n)++;
		s = amp;
	}
}

/**
 * parse_headers - parses header lines and locates the body
 * @p: start of the first header line
 * @req: request to fill
 */
static void parse_headers(char *p, request_t *req)
{
	char *eol, *colon;

	while ((eol = strstr(p, "\r\n")) != NULL && eol != p)
	{
		*eol = '\0';
		colon = strchr(p, ':');
		if (colon && req->nb_headers < MAX_PAIRS)
		{
			*colon++ = '\0';
			while (*colon == ' ')
				colon++;
			req->headers[req->nb_headers].key = p;
			req->headers[req->nb_headers].value = colon;
			req->nb_headers++;
		}
		p = eol + 2;
	}
	if (eol)
		req->body_raw = eol + 2;
}

/**
 * parse_request - parses a raw HTTP request in place
 * @buf: raw request (modified)
 * @req: request to fill
 *
 * Return: 1 on success, 0 if malformed
 */
int parse_request(char *buf, request_t *req)
{
	char *eol, *sp, *q;

	memset(req, 0, sizeof(*req));
	eol = strstr(buf, "\r\n");
	sp = strchr(buf, ' ');
	if (!eol || !sp || sp > eol)
		return (0);
	*eol = '\0';
	*sp++ = '\0';
	req->method = buf;
	req->path = sp;
	sp = strchr(sp, ' ');
	if (!sp)
		return (0);
	*sp = '\0';
	req->version = sp + 1;
	q = strchr(req->path, '?');
	if (q)
	{
		*q++ = '\0';
		parse_pairs(q, req->queries, &req->nb_queries);
	}
	parse_headers(eol + 2, req);
	if (req->body_raw && *req->body_raw)
		parse_pairs(req->body_raw, req->body, &req->nb_body);
	return (1);
}

/**
 * find_pair - looks a key up in a pair array
 * @arr: array
 * @n: array size
 * @key: key to find
 * @nocase: 1 for a case-insensitive match
 *
 * Return: the value, or NULL if not found
 */
const char *find_pair(const pair_t *arr, size_t n, const char *key,
		      int nocase)
{
	size_t i;

	for (i = 0; i < n; i++)
		if ((nocase ? strcasecmp(arr[i].key, key) :
		     strcmp(arr[i].key, key)) == 0)
			return (arr[i].value);
	return (NULL);
}
