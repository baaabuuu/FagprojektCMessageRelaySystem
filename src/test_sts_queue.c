#include "../inc/sts_queue.h"
#include "../inc/dataStructs.h"
#include "../inc/tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>


int testCreate()
{
	StsHeader* emptyQueue = StsQueue.create();
	return assertTrue(emptyQueue != NULL);
}

double speedTestCreate()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	while (counter < times)
	{
		clock_t c0 = clock();
		StsHeader* emptyQueue = StsQueue.create();
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		StsQueue.destroy(emptyQueue);
		counter++;
	}
	return timeDiff/times;
}

int testDestroy()
{
	StsHeader* emptyQueue = StsQueue.create();
	StsQueue.push(emptyQueue, "hello world");
	StsQueue.destroy(emptyQueue);
	return assertTrue(StsQueue.pop(emptyQueue) == NULL);
}

double speedTestDestroy()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	
	while (counter < times)
	{
		StsHeader* emptyQueue = StsQueue.create();
		StsQueue.push(emptyQueue, "hello world");
		clock_t c0 = clock();
		StsQueue.destroy(emptyQueue);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	return timeDiff/times;
}

int testIsEmpty()
{
	StsHeader* emptyQueue = StsQueue.create();
	int result = assertTrue(StsQueue.pop(emptyQueue) == NULL);
	StsQueue.destroy(emptyQueue);
	return result;	
}

int testHasMorethan1()
{
	StsHeader* emptyQueue = StsQueue.create();
	char* string1 = "testString";
	StsQueue.push(emptyQueue, string1);
	StsQueue.push(emptyQueue, "test2");
	int result = assertString(StsQueue.pop(emptyQueue), string1);
	StsQueue.destroy(emptyQueue);
	return result;
}

double speedTestPush()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	StsHeader* emptyQueue = StsQueue.create();
	char* string1 = "testString";
	while (counter < times)
	{
		clock_t c0 = clock();
		StsQueue.push(emptyQueue, string1);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	StsQueue.destroy(emptyQueue);
	return timeDiff/times;
}

double speedTestPop()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	StsHeader* emptyQueue = StsQueue.create();
	char* string1 = "testString";
	while (counter < times)
	{
		StsQueue.push(emptyQueue, string1);
		clock_t c0 = clock();
		StsQueue.pop(emptyQueue);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	StsQueue.destroy(emptyQueue);
	return timeDiff/times;
}

int testHasData()
{
	StsHeader* emptyQueue = StsQueue.create();
	char* string1 = "testString";
	StsQueue.push(emptyQueue, string1);
	int result = assertString(StsQueue.pop(emptyQueue), string1);
	StsQueue.destroy(emptyQueue);
	return result;
}

void* threadTest2(void *arg)
{
	udpHandler* videoSetup = arg;
	StsHeader* emptyQueue = videoSetup -> queue;
	StsQueue.pop(emptyQueue);
	StsQueue.push(emptyQueue, "HELLO WORLD");
}

int threadTest1()
{
	udpHandler videoHandler;
	videoHandler.queue = StsQueue.create();
	StsQueue.push(videoHandler.queue, "hello\0");
	pthread_t dummyThread;
	pthread_create(&dummyThread, NULL, threadTest2, &videoHandler);
	pthread_join(dummyThread, NULL);
	int result = assertString(StsQueue.pop(videoHandler.queue), "HELLO WORLD");
	StsQueue.destroy(videoHandler.queue);
	return assertTrue(result);
}



void StsQueueTest()
{
	printf("Testing			\x1B[33m StsQueue			\x1B[36m result\x1B[0m\n");
	printf("testCreate						%s\x1b[0m\n", getResult(testCreate()));
	printf("speedTestCreate					 	 %lf\x1b[0m\n", speedTestCreate());
	printf("testDestroy						%s\x1b[0m\n", getResult(testDestroy()));
	printf("speedTestDestroy					 %lf\x1b[0m\n", speedTestDestroy());
	
	printf("testIsEmpty						%s\x1b[0m\n", getResult(testIsEmpty()));
	printf("testHasData						%s\x1b[0m\n", getResult(testHasData()));
	printf("testHasMorethan1					%s\x1b[0m\n", getResult(testHasMorethan1()));
	
	printf("speedTestPush						 %lf\x1b[0m\n", speedTestPush());
	printf("speedTestPop						 %lf\x1b[0m\n", speedTestPop());
	printf("ThreadTest						%s\x1b[0m\n", getResult(threadTest1()));
	
	

	
	char* string = malloc(sizeof(char) * 30);
	memset(string, 0, sizeof(char) * 30);
	getTestErrors(&string);
	printf("Tests failed: 						%s\x1b[0m\n", string);
	free(string);
	resetTestErrors();
}



void runStsQueueTest()
{
	StsQueueTest();
}
