#ifndef VM_H
#define VM_H

#include "basetypes.h"
#include "vmconfig.h"
#include "opcodes.h"
#include "gc.h"
#include "native.h"
#include "ucodegen.h"

//exported functions
void vmInit(vm* vm);
lu08 vmRun(vm* vm, readBytes read);


#endif

