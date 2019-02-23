#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "winsock2.h"
#include <iostream>
#include <string>
#include "ws2tcpip.h"

int main(int argc, char** argv) {// Ex1.exe 127.0.0.1 8000
	//agrv[0] = ex1.exe ; 
	//argv[1] = 127.0.0.1
	//const char *addr_ip = "127.0.0.1"; int port_num = 9000;
	char *addr_ip = argv[1];
	int port_num = atoi(argv[2]);
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//client

	SOCKADDR_IN addr;
	int ret;

	if (isalnum(addr_ip[0])) {
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(addr_ip);
		addr.sin_port = htons(port_num);
	}
	
	
	
	ret = connect(server, (SOCKADDR *)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
		printf("Error code: %d", ret);
		system("pause");

		return 1;
	}

	char msg[300];
	printf("Nhap text can gui len server: ");
	gets(msg);
	send(server, msg, strlen(msg), 0);

	char buf[256];

	while (1)
	{
		ret = recv(server, buf, sizeof(buf), 0);
		if (ret <= 0)
			break;

		buf[ret] = '\0';
		printf("Server Say: %s", buf);
		gets(msg);
		send(server, msg, strlen(msg), 0);
	}

	closesocket(server);
	WSACleanup();

	system("pause");

	return 0;
}
