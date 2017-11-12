#ifndef VM_H
#define VM_H

#include "ulua_core/basetypes.h"

//exported functions
void vmInit(vm* vm);
lu08 vmRun(vm* vm, readBytes read);


#endif

