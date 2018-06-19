#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/tests.h"

int totalTestErrors = 0;
int testErrors = 0;

void resetTestErrors()
{
	testErrors = 0;
}

int getTotalErrors()
{
	return totalTestErrors;
}

char *randstring(int lengthVal)
{
	size_t length = sizeof(char) * lengthVal;
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";        
    char *randomString = NULL;

    if (length)
	{
        randomString = malloc(sizeof(char) * (length +1));
        if (randomString)
		{            
            for (int n = 0;n < length;n++)
			{            
                int key = rand() % (int)(sizeof(charset) -1);
                randomString[n] = charset[key];
            }
            randomString[length] = '\0';
        }
    }

    return randomString;
}

void getErrors(char** result)
{
	if (totalTestErrors == 0)
	{
		snprintf(*result, sizeof(*result) , "\x1B[32m %d\x1B[0M", totalTestErrors);
	}
	else
	{
		snprintf(*result, sizeof(*result) , "\x1B[31m %d\x1B[0M", totalTestErrors);
	}
}
void getTestErrors(char** result)
{
	if (totalTestErrors == 0)
	{
		snprintf(*result, sizeof(*result) , "\x1B[32m %d\x1B[0M", testErrors);
	}
	else
	{
		snprintf(*result, sizeof(*result) , "\x1B[31m %d\x1B[0M", testErrors);
	}
}

char* getResult(int result)
{
	return (result) ? "\x1B[32m PASSED\x1B[0M" : "\x1B[31m FAILED\x1B[0M";
}



int assertEquals(int value1, int value2)
{
	int result = (value1 == value2) ? TRUE : FALSE;
	if (!result)
	{
		totalTestErrors++;
		testErrors++;
	}
	return result;
}

int assertNotEquals(int value1, int value2)
{
	int result = (value1 != value2) ? TRUE : FALSE;
	if (!result)
	{
		totalTestErrors++;
		testErrors++;
	}
	return result;

}

int assertTrue(int value)
{
	int result = (value) ? TRUE : FALSE;
	if (!result)
	{
		totalTestErrors++;
		testErrors++;
	}
	return result;
}

int assertFalse(int value)
{
	int result = (!value) ? TRUE : FALSE;
	if (!result)
	{
		totalTestErrors++;
		testErrors++;
	}
	return result;
}

int assertString(char* string1, char* string2)
{
	int result = (strcmp(string1, string2) == 0) ? TRUE : FALSE;
	if (!result)
	{
		totalTestErrors++;
		testErrors++;
	}
	return result;
}