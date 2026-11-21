#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * main - accepts one connection, prints the client IP and its message
 *
 * Return: EXIT_SUCCESS, or EXIT_FAILURE on error
 */
int main(void)
{
	int sock, client, opt = 1;
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	char buf[1024];
	ssize_t n;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		return (EXIT_FAILURE);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(12345);
	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0 ||
	    listen(sock, 10) < 0)
		return (EXIT_FAILURE);
	printf("Server listening on port 12345\n");
	fflush(stdout);
	client = accept(sock, (struct sockaddr *)&addr, &len);
	if (client < 0)
		return (EXIT_FAILURE);
	printf("Client connected: %s\n", inet_ntoa(addr.sin_addr));
	n = recv(client, buf, sizeof(buf) - 1, 0);
	if (n > 0)
	{
		buf[n] = '\0';
		printf("Message received: \"%s\"\n", buf);
	}
	close(client);
	close(sock);
	return (EXIT_SUCCESS);
}
