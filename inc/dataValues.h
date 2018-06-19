#ifndef DATA_VALUES_H
#define DATA_VALUES_H

typedef struct DataValues DataValues;

typedef struct {
  DataValues* (* const create)();
  int (* const getErrors)(DataValues *handle);
  void (* const updateErrors)(DataValues *handle, int upd);
  void (* const destroy)(DataValues *handle);
} _DataSets;

extern _DataSets const DataSets;

#endif
