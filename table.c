#include "table.h"

//create new table instance
gcvarpt* tableNew()
{
	return tableNewSize(DEFAULT_TABLE_SIZE);
}

//create new table instance
gcvarpt* tableNewSize(lu32 tsize)
{
	gcvarpt* table = gcNewVar(VAR_TABLE, sizeof(lu32)*tsize*2 + sizeof(lu32) + sizeof(lu32));
	GCVALUE(hashtable,table).count = 0;
	GCVALUE(hashtable,table).size = tsize;
	GCVALUE(hashtable,table).keys = (lu32*) &((*table)->data[sizeof(lu32) + sizeof(lu32)]);
	GCVALUE(hashtable,table).vals = (lu32*) &((*table)->data[sizeof(lu32) + sizeof(lu32)+sizeof(lu32)*tsize]);
	return table;
}

lu32 getHash(vmregister* val)
{
	switch(val->type)
	{

	case VAR_BOOLEAN:
		return val->numval;

	case VAR_NUMBER:
	case VAR_FLOAT:
    case VAR_NATIVE_FUNC:
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
	lu32 i;
	//get table structure
	hashtable* tb = (hashtable*)(*table)->data;

	//calculate hash for the key and index
	lu32 hashkey = getHash(key);
	lu32 keyindex = hashkey % tb->size;

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