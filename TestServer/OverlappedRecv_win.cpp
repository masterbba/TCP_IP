#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
void CALLBACK CompRutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ErrorHandling(char* message);

WSABUF databuf;
char buf[BUF_SIZE];
DWORD recvBytes = 0;

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hLisnSock, hRecvSock;
	SOCKADDR_IN lisnAdr, recvAdr;
	int idx, recvAdrSz;
	DWORD flags = 0;

	WSAEVENT evObj;
	WSAOVERLAPPED overlapped;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hLisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&lisnAdr, 0, sizeof(lisnAdr));
	lisnAdr.sin_family = AF_INET;
	lisnAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	lisnAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hLisnSock, (SOCKADDR*)&lisnAdr, sizeof(lisnAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error!");
	if (listen(hLisnSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error!");

	recvAdrSz = sizeof(recvAdr);
	hRecvSock = accept(hLisnSock, (SOCKADDR*)&recvAdr, &recvAdrSz);
	if (hRecvSock == INVALID_SOCKET)
		ErrorHandling("accept() error");


	memset(&overlapped, 0, sizeof(overlapped));
	databuf.len = BUF_SIZE;
	databuf.buf = buf;
	evObj = WSACreateEvent();


	while (1)
	{
		if (WSARecv(hRecvSock, &databuf, 1, &recvBytes, &flags, &overlapped, CompRutine) == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSA_IO_PENDING)
			{
				puts("Background data receive");
			}
		}

		idx = WSAWaitForMultipleEvents(1, &evObj, FALSE, WSA_INFINITE, TRUE);
		if (idx == WAIT_IO_COMPLETION)
			puts("overlapped I/O Completed");
		else
			ErrorHandling("WSARecv() error");
	}

	WSACloseEvent(evObj);
	closesocket(hRecvSock);
	closesocket(hLisnSock);
	WSACleanup();
	return 0;
}

void CALLBACK CompRutine(DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	if (dwError != 0)
	{
		ErrorHandling("CompRoutine error");
	}
	else
	{
		recvBytes = szRecvBytes;
		printf("Receive Message : %s \n", buf);
	}
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
