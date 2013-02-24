#ifndef STATE_H
#define STATE_H

#include "basetypes.h"

#define PCSTACKSIZE 32
#define REGISTERSIZE 32
#define GLOBALSIZE 32
#define GLOBALNAMESIZE 32
#define STATEMAXCOUNT 32

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

static void initState(vmstate* state)
{
	state->constp = 0;
	state->funcp = 0;


	for(int i=0; i<REGISTERSIZE; i++) {
		state->reg[i].type = VAR_NULL;
		state->reg[i].numval = 0;
	}

}

static void initvm(vm* vm)
{
	vm->status = STOP;
	vm->pc = 0;
	vm->pcstackpt = 0;
	vm->statept = 0;

	for(int i=0; i<PCSTACKSIZE; i++)
		vm->pcstack[i] = 0;

	for(int i=0; i<GLOBALSIZE; i++) {
		vm->global[i].val.type = VAR_NULL;
		vm->global[i].val.numval = 0;

		for(int j=0; j<GLOBALNAMESIZE; j++)
			vm->global[i].name[j] = 0;
	}

	for(int i=0; i<STATEMAXCOUNT; i++)
		initState(&vm->state[i]);
	
}

#endif