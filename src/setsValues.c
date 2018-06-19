#include "../inc/setsValues.h"
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

struct SetsValues
{
	int BitErrorRate;
	int synchronized;
	int frameUtilRate;
	int modulation;
	int* frameUtilValues;
	int frameUtilCounter;
	int bitsum;
	pthread_mutex_t *mutex;
};

static SetsValues* create();
static SetsValues* create()
{
	SetsValues *values = malloc(sizeof(*values));
	values -> BitErrorRate = 0;
	values -> synchronized = 0;
	values -> frameUtilRate = 0;
	values -> modulation = 0;

	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t* mutexer = malloc(sizeof(*mutexer));
	*mutexer = mutex;
	
	values -> mutex = mutexer;

	return values;
}

static int getBER(SetsValues *handle);
static int getBER(SetsValues *handle)
{
	pthread_mutex_lock(handle -> mutex);
	int value = handle -> BitErrorRate;
	pthread_mutex_unlock(handle -> mutex);
	return value;
}

static int getSYNC(SetsValues *handle);
static int getSYNC(SetsValues *handle)
{
	pthread_mutex_lock(handle -> mutex);
	int value = handle -> synchronized;
	pthread_mutex_unlock(handle -> mutex);
	return value;
}

static int getUTIL(SetsValues *handle);
static int getUTIL(SetsValues *handle)
{
	pthread_mutex_lock(handle -> mutex);
	int value = handle -> frameUtilRate;
	pthread_mutex_unlock(handle -> mutex);
	return value;
}

static int getMOD(SetsValues *handle);
static int getMOD(SetsValues *handle)
{
	pthread_mutex_lock(handle -> mutex);
	int value = handle -> modulation;
	pthread_mutex_unlock(handle -> mutex);
	return value;
}

static void updateBER(SetsValues *handle, int upd);
static void updateBER(SetsValues *handle, int upd)
{
	pthread_mutex_lock(handle -> mutex);
	handle -> BitErrorRate = upd;
	pthread_mutex_unlock(handle -> mutex);
}

static void updateSYNC(SetsValues *handle, int upd);
static void updateSYNC(SetsValues *handle, int upd)
{
	pthread_mutex_lock(handle -> mutex);
	handle -> synchronized = upd;
	pthread_mutex_unlock(handle -> mutex);
}

static void updateUTIL(SetsValues *handle, int upd);
static void updateUTIL(SetsValues *handle, int upd)
{
	pthread_mutex_lock(handle -> mutex);
	handle -> frameUtilRate = upd;
	pthread_mutex_unlock(handle -> mutex);
}

static void updateMOD(SetsValues *handle, int upd);
static void updateMOD(SetsValues *handle, int upd)
{
	pthread_mutex_lock(handle -> mutex);
	handle -> modulation = upd;
	pthread_mutex_unlock(handle -> mutex);
}

static void destroy(SetsValues *handle);
static void destroy(SetsValues *handle)
{
	free(handle -> mutex);
	free(handle);
	handle = NULL;
}



_SettingsSets const SettingsSets =
{
	create,
	getBER,
	getSYNC,
	getMOD,
	getUTIL,
	updateBER,
	updateSYNC,
	updateMOD,
	updateUTIL,
	destroy
};
