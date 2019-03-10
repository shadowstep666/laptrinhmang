// btvn2_chatserver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include "winsock2.h"
#include <string>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

DWORD WINAPI ClientThread(LPVOID);
void RemoveClient(SOCKET);
int CheckID(char*, SOCKET);

SOCKET clients[64];
string client_id[64];
int numClients;
char he[3] = ": ";

int main(int argc, char* argv[])
{

	int listenPort = atoi(argv[1]);
	WSAData wsa;
	SOCKET ListenSocket, ClientSocket;
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(listenPort);

	cout << "Hello hoang!\n";

	WSAStartup(MAKEWORD(2, 2), &wsa);
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(ListenSocket, (sockaddr *)&addr, sizeof(addr));
	listen(ListenSocket, 5);
	printf("create success\n");
	numClients = 0;
	while (true) {
		ClientSocket = accept(ListenSocket, NULL, NULL);
		printf("New client connected: %d\n", ClientSocket);
		clients[numClients] = ClientSocket;
		numClients++;

		CreateThread(0, 0, ClientThread, &ClientSocket, 0, 0);
	}

}

DWORD WINAPI ClientThread(LPVOID lpParameter) {
	SOCKET ClientSocket = *(SOCKET *)lpParameter;
	char buffer[1024];
	char bufferSend[1024] /*= { 'c', 'l', 'i', 'e', 'n', 't','_','i','d',':' }*/;
	int ret;
	int res = 0;
	bool connected = false;

	while (true) {
		while (connected == false) {
			printf("Vui long nhap client_id\n");
			ret = recv(ClientSocket, buffer, sizeof(buffer), 0);
			if (ret <= 0) {
				RemoveClient(ClientSocket);
				break;
			}
			buffer[ret] = 0;
			res = CheckID(buffer, ClientSocket);
			if (res == 1) {
				printf("Connect accepted\n");
				connected = true;
				break;
			}
		}
		ret = recv(ClientSocket, buffer, sizeof(buffer), 0);
		if (ret <= 0) {
			RemoveClient(ClientSocket);
			break;
		}
		buffer[ret] = 0;
		for (int i = 0; i < numClients; i++) {
			if (clients[i] == ClientSocket)
				strcpy_s(bufferSend, client_id[i].c_str());
		}
		strcat_s(bufferSend, he);
		strcat_s(bufferSend, buffer);
		if (connected) {
			printf("Receive: %s\n", buffer);
			for (int i = 0; i <= numClients; i++) {
				if (clients[i] != ClientSocket)
					send(clients[i], bufferSend, strlen(bufferSend), 0);
			}
		}
	}
	closesocket(ClientSocket);
	return 0;
}

void RemoveClient(SOCKET client)
{
	// Tim chi so cua phan tu
	int i = 0;
	for (; i < numClients; i++)
		if (clients[i] == client)
			break;

	// Xoa phan tu
	if (i < numClients - 1)
		clients[i] = clients[numClients - 1];
	numClients--;
}

int CheckID(char* buffer, SOCKET ClientSocket) {
	string RightSyntax = "client_id: ";
	string StringBuffer(buffer);
	StringBuffer.pop_back();
	cout << "Checking..." << endl;
	if (StringBuffer.compare(0, 11, RightSyntax) == 0) {
		cout << "Right" << endl;
		for (int i = 0; i < numClients; i++) {
			if (clients[i] == ClientSocket)
				client_id[i] = StringBuffer.substr(11);
		}
		return 1;
	}
	return 0;
}

