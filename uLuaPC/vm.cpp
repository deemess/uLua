#include "vm.h"
#include <math.h>

//reset structs functions
static void initStructState(vmstate* state)
{
	state->constp = 0;
	state->funcp = 0;
	state->closure = NULL;
	state->retreg = 0;

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
	//pt += 16; //function header;
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

//compare values in register a and b. return:  0: a!=b   1: a=b  -1: a>b  -2: a<b 
s08 compare(vmregister* a, vmregister* b)
{
	if(a->type != b->type)
		return 0;

	if(a->numval == b->numval)
		return 1;

	if(a->type == VAR_FLOAT)
	{ 
		if(a->floatval > b->floatval)
			return -1;
		if(a->floatval < b->floatval)
			return -2;
	}

	if(a->type == VAR_NUMBER)
	{ 
		if(a->numval > b->numval)
			return -1;
		if(a->numval < b->numval)
			return -2;
	}

	return 0;
}

void clearRegister(vmregister* reg)
{
	//check register for GC ref
	if(reg->type == VAR_CLOSURE) {
		GCREFDEC((gcvarpt*)reg->numval);
		GCCHECK((gcvarpt*)reg->numval);
	}
	//reg->type = VAR_NULL;
	//reg->numval = 0;
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
	u16 b = 0;
	u16 c = 0;
	u16 bx = 0;
	s16 sbx = 0;
	u16 constpt = 0;
	u08 type;
	u08 glindex;
	u08* name;
	gcvarpt* gcpointer = NULL;
	vmregister tmp;
	tmp.type = VAR_NULL;
	tmp.numval = 0;

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
		sbx = GETARG_sBx(inst);

		switch(opcode)
		{ 

		case OP_MOVE: //copy R(A) = R(B)
			clearRegister(&curstate->reg[a]);
			curstate->reg[a].type = curstate->reg[b].type;
			curstate->reg[a].numval = curstate->reg[b].numval;
			break;

		case OP_CLOSURE: //Create closure and put it into R(A)
			//create closure in GC
			gcpointer = gcNew(VAR_CLOSURE);
			GCVALUE(vmclosure, gcpointer).funcp = getFuncPt(curstate->funcp, bx);
			GCVALUE(vmclosure, gcpointer).upvalcount = platformReadByte(GCVALUE(vmclosure, gcpointer).funcp + 3*4);

			//init upvalues
			for(int i=0; i < GCVALUE(vmclosure, gcpointer).upvalcount; i++)
			{
				u32 nextint = platformReadDWord(vm->pc);
				u08 nextopcode = GET_OPCODE(nextint);
				u08 nextb = GETARG_B(nextint);

				switch(nextopcode)
				{
				//init upvalue from vm register
				case OP_MOVE:
					GCVALUE(vmclosure, gcpointer).upval[i].type = curstate->reg[b].type;
					GCVALUE(vmclosure, gcpointer).upval[i].numval = curstate->reg[b].numval;
					break;

				//init upvalue from another upvalue
				case OP_GETUPVAL:
					if(curstate->closure != NULL)
					{
						GCVALUE(vmclosure, gcpointer).upval[i].type = GCVALUE(vmclosure, curstate->closure).upval[b].type;
						GCVALUE(vmclosure, gcpointer).upval[i].numval = GCVALUE(vmclosure, curstate->closure).upval[b].numval;
					}
					break;

				default:
					break;
				}

				vm->pc += 4;
			}

			//save pointer to the closure into register
			GCREFINC(gcpointer);
			clearRegister(&curstate->reg[a]);
			curstate->reg[a].type = VAR_CLOSURE;
			curstate->reg[a].numval = (u32)gcpointer;
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
			if(curstate->reg[a].type == VAR_CLOSURE)
			{
				GCREFINC((gcvarpt*)curstate->reg[a].numval);
			}
			clearRegister(&vm->global[glindex].val);
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


			clearRegister(&curstate->reg[a]);
			curstate->reg[a].type = vm->global[glindex].val.type;
			curstate->reg[a].numval = vm->global[glindex].val.numval;
			break;

		case OP_GETUPVAL: //R(A) := UpValue[B]
			clearRegister(&curstate->reg[a]);
			curstate->reg[a].type = GCVALUE(vmclosure, curstate->closure).upval[b].type;
			curstate->reg[a].numval = GCVALUE(vmclosure, curstate->closure).upval[b].numval;
			break;

		case OP_SETUPVAL: //UpValue[B] := R(A)
			clearRegister(&GCVALUE(vmclosure, curstate->closure).upval[b]);
			GCVALUE(vmclosure, curstate->closure).upval[b].type = curstate->reg[a].type;
			GCVALUE(vmclosure, curstate->closure).upval[b].numval = curstate->reg[a].numval;
			break;
		
		case OP_LOADK: //A Bx	R(A) := Kst(Bx)		
			constpt = getConstPt(curstate->constp, bx);
			type = platformReadByte(constpt++);

			clearRegister(&curstate->reg[a]);

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

		
		case OP_LOADNIL: //R(A) := ... := R(B) := nil
			for(u08 i=a; i<=b; i++)
			{
				clearRegister(&curstate->reg[i]);
				curstate->reg[i].type = VAR_NULL;
				curstate->reg[i].numval = 0;
			}
			break;

		case OP_LOADBOOL:
			clearRegister(&curstate->reg[a]);
			curstate->reg[a].type = VAR_BOOLEAN;
			curstate->reg[a].numval = b;
			if(c > 0) //skip instruction
				vm->pc += 4;
			break;

		case OP_CALL: //A B C	R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))
			if(curstate->reg[a].type == VAR_NATIVE_FUNC)
			{//native function call
				nativeCall(vm, a, b, c);
			}
			else if(curstate->reg[a].type == VAR_CLOSURE)
			{//lua function call

				//save next pc instruction address to the stack
				vm->pcstack[vm->pcstackpt++] = vm->pc;
				//set pc = call function address
				//TODO: check reg type
				gcpointer = (gcvarpt*)curstate->reg[a].numval;
				vm->pc = GCVALUE(vmclosure,gcpointer).funcp;
				vm->pc += 16; //skip functino header

				//prepare state
				vm->statept++;
				//get code size function
				codesize = platformReadDWord(vm->pc); 
				vm->pc += 4;
				vm->state[vm->statept].constp = vm->pc + codesize * 4;
				vm->state[vm->statept].funcp = getFuncsPt(vm->state[vm->statept].constp);
				vm->state[vm->statept].retreg = a;
				vm->state[vm->statept].closure = gcpointer;
				//copy args to the new state
				//TODO: support copy all values on the top of the stack (b=0)
				for(int i=0; i<b-1 && b!=0; i++)
				{
					vm->state[vm->statept].reg[i].type   = vm->state[vm->statept-1].reg[a+1+i].type;
					vm->state[vm->statept].reg[i].numval = vm->state[vm->statept-1].reg[a+1+i].numval;
				}

			} else {
#ifdef DEBUGVM
				platformPrintf("VM: Cant make CALL. Got non function pointer at %#x. Type: %d Numval: %d Floatval: %f\n", vm->pc-4, curstate->reg[a].type, curstate->reg[a].numval, curstate->reg[a].floatval);
#endif
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
					vm->state[vm->statept].reg[vm->state[vm->statept+1].retreg+i].type   = vm->state[vm->statept+1].reg[a+i].type;
					vm->state[vm->statept].reg[vm->state[vm->statept+1].retreg+i].numval = vm->state[vm->statept+1].reg[a+i].numval;
				}
			}
			break;

		//mathematic function
		//TODO: string support ?? seems not
		//TODO: check types and nulls
		case OP_ADD://	A B C	R(A) := RK(B) + RK(C)
			clearRegister(&curstate->reg[a]);
			clearRegister(&tmp);
			curstate->reg[a].type = VAR_FLOAT;
			tmp.type = VAR_FLOAT;

			if(b > 255)
			{
				constpt = getConstPt(curstate->constp, b - 256);
				type = platformReadByte(constpt++);
				tmp.floatval = platformReadNumber(constpt);
			}
			else
			{
				tmp.floatval = curstate->reg[b].floatval;
			}

			if(c > 255)
			{
				constpt = getConstPt(curstate->constp, c - 256);
				type = platformReadByte(constpt++);
				curstate->reg[a].floatval = tmp.floatval + platformReadNumber(constpt);
			}
			else
			{
				curstate->reg[a].floatval = tmp.floatval + curstate->reg[c].floatval;
			}

			break;
		case OP_SUB://	A B C	R(A) := RK(B) - RK(C)
			clearRegister(&curstate->reg[a]);
			clearRegister(&tmp);
			curstate->reg[a].type = VAR_FLOAT;
			tmp.type = VAR_FLOAT;

			if(b > 255)
			{
				constpt = getConstPt(curstate->constp, b - 256);
				type = platformReadByte(constpt++);
				tmp.floatval = platformReadNumber(constpt);
			}
			else
			{
				tmp.floatval = curstate->reg[b].floatval;
			}

			if(c > 255)
			{
				constpt = getConstPt(curstate->constp, c - 256);
				type = platformReadByte(constpt++);
				curstate->reg[a].floatval = tmp.floatval - platformReadNumber(constpt);
			}
			else
			{
				curstate->reg[a].floatval = tmp.floatval - curstate->reg[c].floatval;
			}

			break;
		case OP_MUL://	A B C	R(A) := RK(B) * RK(C)
			clearRegister(&curstate->reg[a]);
			clearRegister(&tmp);
			curstate->reg[a].type = VAR_FLOAT;
			tmp.type = VAR_FLOAT;
			if(b > 255)
			{
				constpt = getConstPt(curstate->constp, b - 256);
				type = platformReadByte(constpt++);
				tmp.floatval = platformReadNumber(constpt);
			}
			else
			{
				tmp.floatval = curstate->reg[b].floatval;
			}

			if(c > 255)
			{
				constpt = getConstPt(curstate->constp, c - 256);
				type = platformReadByte(constpt++);
				curstate->reg[a].floatval = tmp.floatval * platformReadNumber(constpt);
			}
			else
			{
				curstate->reg[a].floatval = tmp.floatval * curstate->reg[c].floatval;
			}

			break;
		case OP_DIV://	A B C	R(A) := RK(B) / RK(C)
			clearRegister(&curstate->reg[a]);
			clearRegister(&tmp);
			curstate->reg[a].type = VAR_FLOAT;
			tmp.type = VAR_FLOAT;
			if(b > 255)
			{
				constpt = getConstPt(curstate->constp, b - 256);
				type = platformReadByte(constpt++);
				tmp.floatval = platformReadNumber(constpt);
			}
			else
			{
				tmp.floatval = curstate->reg[b].floatval;
			}

			if(c > 255)
			{
				constpt = getConstPt(curstate->constp, c - 256);
				type = platformReadByte(constpt++);
				curstate->reg[a].floatval = tmp.floatval / platformReadNumber(constpt);
			}
			else
			{
				curstate->reg[a].floatval = tmp.floatval / curstate->reg[c].floatval;
			}

			break;
		case OP_MOD://	A B C	R(A) := RK(B) % RK(C)
			clearRegister(&curstate->reg[a]);
			clearRegister(&tmp);
			curstate->reg[a].type = VAR_FLOAT;
			tmp.type = VAR_FLOAT;
			if(b > 255)
			{
				constpt = getConstPt(curstate->constp, b - 256);
				type = platformReadByte(constpt++);
				tmp.floatval = platformReadNumber(constpt);
			}
			else
			{
				tmp.floatval = curstate->reg[b].floatval;
			}

			if(c > 255)
			{
				constpt = getConstPt(curstate->constp, c - 256);
				type = platformReadByte(constpt++);
				curstate->reg[a].floatval = fmod(tmp.floatval, platformReadNumber(constpt));
			}
			else
			{
				curstate->reg[a].floatval = fmod(tmp.floatval, curstate->reg[c].floatval);
			}

			break;
		case OP_POW://	A B C	R(A) := RK(B) ^ RK(C)
			clearRegister(&curstate->reg[a]);
			clearRegister(&tmp);
			curstate->reg[a].type = VAR_FLOAT;
			tmp.type = VAR_FLOAT;
			if(b > 255)
			{
				constpt = getConstPt(curstate->constp, b - 256);
				type = platformReadByte(constpt++);
				tmp.floatval = platformReadNumber(constpt);
			}
			else
			{
				tmp.floatval = curstate->reg[b].floatval;
			}

			if(c > 255)
			{
				constpt = getConstPt(curstate->constp, c - 256);
				type = platformReadByte(constpt++);
				curstate->reg[a].floatval = pow(tmp.floatval, platformReadNumber(constpt));
			}
			else
			{
				curstate->reg[a].floatval = pow(tmp.floatval, curstate->reg[c].floatval);
			}

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
			
			if( //check limit
				((curstate->reg[a+2].floatval < 0) && (curstate->reg[a].floatval >= curstate->reg[a+1].floatval))
				||
				((curstate->reg[a+2].floatval >= 0) && (curstate->reg[a].floatval <= curstate->reg[a+1].floatval))
			) 
			{
				curstate->reg[a+3].floatval = curstate->reg[a].floatval;
				vm->pc += (bx+1)*4; //TODO: why +1???
			}
			break;
		//jumps and condition jumps
		case OP_JMP: //	sBx	pc+=sBx	
			vm->pc += sbx*4;
			break;
		case OP_EQ: //	A B C	if ((RK(B) == RK(C)) ~= A) then pc++
			if((compare(&curstate->reg[b], &curstate->reg[c]) == 1) != a)
			{
				vm->pc += 4;
			}
			break;
		case OP_LT: //	A B C	if ((RK(B) <  RK(C)) ~= A) then pc++
			if((compare(&curstate->reg[b], &curstate->reg[c]) == -2) != a)
			{
				vm->pc += 4;
			}
			break;
		case OP_LE: //	A B C	if ((RK(B) <= RK(C)) ~= A) then pc++
			if((compare(&curstate->reg[b], &curstate->reg[c]) == -2 || compare(&curstate->reg[b], &curstate->reg[c]) == 0) != a)
			{
				vm->pc += 4;
			}
			break;



		default:
#ifdef DEBUGVM
			platformPrintf("VM: Unknown opcode %d at %#x\n", opcode, vm->pc-4);
#endif
			break;
			//return 1;//TODO: unknown instruction
		}

	}

	return 0;
}