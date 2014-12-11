#ifndef BASETYPES_H
#define BASETYPES_H

#include <stdint.h>
#include "vmconfig.h"

typedef uint8_t lu08;
typedef uint16_t lu16;
typedef uint32_t lu32;

typedef int8_t ls08;
typedef int16_t ls16;
typedef int32_t ls32;

#ifndef NULL
#define NULL 0
#endif

typedef lu08 BOOL;
#define TRUE  1
#define FALSE 0

//callback functions type
typedef void (*readBytes)(lu08* buff, lu16 offset, lu16 size);


//gc types
//variable structure
typedef struct gcvar {
	lu08 refcount;
	lu08 type;
	lu16 size;
	lu08 data[GC_MAX_VAR_SIZE];
	/*union {
		u08 boolean;
		s32 number;
		float fnumber;
		u08 data[GC_MAX_VAR_SIZE];
	} var;*/
} gcvar;

typedef gcvar* gcvarpt;

//vm types
//variable types
typedef enum vartype {
	VAR_BOOLEAN,
	VAR_NUMBER,
	VAR_FLOAT,
	VAR_STRING,
	VAR_NULL,
	VAR_CLOSURE,
	VAR_FILE_POINTER_STR,
	VAR_TABLE,
	VAR_NATIVE_FUNC,
} vartype;

typedef enum threadstate {
	STOP,
	RUN,
	SUSPEND,
	ABORT
} threadstate;

typedef struct vmregister {
	vartype type;
	union {
		lu32   numval;
		float floatval;
		void* pointer;
	};
} vmregister;

typedef struct vmclosure {
	lu16		funcp; //pointer to function
	lu08		upvalcount; //count of upvalues
	//upvalues registers
	vmregister	upval[UPVALUESIZE]; //upvals from GC
} vmclosure;


typedef struct vmglobal {
	char		name[GLOBALNAMESIZE];
	vmregister	val;
} vmglobal;

typedef struct vmstate {
	gcvarpt* closure;
	lu16	pc;
	lu16 constp; //pointer to constants for the current function
	lu16 funcp; //pointer to subfunctions in the current function
	lu08 regcount; //count of register used in current funtion
	lu08 retreg; //return register pointer to store function call result

	//registers
	vmregister reg[REGISTERSIZE];
	//vmregister* reg; //registers for current statre reg[regcount]
} vmstate;

//virtual machine single thread main structure
typedef struct vm {
	//vm thread state
	threadstate	status;

	//global variables
	vmglobal global[GLOBALSIZE];

	//states (call stack)
	vmstate	state[STATEMAXCOUNT];
	lu08	statept;

	//PC means pointer in the file
	lu16 pc; 

	//pc call stack
	lu16 pcstack[PCSTACKSIZE];
	lu08 pcstackpt;
} vm;

#endif
