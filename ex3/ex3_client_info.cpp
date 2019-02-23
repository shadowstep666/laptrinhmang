#include <iostream>
#include <winsock2.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);
	
	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	SOCKADDR_IN addr;
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port=htons(9000);
	
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	int ret = connect(client,(SOCKADDR *)&addr, sizeof(addr));
	if(ret==0) {
		printf("Ket noi thanh cong");
        printf("Thong tin may: \n");
        printf("danh sach o dia trong may: %u\n",sysinfo.dwNumberOfProcessors);
        printf("Kich thuoc cac o dia: %u\n",sysinfo.dwPageSize);
    } else printf("Ket noi that bai");
	closesocket(client);
    WSACleanup();
	
	return 0;
}
