#ifndef VM_H
#define VM_H

#include "basetypes.h"
#include "platform.h"
#include "state.h"
#include "opcodes.h"
#include "gc.h"

#define NULL_TYPE	0x00
#define BOOL_TYPE	0x01
#define NUMBER_TYPE 0x03
#define STRING_TYPE 0x04

void vmInit();
u08 vmRun(vmstate* state);

#endif