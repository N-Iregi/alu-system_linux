#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "todo_api.h"

/**
 * send_all - sends a whole buffer
 * @client: client socket
 * @data: data to send
 * @len: data length
 */
static void send_all(int client, const char *data, size_t len)
{
	ssize_t n;
	size_t sent = 0;

	while (sent < len)
	{
		n = send(client, data + sent, len - sent, 0);
		if (n <= 0)
			return;
		sent += (size_t)n;
	}
}

/**
 * reply - sends an HTTP response
 * @client: client socket
 * @req: request being answered (for logging)
 * @code: status code
 * @reason: status text
 * @body: JSON body, or NULL for a response without body
 */
void reply(int client, const request_t *req, int code, const char *reason,
	   const char *body)
{
	char head[256];
	int n;

	if (LEVEL >= 4)
		printf("%s %s -> %d %s\n", req->method, req->path, code,
		       reason);
	if (body)
		n = sprintf(head, "HTTP/1.1 %d %s\r\nContent-Length: %lu\r\n"
			    "Content-Type: application/json\r\n\r\n", code,
			    reason, (unsigned long)strlen(body));
	else
		n = sprintf(head, "HTTP/1.1 %d %s\r\n\r\n", code, reason);
	send_all(client, head, (size_t)n);
	if (body)
		send_all(client, body, strlen(body));
}
