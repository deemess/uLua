#ifndef VM_H
#define VM_H

#include "basetypes.h"
#include "vmconfig.h"
#include "platform.h"
#include "opcodes.h"
#include "gc.h"

#define NULL_TYPE	0x00
#define BOOL_TYPE	0x01
#define NUMBER_TYPE 0x03
#define STRING_TYPE 0x04


enum threadstate {
	STOP,
	RUN,
	SUSPEND,
	ABORT
};


struct vmregister {
	vartype type;
	union {
		u32   numval;
		float floatval;
	};
};

struct vmglobal {
	char		name[GLOBALNAMESIZE];
	vmregister	val;
};

struct vmstate {
	u16 constp; //pointer to constants for the current functino
	u16 funcp; //pointer to subfunctions in the current function
	
	//registers
	vmregister reg[REGISTERSIZE];
};

//virtual machine single thread main structure
struct vm {
	//vm thread state
	threadstate	status;

	//global variables
	vmglobal global[GLOBALSIZE];

	//states (call stack)
	vmstate	state[STATEMAXCOUNT];
	u08	statept;

	//PC means pointer in the file
	u16 pc; 

	//pc call stack
	u16 pcstack[PCSTACKSIZE];
	u08 pcstackpt;
};

//exported functions
void vmInit(vm* vm);
u08 vmRun(vm* vm);




#endif