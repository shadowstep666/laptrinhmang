#include <iostream>
#include <winsock2.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	WSADATA wsa;
	WSAStartup (MAKEWORD(2,2),&wsa);
	
	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);
 
	bind(listener, (SOCKADDR *)&addr, sizeof(addr));
    listen(listener, 5);
    
    SOCKADDR_IN clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    while (true)
    {
        SOCKET client = accept(listener, (SOCKADDR *)&clientAddr, &clientAddrLen);
 
        printf("Da chap nhan ket noi: %d\n", client);
        printf("Dia chi IP cua client: %s\n", inet_ntoa(clientAddr.sin_addr));
        closesocket(client);
    }
    closesocket(listener);
    WSACleanup();
    
	return 0;
}
