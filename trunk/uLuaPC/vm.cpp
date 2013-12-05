#include "vm.h"
#include <math.h>

//reset structs functions
static void initStructState(vmstate* state)
{
	state->constp = 0;
	state->funcp = 0;

	for(int i=0; i<REGISTERSIZE; i++) {
		state->reg[i].type = VAR_NULL;
		state->reg[i].numval = 0;
	}

}

void initStructVM(vm* vm)
{
	vm->status = STOP;
	vm->pc = 0;
	vm->pcstackpt = 0;
	vm->statept = 0;

	for(int i=0; i<PCSTACKSIZE; i++)
		vm->pcstack[i] = 0;

	for(int i=0; i<GLOBALSIZE; i++) {
		vm->global[i].val.type = VAR_NULL;
		vm->global[i].val.numval = 0;

		for(int j=0; j<GLOBALNAMESIZE; j++)
			vm->global[i].name[j] = 0;
	}

	for(int i=0; i<STATEMAXCOUNT; i++)
		initStructState(&vm->state[i]);
}

//Initialize virtual machine
void vmInit(vm* vm)
{
	//reset structure
	initStructVM(vm);
	//Init garbage collector and mamory management
	gcInit();
	//pre load native functions in global namespace
	nativeInit(vm);
}

//Get pointer to sub functions
u16 getFuncsPt(u16 pt)
{
	u32 csize = platformReadDWord(pt); pt += 4;

	for(u08 i=0; i<csize; i++)
	{
		u08 type = platformReadByte(pt++);
		if(type == NUMBER_TYPE)
		{
			pt += 8;
		}
		else if(type == STRING_TYPE)
		{
			u32 strsize = platformReadDWord(pt); pt += 4;
			pt += strsize;
		}
	}
	return pt;
}

//skeep function which starts on pt
//return pointer to the next byte after function
u16 skeepFunction(u16 pt)
{
	pt += 16; //function header;
	u32 codesize = platformReadDWord(pt); pt += 4;
	pt += codesize * 4;
	pt = getFuncsPt(pt);//subfunctions
	u32 funcsize = platformReadDWord(pt); pt += 4;
	for(u08 i=0; i<funcsize; i++)
	{
		pt = skeepFunction(pt);
	}
	pt += 3 * 4; //TODO: for now ignore lines, locals and upvalues. we think that thay are all zero size.
	return pt;
}

//Get sub function number N
// pt - pointer to sub functions
u16 getFuncPt(u16 pt, u16 N)
{
	u32 funcsize = platformReadDWord(pt); pt += 4;
	for(u08 i=0; N>i; i++)
	{
		pt = skeepFunction(pt);
	}
	pt += 16; //function header;
	return pt;
}

//Get potinter to constant number
u16 getConstPt(u16 constspt, u16 N)
{
	u32 csize = platformReadDWord(constspt); constspt += 4;

	for(u08 i=0; i<csize &&  i<N; i++)
	{
		u08 type = platformReadByte(constspt++);
		if(type == NUMBER_TYPE)
		{
			constspt += 8;
		}
		else if(type == STRING_TYPE)
		{
			u32 strsize = platformReadDWord(constspt); constspt += 4;
			constspt += strsize;
		}
	}
	return constspt;
}

//Find global variable by name .Return global index
u08 getGlobalByName(vm* vm, u08* name)
{
	u08 foundGlobal = 0;
	for(foundGlobal=0; foundGlobal<GLOBALSIZE; foundGlobal++)
	{
		if(vm->global[foundGlobal].val.type == VAR_NULL) //global not found - return free one
			break;

		u08 match = 1;
		for(int i=0; i<GLOBALNAMESIZE && name[i] != 0 && vm->global[foundGlobal].name[i] != 0; i++)
		{
			if(vm->global[foundGlobal].name[i] != name[i])
			{
				match = 0;
				break;
			}
		}
				
		if(match == 1)
			break;
	}

	return foundGlobal;
}

u08 vmRun(vm* vm)
{
	//vmstate* state = &vm->state[0];
	//set running state
	vm->status = RUN;

	//skip header
	vm->pc = 0x1c;

	//get code size for top function
	u32 codesize = platformReadDWord(vm->pc); 
	vm->pc += 4;
	vm->state[vm->statept].constp = vm->pc + codesize * 4;
	vm->state[vm->statept].funcp = getFuncsPt(vm->state[vm->statept].constp);


	u08 a = 0;
	u08 b = 0;
	u08 c = 0;
	s16 bx = 0;
	u16 constpt = 0;
	u08 type;
	u08 glindex;
	u08* name;

	//for(u08 i=0; i<codesize; i++)
	while(vm->status == RUN)
	{
		vmstate* curstate = &vm->state[vm->statept];
		//get first instruction
		u32 inst = platformReadDWord(vm->pc);
		u08 opcode = GET_OPCODE(inst);

		//go to next instruction
		vm->pc += 4;

		//get operands
		a = GETARG_A(inst);
		b = GETARG_B(inst);
		c = GETARG_C(inst);
		bx = GETARG_Bx(inst);

		switch(opcode)
		{ 

		case OP_MOVE: //copy R(A) = R(B)
			curstate->reg[a].type = curstate->reg[b].type;
			curstate->reg[a].numval = curstate->reg[b].numval;
			break;

		case OP_CLOSURE: //Create closure and put it into R(A)
			curstate->reg[a].type = VAR_FILE_POINTER_FUNC;
			curstate->reg[a].numval = getFuncPt(curstate->funcp, bx);
			break;

		case OP_SETGLOBAL: //	A Bx	Gbl[Kst(Bx)] := R(A)
			//read global name
			constpt = getConstPt(curstate->constp, bx);
			type = platformReadByte(constpt++);
			codesize = platformReadDWord(constpt);
			constpt += 4;
			name = platformReadBuffer(constpt, codesize);

			//search global
			glindex = getGlobalByName(vm, name);
			//copy name
			for(int i=0; i<GLOBALNAMESIZE && name[i] != 0; i++)
			{
				vm->global[glindex].name[i] = name[i];
			}
			//set global value
			vm->global[glindex].val.type = curstate->reg[a].type;
			vm->global[glindex].val.numval = curstate->reg[a].numval;
			break;

		case OP_GETGLOBAL: // A Bx	R(A) := Gbl[Kst(Bx)]
			//read global name
			constpt = getConstPt(curstate->constp, bx);
			type = platformReadByte(constpt++);
			codesize = platformReadDWord(constpt);
			constpt += 4;
			name = platformReadBuffer(constpt, codesize);

			//search global
			glindex = getGlobalByName(vm, name);

			curstate->reg[a].type = vm->global[glindex].val.type;
			curstate->reg[a].numval = vm->global[glindex].val.numval;
			break;

		case OP_LOADK: //A Bx	R(A) := Kst(Bx)		
			constpt = getConstPt(curstate->constp, bx);
			type = platformReadByte(constpt++);

			switch(type)
			{
			case BOOL_TYPE:
				curstate->reg[a].type = VAR_BOOLEAN;
				break;

			case NUMBER_TYPE:
				curstate->reg[a].type = VAR_FLOAT;
				curstate->reg[a].floatval = platformReadNumber(constpt);
				break;

			case STRING_TYPE:
				curstate->reg[a].type = VAR_FILE_POINTER_STR;
				curstate->reg[a].numval = constpt;
				break;

			default:
			case NULL_TYPE:
				curstate->reg[a].type = VAR_NULL;
				break;
			}
			break;

		case OP_CALL: //A B C	R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))
			if(curstate->reg[a].type == VAR_NATIVE_FUNC)
			{//native function call
				nativeCall(vm, a, b, c);
			}
			else
			{//lua function call

				//save next pc instruction address to the stack
				vm->pcstack[vm->pcstackpt++] = vm->pc;
				//set pc = call function address
				//TODO: check reg type
				vm->pc = curstate->reg[a].numval;

				//prepare state
				vm->statept++;
				//get code size function
				codesize = platformReadDWord(vm->pc); 
				vm->pc += 4;
				vm->state[vm->statept].constp = vm->pc + codesize * 4;
				vm->state[vm->statept].funcp = getFuncsPt(vm->state[vm->statept].constp);
				//copy args to the new state
				//TODO: support copy all values on the top of the stack (b=0)
				for(int i=0; i<b-1 && b!=0; i++)
				{
					vm->state[vm->statept].reg[i].type   = vm->state[vm->statept-1].reg[a+1+i].type;
					vm->state[vm->statept].reg[i].numval = vm->state[vm->statept-1].reg[a+1+i].numval;
				}

			}
			break;

		case OP_RETURN: //A B	return R(A), ... ,R(A+B-2)	(see note)
			//check if return from main function
			if(vm->pcstackpt == 0)
			{
				vm->status = STOP;
			}
			else
			{
				//restore pc stack address
				vm->pc = vm->pcstack[--vm->pcstackpt];
				vm->statept--;
				//save result to the prev state
				//TODO: support return top of the stack (b=0)
				for(int i=0; i<b-1 && b!=0; i++)
				{
					vm->state[vm->statept].reg[i].type   = vm->state[vm->statept+1].reg[a+i].type;
					vm->state[vm->statept].reg[i].numval = vm->state[vm->statept+1].reg[a+i].numval;
				}
			}
			break;

		//mathematic function
		//TODO: string support
		//TODO: check types and nulls
		case OP_ADD://	A B C	R(A) := RK(B) + RK(C)
			curstate->reg[a].type = VAR_FLOAT;
			curstate->reg[a].floatval = curstate->reg[b].floatval + curstate->reg[c].floatval;
			break;
		case OP_SUB://	A B C	R(A) := RK(B) - RK(C)
			curstate->reg[a].type = VAR_FLOAT;
			curstate->reg[a].floatval = curstate->reg[b].floatval - curstate->reg[c].floatval;
			break;
		case OP_MUL://	A B C	R(A) := RK(B) * RK(C)
			curstate->reg[a].type = VAR_FLOAT;
			curstate->reg[a].floatval = curstate->reg[b].floatval * curstate->reg[c].floatval;
			break;
		case OP_DIV://	A B C	R(A) := RK(B) / RK(C)
			curstate->reg[a].type = VAR_FLOAT;
			curstate->reg[a].floatval = curstate->reg[b].floatval / curstate->reg[c].floatval;
			break;
		case OP_MOD://	A B C	R(A) := RK(B) % RK(C)
			curstate->reg[a].type = VAR_FLOAT;
			curstate->reg[a].floatval = fmod(curstate->reg[b].floatval, curstate->reg[c].floatval);
			break;
		case OP_POW://	A B C	R(A) := RK(B) ^ RK(C)
			curstate->reg[a].type = VAR_FLOAT;
			curstate->reg[a].floatval = pow(curstate->reg[b].floatval , curstate->reg[c].floatval);
			break;
		
		//execution logic functions
		//FOR LOOPS
		case OP_FORPREP:
			//TODO: add checking numeric types in for loops variables
			curstate->reg[a+3].type = VAR_FLOAT;
			curstate->reg[a].floatval -= curstate->reg[a+2].floatval;
			vm->pc += (bx+1)*4; //TODO: why +1???
			break;
		case OP_FORLOOP:
			curstate->reg[a].floatval += curstate->reg[a+2].floatval; //add step
			if(curstate->reg[a].floatval <= curstate->reg[a+1].floatval) //check limit
			{
				curstate->reg[a+3].floatval = curstate->reg[a].floatval;
				vm->pc += (bx+1)*4; //TODO: why +1???
			}
			break;

		default:
			return 1;//TODO: unknown instruction
		}

	}

	return 0;
}
