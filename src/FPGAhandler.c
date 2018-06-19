/*
 * UDPmain.c
 *
 *  Created on: Mar 26, 2018
 *      Author: Patrick
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "../inc/hps_linux.h"
#include "../inc/sts_queue.h"
#include "../inc/dataValues.h"
#include "../inc/UDPmain.h"
#include "../inc/UDPcomm.h"

#include "../inc/dataStructs.h"
#include "../inc/errors.h"
#include "../inc/hps_linux.h"

int UDPsendPortSender = 5004;


/*********************************************************
 * Handles the video and passes it on to FPGA board      *
 * Changes the char* to whatever is needed to recv video *
 ********************************************************/ 
void fpgaVideoHandlingRecv(StsHeader** video)
{
	unsigned int videoValue = 0;
	unsigned char* pointer;
	int counter = 0;
	int counter2 = 0;
	for(;;)
	{
		pointer =  StsQueue.pop(*video);
		if (pointer != NULL)
		{
			//data has been recved from UDP
			while (counter < 8)
			{
				videoValue = (videoValue << 8) + pointer[counter++];
				counter2 = (counter2 + 1) % 4;
				if (!counter2)
				{
					writeVideo(videoValue);
				}	
			}
		}
		if (counter2 > 0)
		{
			writeVideo(videoValue);
			counter2 = 0;
		}
		counter = 0;
		free(pointer);
	}	
}

/*************************************************
 * Handles the video and passes it on to NITROGEN*
 ************************************************/ 
void fpgaVideoHandlingTransmit(StsHeader** video)
{
	uint16_t n;
	int counter = 0;
	unsigned char *videoValues = malloc(8 * sizeof(unsigned char));
	memset(videoValues, 0,8 * sizeof(unsigned char));
	for(;;)
	{

		if (canReadVideo())
		{
			n = readVideoFifo();
			videoValues[counter] = (unsigned char) n & 0xFF;;
			counter++;
			videoValues[counter] = (unsigned char) (n >> 8) & 0xFF;
			counter++;
		}
		
		if (counter >= 8)
		{
			counter = 0;
			StsQueue.push(*video, &videoValues);
			int err;
			if ((err = UDPcomm_init(UDPsendPortSender)) != ERR_OK)
			{
				printf("[UDPmain][fpgaVideoHandlingTransmit] Failed to init transmitter\n");
				break;
			}
			printf("[UDPmain][fpgaVideoHandlingTransmit] Transmitting UDP video\n");
			UDPcomm_send(UDPsendPortSender, videoValues, 8);
			free(videoValues);
			break;
		}
	}
}

/********************************************************************************************
 * Main function for FPGA									                                *
 * If SWN9 is toggled it recieves UDP video and transmits to FPGA. otherwise it collects    *
 * and transmits it to NITROGEN6X                                                           *
 *******************************************************************************************/
void fpgaHandling(StsHeader** video, int is_udpReciever)
{
	if (readSWN(9))
	{
		fpgaVideoHandlingRecv(video);
	}
	else
	{
		for(;;)
		{
			fpgaVideoHandlingTransmit(video);
		}
	}
}