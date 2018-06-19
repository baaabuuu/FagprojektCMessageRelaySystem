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
 


int testCreateSets()
{
	SetsValues* data = SettingsSets.create();
	int value = assertTrue(data != NULL);
	SettingsSets.destroy(data);
	return value;
}

double speedTestCreateSetts()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	while (counter < times)
	{
		clock_t c0 = clock();
		SetsValues* data = SettingsSets.create();
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		SettingsSets.destroy(data);
		counter++;
	}
	return timeDiff/times;
}
//////////////////////////////////////////////////////////////////////
int testDestroySets()
{
	SetsValues* data = SettingsSets.create();
	SettingsSets.destroy(data);
	return assertTrue(data == data);
}

double speedTestDestroySets()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	while (counter < times)
	{
		SetsValues* data = SettingsSets.create();
		clock_t c0 = clock();
		SettingsSets.destroy(data);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	return timeDiff/times;
}
//////////////////////////////////////////////////////////////////


int testgetBER()
{
	SetsValues* data = SettingsSets.create();
	int result = assertEquals(SettingsSets.getBER(data), 0);
	SettingsSets.destroy(data);
	return result;	
}

double speedTestGetBER()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	SetsValues* data = SettingsSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		SettingsSets.getBER(data);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	SettingsSets.destroy(data);
	return timeDiff/times;
}

int testUpdateBER()
{
	SetsValues* data = SettingsSets.create();
	SettingsSets.updateBER(data, 5);
	int result = assertEquals(SettingsSets.getBER(data), 5);
	SettingsSets.destroy(data);
	return result;	
}

double speedTestUpdateBER()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	SetsValues* data = SettingsSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		SettingsSets.updateBER(data, 5);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	SettingsSets.destroy(data);
	return timeDiff/times;
}

void* threadTestBER2(void *arg)
{
	SetsValues* data = arg;
	SettingsSets.updateBER(data, 5);
}

int threadTestBER1()
{
	SetsValues* data = SettingsSets.create();
	pthread_t dummyThread;
	pthread_create(&dummyThread, NULL, threadTestBER2, data);
	pthread_join(dummyThread, NULL);
	int result = assertEquals(SettingsSets.getBER(data), 5);
	SettingsSets.destroy(data);
	return assertTrue(result);
}
/////////////////////////////////////////////////////////////

int testgetSYNC()
{
	SetsValues* data = SettingsSets.create();
	int result = assertEquals(SettingsSets.getSYNC(data), 0);
	SettingsSets.destroy(data);
	return result;	
}

double speedTestGetSYNC()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	SetsValues* data = SettingsSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		SettingsSets.getSYNC(data);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	SettingsSets.destroy(data);
	return timeDiff/times;
}

int testUpdateSYNC()
{
	SetsValues* data = SettingsSets.create();
	SettingsSets.updateSYNC(data, 5);
	int result = assertEquals(SettingsSets.getSYNC(data), 5);
	SettingsSets.destroy(data);
	return result;	
}

double speedTestUpdateSYNC()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	SetsValues* data = SettingsSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		SettingsSets.updateSYNC(data, 5);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	SettingsSets.destroy(data);
	return timeDiff/times;
}

void* threadTestSYNC2(void *arg)
{
	SetsValues* data = arg;
	SettingsSets.updateSYNC(data, 5);
}

int threadTestSYNC1()
{
	SetsValues* data = SettingsSets.create();
	pthread_t dummyThread;
	pthread_create(&dummyThread, NULL, threadTestSYNC2, data);
	pthread_join(dummyThread, NULL);
	int result = assertEquals(SettingsSets.getSYNC(data), 5);
	SettingsSets.destroy(data);
	return assertTrue(result);
}

/////////////////////////////////////////////////////////////


int testgetMOD()
{
	SetsValues* data = SettingsSets.create();
	int result = assertEquals(SettingsSets.getMOD(data), 0);
	SettingsSets.destroy(data);
	return result;	
}

double speedTestGetMOD()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	SetsValues* data = SettingsSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		SettingsSets.getMOD(data);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	SettingsSets.destroy(data);
	return timeDiff/times;
}

int testUpdateMOD()
{
	SetsValues* data = SettingsSets.create();
	SettingsSets.updateMOD(data, 5);
	int result = assertEquals(SettingsSets.getMOD(data), 5);
	SettingsSets.destroy(data);
	return result;	
}

double speedTestUpdateMOD()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	SetsValues* data = SettingsSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		SettingsSets.updateMOD(data, 5);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	SettingsSets.destroy(data);
	return timeDiff/times;
}

void* threadTestMOD2(void *arg)
{
	SetsValues* data = arg;
	SettingsSets.updateMOD(data, 5);
}

int threadTestMOD1()
{
	SetsValues* data = SettingsSets.create();
	pthread_t dummyThread;
	pthread_create(&dummyThread, NULL, threadTestMOD2, data);
	pthread_join(dummyThread, NULL);
	int result = assertEquals(SettingsSets.getMOD(data), 5);
	SettingsSets.destroy(data);
	return assertTrue(result);
}

/////////////////////////////////////////////////////////////


int testgetUTIL()
{
	SetsValues* data = SettingsSets.create();
	int result = assertEquals(SettingsSets.getUTIL(data), 0);
	SettingsSets.destroy(data);
	return result;	
}

double speedTestGetUTIL()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	SetsValues* data = SettingsSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		SettingsSets.getUTIL(data);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	SettingsSets.destroy(data);
	return timeDiff/times;
}

int testUpdateUTIL()
{
	SetsValues* data = SettingsSets.create();
	SettingsSets.updateUTIL(data, 5);
	int result = assertEquals(SettingsSets.getUTIL(data), 5);
	SettingsSets.destroy(data);
	return result;	
}

double speedTestUpdateUTIL()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	SetsValues* data = SettingsSets.create();
	while (counter < times)
	{
		clock_t c0 = clock();
		SettingsSets.updateUTIL(data, 5);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	SettingsSets.destroy(data);
	return timeDiff/times;
}

void* threadTestUTIL2(void *arg)
{
	SetsValues* data = arg;
	SettingsSets.updateUTIL(data, 5);
}

int threadTestUTIL1()
{
	SetsValues* data = SettingsSets.create();
	pthread_t dummyThread;
	pthread_create(&dummyThread, NULL, threadTestUTIL2, data);
	pthread_join(dummyThread, NULL);
	int result = assertEquals(SettingsSets.getUTIL(data), 5);
	SettingsSets.destroy(data);
	return assertTrue(result);
}

///////////////////////////////////////////////////////////////////////

void setsValuesTests()
{
	printf("Testing			\x1B[33m setsValues			\x1B[36m result\x1B[0m\n");
	
	printf("testCreateSets						%s\x1b[0m\n", getResult(testCreateSets()));
	printf("speedTestCreateSetts					 %lf\x1b[0m\n", speedTestCreateSetts());
	
	printf("testDestroySets						%s\x1b[0m\n", getResult(testDestroySets()));
	printf("speedTestDestroySets				  	 %lf\x1b[0m\n", speedTestDestroySets());
	
	printf("testgetBER						%s\x1b[0m\n", getResult(testgetBER()));
	printf("speedTestGetBER						 %lf\x1b[0m\n", speedTestUpdateBER());
	printf("testUpdateBER						%s\x1b[0m\n", getResult(testUpdateBER()));
	printf("speedTestUpdateBER					 %lf\x1b[0m\n", speedTestUpdateBER());
	printf("threadTestBER1						%s\x1b[0m\n", getResult(threadTestBER1()));
	
	printf("testgetSYNC						%s\x1b[0m\n", getResult(testgetSYNC()));
	printf("speedTestGetSYNC					 %lf\x1b[0m\n", speedTestGetSYNC());
	printf("testUpdateSYNC						%s\x1b[0m\n", getResult(testUpdateSYNC()));
	printf("speedTestUpdateSYNC					 %lf\x1b[0m\n", speedTestUpdateSYNC());
	printf("threadTestSYNC1						%s\x1b[0m\n", getResult(threadTestSYNC1()));
	
	printf("testgetMOD						%s\x1b[0m\n", getResult(testgetMOD()));
	printf("speedTestGetMOD						 %lf\x1b[0m\n", speedTestGetMOD());
	printf("testUpdateMOD						%s\x1b[0m\n", getResult(testUpdateMOD()));
	printf("speedTestUpdateMOD					 %lf\x1b[0m\n", speedTestUpdateMOD());
	printf("threadTestMOD1						%s\x1b[0m\n", getResult(threadTestMOD1()));
	
	printf("testgetUTIL						%s\x1b[0m\n", getResult(testgetUTIL()));
	printf("speedTestGetUTIL					 %lf\x1b[0m\n", speedTestGetUTIL());
	printf("testUpdateMOD						%s\x1b[0m\n", getResult(testUpdateMOD()));
	printf("speedTestUpdateMOD					 %lf\x1b[0m\n", speedTestUpdateMOD());
	printf("threadTestUTIL1						%s\x1b[0m\n", getResult(threadTestUTIL1()));
	
	
	
	char* string = malloc( sizeof(char) * 30);
	memset(string, 0, sizeof(char) * 30);
	getTestErrors(&string);
	printf("Tests failed: 						%s\x1b[0m\n", string);
	free(string);
	resetTestErrors();
}


void runsetsValuesTests()
{
	setsValuesTests();
}
