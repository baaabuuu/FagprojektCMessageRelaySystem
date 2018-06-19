#include "../inc/dataValues.h"
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

/*
 * dataValues.c
 *
 *  Created on: June 15, 2018
 *      Author: Patrick
 */

/*****************************************************************************
 * DataValues is a struct build for personalized data for each TCP connection*
 * It is build for threads and is therefor threadsafe using mutex locks      *
 ****************************************************************************/
struct DataValues
{
	int errors;
	pthread_mutex_t *mutex;
};

/*********************
 * Creates DataValues*
 ********************/
static DataValues* create();
static DataValues* create()
{
	DataValues *values = malloc(sizeof(*values));
	values -> errors = 0;

	pthread_mutex_t *mutex = malloc(sizeof(*mutex));
	values -> mutex = mutex;
	//pthread_mutex_unlock(mutex);
	return values;
}

/***********************************
 * returns errors and sets it to 0 *
 * THREAD SAFE                     *
 **********************************/
static int getErrors(DataValues *handle);
static int getErrors(DataValues *handle)
{
	pthread_mutex_lock(handle -> mutex);
	int value = handle -> errors;
	handle -> errors = 0;
	pthread_mutex_unlock(handle -> mutex);
	return value;
}

/************************
 * Updates error count  *
 * THREAD SAFE          *
 ***********************/
static void updateErrors(DataValues *handle, int upd);
static void updateErrors(DataValues *handle, int upd)
{
	pthread_mutex_lock(handle -> mutex);
	handle -> errors = (handle -> errors) + upd;
	pthread_mutex_unlock(handle -> mutex);
}

/************************
 * Frees the memory used*
 ***********************/
static void destroy(DataValues *handle);
static void destroy(DataValues *handle)
{
	free(handle -> mutex);
	free(handle);
	handle = NULL;
}


/*******************
 * Accesor methods *
 ******************/
_DataSets const DataSets =
{
	create,
	getErrors,
	updateErrors,
	destroy
};
