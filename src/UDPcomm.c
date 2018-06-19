/*
 * TCPcomm.c
 *
 *  Created on: April 28, 2018
 *      Author: Patrick
 */
#include "../inc/UDPcomm.h"
#include "../inc/errors.h"
#include "../inc/UDPmain.h"

#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int sock;

/************************************
 * Initializes the UDP functionality*
 ***********************************/
int UDPcomm_init(int addr)
{
	struct sockaddr_in sa;
	printf("[UDPcomm][UDPcomm_init] Ensuring legal address\n");
	if (addr > 65535 || addr < 1024)
	{
		return ERR_INVALID;
	}
	printf("[UDPcomm][UDPcomm_init] Initalizing socket...\n");
	//AF_INET is the domain IPV4 if IPV6 is desired use AF_INET6
	if ((sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		return ERR_FAILED;
	}
	sa.sin_family		=	AF_INET;
	sa.sin_addr.s_addr	=	htonl(INADDR_ANY);
	sa.sin_port		=	htons(addr);
	printf("[UDPcomm][UDPcomm_init] Binding socket\n");
	if (bind(sock, (struct sockaddr*) &sa, sizeof(sa)) == -1)
	{
		printf("[UDPcomm][UDPcomm_init] Failed to bind socket.\n");
		return ERR_FAILED;
	}
	
	return ERR_OK;
}


/************************************
 * Transmits message using UDP      *
 ***********************************/
int UDPcomm_send(int dst, unsigned char* data, int len)
{
	if (len > BUFFER_SIZE || len < 1 || dst > 65535 || dst < 1024)
	{
		return ERR_INVALID;
	}

	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(dst);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	int result;
	if ((result = sendto(sock, data, len, 0, (struct sockaddr*) &sa, sizeof(sa)) == -1))
	{
		printf("[UDPcomm][UDPcomm_send] Failed to connect\n");
	}
	if (result != len)
	{
		return ERR_FAILED;
	}
	return ERR_OK;
}

/*****************************
 * Gets a message using UDP  *
 ****************************/
int UDPcomm_recv(int* src, char* data)
{
	struct sockaddr_in sa;
	int len = -1;
	int rv;
	unsigned int sizeofSOckAddrDAta;

	struct pollfd pfd;

	pfd.events = POLLIN;
	pfd.fd = sock;
	pfd.revents = 0;
	rv = poll(&pfd, 1, 999999);
	if (rv <= 0)
	{
		return ERR_FAILED;
	}
	int saSize = sizeof(sa);
	if ((len = recvfrom(sock, data, BUFFER_SIZE, 0,  (struct sockaddr*)&sa, &saSize)) == -1)
	{
		return ERR_FAILED;
	}
	*src = ntohs(sa.sin_port);
	return len;
}
