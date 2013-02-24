#ifndef GC_H
#define GC_H

#include "basetypes.h"

//memory total size
#define GC_SIZE 256
//maximum data size which variable can hold
#define GC_MAX_VAR_SIZE 32
//maximum variables which GC could create
#define GC_VAR_PT_SIZE 64

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
gcvarpt* gcNew(u08 type);
//delete variable
void gcDelete(gcvarpt* variable);



#endif