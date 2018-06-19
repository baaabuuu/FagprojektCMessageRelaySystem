/*
 * TCPcomm.c
 *
 *  Created on: April 1, 2018
 *      Author: Patrick
 */
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "../inc/errors.h"
#include "../inc/dataStructs.h"

#define RECVPORT 7070
#define BUFFSIZE 100
#define MAXCONN 3

/***********************************
 * Initalizes the TCP setup        *
 **********************************/
int setupTCP(tcpSETUPvars* setup)
{
	int socketValue = setup -> sock;
	struct sockaddr_in serverAddres = setup -> serverAddr;
	int maxCons = setup -> maxConnections;
	if ((socketValue = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		printf("[TCPcomm][setupTCP] to init socket\n");
		return ERR_FAILED;
	}
	printf("[TCPcomm][setupTCP] Initalized socket\n");
	serverAddres.sin_family		 =	AF_INET;
	serverAddres.sin_port		 =	htons(RECVPORT);
	serverAddres.sin_addr.s_addr = INADDR_ANY;
	printf("[TCPcomm][setupTCP] Setup port, allowed addresses (ANY) and IPV4 config\n[TCPcomm][setupTCP] Binding Socket\n");
	if (bind(socketValue, (struct sockaddr*) &serverAddres, sizeof(serverAddres)) < 0)
	{
		printf("[TCPcomm][setupTCP] Failed to bind socket.\n");
		return ERR_FAILED;
	}

	printf("[TCPcomm][setupTCP] Bind completed\n[TCPcomm][setupTCP] Listening\n");
	if (listen(socketValue, maxCons) < 0)
	{
		printf("[TCPcomm][setupTCP] Failed to listen with server\n");
		return ERR_FAILED;
	}
	printf("[TCPcomm][setupTCP] Listening flag set.\n");	
	setup -> sock = socketValue;
	setup -> serverAddr = serverAddres;
	setup -> maxConnections = maxCons;
	return ERR_OK;
}

/******************************************************************************************
 * Listen to incoming TCP connects and hand them off to the TCPmain function using a queue*
 *****************************************************************************************/
void listenTCP(void *arg_queue)
{
	listenStruct* listener = arg_queue;
	StsHeader** newConnections = listener -> newConnection;
	int serversock = listener -> serverSocket;
	struct sockaddr_in clientAddr;
	int clntLen;
	int clientSock;
	int bytesRcvd;
	char buffer[BUFFSIZE];
	for (;;)
	{
		clntLen = sizeof(clientAddr);
		if ((clientSock =	accept(serversock, (struct sockaddr*) &clientAddr, &clntLen)) < 0)
		{
			printf("Failed to accept client\n");
		} 
		
		if (clientSock >= 0)
		{
			StsQueue.push(*newConnections, &clientSock);
		}
	}
}


/***********************************************************************************
 * This function was used for in-house testing of TCP							   *
 **********************************************************************************/
int TCPserver()
{
	printf("starting TCP server\n");
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	int bytesRcvd;
	char* serverIP = "127.0.0.1";
	char* response = "hello scrub\0";
	int socketfd;
	int clntLen;
	int messageLength;
	char buffer[BUFFSIZE];

	if ((socketfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		printf("Failed to init socket\n");
		return 0;
	}
	
	printf("Created client socket\n");
	serverAddr.sin_family	=	AF_INET;
	serverAddr.sin_port	=	htons(RECVPORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	printf("binding\n");

	if (bind(socketfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0)
	{
		printf("Failed to bind server\n");
		return 0;
	}
	printf("Bind completed\n");
	if (listen(socketfd, MAXCONN) < 0)
	{
		printf("Failed to listen with server\n");
		return 0;
	}
	printf("Starting loop\n");
	int currentConnections = 0;
	int clientSock;
	for (;;)
	{
			printf("Looping\n");
			clntLen = sizeof(clientAddr);
			if ((clientSock =
								accept(socketfd,(struct sockaddr*) &clientAddr, &clntLen)) < 0)
			{
				printf("Failed to accept client\n");
			}
			if (clientSock >= 0)
			{
				//HANDLE TCP
				for(;;)
				{
					bytesRcvd = recv(clientSock,
									 buffer,
									 BUFFSIZE - 1, 0);
					if (bytesRcvd >= 0)
						printf("%s\n", buffer);
				}
			}		
	}
	return 0;
}

/***********************************************************************************
 * This shouldn't be used in theory, only for testing purposes.                    *
 * This is due to the DE1-SOC only recieving TCP connections from other various    *
 * machines                                                                        *
 **********************************************************************************/

int tcpClient()
{
	printf("starting client\n");
	struct sockaddr_in serverAddr;
	char* serverIP = "127.0.0.1";
	char* response = "{MOD:2}\0";
	int socketfd;
	int bytesRcvd;
	
	char buffer[BUFFSIZE];

	if ((socketfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		printf("Failed to init socket\n");
		return ERR_FAILED;
	}
	printf("Created client socket\n");
	serverAddr.sin_family	=	AF_INET;
	serverAddr.sin_port	=	htons(RECVPORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if (connect(socketfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0)
	{
		printf("Failed to establish connection\n");
		return 0;
	}
	printf("Connected to the server?\n");
	send(socketfd, "{KEY:1337}\0", 11, 0);
	int stringLength = strlen(response);
	while(1==1)
	{
		if (send(socketfd, response, stringLength, 0) != stringLength)
		{
			printf("failed\n");
		}
		sleep(1);
	}
	printf("terminating connection");
	return 0;
}
