#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * main - listens on port 12345 and hangs
 *
 * Return: EXIT_FAILURE on error
 */
int main(void)
{
	int sock, opt = 1;
	struct sockaddr_in addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("socket");
		return (EXIT_FAILURE);
	}
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(12345);
	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0 ||
	    listen(sock, 10) < 0)
	{
		perror("bind/listen");
		return (EXIT_FAILURE);
	}
	printf("Server listening on port 12345\n");
	fflush(stdout);
	while (1)
		pause();
	return (EXIT_SUCCESS);
}
