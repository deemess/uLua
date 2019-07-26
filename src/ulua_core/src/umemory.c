#include "ulua_core/umemory.h"

lu08* ulua_memory;
ulua_memblock* ulua_first_block;
ulua_memblock* ulua_last_block;
ulua_memvar* ulua_mem_lowvar;
lu16 ulua_memory_size;
lu16 ulua_memory_free_size;
lu16 ulua_mem_freept;
lu16 ulua_vars_count;

void ulua_mem_dump() {
    printf("Memory address: 0x%08x size: 0x%08x free: 0x%08x allocated: %d \n", ulua_memory, ulua_memory_size, ulua_memory_free_size, ulua_vars_count);
    ulua_memblock* block = ulua_first_block;
    while(block != ULUA_NULL) {
        printf("   Block start: 0x%08x var: 0x%08x size: 0x%04x ", block, block->header.var, block->header.size);
        switch (block->header.var->type) {
            case ULUA_MEM_TYPE_NULL:
                printf("NULL\n");
                break;
            case ULUA_MEM_TYPE_DATABLOCK:
                printf("DATABLOCK\n");
                break;
            case ULUA_MEM_TYPE_STRING:
                printf("STRING value: %.*s\n", block->header.size, GCVALUE(lu08*, block->header.var));
                break;
            case ULUA_MEM_TYPE_VMREGISTER:
                printf("VMREGISTER ");
                switch (GCVALUE(vmregister*, block->header.var)->type) {
                    case REGISTER_VAR_NULL:
                        printf("NULL ");
                        break;
                    case REGISTER_VAR_BOOLEAN:
                        printf("BOOLEAN ");
                        break;
                    case REGISTER_VAR_NUMBER:
                        printf("NUMBER ");
                        break;
                    case REGISTER_VAR_FLOAT:
                        printf("FLOAT ");
                        break;
                    case REGISTER_VAR_STRING:
                        printf("STRING ");
                        break;
                    case REGISTER_VAR_CLOSURE:
                        printf("CLOSURE ");
                        break;
                    case REGISTER_VAR_FILE_POINTER_STR:
                        printf("FILE_POINTER_STR ");
                        break;
                    case REGISTER_VAR_TABLE:
                        printf("TABLE ");
                        break;
                    case REGISTER_VAR_NATIVE_FUNC:
                        printf("NATIVE_FUNC ");
                        break;
                }
                printf("number: %d float: %.0f pointer: 0x%08x \n", GCVALUE(vmregister*, block->header.var)->numval,
                        GCVALUE(vmregister*, block->header.var)->floatval,
                        GCVALUE(vmregister*, block->header.var)->pointer);
                break;
            case ULUA_MEM_TYPE_LIST:
                printf("LIST size: %d first: 0x%08x last: 0x%08x \n", GCVALUE(ulua_mem_list*, block->header.var)->size,
                        GCVALUE(ulua_mem_list*, block->header.var)->first,
                        GCVALUE(ulua_mem_list*, block->header.var)->last);
                break;
            case ULUA_MEM_TYPE_LIST_NODE:
                printf("LIST_NODE var: 0x%08x prev: 0x%08x next: 0x%08x \n", GCVALUE(ulua_mem_list_node*, block->header.var)->var,
                       GCVALUE(ulua_mem_list_node*, block->header.var)->prev,
                       GCVALUE(ulua_mem_list_node*, block->header.var)->next);
                break;
            case ULUA_MEM_TYPE_TABLE:
                printf("TABLE size: %d\n", GCVALUE(ulua_mem_table*, block->header.var)->size);
                for(int i=0; i<ULUA_MEM_TABLE_SIZE; i++) {
                    printf("      List[%d] address: 0x%08x \n", i, GCVALUE(ulua_mem_table*, block->header.var)->list_array[i]);
                }
                break;
            case ULUA_MEM_TYPE_TABLE_NODE:
                printf("TABLE_NODE key: 0x%08x value: 0x%08x hash: 0x%08x \n", GCVALUE(ulua_mem_table_node*, block->header.var)->key,
                       GCVALUE(ulua_mem_table_node*, block->header.var)->value,
                       GCVALUE(ulua_mem_table_node*, block->header.var)->hash);
                break;
            case ULUA_MEM_TYPE_VM:
                printf("VM \n");
                break;
        }
        block = block->header.next;
    }
    printf("\n");
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
    var->type = type;
    var->blocked = ULUA_FALSE;
    ulua_vars_count++;
    ulua_memory_free_size -= ( size + sizeof(ulua_memblock_header) + sizeof(ulua_memvar) );
    ulua_mem_lowvar = lowvar;

    ulua_mem_dump();

    return var;
}

lu08* ulua_mem_new_block(lu16 size) { //allocate fixed memory data block. and mark it as locked.
    ulua_memvar* var = ulua_mem_new(ULUA_MEM_TYPE_DATABLOCK, size);
    if(var == ULUA_NULL)
        return ULUA_NULL;
    
    var->blocked = ULUA_TRUE;
    return ((lu08*)&var->memblock)+sizeof(ulua_memblock)+1;
}

void ulua_mem_free(ulua_memvar *var) { //free variable and allocated memory

    ulua_mem_dump();

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
    var->blocked = ULUA_FALSE;
    ulua_vars_count--;

    ulua_mem_dump();
}

void ulua_mem_free_block(lu08 *block) { //free memory block and associated variable with it
    lu16 varcount = ulua_vars_count;
    ulua_memvar* var = (ulua_memvar*)( &ulua_memory[ulua_memory_size-1] - sizeof(ulua_memvar) );

    //find var for that block
    while(((lu08*)&var->memblock)+sizeof(ulua_memblock)+1 != block && varcount > 0 ) {
        var =(ulua_memvar*) ( (lu08*)var - sizeof(ulua_memvar) );
        if(var->type != ULUA_MEM_TYPE_NULL) varcount--;
    }
    
    if(((lu08*)&var->memblock) +sizeof(ulua_memblock)+1 != block)
        return;
    
    //free found var
    var->blocked = ULUA_FALSE;
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
    lu08* string = GCVALUE(lu08*, stringvar);
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
    return ulua_mem_string_cmp(GCVALUE(lu08*, str1), GCVALUE(lu08*, str2));
}

//list functions
ulua_memvar* ulua_mem_list_new() {
    ulua_memvar* varlist = ulua_mem_new(ULUA_MEM_TYPE_LIST, sizeof(ulua_mem_list));
    ulua_mem_list* list = GCVALUE(ulua_mem_list*, varlist);
    list->size = 0;
    list->first = ULUA_NULL;
    list->last = ULUA_NULL;

    return  varlist;
}
lu16           ulua_mem_list_size(ulua_memvar* list) {
    ulua_mem_list* l = GCVALUE(ulua_mem_list*, list);
    return l->size;
}
ulua_memvar*   ulua_mem_list_add(ulua_memvar* list, ulua_memvar* var) {
    ulua_mem_list* l = GCVALUE(ulua_mem_list*, list);
    ulua_memvar* nodevar = ulua_mem_new(ULUA_MEM_TYPE_LIST_NODE, sizeof(ulua_mem_list_node));
    ulua_mem_list_node* node = GCVALUE(ulua_mem_list_node*, nodevar);
    node->var = var;
    node->next = ULUA_NULL;
    node->prev = ULUA_NULL;

    if(l->last == ULUA_NULL) { //first item in a list
        l->last = nodevar;
        if(l->first == ULUA_NULL) {
            l->first = nodevar;
        }
    } else { //normal adding to the end
        ulua_mem_list_node* lastnode = GCVALUE(ulua_mem_list_node*, l->last);

        node->prev = l->last;
        lastnode->next = nodevar;
        l->last = nodevar;
    }

    l->size++;

    return list;
}
ulua_memvar* ulua_mem_list_iter_init(ulua_memvar* list) {
    ulua_mem_list* l = GCVALUE(ulua_mem_list*, list);
    return l->first;
}
ulua_memvar* ulua_mem_list_iter_next(ulua_memvar* iter) {
    if(iter == ULUA_NULL)
        return ULUA_NULL;
    ulua_memvar* iternext = GCVALUE(ulua_mem_list_node*, iter)->next;
    return iternext;
}
ulua_memvar*    ulua_mem_list_iter_value(ulua_memvar* iter){
    if(iter == ULUA_NULL)
        return ULUA_NULL;
    return GCVALUE(ulua_mem_list_node*, iter)->var;
}
ulua_memvar*    ulua_mem_list_iter_remove(ulua_memvar* list, ulua_memvar* iter) {
    ulua_mem_list* l = GCVALUE(ulua_mem_list*, list);
    ulua_mem_list_node* iternode = GCVALUE(ulua_mem_list_node*, iter);
    ulua_memvar* prevnodevar = iternode->prev;
    ulua_memvar* nextnodevar = iternode->next;
    if(prevnodevar != ULUA_NULL) {
        GCVALUE(ulua_mem_list_node*, prevnodevar)->next = nextnodevar;
    }
    if(nextnodevar != ULUA_NULL) {
        GCVALUE(ulua_mem_list_node*, nextnodevar)->prev = prevnodevar;
    }
    if(l->first == iter) {
        l->first = nextnodevar;
        if(nextnodevar != ULUA_NULL)
            GCVALUE(ulua_mem_list_node*, nextnodevar)->prev = ULUA_NULL;
    }
    if(l->last == iter) {
        l->last = prevnodevar;
        if(prevnodevar != ULUA_NULL)
            GCVALUE(ulua_mem_list_node*, prevnodevar)->next = ULUA_NULL;
    }
    ulua_mem_free(iter);
    return nextnodevar;
}
ulua_memvar*   ulua_mem_list_get_first(ulua_memvar* list) {
    ulua_mem_list* l = GCVALUE(ulua_mem_list*, list);
    return GCVALUE(ulua_mem_list_node*, l->first)->var;
}
ulua_memvar*   ulua_mem_list_get_last(ulua_memvar* list) {
    ulua_mem_list* l = GCVALUE(ulua_mem_list*, list);
    return GCVALUE(ulua_mem_list_node*, l->last)->var;
}
//ulua_memvar*   ulua_mem_list_get(ulua_memvar* list, lu16 number);
ulua_memvar*   ulua_mem_list_remove_first(ulua_memvar* list) {
    ulua_mem_list* l = GCVALUE(ulua_mem_list*, list);
    ulua_memvar* firstnodevar = l->first;
    ulua_mem_list_node* firstnode = GCVALUE(ulua_mem_list_node*, firstnodevar);
    l->first = firstnode->next;
    GCVALUE(ulua_mem_list_node*, firstnode->next)->prev = ULUA_NULL;

    ulua_mem_free(firstnodevar);
}
ulua_memvar*   ulua_mem_list_remove_last(ulua_memvar* list) {
    ulua_mem_list* l = GCVALUE(ulua_mem_list*, list);
    ulua_memvar* lastnodevar = l->last;
    ulua_mem_list_node* lastnode = GCVALUE(ulua_mem_list_node*, lastnodevar);
    l->last = lastnode->prev;
    GCVALUE(ulua_mem_list_node*, lastnode->prev)->next = ULUA_NULL;

    ulua_mem_free(lastnodevar);
}
//ulua_memvar*   ulua_mem_list_remove(ulua_memvar* list, lu16 number);
void           ulua_mem_list_free(ulua_memvar* list) {
    ulua_mem_list* l = GCVALUE(ulua_mem_list*, list);
    ulua_memvar* node = l->first;
    while(node != ULUA_NULL) {
        ulua_memvar* nextnode = GCVALUE(ulua_mem_list_node*, node)->next;
        ulua_mem_free(node);
        node = nextnode;
    }
    ulua_mem_free(list);
}


//hashtable functions
ulua_memvar*   ulua_mem_table_new() {
    ulua_memvar* tablevar = ulua_mem_new(ULUA_MEM_TYPE_TABLE, sizeof(ulua_mem_table));
    ulua_mem_table* table = GCVALUE(ulua_mem_table*, tablevar);
    table->size = 0;
    for(lu16 i=0; i<ULUA_MEM_TABLE_SIZE; i++) {
        table->list_array[i] = ulua_mem_list_new();
    }
    return tablevar;
}
lu16           ulua_mem_table_hash(ulua_memvar* key) {
    lu08* string;
    lu08 c;
    lu16 p;
    lu16 hashcode = 0;
    vmregister* reg;
    if(key == ULUA_NULL)
        return 0;
    switch (key->type) {
        case ULUA_MEM_TYPE_NULL:
            return 0;
        case ULUA_MEM_TYPE_STRING:
            string = GCVALUE(lu08*, key);
            for(lu16 i=0; i<key->memblock->header.size; i++) {
                hashcode += string[i];
            }
            return hashcode;
        case ULUA_MEM_TYPE_VMREGISTER:
            reg = GCVALUE(vmregister*, key);
            return (int)reg->pointer;
        default:
            return (int)key;
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
                reg1 = GCVALUE(vmregister*, key1);
                reg2 = GCVALUE(vmregister*, key2);
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
    ulua_mem_table* t = GCVALUE(ulua_mem_table*, table);
    lu16 hashcode = ulua_mem_table_hash(key);
    lu16 index = hashcode % ULUA_MEM_TABLE_SIZE;
    ulua_memvar* listvar = t->list_array[index];

    ulua_memvar* tnodevar = ULUA_NULL;
    ulua_memvar* listiter = ulua_mem_list_iter_init(listvar);
    while(listiter != ULUA_NULL) {//lookup key
        ulua_mem_table_node* tnode = GCVALUE(ulua_mem_table_node*, ulua_mem_list_iter_value(listiter));
        if(tnode->hash == hashcode && ulua_mem_table_key_equals(tnode->key, key)) {// found key - update value
            tnode->value = value;
            tnodevar = ulua_mem_list_iter_value(listiter);
            break;
        }
        listiter = ulua_mem_list_iter_next(listiter);
    }
    if(tnodevar == ULUA_NULL) {//allocate new node
        tnodevar = ulua_mem_new(ULUA_MEM_TYPE_TABLE_NODE, sizeof(ulua_mem_table_node));
        ulua_mem_table_node *tnode = GCVALUE(ulua_mem_table_node*, tnodevar);
        tnode->key = key;
        tnode->value = value;
        tnode->hash = hashcode;
        ulua_mem_list_add(listvar, tnodevar);
    }
    t->size++;
    return table;
}
ulua_memvar*   ulua_mem_table_get(ulua_memvar* table, ulua_memvar* key) {
    ulua_mem_table* t = GCVALUE(ulua_mem_table*, table);
    lu16 hashcode = ulua_mem_table_hash(key);
    lu16 index = hashcode % ULUA_MEM_TABLE_SIZE;
    ulua_memvar* listvar = t->list_array[index];

    ulua_memvar* listiter = ulua_mem_list_iter_init(listvar);
    while(listiter != ULUA_NULL) {//lookup key
        ulua_mem_table_node* tnode = GCVALUE(ulua_mem_table_node*, ulua_mem_list_iter_value(listiter));
        if(tnode->hash == hashcode && ulua_mem_table_key_equals(tnode->key, key)) {// found key - update value
            return tnode->value;
        }
        listiter = ulua_mem_list_iter_next(listiter);
    }
    return ULUA_NULL;
}
ulua_memvar*   ulua_mem_table_remove(ulua_memvar* table, ulua_memvar* key){
    ulua_mem_table* t = GCVALUE(ulua_mem_table*, table);
    lu16 hashcode = ulua_mem_table_hash(key);
    lu16 index = hashcode % ULUA_MEM_TABLE_SIZE;
    ulua_memvar* listvar = t->list_array[index];
    ulua_memvar* value = ULUA_NULL;

    ulua_memvar* listiter = ulua_mem_list_iter_init(listvar);
    while(listiter != ULUA_NULL) {//lookup key
        ulua_memvar* tnodevar = ulua_mem_list_iter_value(listiter);
        ulua_mem_table_node* tnode = GCVALUE(ulua_mem_table_node*, tnodevar);
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
    ulua_mem_table* t = GCVALUE(ulua_mem_table*, table);
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