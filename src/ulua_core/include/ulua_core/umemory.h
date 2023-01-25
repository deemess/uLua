#ifndef ULUA_UMEMORY
#define ULUA_UMEMORY

#include "ulua_core/basetypes.h"
#include <stdio.h>

//variable accessor
//get value from memory variable
#define MEMVARVALUE(type,var) ((type)( ((lu08*)var->memblock)+sizeof(ulua_memblock) ))
//get memory variable from value 
#define VALUEMEMVAR(v) ((ulua_memblock*) (((lu08*)v)-sizeof(ulua_memblock)) )->header.var

//ulua types
typedef enum ulua_vartype {
    ULUA_MEM_TYPE_NULL,
    ULUA_MEM_TYPE_DATABLOCK,
    ULUA_MEM_TYPE_STRING,
    ULUA_MEM_TYPE_VMREGISTER,

    ULUA_MEM_TYPE_LIST,
    ULUA_MEM_TYPE_LIST_NODE,

    ULUA_MEM_TYPE_TABLE,
    ULUA_MEM_TYPE_TABLE_NODE,

    ULUA_MEM_TYPE_VM
} ulua_vartype;

//memory block flags
#define ULUA_MEM_FLAG_BLOCKED (lu08)0b00000001
#define ULUA_MEM_FLAG_MARK (lu08)0b00000010

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
    //data is stored right after header
    //data ...
};

//memory variable structure
struct ulua_memvar {
    lu08 flags;
    ulua_vartype type;
    ulua_memblock* memblock;
};

//list structures
typedef struct ulua_mem_list_node ulua_mem_list_node;
struct ulua_mem_list_node {
    ulua_memvar* prev; //prev ulua_mem_list_node
    ulua_memvar* next; //next ulua_mem_list_node
    ulua_memvar* var; //stored value
};
typedef struct  {
    ulua_memvar* first;
    ulua_memvar* last;
    lu16         size;
} ulua_mem_list;

//hashtable structures
#define ULUA_MEM_TABLE_SIZE 16
typedef struct {
    ulua_memvar* key;
    ulua_memvar* value;
    lu16         hash;
} ulua_mem_table_node;
typedef struct {
    lu16         size;
    ulua_memvar* list_array[ULUA_MEM_TABLE_SIZE];
} ulua_mem_table;

//managed memory functions
void ulua_mem_init(lu08 *memory, lu16 size); //initialize memory
ulua_memvar* ulua_mem_new(ulua_vartype type, lu16 size); //allocate memory for new type
lu08* ulua_mem_new_block(lu16 size); //allocate fixed memory data block. and mark it as locked.
void ulua_mem_free(ulua_memvar *var); //free variable and allocated memory
void ulua_mem_free_block(lu08 *block); //free memory block and associated variable with it
void ulua_mem_dump(); //dump all memory
void ulua_mem_dump_tree(); //dump all memory in tree view
void ulua_mem_dump_tree_memvar(ulua_memvar* memvar, lu08* name, lu08 level, ULUA_BOOL mark, ULUA_BOOL dump); //make tree dump for memory variable and all child variables

//string functions
ulua_memvar*    ulua_mem_string_new(lu08* chars);
ULUA_BOOL       ulua_mem_string_equals(ulua_memvar* str1, ulua_memvar* str2);

//list functions
ulua_memvar*    ulua_mem_list_new();
lu16            ulua_mem_list_size(ulua_memvar* list);
ulua_memvar*    ulua_mem_list_add(ulua_memvar* list, ulua_memvar* var);//return list
ulua_memvar*    ulua_mem_list_iter_init(ulua_memvar* list);//return new iterator
ulua_memvar*    ulua_mem_list_iter_next(ulua_memvar* iter);//return updated iterator
ulua_memvar*    ulua_mem_list_iter_value(ulua_memvar* iter);//return iterator value
ulua_memvar*    ulua_mem_list_iter_remove(ulua_memvar* list, ulua_memvar* iter);//remove current node and return updated iterator
ulua_memvar*    ulua_mem_list_get_first(ulua_memvar* list);//return list
ulua_memvar*    ulua_mem_list_get_last(ulua_memvar* list);//return list
//ulua_memvar*   ulua_mem_list_get(ulua_memvar* list, lu16 number);//return list
ulua_memvar*    ulua_mem_list_remove_first(ulua_memvar* list);//return list
ulua_memvar*    ulua_mem_list_remove_last(ulua_memvar* list);//return list
//ulua_memvar*   ulua_mem_list_remove(ulua_memvar* list, lu16 number);//return list
void            ulua_mem_list_free(ulua_memvar* list);

//hashtable functions
ulua_memvar*   ulua_mem_table_new();
ulua_memvar*   ulua_mem_table_put(ulua_memvar* table, ulua_memvar* key, ulua_memvar* value); //return table
ulua_memvar*   ulua_mem_table_get(ulua_memvar* table, ulua_memvar* key); //return value for key
ulua_memvar*   ulua_mem_table_remove(ulua_memvar* table, ulua_memvar* key); //revome value for key and key node, return value
void           ulua_mem_table_free(ulua_memvar* table);

//garbage collection function
lu16           ulua_mem_gc_collect();

#endif /* defined(ULUA_UMEMORY) */
