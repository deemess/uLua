#ifndef VM_H
#define VM_H

#include "ulua_core/basetypes.h"
#include "ulua_core/umemory.h"
#include "ulua_core/vmconfig.h"

typedef enum threadstate {
    STOP,
    RUN,
    SUSPEND,
    ABORT
} threadstate;

typedef struct vmclosure {
    lu16		funcp; //pointer to function
    lu08		upvalcount; //size of upvalues
    //upvalues registers
    vmregister	upval[UPVALUESIZE]; //upvals from GC
} vmclosure;


typedef struct vmstate {
    ulua_memvar* closure;
    lu16	pc;
    lu16 constp; //pointer to constants for the current function
    lu16 funcp; //pointer to subfunctions in the current function
    lu08 regcount; //size of register used in current funtion
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
    ulua_memvar*    globals_table;

    //states (call stack)
    vmstate	state[STATEMAXCOUNT];
    lu08	statept;

    //PC means pointer in the file
    lu16 pc;

    //pc call stack
    lu16 pcstack[PCSTACKSIZE];
    lu08 pcstackpt;

    //memory
    lu08* memory;
    lu16  memory_size;
} ulua_vm;

//exported functions
ulua_memvar* vmInit(lu08* memory, lu16 memory_size);
lu08 vmRun(ulua_memvar* memvm, readBytes read);


#endif

