#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1024

void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	char message[BUF_SIZE];
	int strLen;
	SOCKADDR_IN servAddr;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>/n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error!");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	else
		puts("Connected.................");


	fputs("Input argCount ", stdout);
	fgets(message, BUF_SIZE, stdin);
	int argCount = atoi(message);
	send(hSocket, (char*)&argCount, sizeof(int), 0);

	for (int i = 0; i < argCount; i++)
	{
		fputs("Input arg ", stdout);
		fgets(message, BUF_SIZE, stdin);
		int arg = atoi(message);
		send(hSocket, (char*)&arg, sizeof(arg), 0);
	}
	
	fputs("Input operator ", stdout);
	fgets(message, BUF_SIZE, stdin);
	send(hSocket, message, strlen(message), 0);
	
	int result = 0;
	recv(hSocket, (char*)&result, sizeof(result), 0);	
	printf("Message from server : %d", result);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}