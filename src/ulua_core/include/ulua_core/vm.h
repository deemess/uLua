#ifndef VM_H
#define VM_H

#include "ulua_core/basetypes.h"
#include "ulua_core/vmconfig.h"
#include "ulua_core/opcodes.h"
#include "ulua_core/gc.h"
#include "ulua_core/native.h"
#include "ulua_core/ucodegen.h"

//exported functions
void vmInit(vm* vm);
lu08 vmRun(vm* vm, readBytes read);


#endif

