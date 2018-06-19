#include "../inc/TCPmain.h"
#include "../inc/tests.h"
#include "../inc/errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

int testtcpSplitString1()
{
	char* string = "{hello:world}";
	char* s1 = calloc(50, sizeof(char));
	char* s2 = calloc(50, sizeof(char));
	tcpSplitString(&string, &s1, &s2);
	int test = (strcmp(s1, "hello") == 0) && (strcmp(s2, "world") == 0);
	free(s1);
	free(s2);
	return assertTrue(test);
}

int testtcpSplitString2()
{
	char* string = "hello::world}";
	char* s1 = calloc(50, sizeof(char));
	char* s2 = calloc(50, sizeof(char));
	tcpSplitString(&string, &s1, &s2);
	int test = (s1[0] == '\0') && (s2[0] == '\0');
	free(s1);
	free(s2);
	return assertTrue(test);
}

int testtcpSplitString3()
{
	char* string = "{hello:";
	char* s1 = calloc(50, sizeof(char));
	char* s2 = calloc(50, sizeof(char));
	tcpSplitString(&string, &s1, &s2);
	int test = (s1[0] == '\0') && (s2[0] == '\0');
	free(s1);
	free(s2);
	return assertTrue(test);
}

int testtcpSplitString4()
{
	char* string = "{hello::";
	char* s1 = calloc(50, sizeof(char));
	char* s2 = calloc(50, sizeof(char));
	tcpSplitString(&string, &s1, &s2);
	int test = (s1[0] == '\0') && (s2[0] == '\0');
	free(s1);
	free(s2);
	return assertTrue(test);
}

double speedTesttcpSplitString()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	char* string = "{hello:world}";
	while (counter < times)
	{
		char* s1 = calloc(50, sizeof(char));
		char* s2 = calloc(50, sizeof(char));
		clock_t c0 = clock();
		tcpSplitString(&string, &s1, &s2);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		free(s1);
		free(s2);
		counter++;
		
	}
	return timeDiff/times;
}

int testtcpCount_illegalChars1()
{
	char* input = "{HACK:HACK:realmsg}";
	int count = tcpCount_illegalChars(input);
	return assertEquals(count, 3);
}

int testtcpCount_illegalChars2()
{
	char* input = ":";
	int count = tcpCount_illegalChars(input);
	return assertEquals(count, 1);
}

int testtcpCount_illegalChars3()
{
	char* input = "}}";
	int count = tcpCount_illegalChars(input);
	return assertEquals(count, 2);
}

int testtcpCount_illegalChars4()
{
	char* input = "helloWorld";
	int count = tcpCount_illegalChars(input);
	return assertEquals(count, 0);
}


double speedTesttcpCount_illegalChars()
{
	int counter = 0;
	double timeDiff = 0;
	int times = 1000;
	char* input = "{HELLO:WORLD}";
	while (counter < times)
	{
		clock_t c0 = clock();
		tcpCount_illegalChars(input);
		clock_t c1 = clock();
		timeDiff += (c1 - c0) * 1000. / CLOCKS_PER_SEC;
		counter++;
	}
	return timeDiff/times;
}




void TCPmainTest()
{
	printf("Testing			\x1B[33m TCPmain			\x1B[36m result\x1B[0m\n");
	
	printf("testtcpSplitString1					%s\x1b[0m\n", getResult(testtcpSplitString1()));
	printf("testtcpSplitString2					%s\x1b[0m\n", getResult(testtcpSplitString2()));
	printf("testtcpSplitString3					%s\x1b[0m\n", getResult(testtcpSplitString3()));
	printf("testtcpSplitString4					%s\x1b[0m\n", getResult(testtcpSplitString4()));
	printf("speedTesttcpSplitString					 %lf\x1b[0m\n", speedTesttcpSplitString());
	
	printf("testtcpCount_illegalChars1				%s\x1b[0m\n", getResult(testtcpCount_illegalChars1()));
	printf("testtcpCount_illegalChars2				%s\x1b[0m\n", getResult(testtcpCount_illegalChars2()));
	printf("testtcpCount_illegalChars3				%s\x1b[0m\n", getResult(testtcpCount_illegalChars3()));
	printf("testtcpCount_illegalChars4				%s\x1b[0m\n", getResult(testtcpCount_illegalChars4()));
	printf("speedTesttcpCount_illegalChars				 %lf\x1b[0m\n", speedTesttcpCount_illegalChars());

	
	char* string = malloc( sizeof(char) * 30);
	memset(string, 0, sizeof(char) * 30);
	getTestErrors(&string);
	printf("Tests failed: 						%s\x1b[0m\n", string);
	free(string);
	resetTestErrors();
}

void runTCPmainTest()
{
	TCPmainTest();
}
