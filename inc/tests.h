#ifndef _TESTS_H_
#define _TESTS_H_


#define TRUE		(1==1)
#define FALSE		(!TRUE)


void resetTestErrors();
int getTotalErrors();
void getTestErrors();
void getErrors();
char* getResult(int result);
int assertEquals(int value1, int value2);
int assertNotEquals(int value1, int value2);
int assertTrue(int value);
int assertFalse(int value);
int assertString(char* string1, char* string2);
char *randstring(int lengthVal);
#endif // _TESTS_H_