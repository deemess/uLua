#include "ulua_core/umemory.h"
#include "ulua_core/vm.h"

lu08* ulua_memory;
ulua_memblock* ulua_first_block;
ulua_memblock* ulua_last_block;
ulua_memvar* ulua_mem_lowvar;
lu16 ulua_memory_size;
lu16 ulua_memory_free_size;
lu16 ulua_mem_freept;
lu16 ulua_vars_count;

void ulua_mem_dump_memvar(ulua_memblock* block) {
    switch (block->header.var->type) {
    case ULUA_MEM_TYPE_NULL:
        printf("NULL\n");
        break;
    case ULUA_MEM_TYPE_DATABLOCK:
        printf("DATABLOCK\n");
        break;
    case ULUA_MEM_TYPE_STRING:
        printf("STRING \"%.*s\"\n", block->header.size, MEMVARVALUE(lu08*, block->header.var));
        break;
    case ULUA_MEM_TYPE_VMREGISTER:
        printf("VMREGISTER ");
        switch (MEMVARVALUE(vmregister*, block->header.var)->type) {
        case REGISTER_VAR_NULL:
            printf("NULL \n");
            break;
        case REGISTER_VAR_BOOLEAN:
            printf("BOOLEAN ");
            printf("float: %.0f \n", MEMVARVALUE(vmregister*, block->header.var)->floatval);
            break;
        case REGISTER_VAR_NUMBER:
            printf("NUMBER ");
            printf("number: %d \n", MEMVARVALUE(vmregister*, block->header.var)->numval);
            break;
        case REGISTER_VAR_FLOAT:
            printf("FLOAT ");
            printf("float: %.0f \n", MEMVARVALUE(vmregister*, block->header.var)->floatval);
            break;
        case REGISTER_VAR_CLOSURE:
            printf("CLOSURE ");
            printf("pointer: 0x%08x \n", MEMVARVALUE(vmregister*, block->header.var)->pointer);
            break;
        case REGISTER_VAR_NATIVE_FUNC:
            printf("NATIVE_FUNC ");
            printf("pointer: 0x%08x \n", MEMVARVALUE(vmregister*, block->header.var)->pointer);
            break;
        case REGISTER_VAR_MEMVAR:
            printf("REGISTER_VAR_MEMVAR ");
            printf("pointer: 0x%08x \n", MEMVARVALUE(vmregister*, block->header.var)->pointer);
            break;
        }
        break;
    case ULUA_MEM_TYPE_LIST:
        printf("LIST size: %d first: 0x%08x last: 0x%08x \n", MEMVARVALUE(ulua_mem_list*, block->header.var)->size,
            MEMVARVALUE(ulua_mem_list*, block->header.var)->first,
            MEMVARVALUE(ulua_mem_list*, block->header.var)->last);
        break;
    case ULUA_MEM_TYPE_LIST_NODE:
        printf("LIST_NODE var: 0x%08x prev: 0x%08x next: 0x%08x \n", MEMVARVALUE(ulua_mem_list_node*, block->header.var)->var,
            MEMVARVALUE(ulua_mem_list_node*, block->header.var)->prev,
            MEMVARVALUE(ulua_mem_list_node*, block->header.var)->next);
        break;
    case ULUA_MEM_TYPE_TABLE:
        printf("TABLE size: %d ", MEMVARVALUE(ulua_mem_table*, block->header.var)->size);
        for (int i = 0; i < ULUA_MEM_TABLE_SIZE; i++) {
            printf(" List[%d] var: 0x%08x", i, MEMVARVALUE(ulua_mem_table*, block->header.var)->list_array[i]);
        }
        printf("\n");
        break;
    case ULUA_MEM_TYPE_TABLE_NODE:
        printf("TABLE_NODE key: 0x%08x value: 0x%08x hash: 0x%08x \n", MEMVARVALUE(ulua_mem_table_node*, block->header.var)->key,
            MEMVARVALUE(ulua_mem_table_node*, block->header.var)->value,
            MEMVARVALUE(ulua_mem_table_node*, block->header.var)->hash);
        break;
    case ULUA_MEM_TYPE_VM:
        printf("VM \n");
        break;
    }
}

void ulua_mem_dump() {
    printf("Memory address: 0x%08x size: %d used: %d free: %d allocated: %d block header size: %d\n", ulua_memory,
            ulua_memory_size, ulua_memory_size- ulua_memory_free_size, ulua_memory_free_size, ulua_vars_count, sizeof(ulua_memblock));
    ulua_memblock* block = ulua_first_block;
    while(block != ULUA_NULL) {
        printf("   Block 0x%08x var: 0x%08x size: %d+%d+%d ", block, block->header.var, sizeof(ulua_memblock),
                sizeof(ulua_memvar), block->header.size);
        ulua_mem_dump_memvar(block);
        block = block->header.next;
    }
    printf("\n");
}

void ulua_mem_dump_tree_print_level(lu08 level) {
    for (lu08 i = 0; i < level; i++) {
        printf(" ");
    }
    if (level != 0) {
        printf("+");
    }
    else {
        printf("-");
    }
    printf("-> ");
}

void ulua_mem_dump_tree_iter_list(ulua_mem_list* memvar, lu08 level, ULUA_BOOL mark, ULUA_BOOL dump) {
    ulua_memvar* nodememvar = memvar->first;
    while (nodememvar != ULUA_NULL) {
        ulua_mem_list_node* node = MEMVARVALUE(ulua_mem_list_node*, nodememvar);
        ulua_mem_dump_tree_memvar(nodememvar, "NextNode", level + 1, mark, dump);
        nodememvar = node->next;
    }
}

void ulua_mem_dump_tree_iter_table(ulua_mem_table* memvar, lu08 level, ULUA_BOOL mark, ULUA_BOOL dump) {
    for (lu08 i = 0; i < ULUA_MEM_TABLE_SIZE; i++) {
        ulua_mem_dump_tree_memvar(memvar->list_array[i], "TableList", level + 1, mark, dump);
    }
}

void ulua_mem_dump_tree_memvar(ulua_memvar* memvar, lu08* name, lu08 level, ULUA_BOOL mark, ULUA_BOOL dump/*, ULUA_BOOL clearflag*/) {
    if (mark == ULUA_TRUE)
        SETBIT(memvar->flags, ULUA_MEM_FLAG_MARK);
        //memvar->flags = memvar->flags | ULUA_MEM_FLAG_MARK; //set MARK flag for memory variable. Once tree is pass, all unmarked memory variables could be deleted

    //if (clearflag == ULUA_TRUE)
    //    memvar->flags = memvar->flags & (~ULUA_MEM_FLAG_MARK); //clear MARK flag

    if (dump) {
        ulua_mem_dump_tree_print_level(level);
        printf("%s 0x%08x ", name, memvar);
        ulua_mem_dump_memvar(memvar->memblock);
    }
    switch (memvar->type) { //TODO: support other composite structures
        case ULUA_MEM_TYPE_LIST:
            ulua_mem_dump_tree_iter_list(MEMVARVALUE(ulua_mem_list*, memvar), level + 1, mark, dump);
            break;
        case ULUA_MEM_TYPE_TABLE:
            ulua_mem_dump_tree_iter_table(MEMVARVALUE(ulua_mem_table*, memvar), level + 1, mark, dump);
            break;
        case ULUA_MEM_TYPE_LIST_NODE:
            ulua_mem_dump_tree_memvar(MEMVARVALUE(ulua_mem_list_node*, memvar)->var, "Value", level + 1, mark, dump);
            break;
        case ULUA_MEM_TYPE_TABLE_NODE:
            ulua_mem_dump_tree_memvar(MEMVARVALUE(ulua_mem_table_node*, memvar)->key, "Key", level + 1, mark, dump);
            ulua_mem_dump_tree_memvar(MEMVARVALUE(ulua_mem_table_node*, memvar)->value, "Value", level + 1, mark, dump);
            break;
        case ULUA_MEM_TYPE_VM:
            //iterate global vars
            ulua_mem_dump_tree_memvar(MEMVARVALUE(ulua_vm*, memvar)->globals_table, "Globals", level + 1, mark, dump);
            //TODO: iterate other commposite structures of VM
            break;
        case ULUA_MEM_TYPE_VMREGISTER:
            if(MEMVARVALUE(vmregister*, memvar)->type == REGISTER_VAR_MEMVAR)
                ulua_mem_dump_tree_memvar((ulua_memvar*)MEMVARVALUE(vmregister*, memvar)->pointer, "VM Register", level + 1, mark, dump);
            break;
    }
}

void ulua_mem_dump_tree() {
    printf("Memory address: 0x%08x size: %d used: %d free: %d allocated: %d block header size: %d\n", ulua_memory,
        ulua_memory_size, ulua_memory_size - ulua_memory_free_size, ulua_memory_free_size, ulua_vars_count, sizeof(ulua_memblock));
    ulua_memblock* block = ulua_first_block;
    //find VM root block
    while (block != ULUA_NULL) {
        if (block->header.var->type != ULUA_MEM_TYPE_VM) {
            block = block->header.next;
            continue;
        }
        break;
    }
    //first pass - print tree
    //iterate VM structures
    lu08 level = 0;
    ulua_mem_dump_tree_memvar(block->header.var, "Lua VM", level, ULUA_TRUE, ULUA_TRUE);

    //second pass - print unused memory variables
    printf(" ................\n");
    printf(" .... unused ....\n");
    printf(" ................\n");
    block = ulua_first_block;
    while (block != ULUA_NULL) {
        if (!(block->header.var->flags & ULUA_MEM_FLAG_MARK) &&
            !(block->header.var->flags & ULUA_MEM_FLAG_BLOCKED)) { //MARK flag is not set and unblocked - unused memory variable
            ulua_mem_dump_tree_memvar(block->header.var, "UNUSED", level, ULUA_FALSE, ULUA_TRUE);
        }
        //clear MARK flag
        RESBIT(block->header.var->flags, ULUA_MEM_FLAG_MARK);
        //block->header.var->flags = block->header.var->flags & (~ULUA_MEM_FLAG_MARK); 
        block = block->header.next;
    }

    printf("\n");
}

lu16 ulua_mem_gc_collect() {
    ulua_memblock* block = ulua_first_block;
    //find VM root block
    while (block != ULUA_NULL) {
        if (block->header.var->type != ULUA_MEM_TYPE_VM) {
            block = block->header.next;
            continue;
        }
        break;
    }
    //first pass - mark used variables
    //iterate VM structures
    lu08 level = 0;
    ulua_mem_dump_tree_memvar(block->header.var, "Lua VM", level, ULUA_TRUE, ULUA_FALSE);

    //second pass - free unused memory variables
    lu16 collected = 0;
    block = ulua_first_block;
    while (block != ULUA_NULL) {
        ulua_memblock* nextblock = block->header.next;
        if (!(block->header.var->flags & ULUA_MEM_FLAG_MARK) &&
            !(block->header.var->flags & ULUA_MEM_FLAG_BLOCKED)) { //MARK flag is not set and unblocked - unused memory variable
            collected += block->header.size;
            ulua_mem_free(block->header.var);
        } else {
            //clear MARK flag
            RESBIT(block->header.var->flags, ULUA_MEM_FLAG_MARK);
            //block->header.var->flags = block->header.var->flags & (~ULUA_MEM_FLAG_MARK); 
        }
        block = nextblock;
    }

    return collected;
}

void ulua_mem_init(lu08 *memory, lu16 size) { //initialize memory
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
        if((iter->header.var->flags & ULUA_MEM_FLAG_BLOCKED) == ULUA_TRUE) //do not move flags memory blocks
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
            if( (lu08*)iter - (((lu08*)&iter->header.prev)+sizeof(ulua_memblock)+1 + iter->header.prev->header.size) >= (size + sizeof(ulua_memblock_header)) ) {
                block  = (ulua_memblock*)( ((lu08*)&iter->header.prev)+sizeof(ulua_memblock)+1 + iter->header.prev->header.size );
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

ulua_memvar* ulua_mem_new(ulua_vartype type, lu16 size) { //allocate memory for new type
    ulua_memblock* block;
    ulua_memvar* lowvar = ulua_mem_lowvar;
    ulua_memvar* var = (ulua_memvar*)( &ulua_memory[ulua_memory_size-1] - sizeof(ulua_memvar) ); //init var to the first one
    
    if(size + sizeof(ulua_memvar) + sizeof(ulua_memblock_header) > ulua_memory_free_size)
        return ULUA_NULL;
    

    if(ulua_mem_lowvar == ULUA_NULL) { //first allocation
        var = (ulua_memvar*) &ulua_memory[ulua_memory_size-1 - sizeof(ulua_memvar)];
        ulua_mem_lowvar = var;
        lowvar = var;
    } else {
        //if we have no holes in vars - just allocate lowest
        if( (ulua_memory+ulua_memory_size - 1 - (lu08*)ulua_mem_lowvar)/sizeof(ulua_memvar) == ulua_vars_count) {
            var = ulua_mem_lowvar - 1;
        } else {
            //find free var in memory (from the tail)
            while(var->type != ULUA_NULL && (lu08*)(var) > &ulua_memory[ulua_mem_freept] ) {
                var = var - 1;
            }
        }
    }

    //if no free memory - make defragmentation
    if((lu08*)(var) <= &ulua_memory[ulua_mem_freept]) {
        ulua_defrag();
        //try to find free var again
        var = (ulua_memvar*)( &ulua_memory[ulua_memory_size-1] - sizeof(ulua_memvar) );
        while(var->type != ULUA_MEM_TYPE_NULL && (lu08*)(var) > &ulua_memory[ulua_mem_freept] ) {
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
        ulua_mem_freept += sizeof(ulua_memblock) + size;
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
    var->type = type;
    var->flags = 0;
    ulua_vars_count++;
    ulua_memory_free_size -= ( size + sizeof(ulua_memblock) + sizeof(ulua_memvar) );
    ulua_mem_lowvar = lowvar;

    return var;
}

lu08* ulua_mem_new_block(lu16 size) { //allocate fixed memory data block. and mark it as locked.
    ulua_memvar* var = ulua_mem_new(ULUA_MEM_TYPE_DATABLOCK, size);
    if(var == ULUA_NULL)
        return ULUA_NULL;
    
    SETBIT(var->flags, ULUA_MEM_FLAG_BLOCKED);
    //var->flags = var->flags & ULUA_MEM_FLAG_BLOCKED;
    return MEMVARVALUE(lu08*, var); // ((lu08*)&var->memblock)+sizeof(ulua_memblock)+1;
}

void ulua_mem_free(ulua_memvar *var) { //free variable and allocated memory
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
    var->memblock = ULUA_NULL;
    var->flags = 0;
    ulua_vars_count--;
}

void ulua_mem_free_block(lu08 *block) { //free memory block and associated variable with it
    ulua_memvar* var = VALUEMEMVAR(block);
    //check if var is aiming to the same block
    if (MEMVARVALUE(lu08*,var) != block)
        return;

    //free found var
    var->flags = 0;
    ulua_mem_free(var);
}

//string functions
ulua_memvar*    ulua_mem_string_new(lu08* chars) {
    lu16 p = 0;
    lu08 c = chars[p];
    lu16 len = 0;
    while(c != 0) {
        len++; p++;
        c = chars[p];
    }
    ulua_memvar* stringvar = ulua_mem_new(ULUA_MEM_TYPE_STRING, len+1);
    lu08* string = MEMVARVALUE(lu08*, stringvar);
    for(lu16 i=0; i<len; i++) {
        string[i] = chars[i];
    }
    string[len] = 0;
    return stringvar;
}
ULUA_BOOL       ulua_mem_string_cmp(lu08* str1, lu08* str2) {
    lu16 p=0;
    lu08 c1 = str1[p];
    lu08 c2 = str2[p];
    while(c1 != 0 && c2 != 0) {
        if(c1 != c2)
            return ULUA_FALSE;
        p++;
        c1 = str1[p];
        c2 = str2[p];
    }
    if(c1 == 0 && c2 == 0)
        return ULUA_TRUE;
    return ULUA_FALSE;
}
ULUA_BOOL       ulua_mem_string_equals(ulua_memvar* str1, ulua_memvar* str2) {
    return ulua_mem_string_cmp(MEMVARVALUE(lu08*, str1), MEMVARVALUE(lu08*, str2));
}

//list functions
ulua_memvar* ulua_mem_list_new() {
    ulua_memvar* varlist = ulua_mem_new(ULUA_MEM_TYPE_LIST, sizeof(ulua_mem_list));
    ulua_mem_list* list = MEMVARVALUE(ulua_mem_list*, varlist);
    list->size = 0;
    list->first = ULUA_NULL;
    list->last = ULUA_NULL;

    return  varlist;
}
lu16           ulua_mem_list_size(ulua_memvar* list) {
    ulua_mem_list* l = MEMVARVALUE(ulua_mem_list*, list);
    return l->size;
}
ulua_memvar*   ulua_mem_list_add(ulua_memvar* list, ulua_memvar* var) {
    ulua_mem_list* l = MEMVARVALUE(ulua_mem_list*, list);
    ulua_memvar* nodevar = ulua_mem_new(ULUA_MEM_TYPE_LIST_NODE, sizeof(ulua_mem_list_node));
    ulua_mem_list_node* node = MEMVARVALUE(ulua_mem_list_node*, nodevar);
    node->var = var;
    node->next = ULUA_NULL;
    node->prev = ULUA_NULL;

    if(l->last == ULUA_NULL) { //first item in a list
        l->last = nodevar;
        if(l->first == ULUA_NULL) {
            l->first = nodevar;
        }
    } else { //normal adding to the end
        ulua_mem_list_node* lastnode = MEMVARVALUE(ulua_mem_list_node*, l->last);

        node->prev = l->last;
        lastnode->next = nodevar;
        l->last = nodevar;
    }

    l->size++;

    return list;
}
ulua_memvar* ulua_mem_list_iter_init(ulua_memvar* list) {
    ulua_mem_list* l = MEMVARVALUE(ulua_mem_list*, list);
    return l->first;
}
ulua_memvar* ulua_mem_list_iter_next(ulua_memvar* iter) {
    if(iter == ULUA_NULL)
        return ULUA_NULL;
    ulua_memvar* iternext = MEMVARVALUE(ulua_mem_list_node*, iter)->next;
    return iternext;
}
ulua_memvar*    ulua_mem_list_iter_value(ulua_memvar* iter){
    if(iter == ULUA_NULL)
        return ULUA_NULL;
    return MEMVARVALUE(ulua_mem_list_node*, iter)->var;
}
ulua_memvar*    ulua_mem_list_iter_remove(ulua_memvar* list, ulua_memvar* iter) {
    ulua_mem_list* l = MEMVARVALUE(ulua_mem_list*, list);
    ulua_mem_list_node* iternode = MEMVARVALUE(ulua_mem_list_node*, iter);
    ulua_memvar* prevnodevar = iternode->prev;
    ulua_memvar* nextnodevar = iternode->next;
    if(prevnodevar != ULUA_NULL) {
        MEMVARVALUE(ulua_mem_list_node*, prevnodevar)->next = nextnodevar;
    }
    if(nextnodevar != ULUA_NULL) {
        MEMVARVALUE(ulua_mem_list_node*, nextnodevar)->prev = prevnodevar;
    }
    if(l->first == iter) {
        l->first = nextnodevar;
        if(nextnodevar != ULUA_NULL)
            MEMVARVALUE(ulua_mem_list_node*, nextnodevar)->prev = ULUA_NULL;
    }
    if(l->last == iter) {
        l->last = prevnodevar;
        if(prevnodevar != ULUA_NULL)
            MEMVARVALUE(ulua_mem_list_node*, prevnodevar)->next = ULUA_NULL;
    }
    ulua_mem_free(iter);
    return nextnodevar;
}
ulua_memvar*   ulua_mem_list_get_first(ulua_memvar* list) {
    ulua_mem_list* l = MEMVARVALUE(ulua_mem_list*, list);
    return MEMVARVALUE(ulua_mem_list_node*, l->first)->var;
}
ulua_memvar*   ulua_mem_list_get_last(ulua_memvar* list) {
    ulua_mem_list* l = MEMVARVALUE(ulua_mem_list*, list);
    return MEMVARVALUE(ulua_mem_list_node*, l->last)->var;
}
//ulua_memvar*   ulua_mem_list_get(ulua_memvar* list, lu16 number);
ulua_memvar*   ulua_mem_list_remove_first(ulua_memvar* list) {
    ulua_mem_list* l = MEMVARVALUE(ulua_mem_list*, list);
    ulua_memvar* firstnodevar = l->first;
    ulua_mem_list_node* firstnode = MEMVARVALUE(ulua_mem_list_node*, firstnodevar);
    l->first = firstnode->next;
    MEMVARVALUE(ulua_mem_list_node*, firstnode->next)->prev = ULUA_NULL;

    ulua_mem_free(firstnodevar);

    return l->first;
}
ulua_memvar*   ulua_mem_list_remove_last(ulua_memvar* list) {
    ulua_mem_list* l = MEMVARVALUE(ulua_mem_list*, list);
    ulua_memvar* lastnodevar = l->last;
    ulua_mem_list_node* lastnode = MEMVARVALUE(ulua_mem_list_node*, lastnodevar);
    l->last = lastnode->prev;
    MEMVARVALUE(ulua_mem_list_node*, lastnode->prev)->next = ULUA_NULL;

    ulua_mem_free(lastnodevar);

    return l->last;
}
void          ulua_mem_list_free(ulua_memvar* list) {
    ulua_mem_list* l = MEMVARVALUE(ulua_mem_list*, list);
    ulua_memvar* node = l->first;
    while(node != ULUA_NULL) {
        ulua_memvar* nextnode = MEMVARVALUE(ulua_mem_list_node*, node)->next;
        ulua_mem_free(node);
        node = nextnode;
    }
    ulua_mem_free(list);
}


//hashtable functions
ulua_memvar*   ulua_mem_table_new() {
    ulua_memvar* tablevar = ulua_mem_new(ULUA_MEM_TYPE_TABLE, sizeof(ulua_mem_table));
    ulua_mem_table* table = MEMVARVALUE(ulua_mem_table*, tablevar);
    table->size = 0;
    for(lu16 i=0; i<ULUA_MEM_TABLE_SIZE; i++) {
        table->list_array[i] = ulua_mem_list_new();
    }
    return tablevar;
}
lu16           ulua_mem_table_hash(ulua_memvar* key) {
    lu08* string;
    lu16 hashcode = 0;
    vmregister* reg;
    if(key == ULUA_NULL)
        return 0;
    switch (key->type) {
        case ULUA_MEM_TYPE_NULL:
            return 0;
        case ULUA_MEM_TYPE_STRING:
            string = MEMVARVALUE(lu08*, key);
            for(lu16 i=0; i<key->memblock->header.size; i++) {
                hashcode += string[i];
            }
            return hashcode;
        case ULUA_MEM_TYPE_VMREGISTER:
            reg = MEMVARVALUE(vmregister*, key);
            return (uintptr_t)reg->pointer;
        default:
            return (uintptr_t)key;
    }
}
ULUA_BOOL      ulua_mem_table_key_equals(ulua_memvar* key1, ulua_memvar* key2) {
    vmregister* reg1;
    vmregister* reg2;
    if(key1 == ULUA_NULL || key2 == ULUA_NULL)
        return ULUA_FALSE;
    if(key1->type == key2->type) {
        switch (key1->type) {
            case ULUA_MEM_TYPE_STRING:
                return ulua_mem_string_equals(key1, key2);
            case ULUA_MEM_TYPE_VMREGISTER:
                reg1 = MEMVARVALUE(vmregister*, key1);
                reg2 = MEMVARVALUE(vmregister*, key2);
                if(reg1->type == reg2->type && reg1->pointer == reg2->pointer) {
                    return ULUA_TRUE;
                }
            default:
                if(key1 == key2)
                    return ULUA_TRUE;
                break;
        }
    }
    return ULUA_FALSE;
}
ulua_memvar*   ulua_mem_table_put(ulua_memvar* table, ulua_memvar* key, ulua_memvar* value) {
    ulua_mem_table* t = MEMVARVALUE(ulua_mem_table*, table);
    lu16 hashcode = ulua_mem_table_hash(key);
    lu16 index = hashcode % ULUA_MEM_TABLE_SIZE;
    ulua_memvar* listvar = t->list_array[index];

    ulua_memvar* tnodevar = ULUA_NULL;
    ulua_memvar* listiter = ulua_mem_list_iter_init(listvar);
    while(listiter != ULUA_NULL) {//lookup key
        ulua_mem_table_node* tnode = MEMVARVALUE(ulua_mem_table_node*, ulua_mem_list_iter_value(listiter));
        if(tnode->hash == hashcode && ulua_mem_table_key_equals(tnode->key, key)) {// found key - update value
            tnode->value = value;
            tnodevar = ulua_mem_list_iter_value(listiter);
            break;
        }
        listiter = ulua_mem_list_iter_next(listiter);
    }
    if(tnodevar == ULUA_NULL) {//allocate new node
        tnodevar = ulua_mem_new(ULUA_MEM_TYPE_TABLE_NODE, sizeof(ulua_mem_table_node));
        ulua_mem_table_node *tnode = MEMVARVALUE(ulua_mem_table_node*, tnodevar);
        tnode->key = key;
        tnode->value = value;
        tnode->hash = hashcode;
        ulua_mem_list_add(listvar, tnodevar);
    }
    t->size++;
    return table;
}
ulua_memvar*   ulua_mem_table_get(ulua_memvar* table, ulua_memvar* key) {
    ulua_mem_table* t = MEMVARVALUE(ulua_mem_table*, table);
    lu16 hashcode = ulua_mem_table_hash(key);
    lu16 index = hashcode % ULUA_MEM_TABLE_SIZE;
    ulua_memvar* listvar = t->list_array[index];

    ulua_memvar* listiter = ulua_mem_list_iter_init(listvar);
    while(listiter != ULUA_NULL) {//lookup key
        ulua_mem_table_node* tnode = MEMVARVALUE(ulua_mem_table_node*, ulua_mem_list_iter_value(listiter));
        if(tnode->hash == hashcode && ulua_mem_table_key_equals(tnode->key, key)) {// found key - update value
            return tnode->value;
        }
        listiter = ulua_mem_list_iter_next(listiter);
    }
    return ULUA_NULL;
}
ulua_memvar*   ulua_mem_table_remove(ulua_memvar* table, ulua_memvar* key){
    ulua_mem_table* t = MEMVARVALUE(ulua_mem_table*, table);
    lu16 hashcode = ulua_mem_table_hash(key);
    lu16 index = hashcode % ULUA_MEM_TABLE_SIZE;
    ulua_memvar* listvar = t->list_array[index];
    ulua_memvar* value = ULUA_NULL;

    ulua_memvar* listiter = ulua_mem_list_iter_init(listvar);
    while(listiter != ULUA_NULL) {//lookup key
        ulua_memvar* tnodevar = ulua_mem_list_iter_value(listiter);
        ulua_mem_table_node* tnode = MEMVARVALUE(ulua_mem_table_node*, tnodevar);
        if(tnode->hash == hashcode && ulua_mem_table_key_equals(tnode->key, key)) {// found key - update value
            value = tnode->value;
            listiter = ulua_mem_list_iter_remove(listvar, listiter);
            ulua_mem_free(tnodevar);
            break;
        }
        listiter = ulua_mem_list_iter_next(listiter);
    }
    t->size--;
    return value;
}
void           ulua_mem_table_free(ulua_memvar* table) {
    ulua_mem_table* t = MEMVARVALUE(ulua_mem_table*, table);
    for(lu16 i=0; i<ULUA_MEM_TABLE_SIZE; i++) {
        ulua_memvar* listvar = t->list_array[i];
        ulua_memvar* listiter = ulua_mem_list_iter_init(listvar);
        while(listiter != ULUA_NULL) {
            ulua_mem_free(ulua_mem_list_iter_value(listiter));
            listiter = ulua_mem_list_iter_next(listiter);
        }
        ulua_mem_list_free(listvar);
    }
    ulua_mem_free(table);
}
