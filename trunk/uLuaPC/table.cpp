#include "table.h"

//create new table instance
gcvarpt* tableNew()
{
	return gcNew(VAR_TABLE, DEFAULT_TABLE_SIZE);
}

u32 getHash(vmregister* val)
{
	switch(val->type)
	{

	case VAR_BOOLEAN:
		return val->numval;

	case VAR_NUMBER:
	case VAR_FLOAT:
		return val->numval;

	case VAR_STRING:
		//TODO: implement hashing
		return 0;

	case VAR_NULL:
		return 0;

	case VAR_CLOSURE:
		return val->numval;

	case VAR_FILE_POINTER_STR:
		return val->numval;

	case VAR_TABLE:
		//TODO: implement hashing
		return 0;

	}
	return 0;
}

//put value into table for a key
void tablePut(gcvarpt* table, vmregister* key, vmregister* val)
{
	//get table structure
	hashtable* tb = (hashtable*)(*table)->data;

	//calculate hash for the key and index
	u32 hashkey = getHash(key);
	u08 keyindex = hashkey % tb->size;

	//find empty slot
	//TODO: process running out from size
	for(u08 i=keyindex; i < tb->size; i++)
	{
		if(tb->keys[i] = NULL)
		{
			keyindex = i; 
			break;
		}
	}
}

//get value from table for key and save it into val
void tableGet(gcvarpt* table, vmregister* key, vmregister* val)
{
}