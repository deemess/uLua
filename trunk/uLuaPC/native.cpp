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


void printRegister(vmregister reg)
{
	u16 constpt;
	u32 size;
	u08* name;
	switch(reg.type)
	{
		case VAR_BOOLEAN:
			if(reg.numval == TRUE) platformPrintf("true\t"); else platformPrintf("false\t");
			break;

		case VAR_NUMBER:
			platformPrintf("%d\t", reg.numval);
			break;

		case VAR_FLOAT:
			platformPrintf("%f\t", reg.floatval);
			break;

		case VAR_STRING:
			platformPrintf("%s\t", (char*)reg.numval);
			break;

		case VAR_NULL:
			platformPrintf("nil\t");
			break;

		case VAR_CLOSURE:
			platformPrintf("luc function at %d\t", reg.numval);
			break;

		case VAR_FILE_POINTER_STR:
			constpt = reg.numval;
			size = platformReadDWord(constpt); constpt += 4;
			name = platformReadBuffer(constpt, size);
			platformPrintf("%s\t", name);
			break;

		case VAR_TABLE:
			platformPrintf("Table{}\t");
			break;
	}
}

//native print(..) function
void nativePrint(vm* vm, u08 a, u16 b, u16 c)
{

	//print all variables
	for(u08 i=0; i<b-1; i++)
	{
		vmregister reg = vm->state[vm->statept].reg[a+1+i];
		printRegister(reg);
	}

	if(b==0)
		printRegister(vm->state[vm->statept].reg[a+1]);

	platformPrintf("\n");
}

//////////////////////////////////////////////////////////////////////////////



//Find  free global variable .Return global index
u08 getFreeGlobal(vm* vm)
{
	u08 foundGlobal = 0;
	for(foundGlobal=0; foundGlobal<GLOBALSIZE; foundGlobal++)
	{
		if(vm->global[foundGlobal].val.type == VAR_NULL) //find free global
			break;
	}

	return foundGlobal;
}

void putNative(vm* v, u08* name, nativeFunc func)
{
	u08 freeIndex = getFreeGlobal(v);
	//set global
	v->global[freeIndex].val.type = VAR_NATIVE_FUNC;
	v->global[freeIndex].val.numval = (u32)(func);

	//copy name
	for(int i=0; i<GLOBALNAMESIZE && name[i] != 0; i++)
	{
		v->global[freeIndex].name[i] = name[i];
	}
}


//preload native functions in global namespace
void nativeInit(vm* vm)
{
	//add native "print" function
	putNative(vm, (u08*)"print", &nativePrint);
}

//call native function stored in reg
void nativeCall(vm* vm, u08 a, u16 b, u16 c)
{
	nativeFunc func = (nativeFunc)vm->state[vm->statept].reg[a].numval;
	func(vm, a, b, c);
}
