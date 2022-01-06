​#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cerrno>

#define MAX_BUFFER_LEN (8 * 1024)

int main()
{
	int sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("create socket failed: %d\n", errno);
		return -1;
	}
	
	struct sockaddr_in localaddr;
	localaddr.sin_family = AF_INET;
	localaddr.sin_addr.s_addr = INADDR_ANY;
	localaddr.sin_port = htons(6666);
	
	if (bind(sock, (const struct sockaddr *)&localaddr, sizeof(localaddr)) < 0)
	{
		printf("bind failed: %d\n", errno);
		return -1;
	}
	
	struct sockaddr_in clientaddr;
	int addrlen = sizeof(clientaddr);
	char buffer[MAX_BUFFER_LEN];
	while (true)
	{
		memset(buffer, 0, sizeof(buffer));
		int recvlen = recvfrom(sock, (char*)buffer, MAX_BUFFER_LEN, MSG_WAITALL, (struct sockaddr*)&clientaddr, (socklen_t*)&addrlen);
		printf("recv %s\n", buffer);
	}
	
	return 0;
}
