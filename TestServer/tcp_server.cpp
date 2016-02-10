#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
#define OPSZ 4
void ErrorHandling(char* message);
int calculate(int opnum, int opnds[], char oprator);

int main(int argc, char* argv[])
{
	WSADATA wasData;
	SOCKET hServSock, hClntSock;	
	char opinfo[BUF_SIZE];
	int result, opndCnt, i;
	int recvCnt, recvLen;
	SOCKADDR_IN servAddr, clntAddr;
	int clntAdrSize;

	if (argc != 2)
	{
		printf("Usage : %s <port>/n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wasData) != 0)
		ErrorHandling("WASStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("Socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if(bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr))==SOCKET_ERROR)
		ErrorHandling("bind() error");

	if(listen(hServSock, 5)==SOCKET_ERROR)
		ErrorHandling("listen() error");

	clntAdrSize = sizeof(clntAddr);

	for (i = 0; i < 1; i++)
	{
		opndCnt = 0;
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAdrSize);
		recv(hClntSock, (char*)&opndCnt, 1, 0);

		recvLen = 0;
		while ((opndCnt*OPSZ + 1)>recvLen)
		{
			recvCnt = recv(hClntSock, &opinfo[recvLen], BUF_SIZE - 1, 0);
			recvLen += recvCnt;
		}

		result = calculate(opndCnt, (int*)opinfo, opinfo[recvLen - 1]);
		send(hClntSock, (char*)&result, sizeof(result), 0);
		closesocket(hClntSock);
	}

	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int calculate(int opnum, int opnds[], char op)
{
	int result = opnds[0], i;
	switch (op)
	{
	case '+':
		for (i = 1; i < opnum; i++) result += opnds[i];
		break;
	case '-':
		for (i = 1; i < opnum; i++) result -= opnds[i];
		break;
	case '*':
		for (i = 1; i < opnum; i++) result *= opnds[i];
		break;
	case '/':
		for (i = 1; i < opnum; i++) result /= opnds[i];
		break;
	}

	return result;
}