#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/**
 * main - connects to a server
 * @argc: argument count
 * @argv: argument vector (host, port)
 *
 * Return: EXIT_SUCCESS, or EXIT_FAILURE on error
 */
int main(int argc, char **argv)
{
	struct addrinfo hints, *res;
	int sock;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
		return (EXIT_FAILURE);
	}
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(argv[1], argv[2], &hints, &res) != 0)
		return (EXIT_FAILURE);
	sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sock < 0 || connect(sock, res->ai_addr, res->ai_addrlen) < 0)
	{
		perror("connect");
		freeaddrinfo(res);
		return (EXIT_FAILURE);
	}
	printf("Connected to %s:%s\n", argv[1], argv[2]);
	freeaddrinfo(res);
	close(sock);
	return (EXIT_SUCCESS);
}
