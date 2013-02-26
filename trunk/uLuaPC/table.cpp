#include "table.h"

//create new table instance
gcvarpt* tableNew()
{
	return gcNew(VAR_TABLE, DEFAULT_TABLE_SIZE);
}

//put value into table for a key
void tablePut(vmregister* key, vmregister* val)
{
}

//get value from table for key and save it into val
void tableGet(vmregister* key, vmregister* val)
{
}