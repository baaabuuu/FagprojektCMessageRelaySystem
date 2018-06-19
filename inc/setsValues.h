#ifndef SETS_VALUES_H
#define SETS_VALUES_H

typedef struct SetsValues SetsValues;

typedef struct {
  SetsValues* (* const create)();
  int (* const getBER)(SetsValues *handle);
  int (* const getSYNC)(SetsValues *handle);
  int (* const getUTIL)(SetsValues *handle);
  int (* const getMOD)(SetsValues *handle);
  void (* const updateBER)(SetsValues *handle, int upd);
  void (* const updateSYNC)(SetsValues *handle, int upd);
  void (* const updateUTIL)(SetsValues *handle, int upd);
  void (* const updateMOD)(SetsValues *handle, int upd);
  void (* const destroy)(SetsValues *handle);
} _SettingsSets;

extern _SettingsSets const SettingsSets;

#endif
