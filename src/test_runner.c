#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/tests.h"
#include "../inc/test_sts_queue.h"
#include "../inc/test_FPGAhandler.h"
#include "../inc/test_TCPmain.h"
#include "../inc/test_dataValues.h"
#include "../inc/test_connValues.h"
#include "../inc/test_setsValues.h"

void runTests()
{
	runStsQueueTest();
	runFPGAHandlertest();
	runTCPmainTest();
	rundataValuesTest();
	runconnValuesTest();
	runsetsValuesTests();
	char* string =  malloc(sizeof(char) * 30);
	memset(string, 0, sizeof(char) * 30);
	getErrors(&string);
	printf("Total amount of failed tests:				%s\x1b[0m\n", string);
	free(string);
}
