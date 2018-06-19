/*
 * UDPmain.c
 *
 *  Created on: Mar 26, 2018
 *      Author: Patrick
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "../inc/sts_queue.h"
#include "../inc/UDPmain.h"

int UDPis_recver = 0;
int UDPrecvPort = 5004;
int UDPsendPort = 5004;
int offset = 0;

/************************************
 * The UDP reciever used on one side*
 ***********************************/
int UDPreciever(StsHeader** video)
{
	unsigned char* buf;
	int err, len;
	int source = 0;
	printf("[UDPmain][UDPreciever] Acting as reciever with the address %d\n", UDPrecvPort);

	if ((err=UDPcomm_init(UDPrecvPort)) != ERR_OK)
	{
		printf("[UDPmain][UDPreciever] Failed to init UDPreciever\n");
		return ERR_FAILED;
	}
	printf("[UDPmain][UDPreciever] UDPReciever initalized\n");
	buf = malloc(sizeof(char) * 10);
	memset(buf, 0, 10*sizeof(char));
	for(;;)
	{
		if ((len = UDPcomm_recv(&source, buf)) < 0)
		{
			printf("[UDPmain][UDPreciever] Error, UDPreciever failed with %d\n", ERR_FAILED);
		}
		else
		{
			buf[len] = '\0';
			StsQueue.push(*video, buf);
			memset(buf, 0, 10*sizeof(char));
		}
	}
	return 0;
}

/*******************************************************************
 * Transmits whatever data is on the video queue to the destination*
 ******************************************************************/
int UDPtransmitter(StsHeader** video)
{
	int err, last;
	int source = 0;
	printf("[UDPmain][UDPtransmitter] Starting transmitter\n");
	if ((err = UDPcomm_init(UDPsendPort)) != ERR_OK)
	{
		printf("[UDPmain][UDPtransmitter] Failed to init transmitter\n");
		return 1;
	}
	unsigned char* input;
	printf("[UDPmain][UDPtransmitter] Initialized transmitter\n");
	for(;;)
	{
		sleep(1000);
	}
	return 0;
}
