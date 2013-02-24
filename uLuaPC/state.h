#ifndef STATE_H
#define STATE_H

#include "basetypes.h"

#define PCSTACKSIZE 32
#define REGISTERSIZE 32
#define GLOBALSIZE 32

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

struct vmstate {
	u16 pc; //PC means pointer in the file
	u16 constp; //pointer to constants for the current functino
	u16 funcp; //pointer to subfunctions in the current function
	
	threadstate thstate;

	//pc call stack
	u16 pcstack[PCSTACKSIZE];
	u08 pcstackpt;

	//registers
	vmregister reg[REGISTERSIZE];

	//globals
	vmregister global[GLOBALSIZE];
};

static void initState(vmstate* state)
{
	state->thstate = STOP;
	state->pc = 0;
	state->pcstackpt = 0;
	state->constp = 0;
	state->funcp = 0;

	for(int i=0; i<PCSTACKSIZE; i++)
		state->pcstack[i] = 0;
	for(int i=0; i<REGISTERSIZE; i++) {
		state->reg[i].type = VAR_NULL;
		state->reg[i].numval = 0;
	}
	for(int i=0; i<GLOBALSIZE; i++) {
		state->global[i].type = VAR_NULL;
		state->global[i].numval = 0;
	}
}

#endif