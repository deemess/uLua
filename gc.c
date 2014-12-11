#include "gc.h"

//global variables for gc
lu08 memory[GC_SIZE]; //gc heap
gcvarpt vars[GC_VAR_PT_SIZE];//gc pointers to the heap
lu08 freePt;//free pointer in vars[]

//initialize garbage collector and memory management
void gcInit()
{
	lu16 i=0;
	for(i=0; i<GC_SIZE;i++)
	{
		memory[i] = 0;
	}
	freePt=0;
}

//create new variable and return its number
gcvarpt* gcNew(vartype type)
{
	lu16 size=0;
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
		size = (lu16)(sizeof(vmclosure));
		break;
	case VAR_FILE_POINTER_STR:
		size = 4;//u32 - point to some address in the source byte code
		break;
	case VAR_NULL:
	default:
		size = 0;//null doesnt hold anything
		break;
	}

	return gcNewVar(type,size);
}

//create new variable with given size and return its number
gcvarpt* gcNewVar(vartype type, lu16 size)
{
	lu16 i=0;
	lu16 foundFreePt = 0;
	//allocate memory for a new variable
	gcvar* newvar = (gcvar*)(&memory[freePt]);
	newvar->type = type;
	newvar->size = size;

	freePt += newvar->size + 4; // + type + size + refcount

	//reset variable
	for(i=0; i<newvar->size; i++)
		newvar->data[i] = 0;

	//save new variable in our varlist
	for(i=0; i<GC_VAR_PT_SIZE; i++)
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
	lu16 i;
	//get variable number
	lu08 varNum = variable - &vars[0];
	lu08 varMemoryAddr = (lu08*)(*variable) - &memory[0];
	lu08 varSize = (*variable)->size + 2; // + type + size

	//copy all vars to the left in the array
	/*for(u08 i=freeVarPt-varNum; i++; i<freeVarPt)
	{
		vars[i]=vars[i+1];
	}*/

	//free variable
	vars[varNum] = NULL;

	//copy memory
	for(i=varMemoryAddr; i<GC_SIZE || i == freePt ; i++)
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
	for(i=0; i<GC_VAR_PT_SIZE; i++)
	{
		if(vars[i] == NULL)
			continue;
		if((lu08*)vars[i] - &memory[0] > varMemoryAddr)
		{
			vars[i] = (gcvarpt)( ((lu08*)vars[i]) - varSize );
		}
	}

	//shift free memory pointer to the size
	freePt -= varSize;
}

#ifdef DEBUGVM
//dump gc memory
void gcDump(readBytes read)
{
	lu16 constpt;
	lu16 size;
	lu08 name[32];
	lu16 i=0;

	printf("Dumping memory: size=%d, address=%d\n", GC_SIZE, (int)&memory);
	for(i=0; i<GC_VAR_PT_SIZE; i++)
	{
		if(vars[i] == NULL)
			continue;
		switch(vars[i]->type)
		{
		case VAR_BOOLEAN:
			printf("VAR_BOOLEAN: size=%d, address=%d(m+%ld), value=%d\n",vars[i]->size, (int)vars[i], (lu08*)vars[i] - memory, (int)GCVALUE(lu08, &vars[i]));
			break;
		case VAR_NUMBER:
			printf("VAR_NUMBER: size=%d, address=%d(m+%ld), value=%d\n",vars[i]->size, (int)vars[i], (lu08*)vars[i] - memory, GCVALUE(lu32, &vars[i]));
			break;
		case VAR_FLOAT:
			printf("VAR_FLOAT: size=%d, address=%d(m+%ld), value=%f\n",vars[i]->size, (int)vars[i],  (lu08*)vars[i] - memory, GCVALUE(float, &vars[i]));
			break;
		case VAR_STRING:
			printf("VAR_STRING: size=%d, address=%d(m+%ld), value=%s\n",vars[i]->size, (int)vars[i], (lu08*)vars[i] - memory, vars[i]->data);
			break;
		case VAR_FILE_POINTER_STR:
			constpt = GCVALUE(lu16, &vars[i]);
			read(name, constpt, 2);
			size = *(lu16*)(&name[0]);
			read(name, constpt, size);
			printf("VAR_FILE_POINTER_STR: size=%d, address=%d(m+%ld), value=%s\n",vars[i]->size, (int)vars[i], (lu08*)vars[i] - memory, name);
			break;
		case VAR_CLOSURE:
			printf("VAR_CLOSURE: size=%d, address=%d(m+%ld), value=%d\n",vars[i]->size, (int)vars[i], (lu08*)vars[i] - memory, GCVALUE(lu16, &vars[i]));
			break;
		case VAR_NULL:
			printf("VAR_NULL: size=%d, address=%d(m+%ld)\n",vars[i]->size, (int)vars[i], (lu08*)vars[i] - memory);
			break;
		default:
			printf("UNKNOWN_TYPE: size=%d, address=%d(m+%ld)\n",vars[i]->size, (int)vars[i], (lu08*)vars[i] - memory);
			break;
		}
	}
}
#endif
