
#include "../inc/dataStructs.h"

#include "../inc/dataValues.h"
#include "../inc/connValues.h"
#include "../inc/setsValues.h"

#include "../inc/sts_queue.h"
#include "../inc/tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <pthread.h>

/*
 * test_dataValues.c
 *
 *  Created on: June 15, 2018
 *      Author: Patrick
 */

int testCreateConn()
{
	ConnValues* data = ConnSets.create();
	int value = assertTrue(data != NULL);
	ConnSets.destroy(data);
	return assertTrue(value);
}

double speedTestCreateConn()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	while (counter < times)
	{
		clock_t c0 = clock();
		ConnValues* data = ConnSets.create();
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		ConnSets.destroy(data);
		counter++;
	}
	return timeDiff/times;
}
//////////////////////////////////////////////////////////////////////
int testDestroyConn()
{
	ConnValues* data = ConnSets.create();
	ConnSets.destroy(data);
	return assertTrue(data == data);
}

double speedTestDestroyConn()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	while (counter < times)
	{
		ConnValues* data = ConnSets.create();
		clock_t c0 = clock();
		ConnSets.destroy(data);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	return timeDiff/times;
}
//////////////////////////////////////////////////////////////////


int testGetKey()
{
	ConnValues* data = ConnSets.create();
	int result = assertEquals(ConnSets.getKey(data), -1);
	ConnSets.destroy(data);
	return result;	
}

double speedTestGetKey()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	ConnValues* data = ConnSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		ConnSets.getKey(data);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	ConnSets.destroy(data);
	return timeDiff/times;
}

int testUpdateKey()
{
	ConnValues* data = ConnSets.create();
	ConnSets.updateKey(data, 5);
	int result = assertEquals(ConnSets.getKey(data), 5);
	ConnSets.destroy(data);
	return result;	
}

double speedTestUpdateKey()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	ConnValues* data = ConnSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		ConnSets.updateKey(data, 5);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	ConnSets.destroy(data);
	return timeDiff/times;
}

void* threadTestKey2(void *arg)
{
	ConnValues* data = arg;
	ConnSets.updateKey(data, 5);
}

int threadTestGetKey1()
{
	ConnValues* data = ConnSets.create();
	pthread_t dummyThread;
	pthread_create(&dummyThread, NULL, threadTestKey2, data);
	pthread_join(dummyThread, NULL);
	int result = assertEquals(ConnSets.getKey(data), 5);
	ConnSets.destroy(data);
	return assertTrue(result);
}
/////////////////////////////////////////////////////////////


int testGetSocket()
{
	ConnValues* data = ConnSets.create();
	int result = assertEquals(*ConnSets.getSocket(data), -1);
	ConnSets.destroy(data);
	return result;	
}

double speedTestGetSocket()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	ConnValues* data = ConnSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		ConnSets.getSocket(data);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	ConnSets.destroy(data);
	return timeDiff/times;
}

int testUpdateSocket()
{
	ConnValues* data = ConnSets.create();
	int tempValue = 1;
	ConnSets.updateSocket(data, &tempValue);
	int result = assertEquals(*ConnSets.getSocket(data), 1);
	ConnSets.destroy(data);
	return result;	
}

double speedTestUpdateSocket()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	ConnValues* data = ConnSets.create();
	int tempValue = 1;
	while (counter < times)
	{
		clock_t c0 = clock();
		ConnSets.updateSocket(data, &tempValue);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	ConnSets.destroy(data);
	return timeDiff/times;
}

void* threadTestGetSocket2(void *arg)
{
	ConnValues* data = arg;
	int tempVal = 5;
	ConnSets.updateSocket(data, &tempVal);
}

int threadTestGetSocket1()
{
	ConnValues* data = ConnSets.create();
	pthread_t dummyThread;
	pthread_create(&dummyThread, NULL, threadTestGetSocket2, data);
	pthread_join(dummyThread, NULL);
	int result = assertEquals(*ConnSets.getSocket(data), *ConnSets.getSocket(data));
	ConnSets.destroy(data);
	return assertTrue(result);
}

///////////////////////////////////////////////////////////////////////
int testGetisConnected()
{
	ConnValues* data = ConnSets.create();
	int result = assertEquals(ConnSets.getisConnected(data), 0);
	ConnSets.destroy(data);
	return result;	
}

double speedTestgetisConnected()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	ConnValues* data = ConnSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		ConnSets.getisConnected(data);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	ConnSets.destroy(data);
	return timeDiff/times;
}

int testupdateIsConnected()
{
	ConnValues* data = ConnSets.create();
	ConnSets.updateisConnected(data, 1);
	int result = assertEquals(ConnSets.getisConnected(data), 1);
	ConnSets.destroy(data);
	return result;	
}

double speedTestupdateIsConnected()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	ConnValues* data = ConnSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		ConnSets.updateisConnected(data, 1);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	ConnSets.destroy(data);
	return timeDiff/times;
}

void* threadTestupdateIsConnected2(void *arg)
{
	ConnValues* data = arg;
	ConnSets.updateisConnected(data, 1);
}

int threadTestupdateIsConnected1()
{
	ConnValues* data = ConnSets.create();
	pthread_t dummyThread;
	pthread_create(&dummyThread, NULL, threadTestupdateIsConnected2, data);
	pthread_join(dummyThread, NULL);
	int result = assertEquals(ConnSets.getisConnected(data), 1);
	ConnSets.destroy(data);
	return assertTrue(result);
}

///////////////////////////////////////////////////////////////////////


int testGetThread()
{
	ConnValues* data = ConnSets.create();
	int result = assertTrue(ConnSets.getThread(data) == NULL);
	ConnSets.destroy(data);
	return result;	
}

double speedTestGetThread()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	ConnValues* data = ConnSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		ConnSets.getThread(data);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	ConnSets.destroy(data);
	return timeDiff/times;
}
int testupdateThread()
{
	ConnValues* data = ConnSets.create();
	pthread_t dummyThread;
	ConnSets.updateThread(data, &dummyThread);
	int result = assertTrue(ConnSets.getThread(data) == &dummyThread);
	ConnSets.destroy(data);
	return result;	
}

double speedTestupdateThread()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	ConnValues* data = ConnSets.create();
	pthread_t dummyThread;
	while (counter < times)
	{
		clock_t c0 = clock();
		ConnSets.updateThread(data, &dummyThread);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	ConnSets.destroy(data);
	return timeDiff/times;
}

void* threadTestupdateThread2(void *arg)
{
	ConnValues* data = arg;
	pthread_t dummyThread;
	ConnSets.updateThread(data, &dummyThread);
}

int threadTestupdateThread1()
{
	ConnValues* data = ConnSets.create();
	pthread_t dummyThread;
	pthread_create(&dummyThread, NULL, threadTestupdateThread2, data);
	pthread_join(dummyThread, NULL);
	int result = assertTrue(ConnSets.getThread(data) != NULL);
	ConnSets.destroy(data);
	return assertTrue(result);
}

///////////////////////////////////////////////////////////////////////

int testgetQueue()
{
	ConnValues* data = ConnSets.create();
	int result = assertTrue(ConnSets.getQueue(data) == NULL);
	ConnSets.destroy(data);
	return result;	
}

double speedTestgetQueue()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	ConnValues* data = ConnSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		ConnSets.getQueue(data);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	ConnSets.destroy(data);
	return timeDiff/times;
}

int testupdateQueue()
{
	StsHeader* queue = StsQueue.create();
	ConnValues* data = ConnSets.create();
	ConnSets.updateQueue(data, &queue);
	int result = assertTrue(ConnSets.getQueue(data) == &queue);
	ConnSets.destroy(data);
	StsQueue.destroy(queue);
	return result;
}

double speedTestupdateQueue()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	ConnValues* data = ConnSets.create();
	StsHeader* queue = StsQueue.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		ConnSets.updateQueue(data, &queue);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	StsQueue.destroy(queue);
	ConnSets.destroy(data);
	return timeDiff/times;
}

void* threadTestupdateQueue2(void *arg)
{
	StsHeader* queue = StsQueue.create();
	ConnValues* data = arg;
	ConnSets.updateQueue(data, &queue);
}

int threadTestupdateQueue1()
{
	ConnValues* data = ConnSets.create();
	pthread_t dummyThread;
	pthread_create(&dummyThread, NULL, threadTestupdateQueue2, data);
	pthread_join(dummyThread, NULL);
	int result = assertTrue(ConnSets.getQueue(data) != NULL);
	ConnSets.destroy(data);
	return assertTrue(result);
}

///////////////////////////////////////////////////////////////////////

int testgetsetPointer()
{
	ConnValues* data = ConnSets.create();
	int result = assertTrue(ConnSets.getsetPointer(data) == NULL);
	ConnSets.destroy(data);
	return result;	
}

double speedTestgetsetPointer()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	ConnValues* data = ConnSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		ConnSets.getsetPointer(data);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	ConnSets.destroy(data);
	return timeDiff/times;
}

int testupdatesetPointer()
{
	SetsValues* sets = SettingsSets.create();
	ConnValues* data = ConnSets.create();
	ConnSets.updatesetPointer(data, sets);
	int result = assertTrue(ConnSets.getsetPointer(data) == sets);
	ConnSets.destroy(data);
	SettingsSets.destroy(sets);
	return result;
}

double speedTestsetPointer()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	ConnValues* data = ConnSets.create();
	SetsValues* sets = SettingsSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		ConnSets.updatesetPointer(data, sets);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	SettingsSets.destroy(sets);
	ConnSets.destroy(data);
	return timeDiff/times;
}

void* threadTestPointer2(void *arg)
{
	SetsValues* sets = SettingsSets.create();
	ConnValues* data = arg;
	ConnSets.updatesetPointer(data, sets);
}

int threadTestPointer1()
{
	ConnValues* data = ConnSets.create();
	pthread_t dummyThread;
	pthread_create(&dummyThread, NULL, threadTestPointer2, data);
	pthread_join(dummyThread, NULL);
	int result = assertTrue(ConnSets.getsetPointer(data) != NULL);
	SettingsSets.destroy(ConnSets.getsetPointer(data));
	ConnSets.destroy(data);
	
	return assertTrue(result);
}

///////////////////////////////////////////////////////////////////////

int testgetData()
{
	ConnValues* data = ConnSets.create();
	int result = assertTrue(ConnSets.getData(data) == NULL);
	ConnSets.destroy(data);
	return result;	
}

double speedTestgetData()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	ConnValues* data = ConnSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		ConnSets.getData(data);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	ConnSets.destroy(data);
	return timeDiff/times;
}

int testupdateupdateData()
{
	DataValues** values;
	DataValues** values2;
	ConnValues* data = ConnSets.create();
	ConnSets.updateData(data, values);
	values2 = ConnSets.getData(data);
	int result = assertTrue(values2 == values);
	ConnSets.destroy(data);
	return result;
}

double speedTestupdateData()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	ConnValues* data = ConnSets.create();
	DataValues** values;
	while (counter < times)
	{
		clock_t c0 = clock();
		ConnSets.updateData(data, values);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	ConnSets.destroy(data);
	return timeDiff/times;
}

void* threadTestData2(void *arg)
{
	DataValues** values = malloc(sizeof(values));
	ConnValues* data = arg;
	ConnSets.updateData(data, values);
}

int threadTestData1()
{
	ConnValues* data = ConnSets.create();
	pthread_t dummyThread;
	pthread_create(&dummyThread, NULL, threadTestData2, data);
	pthread_join(dummyThread, NULL);
	int result = assertTrue(ConnSets.getData(data) != NULL);
	free(ConnSets.getData(data));
	ConnSets.destroy(data);
	
	return assertTrue(result);
}

///////////////////////////////////////////////////////////////////////

void connValueTests()
{
	printf("Testing			\x1B[33m connValues			\x1B[36m result\x1B[0m\n");
	
	printf("testCreateConn						%s\x1b[0m\n", getResult(testCreateConn()));
	printf("speedTestCreateConn					 %lf\x1b[0m\n", speedTestCreateConn());
	
	printf("testDestroyConn						%s\x1b[0m\n", getResult(testDestroyConn()));
	printf("speedTestDestroyConn				  	 %lf\x1b[0m\n", speedTestDestroyConn());
	
	printf("testGetKey						%s\x1b[0m\n", getResult(testGetKey()));
	printf("speedTestGetKey						 %lf\x1b[0m\n", speedTestGetKey());
	printf("testUpdateKey						%s\x1b[0m\n", getResult(testUpdateKey()));
	printf("speedTestUpdateKey					 %lf\x1b[0m\n", speedTestUpdateKey());
	printf("threadTestGetKey1					%s\x1b[0m\n", getResult(threadTestGetKey1()));
	
	printf("testGetSocket						%s\x1b[0m\n", getResult(testGetSocket()));
	printf("speedTestGetSocket					 %lf\x1b[0m\n", speedTestGetSocket());
	printf("testUpdateSocket					%s\x1b[0m\n", getResult(testUpdateSocket()));
	printf("speedTestUpdateSocket					 %lf\x1b[0m\n", speedTestUpdateSocket());
	printf("threadTestGetSocket1					%s\x1b[0m\n", getResult(threadTestGetSocket1()));
	
	printf("testGetThread						%s\x1b[0m\n", getResult(testGetThread()));
	printf("speedTestGetThread					 %lf\x1b[0m\n", speedTestGetThread());
	printf("testupdateThread					%s\x1b[0m\n", getResult(testupdateThread()));
	printf("speedTestupdateThread					 %lf\x1b[0m\n", speedTestupdateThread());
	printf("threadTestupdateThread1					%s\x1b[0m\n", getResult(threadTestupdateThread1()));
	
	printf("testgetQueue						%s\x1b[0m\n", getResult(testgetQueue()));
	printf("speedTestgetQueue					 %lf\x1b[0m\n", speedTestgetQueue());
	printf("testupdateQueue						%s\x1b[0m\n", getResult(testupdateQueue()));
	printf("speedTestupdateQueue					 %lf\x1b[0m\n", speedTestupdateQueue());
	printf("threadTestupdateQueue1					%s\x1b[0m\n", getResult(threadTestupdateQueue1()));
		
	printf("testgetsetPointer					%s\x1b[0m\n", getResult(testgetsetPointer()));
	printf("speedTestgetsetPointer					 %lf\x1b[0m\n", speedTestgetsetPointer());
	printf("testupdatesetPointer					%s\x1b[0m\n", getResult(testupdatesetPointer()));
	printf("speedTestsetPointer					 %lf\x1b[0m\n", speedTestsetPointer());
	printf("threadTestPointer1					%s\x1b[0m\n", getResult(threadTestPointer1()));
	
	printf("testgetData						%s\x1b[0m\n", getResult(testgetData()));
	printf("speedTestgetData					 %lf\x1b[0m\n", speedTestgetData());
	printf("testupdateupdateData					%s\x1b[0m\n", getResult(testupdateupdateData()));
	printf("speedTestupdateData					 %lf\x1b[0m\n", speedTestupdateData());
	printf("threadTestData1						%s\x1b[0m\n", getResult(threadTestData1()));
	
	
	char* string = malloc( sizeof(char) * 30);
	memset(string, 0, sizeof(char) * 30);
	getTestErrors(&string);
	printf("Tests failed: 						%s\x1b[0m\n", string);
	free(string);
	resetTestErrors();
}


void runconnValuesTest()
{
	connValueTests();
}
