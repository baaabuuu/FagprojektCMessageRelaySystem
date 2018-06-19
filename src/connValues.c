#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>


#include "../inc/connValues.h"
#include "../inc/setsValues.h"
#include "../inc/dataValues.h"
#include "../inc/sts_queue.h"

struct ConnValues
{
	int key;
	int* socket;
	int isConnected;
	pthread_t*	thread;
	StsHeader** queue;
	SetsValues* setPointer;
	DataValues** data;
	pthread_mutex_t* mutex;
};

static ConnValues* create();
static ConnValues* create()
{
	ConnValues* values = malloc(sizeof(*values));
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t* mutexer = malloc(sizeof(*mutexer));
	*mutexer = mutex;
	int temp = -1;
	values -> key = -1;
	values -> isConnected = 0;
	values -> thread = NULL;
	values -> queue = NULL;
	values -> setPointer = NULL;
	values -> data = NULL;
	values -> mutex = mutexer;
	values -> socket = &temp;
	return values;
}

static int getKey(ConnValues *handle);
static int getKey(ConnValues *handle)
{
	pthread_mutex_lock(handle -> mutex);
	int value = (handle -> key);
	pthread_mutex_unlock(handle -> mutex);
	return value;
}

static int* getSocket(ConnValues *handle);
static int* getSocket(ConnValues *handle)
{
	pthread_mutex_lock(handle -> mutex);
	int* value = (handle -> socket);
	pthread_mutex_unlock(handle -> mutex);
	return value;
}

static int getisConnected(ConnValues *handle);
static int getisConnected(ConnValues *handle)
{
	pthread_mutex_lock(handle -> mutex);
	int value = (handle -> isConnected);
	pthread_mutex_unlock(handle -> mutex);
	return value;
}

static pthread_t* getThread(ConnValues *handle);
static pthread_t* getThread(ConnValues *handle)
{
	pthread_mutex_lock(handle -> mutex);
	pthread_t* value = (handle -> thread);
	pthread_mutex_unlock(handle -> mutex);
	return value;
}

static StsHeader** getQueue(ConnValues *handle);
static StsHeader** getQueue(ConnValues *handle)
{
	pthread_mutex_lock(handle -> mutex);
	StsHeader** value = (handle -> queue);
	pthread_mutex_unlock(handle -> mutex);
	return value;
}

static SetsValues* getsetPointer(ConnValues *handle);
static SetsValues* getsetPointer(ConnValues *handle)
{
	pthread_mutex_lock(handle -> mutex);
	SetsValues* value = (handle -> setPointer);
	pthread_mutex_unlock(handle -> mutex);
	return value;
}

static DataValues** getData(ConnValues *handle);
static DataValues** getData(ConnValues *handle)
{
	pthread_mutex_lock(handle -> mutex);
	DataValues** value = (handle -> data);
	pthread_mutex_unlock(handle -> mutex);
	return value;
}

static void updateKey(ConnValues *handle, int upd);
static void updateKey(ConnValues *handle, int upd)
{
	pthread_mutex_lock(handle -> mutex);
	handle -> key = upd;
	pthread_mutex_unlock(handle -> mutex);
}

static void updateSocket(ConnValues *handle, int* upd);
static void updateSocket(ConnValues *handle, int* upd)
{
	pthread_mutex_lock(handle -> mutex);
	handle -> socket = upd;
	pthread_mutex_unlock(handle -> mutex);
}

static void updateisConnected(ConnValues *handle, int upd);
static void updateisConnected(ConnValues *handle, int upd)
{
	pthread_mutex_lock(handle -> mutex);
	handle -> isConnected = upd;
	pthread_mutex_unlock(handle -> mutex);
}

static void updateThread(ConnValues *handle, pthread_t* upd);
static void updateThread(ConnValues *handle, pthread_t* upd)
{
	pthread_mutex_lock(handle -> mutex);
	handle -> thread = upd;
	pthread_mutex_unlock(handle -> mutex);
}

static void updateQueue(ConnValues *handle, StsHeader** upd);
static void updateQueue(ConnValues *handle, StsHeader** upd)
{
	pthread_mutex_lock(handle -> mutex);
	handle -> queue = upd;
	pthread_mutex_unlock(handle -> mutex);
}

static void updatesetPointer(ConnValues *handle, SetsValues* upd);
static void updatesetPointer(ConnValues *handle, SetsValues* upd)
{
	pthread_mutex_lock(handle -> mutex);
	handle -> setPointer = upd;
	pthread_mutex_unlock(handle -> mutex);
}


static void updateData(ConnValues *handle, DataValues** upd);
static void updateData(ConnValues *handle, DataValues** upd)
{
	pthread_mutex_lock(handle -> mutex);
	handle -> data = upd;
	pthread_mutex_unlock(handle -> mutex);
}

static void destroy(ConnValues *handle);
static void destroy(ConnValues *handle)
{
	free(handle -> mutex);
	free(handle);
	handle = NULL;
}

_ConnSets const ConnSets =
{
	create,
	getKey,
	getSocket,
	getisConnected,
	getThread,
	getQueue,
	getsetPointer,
	getData,
	updateKey,
	updateSocket,
	updateisConnected,
	updateThread,
	updateQueue,
	updatesetPointer,
	updateData,
	destroy
};
