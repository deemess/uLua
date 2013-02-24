#include "gc.h"

//global variables for gc
u08 memory[GC_SIZE];
gcvarpt vars[GC_VAR_PT_SIZE];
u08 freePt;

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
gcvarpt* gcNew(u08 type)
{
	//allocate memory for a new variable
	gcvar* newvar = (gcvar*)(&memory[freePt]);
	newvar->type = type;

	//init size for new type
	switch(newvar->type)
	{
	case VAR_BOOLEAN:
		newvar->size = 1;
		break;
	case VAR_NUMBER:
		newvar->size = 4;//long s32
		/*newvar->var.boolean = TRUE; //for testing
		newvar->var.number = 0xABCDEFBA;
		newvar->var.fnumber = 1.9f;*/
		break;
	case VAR_FLOAT:
		newvar->size = 4;//float
		break;
	case VAR_STRING:
		newvar->size = 0;//empty string
		break;
	case VAR_FILE_POINTER:
		newvar->size = 4;//u32 - point to some address in the source byte code
		break;
	case VAR_NULL:
	default:
		newvar->size = 0;//null doesnt hold anything
		break;
	}

	freePt += newvar->size + 2; // + type + size

	//reset variable
	for(u16 i=0; i<newvar->size; i++)
	{
		//newvar->var.data[i] = 0;
		newvar->data[i] = 0;
	}

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
	for(u16 i=varMemoryAddr; i<GC_MAX_VAR_SIZE || i == freePt ; i++)
	{
		if(i+varSize >= GC_MAX_VAR_SIZE)
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