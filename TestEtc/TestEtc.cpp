#undef UNICODE
#undef _UNICODE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	char *strAddr = "203.211.218.102:9190";
	char strAddrBuf[50];
	SOCKADDR_IN servAddr;
	int size;
	DWORD zzz;

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	size = sizeof(servAddr);
	WSAStringToAddress(strAddr, AF_INET, NULL, (SOCKADDR*)&servAddr, &size);
	zzz = sizeof(strAddrBuf);
	WSAAddressToString((SOCKADDR*)&servAddr, sizeof(servAddr), NULL, strAddrBuf, &zzz);
	printf("Second conv result : %s \n", strAddrBuf);

	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
