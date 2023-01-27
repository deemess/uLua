#include <stdio.h>
#include "ulua_core/native.h"
#include "ulua_core/umemory.h"



//////////////////////////////////////////////////////////////////////////////
//Native functions
//////////////////////////////////////////////////////////////////////////////
//with register R(A) holding the reference to the
//function object to be called. Parameters to the function are placed in the
//registers following R(A). If B is 1, the function has no parameters. If B is 2
//or more, there are (B-1) parameters.

//If B is 0, the function parameters range from R(A+1) to the top of the stack.
//This form is used when the last expression in the parameter list is a
//function call, so the number of actual parameters is indeterminate.

//Results returned by the function call is placed in a range of registers
//starting from R(A). If C is 1, no return results are saved. If C is 2 or more,
//(C-1) return values are saved. If C is 0, th


void printRegister(readBytes read, vmregister reg)
{
	lu16 constpt;
	lu16 size;
	lu08 name[32];
	switch(reg.type)
	{
		case REGISTER_VAR_NATIVE_FUNC:
			printf("Native function\t");
			break;
			
		case REGISTER_VAR_BOOLEAN:
			if(reg.floatval == 1) printf("true\t"); else printf("false\t");
			break;

		case REGISTER_VAR_NUMBER:
			printf("%d\t", reg.numval);
			break;

		case REGISTER_VAR_FLOAT:
			//printf("%.6f\t", reg.floatval);
			printf("%f\t", reg.floatval);
			break;

		case REGISTER_VAR_NULL:
			printf("nil\t");
			break;

		case REGISTER_VAR_CLOSURE:
			printf("luc function at %d\t", reg.numval);
			break;

        case REGISTER_VAR_MEMVAR:
            switch (((ulua_memvar*)reg.pointer)->type) {
                case ULUA_MEM_TYPE_STRING:
                    printf("%.*s\t", ((ulua_memvar*)reg.pointer)->memblock->header.size, MEMVARVALUE(lu08*, ((ulua_memvar*)reg.pointer)));
                    break;

            }
            break;

	}
}

//native print(..) function
void nativePrint(ulua_vm* vm, readBytes read, lu08 a, lu16 b, lu16 c)
{
	lu08 i=0;
	//print all variables
	for(i=0; i<b-1; i++)
	{
		vmregister reg = vm->state[vm->statept].reg[a+1+i];
		printRegister(read, reg);
	}

	if(b==0)
		printRegister(read, vm->state[vm->statept].reg[a+1]);

	printf("\n");
}

//native memdump(..) function
void nativeMemDump(ulua_vm* vm, readBytes read, lu08 a, lu16 b, lu16 c) {
    ulua_mem_dump();
}

//native memtree(..) function
void nativeMemDumpTree(ulua_vm* vm, readBytes read, lu08 a, lu16 b, lu16 c) {
	ulua_mem_dump_tree();
}

//native gc(..) function
void nativeGC(ulua_vm* vm, readBytes read, lu08 a, lu16 b, lu16 c) {
	ulua_mem_gc_collect();
}

//////////////////////////////////////////////////////////////////////////////

void putNative(ulua_vm* v, lu08* name, nativeFunc func)
{
    ulua_memvar* stringvar = ulua_mem_string_new(name);
    ulua_memvar* regvar = ulua_mem_new(ULUA_MEM_TYPE_VMREGISTER, sizeof(vmregister));
    vmregister* reg = MEMVARVALUE(vmregister*, regvar);

	//set global
    reg->type = REGISTER_VAR_NATIVE_FUNC;
	reg->pointer = (void*)(func);

	ulua_mem_table_put(v->globals_table, stringvar, regvar);
}

//preload native functions in global namespace
void nativeInit(ulua_vm* vm)
{
	//add native functions
	putNative(vm, (lu08*)"print", &nativePrint);
	putNative(vm, (lu08*)"memdump", &nativeMemDump);
	putNative(vm, (lu08*)"memtree", &nativeMemDumpTree);
	putNative(vm, (lu08*)"gc", &nativeGC);
}

//call native function stored in reg
void nativeCall(ulua_vm* vm, readBytes read, lu08 a, lu16 b, lu16 c)
{
	nativeFunc func = (nativeFunc)vm->state[vm->statept].reg[a].pointer;
	func(vm, read, a, b, c);
}
