#include "umemory.h"

lu08 ulua_memory[UMEMORY_SIZE];
lu16 ulua_mem_freept;
lu16 ulua_vars_count;

void ulua_memoryinit() { //initialize memory
    lu16 i;
    for(i=0; i<UMEMORY_SIZE; i++) {
        ulua_memory[i] = 0;
    }
    ulua_mem_freept = 0;
    ulua_vars_count = 0;
}

void ulua_defrag() { //make memory defragmentation to compress variables.
    
}

ulua_memvar* ulua_allocate(ulua_vartype type, lu16 size) { //allocate memory for new type
    ulua_memvar* var = (ulua_memvar*)( &ulua_memory[UMEMORY_SIZE-1] - sizeof(ulua_memvar) );
    
    //find free var in memory (from the tail)
    while(var->type != ULUA_NULL && (lu08*)(var) > &ulua_memory[ulua_mem_freept] ) {
        var =(ulua_memvar*) ( (lu08*)var - sizeof(ulua_memvar) );
    }
    
    //if no free memory - make defragmentation
    if((lu08*)(var) <= &ulua_memory[ulua_mem_freept]) {
        ulua_defrag();
        //try to find free var again
        var = (ulua_memvar*)( &ulua_memory[UMEMORY_SIZE-1] - sizeof(ulua_memvar) );
        while(var->type != ULUA_TYPE_NULL && (lu08*)(var) > &ulua_memory[ulua_mem_freept] ) {
            var =(ulua_memvar*) ( (lu08*)var - sizeof(ulua_memvar) );
        }
        //if still no free memory - return null
        if((lu08*)(var) <= &ulua_memory[ulua_mem_freept])
            return ULUA_NULL;
    }
    
    var->datapt = &ulua_memory[ulua_mem_freept];
    //if no enought memory - try to make defrag
    if( &ulua_memory[ulua_mem_freept] + size > (lu08*)(var) ) {
        ulua_defrag();
        //if still not enought memory - try to find between vars
        if(&ulua_memory[ulua_mem_freept] + size > (lu08*)(var)) {
            //TODO: find free memory between vars
        }
    }
    
    //init var and return it
    var->refcount = 0;
    var->size = size;
    var->type = type;
    var->blocked = ULUA_FALSE;
    ulua_vars_count++;
    
    return var;
}

lu08* ulua_allocate_block(lu16 size) { //allocate fixed memory data block. and mark it as locked.
    ulua_memvar* var = ulua_allocate(ULUA_TYPE_DATABLOCK, size);
    if(var == ULUA_NULL)
        return ULUA_NULL;
    
    var->blocked = ULUA_TRUE;
    var->refcount = 1;
    return var->datapt;
}

void ulua_free(ulua_memvar* var) { //free variable and allocated memory
    if (var == ULUA_NULL)
        return;
    
    if((var->datapt + var->size) == &ulua_memory[ulua_mem_freept])
        ulua_mem_freept = var->datapt - &ulua_memory[0];
    
    var->type = ULUA_NULL;
    var->size = 0;
    var->refcount = 0;
    var->datapt = ULUA_NULL;
    var->blocked = ULUA_FALSE;
    ulua_vars_count--;
}

void ulua_free_block(lu08* block) { //free memory block and associated variable with it
    lu16 varcount = ulua_vars_count;
    ulua_memvar* var = (ulua_memvar*)( &ulua_memory[UMEMORY_SIZE-1] - sizeof(ulua_memvar) );

    //find var for that block
    while(var->datapt != block && varcount > 0 ) {
        var =(ulua_memvar*) ( (lu08*)var - sizeof(ulua_memvar) );
        if(var->type != ULUA_TYPE_NULL) varcount--;
    }
    
    if(var->datapt != block)
        return;
    
    //free found var
    var->blocked = ULUA_FALSE;
    var->refcount--;
    ulua_free(var);

}
