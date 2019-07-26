#ifndef NATIVE_H
#define NATIVE_H

#include "ulua_core/basetypes.h"
#include "ulua_core/vm.h"

typedef void (*nativeFunc)(ulua_vm* vm, readBytes read, lu08 a, lu16 b, lu16 c);

//exported functions

//preload native functions in global namespace
void nativeInit(ulua_vm* vm);

//call native function stored in reg
void nativeCall(ulua_vm* vm, readBytes read, lu08 a, lu16 b, lu16 c);

#endif

