// baithuchanh2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "winsock2.h"

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8080);

	bind(listener, (SOCKADDR *)&addr, sizeof(addr));
	listen(listener, 1);

	while (true)
	{
		SOCKET client = accept(listener, NULL, NULL);

		// Nhan lenh gui tu client
		char buf[1024];
		int ret = recv(client, buf, sizeof(buf), 0);

		buf[ret] = 0;
		printf(buf);

		if (strncmp(buf, "GET / ", 6) == 0)
		{
			char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>";
			char msg[2056] = "";
			int offset = 0;
			sprintf(msg, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>");
			WIN32_FIND_DATAA DATA;
			HANDLE h = FindFirstFileA("C:\\*.*", &DATA);
			do {
				if (DATA.dwFileAttributes &
					FILE_ATTRIBUTE_DIRECTORY) {
					// Đây là một thư mục
					// In ra tên thư mục
					sprintf(msg + strlen(msg), "<br><i>%s</i><br>", DATA.cFileName);
					
				}
				else {
					// Đây là một file
					// In ra tên file và kích thước file
					char tmp[] = "";
					INT64 size = (DATA.nFileSizeHigh*(MAXDWORD + 1)) + (unsigned long)DATA.nFileSizeLow;
					sprintf(msg + strlen(msg), "<br><i>%s : %ul</i><br>", DATA.cFileName, size);
				}
			} while (FindNextFileA(h, &DATA));
			sprintf(msg + strlen(msg), "</body></html>");
			printf(msg);
			send(client, msg, strlen(msg), 0);
		}
		else
		{
			// Tra ve du lieu text html
			char msg[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body>Yeu cau khong duoc ho tro</body></html>";
			send(client, msg, strlen(msg), 0);
		}

		closesocket(client);
	}

	WSACleanup();
	system("PAUSE");

	return 0;
}
