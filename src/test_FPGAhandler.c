#include "../inc/FPGAhandler.h"
#include "../inc/tests.h"
#include "../inc/errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * dataValues.c
 *
 *  Created on: April 19, 2018
 *      Author: Patrick
 */

void FPGAHandlertest()
{
	printf("Testing			\x1B[33m FPGAhandler			\x1B[36m result\x1B[0m\n");

	
	char* string = malloc( sizeof(char) * 30);
	memset(string, 0, sizeof(char) * 30);
	getTestErrors(&string);
	printf("Tests failed: 						%s\x1b[0m\n", string);
	free(string);
	resetTestErrors();
}


void runFPGAHandlertest()
{
	FPGAHandlertest();
}
