#include "../inc/dataValues.h"
#include "../inc/dataStructs.h"
#include "../inc/tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>


/*
 * test_dataValues.c
 *
 *  Created on: June 15, 2018
 *      Author: Patrick
 */

int testCreateData()
{
	DataValues* data = DataSets.create();
	int value = assertTrue(data != NULL);
	DataSets.destroy(data);
	return assertTrue(value);
}

double speedTestCreateData()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	while (counter < times)
	{
		clock_t c0 = clock();
		DataValues* data = DataSets.create();
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		DataSets.destroy(data);
		counter++;
	}
	return timeDiff/times;
}

int testDestroyData()
{
	DataValues* data = DataSets.create();
	DataSets.destroy(data);
	return assertTrue(data == data);
}

double speedTestDestroyData()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	while (counter < times)
	{
		DataValues* data = DataSets.create();
		clock_t c0 = clock();
		DataSets.destroy(data);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	return timeDiff/times;
}

int testGetErrors()
{
	DataValues* data = DataSets.create();
	int result = assertEquals(DataSets.getErrors(data), 0);
	DataSets.destroy(data);
	return result;	
}

double speedTestGetErrors()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	DataValues* data = DataSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		DataSets.getErrors(data);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	DataSets.destroy(data);
	return timeDiff/times;
}

int testUpdateErrors1()
{
	DataValues* data = DataSets.create();
	DataSets.updateErrors(data, 5);
	int result = assertEquals(DataSets.getErrors(data), 5);
	DataSets.destroy(data);
	return result;	
}

int testUpdateErrors2()
{
	DataValues* data = DataSets.create();
	DataSets.updateErrors(data, 5);
	DataSets.updateErrors(data, 10);
	int result = assertEquals(DataSets.getErrors(data), 15);
	DataSets.destroy(data);
	return result;	
}

void* threadTestErrors2(void *arg)
{
	DataValues* data = arg;
	DataSets.updateErrors(data, 5);
}

int threadTestErrors1()
{
	
	DataValues* data = DataSets.create();
	pthread_t dummyThread;
	pthread_create(&dummyThread, NULL, threadTestErrors2, data);
	pthread_join(dummyThread, NULL);
	int result = assertEquals(DataSets.getErrors(data), 5);
	DataSets.destroy(data);
	return assertTrue(result);
}


void dataValuesTest()
{
	printf("Testing			\x1B[33m dataValues			\x1B[36m result\x1B[0m\n");
	
	printf("testCreateData						%s\x1b[0m\n", getResult(testCreateData()));
	printf("speedTestCreateData					 %lf\x1b[0m\n", speedTestCreateData());
	
	printf("testDestroyData						%s\x1b[0m\n", getResult(testDestroyData()));
	printf("speedTestDestroyData				  	 %lf\x1b[0m\n", speedTestDestroyData());
	
	printf("testGetErrors						%s\x1b[0m\n", getResult(testGetErrors()));
	printf("speedTestGetErrors					 %lf\x1b[0m\n", speedTestGetErrors());
	
	printf("testUpdateErrors1					%s\x1b[0m\n", getResult(testUpdateErrors1()));
	printf("testUpdateErrors2					%s\x1b[0m\n", getResult(testUpdateErrors2()));
	printf("speedTestGetErrors					 %lf\x1b[0m\n", speedTestGetErrors());

	printf("threadTestErrors					%s\x1b[0m\n", getResult(threadTestErrors1()));
	
	char* string = malloc( sizeof(char) * 30);
	memset(string, 0, sizeof(char) * 30);
	getTestErrors(&string);
	printf("Tests failed: 						%s\x1b[0m\n", string);
	free(string);
	resetTestErrors();
}


void rundataValuesTest()
{
	dataValuesTest();
}
