// ex8_select_telnetchat.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"
#include "winsock2.h"
#define CRT_SECURE_NO_WARNINGS

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	bind(listener, (SOCKADDR *)&addr, sizeof(addr));
	listen(listener, 5);

	fd_set fdread;
	int ret;

	SOCKET clients[64];
	int numClients = 0;
	char filebuf[256];
	char buf[256];
	char cmdbuf[256];

	while (true)
	{
		FD_ZERO(&fdread);
		for (int i = 0; i < numClients; i++)
		{
			FD_SET(listener, &fdread);
		}

		ret = select(0, &fdread, NULL, NULL, NULL);

		if (ret == SOCKET_ERROR)
			break;

		if (ret > 0)
		{
			// Xu ly su kien co ket noi moi
			if (FD_ISSET(listener, &fdread))
			{
				SOCKET client = accept(listener, NULL, NULL);
				printf("New client accepted: %d\n", client);

				clients[numClients] = client;
				numClients++;
			}

			// Xu ly su kien khi co du lieu den cac client
			for (int i = 0; i < numClients; i++)
				if (FD_ISSET(clients[i], &fdread))
				{
					ret = recv(clients[i], buf, sizeof(buf), 0);

					if (ret <= 0)
					{
						// Ket noi bi huy
						continue;
					}

					buf[ret] = 0;
					printf("Received from %d: %s\n", clients[i], buf);

					int found = 0;
					FILE *f = fopen("C:\\Users\\admin\\Desktop\\lap trinh mang\\btvn_3\\select_telnetchat\\text.txt", "r");
					while (fgets(filebuf, sizeof(filebuf), f))
					{
						if (strcmp(buf, filebuf) == 0)
						{
							found = 1;
							break;
						}
					}
					fclose(f);

					if (found == 1)
					{
						char *msg = "dang nhap thanh cong";
						send(clients[i], msg, strlen(msg), 0);
						break;
					}
					else {
						char*msg = "dang nhap that bai.hay thu lai \n";
						send(clients[i], msg, strlen(msg), 0);
					}
					while (true)
					{
						ret = recv(clients[i], buf, sizeof(buf), 0);

						buf[ret] = 0;

						if (buf[ret - 1] == '\n')
							buf[ret - 1] = 0;

						printf("Received: %s\n", buf);

						sprintf(cmdbuf, "%s > C:\\Users\\admin\\Desktop\\lap trinh mang\\btvn_3\\select_telnetchat\\text.txt", buf);
						system(cmdbuf);

						FILE *f = fopen("C:\\C:\\Users\\admin\\Desktop\\lap trinh mang\\btvn_3\\select_telnetchat\\text.txt ", "r");
						while (fgets(filebuf, sizeof(filebuf), f))
						{
							send(clients[i], filebuf, strlen(filebuf), 0);
						}
						fclose(f);
					}

				}
		}
	}
	closesocket(listener);
	WSACleanup();
	return 0;
}


