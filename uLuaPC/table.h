#ifndef TABLE_H
#define TABLE_H

#include "basetypes.h"
#include "vmconfig.h"
#include "gc.h"

//table structure
struct hashtable {
	u08 size; //key\value size
	u32* keys;
	u32* vals;
};

//create new table instance
gcvarpt* tableNew();
//put value into table for a key
void tablePut(gcvarpt* table, vmregister* key, vmregister* val);
//get value from table for key and save it into val
void tableGet(gcvarpt* table, vmregister* key, vmregister* val);

#endif