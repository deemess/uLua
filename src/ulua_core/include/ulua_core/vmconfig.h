#ifndef VMCONFIG_H
#define VMCONFIG_H

// -----  Memory ---------
#define UMEMORY_SIZE 1024

// -----  GC ---------
//memory total size
#define GC_SIZE 1024
//maximum data size which variable can hold
#define GC_MAX_VAR_SIZE 32
//maximum variables which GC could create
#define GC_VAR_PT_SIZE 64

// ------ TABLE ------
#define DEFAULT_TABLE_SIZE 32

// ------ VM ---------
#define DEBUGVM //comment it out to optimize code
#define PCSTACKSIZE 32
#define REGISTERSIZE 32
#define UPVALUESIZE 8
#define STATEMAXCOUNT 32

#ifdef DEBUGVM
	#define DPRINTF(a, ...) printf(a, ##__VA_ARGS__)
#else
	#define DPRINTF(a, ...)
#endif

#endif

