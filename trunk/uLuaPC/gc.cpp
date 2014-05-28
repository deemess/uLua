#include "gc.h"

//global variables for gc
u08 memory[GC_SIZE]; //gc heap
gcvarpt vars[GC_VAR_PT_SIZE];//gc pointers to the heap
u08 freePt;//free pointer in vars[]

//initialize garbage collector and memory management
void gcInit()
{
	for(u16 i=0; i<GC_SIZE;i++)
	{
		memory[i] = 0;
	}
	freePt=0;
}

//create new variable and return its number
gcvarpt* gcNew(vartype type)
{
	u16 size=0;
	//init size for new type
	switch(type)
	{
	case VAR_BOOLEAN:
		size = 1;
		break;
	case VAR_NUMBER:
		size = 4;//long s32
		break;
	case VAR_FLOAT:
		size = 4;//float
		break;
	case VAR_STRING:
		size = 0;//empty string
		break;
	case VAR_CLOSURE:
		//u16			funcp; //pointer to function
		//u08			upvalcount; //count of upvalues
		//vmregister	upval[UPVALUESIZE]; //upvals from GC
		size = (u16)(sizeof(vmclosure));
		break;
	case VAR_FILE_POINTER_STR:
		size = 4;//u32 - point to some address in the source byte code
		break;
	case VAR_NULL:
	default:
		size = 0;//null doesnt hold anything
		break;
	}

	return gcNew(type,size);
}

//create new variable with given size and return its number
gcvarpt* gcNew(vartype type, u16 size)
{
	//allocate memory for a new variable
	gcvar* newvar = (gcvar*)(&memory[freePt]);
	newvar->type = type;
	newvar->size = size;

	freePt += newvar->size + 4; // + type + size + refcount

	//reset variable
	for(u16 i=0; i<newvar->size; i++)
		newvar->data[i] = 0;

	//save new variable in our varlist
	u16 foundFreePt = 0;
	for(u16 i=0; i<GC_VAR_PT_SIZE; i++)
	{
		if(vars[i] == NULL)
		{
			foundFreePt = i;
			break;
		}
	}
	vars[foundFreePt] = newvar;

	//return new var
	return &vars[foundFreePt];
}


//delete variable
void gcDelete(gcvarpt* variable)
{
	//get variable number
	u08 varNum = variable - &vars[0];
	u08 varMemoryAddr = (u08*)(*variable) - &memory[0];
	u08 varSize = (*variable)->size + 2; // + type + size

	//copy all vars to the left in the array
	/*for(u08 i=freeVarPt-varNum; i++; i<freeVarPt)
	{
		vars[i]=vars[i+1];
	}*/

	//free variable
	vars[varNum] = NULL;

	//copy memory
	for(u16 i=varMemoryAddr; i<GC_SIZE || i == freePt ; i++)
	{
		if(i+varSize >= GC_SIZE)
		{
			memory[i] = 0;
		}
		else
		{
			memory[i] = memory[i+varSize];
		}
	}

	//shift all pointers for right vars in array
	for(u16 i=0; i<GC_VAR_PT_SIZE; i++)
	{
		if(vars[i] == NULL)
			continue;
		if((u08*)vars[i] - &memory[0] > varMemoryAddr)
		{
			vars[i] = (gcvarpt)( ((u08*)vars[i]) - varSize );
		}
	}

	//shift free memory pointer to the size
	freePt -= varSize;
}

#ifdef DEBUGVM
//dump gc memory
void gcDump()
{
	platformPrintf("Dumping memory: size=%d, address=%d\n", GC_SIZE, &memory);
	u16 constpt;
	u32 size;
	u08* name;

	for(int i=0; i<GC_VAR_PT_SIZE; i++)
	{
		if(vars[i] == NULL)
			continue;
		switch(vars[i]->type)
		{
		case VAR_BOOLEAN:
			platformPrintf("VAR_BOOLEAN: size=%d, address=%d(m+%d), value=%d\n",vars[i]->size, vars[i], (u08*)vars[i] - memory, GCVALUE(u08, &vars[i]));
			break;
		case VAR_NUMBER:
			platformPrintf("VAR_NUMBER: size=%d, address=%d(m+%d), value=%d\n",vars[i]->size, vars[i], (u08*)vars[i] - memory, GCVALUE(u32, &vars[i]));
			break;
		case VAR_FLOAT:
			platformPrintf("VAR_FLOAT: size=%d, address=%d(m+%d), value=%f\n",vars[i]->size, vars[i],  (u08*)vars[i] - memory, GCVALUE(float, &vars[i]));
			break;
		case VAR_STRING:
			platformPrintf("VAR_STRING: size=%d, address=%d(m+%d), value=%s\n",vars[i]->size, vars[i], (u08*)vars[i] - memory, vars[i]->data);
			break;
		case VAR_FILE_POINTER_STR:
			constpt = GCVALUE(u16, &vars[i]);
			size = platformReadDWord(constpt); constpt += 4;
			name = platformReadBuffer(constpt, size);
			platformPrintf("VAR_FILE_POINTER_STR: size=%d, address=%d(m+%d), value=%s\n",vars[i]->size, vars[i], (u08*)vars[i] - memory, name);
			break;
		case VAR_CLOSURE:
			platformPrintf("VAR_CLOSURE: size=%d, address=%d(m+%d), value=%d\n",vars[i]->size, vars[i], (u08*)vars[i] - memory, GCVALUE(u16, &vars[i]));
			break;
		case VAR_NULL:
			platformPrintf("VAR_NULL: size=%d, address=%d(m+%d)\n",vars[i]->size, vars[i], (u08*)vars[i] - memory);
			break;
		default:
			platformPrintf("UNKNOWN_TYPE: size=%d, address=%d(m+%d)\n",vars[i]->size, vars[i], (u08*)vars[i] - memory);
			break;
		}
	}
}
#endif