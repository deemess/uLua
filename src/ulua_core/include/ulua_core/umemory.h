#ifndef ULUA_UMEMORY
#define ULUA_UMEMORY

#include "ulua_core/basetypes.h"
#include "ulua_core/vmconfig.h"

//variable accessors
#define GCVALUE(type,var) ((type*)&((*var)->memblock->data))[0]
#define GCREFDEC(var) (*var)->refcount--
#define GCREFINC(var) (*var)->refcount++
#define GCCHECK(var) if((*var)->refcount == 0) ulua_free(var);

//ulua types
typedef enum ulua_vartype {
    ULUA_TYPE_NULL,
    ULUA_TYPE_DATABLOCK,
    ULUA_TYPE_STRING,
} ulua_vartype;

typedef struct ulua_memblock ulua_memblock;
typedef struct ulua_memvar ulua_memvar;

//memory block header
typedef struct ulua_memblock_header {
    ulua_memblock* prev;
    ulua_memblock* next;
    lu16 size;
    ulua_memvar* var;
} ulua_memblock_header;

//memory block with header and data allocated
struct ulua_memblock {
    ulua_memblock_header header;
    lu08 data;
};

//memory variable structure
struct ulua_memvar {
    lu08 refcount;
    lu08 blocked;
    ulua_vartype type;
    ulua_memblock* memblock;
};


void ulua_memoryinit(lu08* memory, lu16 size); //initialize memory
ulua_memvar* ulua_allocate(ulua_vartype type, lu16 size); //allocate memory for new type
lu08* ulua_allocate_block(lu16 size); //allocate fixed memory data block. and mark it as locked.
void ulua_free(ulua_memvar* var); //free variable and allocated memory
void ulua_free_block(lu08* block); //free memory block and associated variable with it

#endif /* defined(ULUA_UMEMORY) */
