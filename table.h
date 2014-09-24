#ifndef TABLE_H
#define TABLE_H

#include "basetypes.h"
#include "vmconfig.h"
#include "gc.h"

//table structure
typedef struct hashtable {
	u32 size; //key\value size
	u32 count; //items count stored in this table
	u32* keys;
	u32* vals;
} hashtable;

//create new table instance
gcvarpt* tableNew();
//create new table instance
gcvarpt* tableNewSize(u32 size);
//put value into table for a key
void tablePut(gcvarpt* table, vmregister* key, vmregister* val);
//get value from table for key and save it into val
void tableGet(gcvarpt* table, vmregister* key, vmregister* val);

#endif