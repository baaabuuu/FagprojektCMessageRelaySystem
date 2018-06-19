#ifndef _UDPMAIN_H_
#define _UDPMAIN_H_


#include <string.h>
#include "sts_queue.h"
#include "errors.h"
#include "UDPcomm.h"

#define BUFFER_SIZE 1024

void read_args(int argc, char * argv[]);
int UDPreciever(StsHeader** video);
int UDPtransmitter(StsHeader** video);

#endif // _UDPMAIN_H_
