​#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cerrno>

int main()
{
	int sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("create socket failed: %d\n", errno);
		return -1;
	}
	
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(6666);
	
    // 调用connect指定服务器地址
	if (connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
	{
		printf("connect failed: %d\n", errno);
		return -1;
	}
	
	int num = 0;
	char buffer[100] = {0};
	while (true)
	{
		snprintf(buffer, 99, "msg %d", ++num);
		send(sock, buffer, strlen(buffer), 0); // 直接send数据
		printf("send %s\n", buffer);
		sleep(1);
	}
	
	return 0;
}
