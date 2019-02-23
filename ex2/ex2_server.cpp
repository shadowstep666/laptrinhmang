#include "winsock2.h"
#include <iostream>
#include <fstream>
#include <stdio.h>

char *hello(char *filePaths) {
	FILE *f;
	f = fopen(filePaths, "r");
	char *helloString = new char[50];
	fgets(helloString, 255, (FILE*)f);
	fclose(f);
	return helloString;
}

void saveMsg(char *mgs, char *filePath) {
	FILE *f;
	f = fopen(filePath, "w");
	fputs(mgs, f);
	fclose(f);
}

int main(int argc, char** argv) {
	
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(atoi(argv[1]));

	bind(listener, (SOCKADDR *)&addr, sizeof(addr));
	listen(listener, 5);

	printf("Dang cho ket noi...\n");
	while (1) {
		SOCKET client = accept(listener, NULL, NULL);
		printf("Ket noi thanh cong...\n");
		send(client, hello(argv[2]), strlen(hello(argv[2])), 0);

		char buf[255];
		int ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
			break;
		buf[ret] = '\0';
		saveMsg(buf, argv[3]);
		printf(buf);
		printf("saved msg!");
		//closesocket(client);
		if (!strcmp(buf, "tam biet")) break;
	}
	closesocket(listener);
	WSACleanup();
	return 0;
}
