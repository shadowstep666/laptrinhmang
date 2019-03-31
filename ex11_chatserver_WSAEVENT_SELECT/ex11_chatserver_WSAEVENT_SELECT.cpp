// ex11_chatserver_WSAEVENT_SELECT.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include"winsock2.h"

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET listener = socket(AF_INET, SOCK_STREAM , IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(9000);

	bind(listener, (SOCKADDR *)&addr, sizeof(addr));
	listen(listener, 5);

	SOCKET sockets[64];
	WSAEVENT events[64];
	int numEvents = 0;

	SOCKET registeredClients[64];   // Chua cac client da dang nhap
	int numRegisteredClients = 0;
	char *ids[64];

	WSAEVENT newEvent = WSACreateEvent();
	WSAEventSelect(listener, newEvent, FD_ACCEPT);

	sockets[numEvents] = listener;
	events[numEvents] = newEvent;
	numEvents ++;
	int ret;
	char buf[256];

	char cmd[64];
	char id[64];
	char tmp[64];
	char *errorMsg = "loi cu phap .Thu nhap lai \n";

	char sendBuf[256];
	char targetId[64];

	while (true)
	{
		//doi tat ca su kien 
		ret = WSAWaitForMultipleEvents(numEvents, events, FALSE, 0, FALSE);
		if (ret == WSA_WAIT_FAILED)
			break;

	

		int index = ret - WSA_WAIT_EVENT_0;
		//duyet de xem phan tu nao duoc bao hieu
		for (int i = index; i < numEvents; i++)
		{
			ret = WSAWaitForMultipleEvents(1, &events[i], TRUE, 0, FALSE);
			if (ret == WSA_WAIT_FAILED)
				continue;
			if (ret == WSA_WAIT_TIMEOUT)
				continue;

			WSANETWORKEVENTS networkEvents;
			WSAEnumNetworkEvents(sockets[i], events[i], &networkEvents);
			WSAResetEvent(events[i]);

			//kiem tra su kien FD_ACCEPT
			if (networkEvents.lNetworkEvents & FD_ACCEPT)
			{
				if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
				{
					printf("FD_ACCEPT failed\n");
					continue;
				}

				if (numEvents == WSA_MAXIMUM_WAIT_EVENTS)
				{
					printf("Too many connection\n");
					continue;
				}

				SOCKET client = accept(sockets[i], NULL, NULL);

				newEvent = WSACreateEvent();
				WSAEventSelect(client, newEvent, FD_READ | FD_CLOSE);

				events[numEvents] = newEvent;
				sockets[numEvents] = client;
				numEvents++;

				printf("New client connected %d\n", client);
			}
			//kiem tra su kien FD_READ
			if (networkEvents.lNetworkEvents & FD_READ)
			{
				if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
				{
					printf("FD_READ failed\n");
					continue;
				}

				ret = recv(sockets[i], buf, sizeof(buf), 0);
				if (ret <= 0)
				{
					printf("FD_READ failed\n");
					continue;
				}

				buf[ret] = 0;
				printf("Received: %s\n", buf);
				//kiem tra trang thai cua client
				int j = 0;
				for (; j < numRegisteredClients; j++)
					if (sockets[i] == registeredClients[j])
						break;	
				if (j == numRegisteredClients)
				{
					// Trang thai chua dang nhap
					// Kiem tra cu phap client_id: [id]
					ret = sscanf(buf, "%s %s %s", cmd, id, tmp);
					if (ret == 2)
					{
						if (strcmp(cmd, "client_id:") == 0)
						{
							char *okMsg = "Dung cu phap. Hay nhap thong diep muon gui.\n";
							send(sockets[i], okMsg, strlen(okMsg), 0);

							// Luu client dang nhap thanh cong vao mang
							registeredClients[numRegisteredClients] = sockets[i];
							ids[numRegisteredClients] = (char *)malloc(strlen(id) + 1);
							memcpy(ids[numRegisteredClients], id, strlen(id) + 1);
							numRegisteredClients++;
						}
						else
							send(sockets[i], errorMsg, strlen(errorMsg), 0);
					}
					else
						send(sockets[i], errorMsg, strlen(errorMsg), 0);
				}
				else
				{
					// Trang thai da dang nhap
					ret = sscanf(buf, "%s", targetId);
					if (ret == 1)
					{
						if (strcmp(targetId, "all") == 0)
						{
							sprintf(sendBuf, "%s: %s", ids[j], buf + strlen(targetId) + 1);

							for (int j = 0; j < numRegisteredClients; j++)
								if (registeredClients[j] != sockets[i])
									send(registeredClients[j], sendBuf, strlen(sendBuf), 0);
						}
						else
						{
							sprintf(sendBuf, "%s: %s", ids[j], buf + strlen(targetId) + 1);

							for (int j = 0; j < numRegisteredClients; j++)
								if (strcmp(ids[j], targetId) == 0)
									send(registeredClients[j], sendBuf, strlen(sendBuf), 0);
						}
					}
				}
			}
			
			//kiem tra su kien FD_CLOSE
			if (networkEvents.lNetworkEvents & FD_CLOSE)
			{
				if (networkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
				{
					printf("FD_CLOSE failed\n");
					continue;
				}

				closesocket(sockets[i]);
				// Xoa socket va event khoi mang

				printf("Client disconnected\n");
			}
		}
	}

	return 0;
}

