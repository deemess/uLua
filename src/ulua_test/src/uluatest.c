#include <stdio.h>
#include <ulua_core/umemory.h>
#include <ulua_core/vm.h>
#include "ulua_core/basetypes.h"

#define RAM_SIZE 65535
lu08 ram[RAM_SIZE];

int main(int argc, char **argv) {
    //ulua_memvar* thread = vmInit((lu08*)&ram, 65535);
    for(int i=0; i<RAM_SIZE; i++) {
        ram[i] = 0;
    }

    ulua_mem_init((lu08*)&ram, RAM_SIZE);
    ulua_memvar* string1 = ulua_mem_string_new("test string 1");
    ulua_memvar* string2 = ulua_mem_string_new("test string 2");
    ulua_memvar* string3 = ulua_mem_string_new("test string 3");

    ulua_mem_dump();

}