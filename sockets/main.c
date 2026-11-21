#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "todo_api.h"

/**
 * setup_server - creates the listening socket on PORT
 *
 * Return: the socket (exits on error)
 */
static int setup_server(void)
{
	int sock, opt = 1;
	struct sockaddr_in addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);
	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0 ||
	    listen(sock, 10) < 0)
	{
		perror("bind/listen");
		exit(EXIT_FAILURE);
	}
	printf("Server listening on port %d\n", PORT);
	fflush(stdout);
	return (sock);
}

/**
 * read_request - reads headers, then as much body as Content-Length says
 * @client: client socket
 * @buf: destination buffer (BUF_SIZE bytes)
 *
 * Return: 0 on success, -1 if nothing was received
 */
static int read_request(int client, char *buf)
{
	ssize_t n;
	size_t len = 0;
	char *end;

	while (len < BUF_SIZE - 1)
	{
		n = recv(client, buf + len, BUF_SIZE - 1 - len, 0);
		if (n <= 0)
			break;
		len += (size_t)n;
		buf[len] = '\0';
		end = strstr(buf, "\r\n\r\n");
		if (end && len >= (size_t)(end + 4 - buf) +
		    content_length(buf))
			break;
	}
	buf[len] = '\0';
	return (len > 0 ? 0 : -1);
}

/**
 * serve - handles one client connection
 * @client: client socket
 */
static void serve(int client)
{
	static char buf[BUF_SIZE];
	request_t req;

	if (read_request(client, buf) < 0)
		return;
	if (LEVEL < 4)
		printf("Raw request: \"%s\"\n", buf);
	if (!parse_request(buf, &req))
		return;
	if (LEVEL < 4)
	{
		print_parsed(&req);
		reply(client, &req, 200, "OK", NULL);
	}
	else
		route(client, &req);
	fflush(stdout);
}

/**
 * main - HTTP server loop
 *
 * Return: never returns
 */
int main(void)
{
	int srv, client;
	struct sockaddr_in addr;
	socklen_t len;

	signal(SIGPIPE, SIG_IGN);
	srv = setup_server();
	while (1)
	{
		len = sizeof(addr);
		client = accept(srv, (struct sockaddr *)&addr, &len);
		if (client < 0)
			continue;
		printf("Client connected: %s\n", inet_ntoa(addr.sin_addr));
		fflush(stdout);
		serve(client);
		close(client);
	}
	return (EXIT_SUCCESS);
}
