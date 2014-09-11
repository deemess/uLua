#ifndef VM_H
#define VM_H

#include "basetypes.h"
#include "vmconfig.h"
#include "platform.h"
#include "opcodes.h"
#include "gc.h"
#include "native.h"

#define NULL_TYPE	0x00
#define BOOL_TYPE	0x01
#define NUMBER_TYPE 0x03
#define STRING_TYPE 0x04

//exported functions
void vmInit(vm* vm);
u08 vmRun(vm* vm);


#endif