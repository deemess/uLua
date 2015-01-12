#ifndef ULUA_UMEMORY
#define ULUA_UMEMORY

#include "basetypes.h"
#include "vmconfig.h"

//variable accessors
#define GCVALUE(type,var) ((type*)(*var)->datapt)[0]
#define GCREFDEC(var) (*var)->refcount--
#define GCREFINC(var) (*var)->refcount++
#define GCCHECK(var) if((*var)->refcount == 0) ulua_free(var);

//ulua types
typedef enum ulua_vartype {
    ULUA_TYPE_NULL,
    ULUA_TYPE_DATABLOCK,
} ulua_vartype;

//memory variable structure
typedef struct ulua_memvar {
    lu08 refcount;
    lu08 blocked;
    ulua_vartype type;
    lu16 size;
    lu08* datapt;
} ulua_memvar;

void ulua_memoryinit(); //initialize memory
ulua_memvar* ulua_allocate(ulua_vartype type, lu16 size); //allocate memory for new type
lu08* ulua_allocate_block(lu16 size); //allocate fixed memory data block. and mark it as locked.
void ulua_free(ulua_memvar* var); //free variable and allocated memory
void ulua_free_block(lu08* block); //free memory block and associated variable with it

#endif /* defined(ULUA_UMEMORY) */
