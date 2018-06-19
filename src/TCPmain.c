/*
 * TCPcomm.c
 *
 *  Created on: April 1, 2018
 *      Author: Patrick
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../inc/TCPcomm.h"
#include "../inc/TCPmain.h"
#include "../inc/errors.h"
#include "../inc/sts_queue.h"
#include "../inc/connValues.h"
#include "../inc/dataValues.h"
#include "../inc/setsValues.h"
#include "../inc/dataStructs.h"
#include "../inc/FPGAhandler.h"
#include "../inc/hps_linux.h"



int currConnections = 0;
int maxConnections = 3;
int bufferSize = 15;
int logger = 1;
StsHeader* globalUpdates;
StsHeader** globalUpdater;

DataValues** dataPointer;
SetsValues* settings;


/**************************************************************************
 * Main function, passes on and starts TCP and other threads              *
 * Maintains control FIFO and updates according to those things.
 *************************************************************************/
void TCPmainFunc(DataValues** data, SetsValues* settingsStruct)
{

	globalUpdates = StsQueue.create();
	pthread_t connListenerThread;
	pthread_t connThreads[maxConnections];
	int serverSocket;
	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;
	tcpSETUPvars setup;
	setup.maxConnections = maxConnections;
	int side = readSWN(9);
	if (setupTCP(&setup) != ERR_OK)
	{
		printf("[TCPmain][TCPmainFunc] Failed to setup TCP server side\n");
		return;
	}
	
	printf("[TCPmain][TCPmainFunc] setup the TCP settings\n[TCPmain][TCPmainFunc] Starting to accept connections\n");
	StsHeader* newConnections = StsQueue.create();

	int dummySocket = -1;
	
	char* setUpdate;
		
	listenStruct listener;
	listener.newConnection = &newConnections;
	listener.serverSocket = setup.sock;
	
	
	ConnValues** dataCon = malloc(sizeof(ConnValues*) * maxConnections);
	memset(dataCon, 0, sizeof(ConnValues*) * maxConnections);
	dataCon[0] = ConnSets.create();
	dataCon[1] = ConnSets.create();
	dataCon[2] = ConnSets.create();
	ConnSets.updateQueue(dataCon[0], &globalUpdates);
	ConnSets.updateQueue(dataCon[1], &globalUpdates);
	ConnSets.updateQueue(dataCon[2], &globalUpdates);
	ConnSets.updateData(dataCon[0], data);
	ConnSets.updateData(dataCon[1], data);
	ConnSets.updateData(dataCon[2], data);
	ConnSets.updatesetPointer(dataCon[0], settingsStruct);
	ConnSets.updatesetPointer(dataCon[1], settingsStruct);
	ConnSets.updatesetPointer(dataCon[2], settingsStruct);
	pthread_create(&connListenerThread, NULL, listenTCP, &listener);
	for (;;)
	{
		int* tempSocket;
		SettingsSets.updateMOD(settingsStruct, readLEDRN(0));
		if (side) //UTIL UDP SIDE
		{
			if (canReadControl)
				SettingsSets.updateUTIL(settingsStruct, readControl());
		} 
		else
		{ //BER, ERR
			if (canReadControl)
			{
				int value = readControl();
				SettingsSets.updateBER(settingsStruct, value);
				DataSets.updateErrors(data[0],  value);
				DataSets.updateErrors(data[1],  value);
				DataSets.updateErrors(data[2],  value);
			}		
		}
		if ((tempSocket = StsQueue.pop(newConnections)) != NULL)
		{
			if (logger == 1)
			{
				printf("[TCPmain][TCPmainFunc] New TCP connection added\n");
			}
			int i = 0;
			while(i < maxConnections)
			{
				if (logger == 1)
					printf("[TCPmain][TCPmainFunc] i: %d, maxcon: %d\n", i, maxConnections);
				if (!ConnSets.getisConnected(dataCon[i]))
				{
					ConnSets.updateSocket(dataCon[i], tempSocket);
					if (logger == 1)
					{
						printf("[TCPmain][TCPmainFunc] i: %d was not connected - starting thread\n", i);
						printf("[TCPmain][TCPmainFunc] socket: %d \n", *ConnSets.getSocket(dataCon[i]));

					}
					ConnSets.updateThread(dataCon[i], &connThreads[i]);
					ConnSets.updateisConnected(dataCon[i], 1);
					pthread_create(&connThreads[i], NULL, recvMessage, &dataCon[i]);
					break;
				}
				i++;
			}
			if (i == maxConnections && logger == 1)
			{
				printf("[TCPmain][TCPmainFunc] Deleted Connection, no available space\n");
			}
		}
		if ((setUpdate = StsQueue.pop(globalUpdates)) != NULL)
		{
			for (int i = 0; i < maxConnections; i++)
			{
				if (*ConnSets.getSocket(dataCon[i]) != -1)
				{
					send(*ConnSets.getSocket(dataCon[i]), setUpdate, strlen(setUpdate), 0);
				}
			}
			free(setUpdate);
		}
	}
}


int tcpCount_illegalChars(const char* string)
{
	int c = 0;
	while (*string)
	{
		char identifier = *(string++);
		if (identifier == '}')
			c++;
		if (identifier == ':') 
			c++;
	}
	return c;
}


/********************************************************************************
 * Message listener function, whenever something is recieved pass the message on*
 *******************************************************************************/
void* recvMessage(void *arg_queue)
{
	ConnValues** conn = arg_queue;
	int clientSock = *ConnSets.getSocket(*conn);
	char*  buffer = malloc(bufferSize);
		memset(buffer, 0, bufferSize);
	int bytesRcvd = 0;
	int id = -1;
	for(;;)
	{
		bytesRcvd = recv(clientSock, buffer, bufferSize, 0);
		if (bytesRcvd > 0)
		{
			printf("[TCPmain][recvMessage] RECVD: %s\n", buffer);
			if (strcmp(buffer, "{KEY:1337}") == 0)
			{
				if (logger)
					printf("[TCPmain][recvMessage] Rasp logged on!\n");
				id = 0;
				break;
			}
			else if (strcmp(buffer, "{KEY:9090}") == 0)
			{
				if (logger)
					printf("[TCPmain][recvMessage] ARDUINO 1 logged on!\n");
				id = 1;
				break;
			}
			else if (strcmp(buffer, "{KEY:2424}") == 0)
			{
				if (logger)
					printf("[TCPmain][recvMessage] ARDUNIO2 1 logged on!\n");
				id = 2;
				break;
			}
		}
	}
	free(buffer);
	ConnSets.updateKey(*conn, id);
	
	globalUpdater = ConnSets.getQueue(*conn);
	settings = ConnSets.getsetPointer(*conn);
	dataPointer = ConnSets.getData(*conn);
	for (;;)
	{
		buffer = malloc(bufferSize);
		memset(buffer, 0, bufferSize);
		bytesRcvd = recv(clientSock, buffer, bufferSize - 1, 0);
		if (bytesRcvd > 5)
		{
			if (logger)
				printf("[TCPmain][recvMessage] id: %d message recvd: %s\n", id, buffer);
			tcpHandleInput(&buffer, &clientSock, id);
			if (logger)
				printf("[TCPmain][recvMessage] message handled\n");
		}
		free(buffer);
	}
}

/**********************************************************************
 * Moved from FPGAhandler to increase speed of UDP transmission System*
 *********************************************************************/
 
void tcpHandleInput(char** buffer, int *clientSock, int id)
{
	char* s1 = malloc(bufferSize/2);
	memset(s1, 0, bufferSize/2);
	char* s2 = malloc(bufferSize/2);
	memset(s2, 0, bufferSize/2);
	tcpSplitString(buffer, &s1, &s2);
	if (s1[0] != '\0')
	{
		TCPhandleString(s1, s2, clientSock, id);
	}
	free(s1);
	free(s2);
}
/*************************************************************
* Takes a string and split in into two substrings.           *
* input is the original string in the format {s1:s2}         *
* s1 is the identifier                                       *
* s2 is the value to handled                                 *
* DEPRECRATION IN PROCESS, REWRITE TO USE MANUAL INDEXING    *
*************************************************************/
void tcpSplitString(char** input, char** s1, char** s2)
{
	int index = tcpCount_illegalChars(*input);
	if (index > 2)
	{
		*s1[0] = '\0';
		*s2[0] = '\0';
		return;
	}
	int index2;
	char* split;
	split = strchr(*input, ':');
	if (split != NULL)
	{
		index = split - *input;
		strncpy(*s1, *input + 1, index - 1);
		split = strchr(*input, '}');
		if (split != NULL && index > 1)
		{
			index2 = split - *input;
			strncpy(*s2, *input + index + 1, index2 - index - 1);
			split = strchr(*input, '[');
		}
		else
		{
			*s1[0] = '\0';
			*s2[0] = '\0';
			return;
		}
	}
	else
	{
		*s1[0] = '\0';
		*s2[0] = '\0';
		return;
	}
}

/******************************************************************************
 * Takes a series of strings and uses them to figure out what the request was *
 * The data is then transmitted accordingly                                   *
 *****************************************************************************/
void TCPhandleString(char* s1, char* s2, int* clientSock, int id)
{
	char* buffer = malloc(bufferSize);
	memset(buffer, 0, bufferSize);
	if (logger)
	{
		printf("[TCPmain][TCPhandleString] Handling string requests %s and %s\n", s1, s2);
	}
	if (strcmp(s1, "REQ") == 0)
	{
		if (strcmp(s2, "ALL") == 0)
		{	
			sprintf(buffer, "{BER:%d}\n", SettingsSets.getBER(settings));
			send(*clientSock, buffer, strlen(buffer), 0);
			memset(buffer, 0, bufferSize);
			sprintf(buffer, "{ERR:%d}\n", DataSets.getErrors(dataPointer[id]));
			send(*clientSock, buffer, strlen(buffer), 0);
			memset(buffer, 0, bufferSize);
			sprintf(buffer, "{SYN:%d}\n", SettingsSets.getSYNC(settings));
			send(*clientSock, buffer, strlen(buffer), 0);
			memset(buffer, 0, bufferSize);
			sprintf(buffer, "{UTI:%d}\n", SettingsSets.getUTIL(settings));
			send(*clientSock, buffer, strlen(buffer), 0);
			memset(buffer, 0, bufferSize);
			sprintf(buffer, "{MOD:%d}\n", SettingsSets.getMOD(settings));
			send(*clientSock, buffer, strlen(buffer), 0);
			free(buffer);
			return;
		}
		else if (strcmp(s2, "DAT") == 0)
		{	
			sprintf(buffer, "{BER:%d}\n", SettingsSets.getBER(settings));
			send(*clientSock, buffer, strlen(buffer), 0);
			memset(buffer, 0, bufferSize);
			sprintf(buffer, "{ERR:%d}\n", DataSets.getErrors(dataPointer[id]));
			send(*clientSock, buffer, strlen(buffer), 0);
			memset(buffer, 0, bufferSize);
			sprintf(buffer, "{SYN:%d}\n", SettingsSets.getSYNC(settings));
			send(*clientSock, buffer, strlen(buffer), 0);
			memset(buffer, 0, bufferSize);
			sprintf(buffer, "{UTI:%d}\n", SettingsSets.getUTIL(settings));
			send(*clientSock, buffer, strlen(buffer), 0);
			free(buffer);
			return;
		}
		else if (strcmp(s2, "SET") == 0)
		{	
			sprintf(buffer, "{MOD:%d}\n", SettingsSets.getMOD(settings));
			send(*clientSock, buffer, strlen(buffer), 0);
			free(buffer);
			return;
		}
		else if (strcmp(s2, "BER") == 0)
		{	
			sprintf(buffer, "{BER:%d}\n",  SettingsSets.getBER(settings));
			send(*clientSock, buffer, strlen(buffer), 0);
			free(buffer);
			return;
		}
		else if (strcmp(s2, "SYN") == 0)
		{	
			sprintf(buffer, "{SYN:%d}\n",  SettingsSets.getSYNC(settings));
			send(*clientSock, buffer, strlen(buffer), 0);
			free(buffer);
			return;
		}
		else if (strcmp(s2, "UTI") == 0)
		{	
			sprintf(buffer, "{UTI:%d}\n",  SettingsSets.getUTIL(settings));
			send(*clientSock, buffer, strlen(buffer), 0);
			free(buffer);
			return;
		}
		else if (strcmp(s2, "MOD") == 0)
		{	
			sprintf(buffer, "{MOD:%d}\n",  SettingsSets.getMOD(settings));
			send(*clientSock, buffer, strlen(buffer), 0);
			free(buffer);
			return;
		}
		else if (strcmp(s2, "ERR") == 0)
		{	
			sprintf(buffer, "{ERR:%d}\n", DataSets.getErrors(dataPointer[id]));
			send(*clientSock, buffer, strlen(buffer), 0);
			free(buffer);
			return;
		}
		else
		{
			sprintf(buffer, "{NUL:NUL}\n");
			send(*clientSock, buffer, 10, 0);
			free(buffer);
			return;
		}
	}
	if (strcmp(s1, "MOD") == 0)
	{
		if (logger)
		{
			printf("[TCPmain][TCPhandleString] Handling MOD.\n");
		}
		int value = atoi(s2);
		if (value >= 0 && value <= 2)
		{
			writeMod(value);
			sprintf(buffer, "{MOD:%d}\n", value);
			StsQueue.push(*globalUpdater, buffer);
			return;
		}
		else
		{
			if (logger)
			{
				printf("[TCPmain][TCPhandleString] handling ILLEGAL mod.\n");
			}
			sprintf(buffer, "{MOD:NUL}\n");
			send(*clientSock, buffer, strlen(buffer), 0);
			free(buffer);
			return;
		}
	}
	else
	{
		sprintf(buffer, "{NUL:NUL}\n");
		send(*clientSock, buffer, 10, 0);
		free(buffer);
		return;
	}
}

