#include "../inc/dataStructs.h"
#include "sts_queue.h"


/*
 * Communications device
 */
#ifndef _TCPCOMM_H_
#define _TCPCOMM_H_


int setupTCP(tcpSETUPvars* setup);
void* listenTCP(void *arg_queue);
int tcpClient();
int TCPserver();

#endif // _TCPCOMM_H_
