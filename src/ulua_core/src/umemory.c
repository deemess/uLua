#include "ulua_core/umemory.h"

lu08* ulua_memory;
ulua_memblock* ulua_first_block;
ulua_memblock* ulua_last_block;
ulua_memvar* ulua_mem_lowvar;
lu16 ulua_memory_size;
lu16 ulua_memory_free_size;
lu16 ulua_mem_freept;
lu16 ulua_vars_count;

void ulua_memoryinit(lu08* memory, lu16 size) { //initialize memory
    lu16 i;
    ulua_memory = memory;
    ulua_memory_size = size;
    ulua_memory_free_size = size;
    for(i=0; i<ulua_memory_size; i++) {
        ulua_memory[i] = 0;
    }
    ulua_mem_freept = 0;
    ulua_vars_count = 0;
    ulua_first_block = ULUA_NULL;
    ulua_last_block = ULUA_NULL;
    ulua_mem_lowvar = ULUA_NULL;
}

void ulua_defrag() { //make memory defragmentation to compress variables.
    lu16 i;
    lu16 size;
    ulua_memvar* var;
    lu08* pointer;
    ulua_memblock* iter = ulua_first_block;
    
    while(iter != ULUA_NULL) {
        if(iter->header.var->blocked == ULUA_TRUE) //do not move blocked memory blocks
            continue;
        
        if(iter->header.prev == ULUA_NULL) {
            if((lu08*)iter != ulua_memory) {
                //shift first block to the start of the memory
                var = iter->header.var;
                for(i=0, size=iter->header.size + sizeof(ulua_memblock_header); i<size; i++) {
                    *(ulua_memory + i) = ((lu08*)iter)[i];
                }
                iter = (ulua_memblock*)ulua_memory;
                var->memblock = iter;
                if(iter->header.next != ULUA_NULL)
                    iter->header.next->header.prev = iter;
            }
        } else {
            if( (lu08*)iter->header.prev + sizeof(ulua_memblock_header) + iter->header.prev->header.size < (lu08*)iter - 1  ) {
                pointer = (lu08*)iter->header.prev + sizeof(ulua_memblock_header) + iter->header.prev->header.size;
                var = iter->header.var;
                for(i=0, size=iter->header.size + sizeof(ulua_memblock_header); i<size; i++) {
                    *(pointer + i) = ((lu08*)iter)[i];
                }
                iter = (ulua_memblock*)pointer;
                var->memblock = iter;
                iter->header.prev->header.next = iter;
                if(iter->header.next != ULUA_NULL) {
                    iter->header.next->header.prev = iter;
                } else {
                    ulua_mem_freept = ( (lu08*)iter + sizeof(ulua_memblock_header) + size ) - ulua_memory;
                }
            }
        }
        iter = iter->header.next;
    }
}

ulua_memblock* ulua_mem_findfreespace(lu16 size) { //try to find free space between blocks
    ulua_memblock* block;
    ulua_memblock* iter = ulua_first_block;
    
    while(iter != ULUA_NULL) {
        if(iter->header.prev == ULUA_NULL) {
            if( (lu08*)iter - ulua_memory >= (size + sizeof(ulua_memblock_header)) ) {
                block  = (ulua_memblock*)ulua_memory;
                iter->header.prev = block;
                block->header.next = iter;
                block->header.prev = ULUA_NULL;
                ulua_first_block = block;
                return block;
            }
        } else {
            if( (lu08*)iter - (&iter->header.prev->data + iter->header.prev->header.size) >= (size + sizeof(ulua_memblock_header)) ) {
                block  = (ulua_memblock*)( &iter->header.prev->data + iter->header.prev->header.size );
                block->header.prev = iter->header.prev;
                block->header.next = iter;
                iter->header.prev->header.next = block;
                iter->header.prev = block;
                return block;
            }
        }
        iter = iter->header.next;
    }
    
    return ULUA_NULL;
}

ulua_memvar* ulua_allocate(ulua_vartype type, lu16 size) { //allocate memory for new type
    ulua_memblock* block;
    ulua_memvar* lowvar = ulua_mem_lowvar;
    ulua_memvar* var = (ulua_memvar*)( &ulua_memory[ulua_memory_size] - sizeof(ulua_memvar) );
    
    if(size + sizeof(ulua_memvar) + sizeof(ulua_memblock_header) > ulua_memory_free_size)
        return ULUA_NULL;
    
    //if we have no hols in vars - just allocate lowest
    if( (ulua_memory+ulua_memory_size - (lu08*)ulua_mem_lowvar)/sizeof(ulua_memvar) == ulua_vars_count) {
        var = ulua_mem_lowvar - 1;
    } else {
        //find free var in memory (from the tail)
        while(var->type != ULUA_NULL && (lu08*)(var) > &ulua_memory[ulua_mem_freept] ) {
            var =(ulua_memvar*) ( (lu08*)var - sizeof(ulua_memvar) );
        }
    }
    
    //if no free memory - make defragmentation
    if((lu08*)(var) <= &ulua_memory[ulua_mem_freept]) {
        ulua_defrag();
        //try to find free var again
        var = (ulua_memvar*)( &ulua_memory[ulua_memory_size-1] - sizeof(ulua_memvar) );
        while(var->type != ULUA_TYPE_NULL && (lu08*)(var) > &ulua_memory[ulua_mem_freept] ) {
            var =(ulua_memvar*) ( (lu08*)var - sizeof(ulua_memvar) );
        }
        //if still no free memory - return null
        if((lu08*)(var) <= &ulua_memory[ulua_mem_freept])
            return ULUA_NULL;
    }

    if(lowvar == ULUA_NULL || var < lowvar)
        lowvar = var;
    
    //find free block
    if( &ulua_memory[ulua_mem_freept] + sizeof(ulua_memblock_header) + size > (lu08*)(lowvar) ) {
        //no memory in the end of heap
        block = ulua_mem_findfreespace(size);
        //if no enought memory - try to make defrag
        if(block == ULUA_NULL) {
            ulua_defrag();
            block = (ulua_memblock*)( &ulua_memory[ulua_mem_freept] );
            //if still not enought memory - return null
            if(&ulua_memory[ulua_mem_freept] + sizeof(ulua_memblock_header) + size > (lu08*)(lowvar)) {
                return ULUA_NULL;
            } else {
                //init allocated block
                if(ulua_last_block == ULUA_NULL && ulua_first_block == ULUA_NULL) {
                    ulua_last_block = block;
                    ulua_first_block = block;
                    block->header.next = ULUA_NULL;
                    block->header.prev = ULUA_NULL;
                } else {
                    block->header.prev = ulua_last_block;
                    block->header.next = ULUA_NULL;
                    ulua_last_block = block;
                }
            }
        }
    } else {
        //normal allocation in the end of heap
        block = (ulua_memblock*)( &ulua_memory[ulua_mem_freept] );
        ulua_mem_freept += sizeof(ulua_memblock_header) + size;
        if(ulua_last_block == ULUA_NULL && ulua_first_block == ULUA_NULL) {
            ulua_last_block = block;
            ulua_first_block = block;
            block->header.next = ULUA_NULL;
            block->header.prev = ULUA_NULL;
        } else {
            block->header.prev = ulua_last_block;
            block->header.next = ULUA_NULL;
            ulua_last_block->header.next = block;
            ulua_last_block = block;
        }
    }
    
    //init var and return it
    block->header.size = size;
    block->header.var = var;
    var->memblock = block;
    var->refcount = 0;
    var->type = type;
    var->blocked = ULUA_FALSE;
    ulua_vars_count++;
    ulua_memory_free_size -= ( size + sizeof(ulua_memblock_header) + sizeof(ulua_memvar) );
    ulua_mem_lowvar = lowvar;
    return var;
}

lu08* ulua_allocate_block(lu16 size) { //allocate fixed memory data block. and mark it as locked.
    ulua_memvar* var = ulua_allocate(ULUA_TYPE_DATABLOCK, size);
    if(var == ULUA_NULL)
        return ULUA_NULL;
    
    var->blocked = ULUA_TRUE;
    var->refcount = 1;
    return &var->memblock->data;
}

void ulua_free(ulua_memvar* var) { //free variable and allocated memory
    if (var == ULUA_NULL)
        return;

    if(var->memblock->header.prev != ULUA_NULL && var->memblock->header.next != ULUA_NULL) {
        var->memblock->header.prev->header.next = var->memblock->header.next;
        var->memblock->header.next->header.prev = var->memblock->header.prev;
        
    } else if (var->memblock->header.prev != ULUA_NULL && var->memblock->header.next == ULUA_NULL) {
        //last block - update free pointer
        ulua_mem_freept = ( (lu08*)var->memblock->header.prev + var->memblock->header.prev->header.size + sizeof(ulua_memblock_header)) - ulua_memory;
        ulua_last_block = var->memblock->header.prev;
        
    } else if(var->memblock->header.prev == ULUA_NULL && var->memblock->header.next != ULUA_NULL) {
        var->memblock->header.next->header.prev = ULUA_NULL;
        ulua_first_block = var->memblock->header.next;

    } else { //var->memblock->header.prev == ULUA_NULL && var->memblock->header.next == ULUA_NULL
        //last block
        ulua_mem_freept = 0;
        ulua_last_block = ULUA_NULL;
        ulua_first_block = ULUA_NULL;
    }
    
    ulua_memory_free_size += ( var->memblock->header.size + sizeof(ulua_memblock_header) + sizeof(ulua_memvar) );
    
    var->memblock->header.size = 0;
    var->memblock->header.var = ULUA_NULL;
    var->memblock->header.prev = ULUA_NULL;
    var->memblock->header.next = ULUA_NULL;

    var->type = ULUA_NULL;
    var->refcount = 0;
    var->memblock = ULUA_NULL;
    var->blocked = ULUA_FALSE;
    ulua_vars_count--;
}

void ulua_free_block(lu08* block) { //free memory block and associated variable with it
    lu16 varcount = ulua_vars_count;
    ulua_memvar* var = (ulua_memvar*)( &ulua_memory[ulua_memory_size-1] - sizeof(ulua_memvar) );

    //find var for that block
    while(&var->memblock->data != block && varcount > 0 ) {
        var =(ulua_memvar*) ( (lu08*)var - sizeof(ulua_memvar) );
        if(var->type != ULUA_TYPE_NULL) varcount--;
    }
    
    if(&var->memblock->data != block)
        return;
    
    //free found var
    var->blocked = ULUA_FALSE;
    var->refcount--;
    ulua_free(var);

}
