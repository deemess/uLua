#ifndef NATIVE_H
#define NATIVE_H

#include "basetypes.h"
#include "vmconfig.h"
#include <stdio.h>

typedef void (*nativeFunc)(vm* vm, readBytes read, lu08 a, lu16 b, lu16 c);

//exported functions

//preload native functions in global namespace
void nativeInit(vm* vm);

//call native function stored in reg
void nativeCall(vm* vm, readBytes read, lu08 a, lu16 b, lu16 c);

#endif

