#ifndef TABLE_H
#define TABLE_H

#include "basetypes.h"
#include "vmconfig.h"
#include "gc.h"

//create new table instance
gcvarpt* tableNew();
//put value into table for a key
void tablePut(vmregister* key, vmregister* val);
//get value from table for key and save it into val
void tableGet(vmregister* key, vmregister* val);

#endif