
/*
 * Communications device
 */
#ifndef _UDPCOMM_H_
#define _UDPCOMM_H_
#include "errors.h"
#include "UDPmain.h"
#include "sts_queue.h"

int UDPcomm_init(int addr);
int UDPcomm_send(int dst, unsigned char* data, int len);
int UDPcomm_recv(int *src, char* data);

#endif // _COMM_H_
