#ifndef GC_H
#define GC_H

#include "vmconfig.h"
#include "basetypes.h"
#include "platform.h"

//variable accessors
#define GCVALUE(type,var) ((type*)(*var)->data)[0]

//variable structure
struct gcvar {
	u08 type;
	u08 size;
	u08 data[GC_MAX_VAR_SIZE];
	/*union {
		u08 boolean;
		s32 number;
		float fnumber;
		u08 data[GC_MAX_VAR_SIZE];
	} var;*/
};

typedef gcvar* gcvarpt;

//initialize garbage collector and memory management
void gcInit();
//create new variable and return its number
gcvarpt* gcNew(vartype type);
//create new variable with given size and return its number
gcvarpt* gcNew(vartype type, u08 size);
//delete variable
void gcDelete(gcvarpt* variable);

#ifdef DEBUGVM
//dump gc memory
void gcDump();
#endif

#endif