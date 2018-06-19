#include "errors.h"
#include "sts_queue.h"
#include "dataStructs.h"

/*
 * Communications device
 */
#ifndef _TCPmain_H_
#define _TCPmain_H_


void TCPmainFunc(DataValues** data, SetsValues* settingsStruct);
void recvHandleMsg(int* bytesRcvd, char* buffer, StsHeader** queue, int* connecID);
void *recvMessage(void *arg_queue);
void tcpHandleInput(char** buffer, int *clientSock, int id);
void tcpSplitString(char** input, char** s1, char** s2);
void TCPhandleString(char* s1, char* s2, int* clientSock, int id);
int tcpCount_illegalChars(const char* string);
#endif // _TCPmain_H_
