#include "setsValues.h"
#include "dataValues.h"
#include "sts_queue.h"

#ifndef CONN_VALUES_H
#define CONN_VALUES_H



typedef struct ConnValues ConnValues;

typedef struct {
  ConnValues* (* const create)();
  int (* const getKey)(ConnValues *handle);
  int* (* const getSocket)(ConnValues *handle);
  int  (* const getisConnected)(ConnValues *handle);
  pthread_t* (* const getThread)(ConnValues *handle);
  StsHeader** (* const getQueue)(ConnValues *handle);
  SetsValues* (* const getsetPointer)(ConnValues *handle);
  DataValues** (* const getData)(ConnValues *handle);
  void (* const updateKey)(ConnValues *handle, int upd);
  void (* const updateSocket)(ConnValues *handle, int* upd);
  void (* const updateisConnected)(ConnValues *handle, int upd);
  void (* const updateThread)(ConnValues *handle, pthread_t* upd);
  void (* const updateQueue)(ConnValues *handle, StsHeader** upd);
  void (* const updatesetPointer)(ConnValues *handle, SetsValues* upd);
  void (* const updateData)(ConnValues *handle, DataValues** upd);
  void (* const destroy)(ConnValues *handle);
  
} _ConnSets;

extern _ConnSets const ConnSets;

#endif
