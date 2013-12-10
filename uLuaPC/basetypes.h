#ifndef BASETYPES_H
#define BASETYPES_H

#include <stdint.h>
#include "vmconfig.h"

typedef uint8_t u08;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t s08;
typedef int16_t s16;
typedef int32_t s32;

#define NULL 0

#define TRUE  1
#define FALSE 0

//vm types
//variable types
enum vartype {
	VAR_BOOLEAN,
	VAR_NUMBER,
	VAR_FLOAT,
	VAR_STRING,
	VAR_NULL,
	VAR_FILE_POINTER_FUNC,
	VAR_FILE_POINTER_STR,
	VAR_TABLE,
	VAR_NATIVE_FUNC,
};

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
	u16 constp; //pointer to constants for the current function
	u16 funcp; //pointer to subfunctions in the current function
	u08 retreg; //return register pointer to store function call result

	//registers
	vmregister reg[REGISTERSIZE];
	//upvalues registers
	vmregister upval[UPVALUESIZE];
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

#endif
