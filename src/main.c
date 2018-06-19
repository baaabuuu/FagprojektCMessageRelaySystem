/*
 * main.c
 *
 *  Created on: March 26, 2018
 *      Author: Patrick
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "../inc/sts_queue.h"
#include "../inc/UDPmain.h"
#include "../inc/dataStructs.h"
#include "../inc/TCPmain.h"
#include "../inc/TCPcomm.h"
#include "../inc/FPGAhandler.h"
#include "../inc/test_runner.h"
#include "../inc/hps_linux.h"

#include "../inc/dataValues.h"

int test = 0;


/***********************************************************
 * Starts the specific UDP function depending on which side*
 * To swap side, toggle the SW9			                   *
 **********************************************************/
void* UDPThreadFunc(void *arg_queue)
{
	udpHandler* videoSetup = arg_queue;
	StsHeader* videoQueue = videoSetup -> queue;
	if (readSWN(9))
	{
		printf("[main][UDPThreadFunc] Starting UDPreciever\n");
		UDPreciever(&videoQueue);
	}
	else
	{
		printf("[main][UDPThreadFunc] Starting UDPtransmitter\n");
		UDPtransmitter(&videoQueue);
	}
}

/*******************************************************************************
 * Starts the FPGA thread and runs                                             *
 ******************************************************************************/
void* FPGAThreadFunc(void *arg_queue)
{
	fpgaHandler* fpgaSetup	= arg_queue;
	StsHeader** videoQueue	= fpgaSetup -> video;
	fpgaHandling(videoQueue);
	return NULL;
}

/*************************************
 * Starts the TCP thread and runs it.*
 ************************************/
void* tcpThreadFunc(void *arg_queue)
{
	tcpGlobal* tcpSetup = arg_queue;
	DataValues** dataPointers = tcpSetup -> dataPointer;
	SetsValues* sets = tcpSetup -> settings;

	TCPmainFunc(dataPointers, sets);
	
	return NULL;
}

/*****************************************************************
 * Starts the TCP handler, only used for thread                  *
 ****************************************************************/
void startTCPhandler(pthread_t* thread, tcpGlobal* tcpSettings)
{
	pthread_create(thread, NULL, tcpThreadFunc, tcpSettings);
}

/*****************************************************************
 * Starts the UDP handler, only used for thread                  *
 ****************************************************************/
void startUDP(pthread_t* thread, udpHandler* video)
{
	pthread_create(thread, NULL, UDPThreadFunc, video);
}

/*****************************************************************
 * Starts the FPGA handler, only used for thread                 *
 ****************************************************************/
void startFPGA(pthread_t *thread, fpgaHandler *commands)
{
	pthread_create(thread, NULL, FPGAThreadFunc, commands);
}

/**************************************************************
 * Initlizes the video handler, fpga commands and TCPsettings *
 * It creates the queue's and points towards them.            *
 *************************************************************/
void setupVars(udpHandler*  videoHandler, 
			   fpgaHandler* fpgaCommands, 
			   tcpGlobal*   TCPsettings
			  )
{
	videoHandler -> queue = StsQueue.create();
	SetsValues* sets = SettingsSets.create();
	DataValues** data = malloc(3*sizeof(DataValues*));
	data[0] = DataSets.create();
	data[1] = DataSets.create();
	data[2] = DataSets.create();
	
	fpgaCommands -> video		=	&(videoHandler -> queue);
	
	TCPsettings -> settings		=	sets;
	TCPsettings -> dataPointer	=	data;
}

/*****************************************************************
 * Read additional arguments passed down through the launch      *
 * -t makes it a transmitter, transmitting UDP to another machine*
 ****************************************************************/
void read_args(int argc, char* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-t") == 0)
		{
			test = 1;
			continue;
		}
		if (strcmp(argv[i], "./main") == 0)
		{
			continue;
		}
		printf("[main][read_args] UNKNOWN OPTION: %s\n", argv[i]);
	}
}

/*************************************************************************
 * Main function, launches everything and then gets stuck in a while loop*
 * It is a UDP reciever if SWN 9 is toggled. if SWN 0 is toggled test are*
 * are run upon startup.												 *
 ************************************************************************/
int main(int argc, char * argv[])
{
	pthread_t udpThread, tcpThread, fpgaThread;
	udpHandler* udpVideo = malloc(sizeof(udpHandler));
	fpgaHandler* fpgaCommands = malloc(sizeof(fpgaHandler));
	tcpGlobal* tcpSettings = malloc(sizeof(tcpGlobal));
	printf("[main][main] Initializing variables\n");
	setupVars(udpVideo, fpgaCommands, tcpSettings);
	setupFPGA();
	read_args(argc, argv);
	if (readSWN(0))
	{
		printf("[main][main] Running tests....\n");
		runTests();
	}
	
	
	printf("[main][main] Starting FPGA thread\n");
	startFPGA(&fpgaThread, fpgaCommands);
	
	printf("[main][main] Starting UDP thread\n");
	startUDP(&udpThread, udpVideo);
	
	printf("[main][main] Starting TCP handler thread\n");
	startTCPhandler(&tcpThread, tcpSettings);

	int counter = 0;
	int ledFun[12] = {1, 3, 7, 14, 28, 56, 112, 224, 448, 896, 385, 259 };

	if (readSWN(9))
	{
		for(;;)
		{
			setLEDRN(ledFun[counter]);
			counter = counter +1;
			if (counter > 11)
			{
				counter = 2;
			}
			usleep(50000);
		}
	}
	for(;;)
	{
		sleep(10000);
	}
		
	

	return 0;
}
