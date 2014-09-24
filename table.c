#include "table.h"

//create new table instance
gcvarpt* tableNew()
{
	return tableNewSize(DEFAULT_TABLE_SIZE);
}

//create new table instance
gcvarpt* tableNewSize(u32 tsize)
{
	gcvarpt* table = gcNewVar(VAR_TABLE, sizeof(u32)*tsize*2 + sizeof(u32) + sizeof(u32));
	GCVALUE(hashtable,table).count = 0;
	GCVALUE(hashtable,table).size = tsize;
	GCVALUE(hashtable,table).keys = (u32*) &((*table)->data[sizeof(u32) + sizeof(u32)]);
	GCVALUE(hashtable,table).vals = (u32*) &((*table)->data[sizeof(u32) + sizeof(u32)+sizeof(u32)*tsize]);
	return table;
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
	u32 i;
	//get table structure
	hashtable* tb = (hashtable*)(*table)->data;

	//calculate hash for the key and index
	u32 hashkey = getHash(key);
	u32 keyindex = hashkey % tb->size;

	//find empty slot
	//TODO: process running out from size
	for(i=keyindex; i < tb->size; i++)
	{
		if(tb->keys[i] == NULL)
		{
			keyindex = i; 
			break;
		}
	}

	//save kay and value
	//tb->keys[keyIndex] = 
}

//get value from table for key and save it into val
void tableGet(gcvarpt* table, vmregister* key, vmregister* val)
{
}