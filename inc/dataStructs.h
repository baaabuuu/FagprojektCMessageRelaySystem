/*
 * dataStructs.h
 *
 *  Created on: April 1, 2018
 *      Author: Patrick
 */
#include <sys/socket.h>
#include <arpa/inet.h>
#include "sts_queue.h"
#include "dataValues.h"
#include "setsValues.h"
#include <pthread.h>

#ifndef _DATASTRUCTS_H_
#define _DATASTRUCTS_H_
/******************************************************************
 * Big one, for sending updates to all the TCP threads at once.   *
 * Also takes the data from the FPGA thread                       *
 * Input is defined as the data recieved from the update in FPGA  *
 * Ouput is defined as the data sent to the FPGA to be handled    *
 *****************************************************************/
typedef struct
{
	DataValues** dataPointer;
	SetsValues* settings;
} tcpGlobal;

/*************************************************************************
 * Data is received or sent to the UDP depending on which side we are on.*
 * Use the UDP_IS_Reciever flag to set the side by adding -t to the call *
 * of main.                                                              *
 ************************************************************************/
typedef struct
{
    StsHeader* queue;
	int side;
} udpHandler;

/*************************************************************************
 * Utilized for setting up the server, maximum connections etc.          *
 ************************************************************************/
typedef struct
{
    int sock;
	struct sockaddr_in serverAddr;
	int maxConnections;
} tcpSETUPvars;

/********************************************************************
 * Video is reserved for the video data stream                      *
 * dataPointer is reserved for the individual data updates size is 3*
 * settingStruct is used to setup settings                          *
 *******************************************************************/
typedef struct
{
	StsHeader** video;
	DataValues** dataPointer;
	SetsValues* settings;
	int UDP_side;
} fpgaHandler;

/*******************************************************************
 * Listener thread setup variables, the server socket and a queue  *
 * pointer to get the data from the thread to the main.            *
 ******************************************************************/
typedef struct
{
	StsHeader** newConnection;
	int serverSocket;
} listenStruct;

#endif // _DATASTRUCTS_H_
