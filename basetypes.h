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

#ifndef NULL
#define NULL 0
#endif

typedef u08 BOOL;
#define TRUE  1
#define FALSE 0

//gc types
//variable structure
struct gcvar {
	u08 refcount;
	u08 type;
	u16 size;
	u08 data[GC_MAX_VAR_SIZE];
	/*union {
		u08 boolean;
		s32 number;
		float fnumber;
		u08 data[GC_MAX_VAR_SIZE];
	} var;*/
};

typedef gcvar* gcvarpt;

//vm types
//variable types
enum vartype {
	VAR_BOOLEAN,
	VAR_NUMBER,
	VAR_FLOAT,
	VAR_STRING,
	VAR_NULL,
	VAR_CLOSURE,
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
		void* pointer;
	};
};

struct vmclosure {
	u16			funcp; //pointer to function
	u08			upvalcount; //count of upvalues
	//upvalues registers
	vmregister	upval[UPVALUESIZE]; //upvals from GC
};


struct vmglobal {
	char		name[GLOBALNAMESIZE];
	vmregister	val;
};

struct vmstate {
	gcvarpt* closure;
	u16	pc;
	u16 constp; //pointer to constants for the current function
	u16 funcp; //pointer to subfunctions in the current function
	u08 regcount; //count of register used in current funtion
	u08 retreg; //return register pointer to store function call result

	//registers
	vmregister reg[REGISTERSIZE];
	//vmregister* reg; //registers for current statre reg[regcount]
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
