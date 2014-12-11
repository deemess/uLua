#ifndef GC_H
#define GC_H

#include "vmconfig.h"
#include "basetypes.h"
#include <stdio.h>

//variable accessors
#define GCVALUE(type,var) ((type*)(*var)->data)[0]
#define GCREFDEC(var) (*var)->refcount--
#define GCREFINC(var) (*var)->refcount++
#define GCCHECK(var) if((*var)->refcount == 0) gcDelete(var);

//initialize garbage collector and memory management
void gcInit();
//create new variable and return its number
gcvarpt* gcNew(vartype type);
//create new variable with given size and return its number
gcvarpt* gcNewVar(vartype type, lu16 size);
//delete variable
void gcDelete(gcvarpt* variable);
//check if variable should be deleted

#ifdef DEBUGVM
//dump gc memory
void gcDump();
#endif

#endif

