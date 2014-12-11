#include "native.h"

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
		case VAR_NATIVE_FUNC:
			printf("Native function\t");
			break;
			
		case VAR_BOOLEAN:
			if(reg.floatval == 1) printf("true\t"); else printf("false\t");
			break;

		case VAR_NUMBER:
			printf("%d\t", reg.numval);
			break;

		case VAR_FLOAT:
			printf("%.0f\t", reg.floatval);
			break;

		case VAR_STRING:
			printf("%s\t", (char*)reg.pointer);
			break;

		case VAR_NULL:
			printf("nil\t");
			break;

		case VAR_CLOSURE:
			printf("luc function at %d\t", reg.numval);
			break;

		case VAR_FILE_POINTER_STR:
			constpt = reg.numval;
			read(name, constpt, 2); constpt += 2;
			size = *(lu16*)(&name[0]);
			read(name, constpt, size);
			printf("%s\t", name);
			break;

		case VAR_TABLE:
			printf("Table{}\t");
			break;
	}
}

//native print(..) function
void nativePrint(vm* vm, readBytes read, lu08 a, lu16 b, lu16 c)
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

//////////////////////////////////////////////////////////////////////////////



//Find  free global variable .Return global index
lu08 getFreeGlobal(vm* vm)
{
	lu08 foundGlobal = 0;
	for(foundGlobal=0; foundGlobal<GLOBALSIZE; foundGlobal++)
	{
		if(vm->global[foundGlobal].val.type == VAR_NULL) //find free global
			break;
	}

	return foundGlobal;
}

void putNative(vm* v, lu08* name, nativeFunc func)
{
	int i=0;
	lu08 freeIndex = getFreeGlobal(v);
	//set global
	v->global[freeIndex].val.type = VAR_NATIVE_FUNC;
	v->global[freeIndex].val.pointer = (void*)(func);

	//copy name
	for(i=0; i<GLOBALNAMESIZE && name[i] != 0; i++)
	{
		v->global[freeIndex].name[i] = name[i];
	}
}


//preload native functions in global namespace
void nativeInit(vm* vm)
{
	//add native "print" function
	putNative(vm, (lu08*)"print", &nativePrint);
}

//call native function stored in reg
void nativeCall(vm* vm, readBytes read, lu08 a, lu16 b, lu16 c)
{
	nativeFunc func = (nativeFunc)vm->state[vm->statept].reg[a].pointer;
	func(vm, read, a, b, c);
}
