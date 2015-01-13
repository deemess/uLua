#include <stdio.h>
#include <string.h>
#include "basetypes.h"
#include "umemory.h"

#define MEMSIZE (sizeof(ulua_memblock_header)+sizeof(ulua_memvar))*3 +30

int main() {
    lu08 memory[ MEMSIZE ];
    
    printf("Memory test:\n");
    printf("Overhead for every memory allocation %lu bytes header + %lu bytes var\n", sizeof(ulua_memblock_header), sizeof(ulua_memvar));
    ulua_memoryinit((lu08*)&memory, MEMSIZE);
    
    ulua_memvar* var1 = ulua_allocate(ULUA_TYPE_STRING, 10);
    if(var1 != ULUA_NULL) memcpy(&var1->memblock->data, &"1", sizeof("1"));
    ulua_memvar* var2 = ulua_allocate(ULUA_TYPE_STRING, 10);
    if(var2 != ULUA_NULL) memcpy(&var2->memblock->data, &"2", sizeof("2"));
    ulua_memvar* var3 = ulua_allocate(ULUA_TYPE_STRING, 10);
    if(var3 != ULUA_NULL) memcpy(&var3->memblock->data, &"3", sizeof("3"));
    ulua_memvar* var4 = ulua_allocate(ULUA_TYPE_STRING, 5);
    if(var4 != ULUA_NULL) memcpy(&var4->memblock->data, &"4", sizeof("4"));

    ulua_free(var2);
    var4 = ulua_allocate(ULUA_TYPE_STRING, 5);
    if(var4 != ULUA_NULL) memcpy(&var4->memblock->data, &"4", sizeof("4"));
    
    
    return 0;
}
