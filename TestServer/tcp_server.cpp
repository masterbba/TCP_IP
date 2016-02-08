#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wasData;
	SOCKET hServSock, hClntSock;	
	char message[BUF_SIZE];
	int strLen, i;

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
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAdrSize);
		if (hClntSock == INVALID_SOCKET)
			ErrorHandling("accept() error");
		else
			printf("Connected client %d \n", i + 1);

		int argCount = 0;
		recv(hClntSock, (char*)&argCount, sizeof(int), 0);
		
		int* argList = (int*)malloc(sizeof(int)*argCount);
		for (int l = 0; l < argCount; l++)
			recv(hClntSock, (char*)&argList[l], sizeof(int), 0);
		
		recv(hClntSock, message, BUF_SIZE, 0);

		int result = argList[0];

		switch (message[0])
		{
		case '+':
			for (int l = 1; l < argCount; l++) 
				result += argList[l];
			break;
		case '-':
			for (int l = 1; l < argCount; l++)
				result -= argList[l];
			break;
		case '*':
			for (int l = 1; l < argCount; l++)
				result *= argList[l];
			break;
		case '/':
			for (int l = 1; l < argCount; l++)
				result /= argList[l];
			break;
		default:
			printf("wtf???\n");
			break;
		}

		printf("%d\n", result);
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
