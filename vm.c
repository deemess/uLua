#include "vm.h"
#include <math.h>
#include <stdio.h>

//reset structs functions
static void initStructState(vmstate* state)
{
	int i=0;
	state->constp = 0;
	state->funcp = 0;
	state->closure = NULL;
	state->retreg = 0;

	for(i=0; i<REGISTERSIZE; i++) {
		state->reg[i].type = VAR_NULL;
		state->reg[i].numval = 0;
	}

}

void initStructVM(vm* vm)
{
	int i=0;
	int j=0;

	vm->status = STOP;
	vm->pc = 0;
	vm->pcstackpt = 0;
	vm->statept = 0;

	for(i=0; i<PCSTACKSIZE; i++)
		vm->pcstack[i] = 0;

	for(i=0; i<GLOBALSIZE; i++) {
		vm->global[i].val.type = VAR_NULL;
		vm->global[i].val.numval = 0;

		for(j=0; j<GLOBALNAMESIZE; j++)
			vm->global[i].name[j] = 0;
	}

	for(i=0; i<STATEMAXCOUNT; i++)
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

//read from bytecode functions

u08 buff[4];
u08 platformReadByte(readBytes read, u16 offset)
{
	read(buff, offset, 1);
	return buff[0];
}

u16 platformReadWord(readBytes read, u16 offset)
{
	read(buff, offset, 2);
	return *(u16*)(&buff[0]);
}

float platformReadNumber(readBytes read, u16 offset)
{
	read(buff, offset, sizeof(float));
	return  *(float*)(&buff[0]) ;
}

//Get pointer to sub functions
u16 getFuncsPt(readBytes read, u16 pt)
{
	u08 i=0;
	u16 csize = platformReadWord(read, pt); pt += 2;

	for(i=0; i<csize; i++)
	{
		u08 type = platformReadByte(read, pt++);
		if(type == NUMBER_TYPE)
		{
			pt += sizeof(float);
		}
		else if(type == STRING_TYPE)
		{
			u16 strsize = platformReadWord(read, pt); pt += 2;
			pt += strsize;
		}
	}
	return pt;
}

//skeep function which starts on pt
//return pointer to the next byte after function
u16 skeepFunction(readBytes read, u16 pt)
{
	u08 i=0;
	u16 codesize;
	u16 funcsize;

	//pt += 16; //function header;
	codesize = platformReadWord(read, pt); pt += 2;
	pt += codesize * 4;
	pt = getFuncsPt(read, pt);//subfunctions
	funcsize = platformReadWord(read, pt); pt += 2;
	for(i=0; i<funcsize; i++)
	{
		pt = skeepFunction(read, pt);
	}
	//pt += 3 * 4; //TODO: for now ignore lines, locals and upvalues. we think that thay are all zero size.
	return pt;
}

//Get sub function number N
// pt - pointer to sub functions
u16 getFuncPt(readBytes read, u16 pt, u16 N)
{
	u08 i=0;
	//u32 funcsize = platformReadWord(read, pt); 
	pt += 2;
	for(i=0; N>i; i++)
	{
		pt = skeepFunction(read, pt);
	}
	//pt += 16; //function header;
	return pt;
}

//Get potinter to constant number
u16 getConstPt(readBytes read, u16 constspt, u16 N)
{
	u08 i;
	u16 strsize;
	u16 csize = platformReadWord(read, constspt); constspt += 2;

	for(i=1; i<csize &&  i<N; i++)
	{
		u08 type = platformReadByte(read, constspt++);
		if(type == NUMBER_TYPE)
		{
			constspt += sizeof(float);
		}
		else if(type == STRING_TYPE)
		{
			strsize = platformReadWord(read, constspt); constspt += 2;
			constspt += strsize;
		}
	}
	return constspt;
}

//Find global variable by name .Return global index
u08 getGlobalByName(vm* vm, u08* name)
{
	u08 foundGlobal = 0;
	u08 match;
	int i;

	for(foundGlobal=0; foundGlobal<GLOBALSIZE; foundGlobal++)
	{
		if(vm->global[foundGlobal].val.type == VAR_NULL) //global not found - return free one
			break;

		match = 1;
		for(i=0; i<GLOBALNAMESIZE && name[i] != 0 && vm->global[foundGlobal].name[i] != 0; i++)
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
		GCREFDEC((gcvarpt*)reg->pointer);
		GCCHECK((gcvarpt*)reg->pointer);
	}
	//reg->type = VAR_NULL;
	//reg->numval = 0;
}


u08 vmRun(vm* vm, readBytes read)
{
	u08 a = 0;
	u08 b = 0;
	u08 c = 0;
	s16 sbx = 0;
	u16 constpt = 0;
	u08 type;
	u08 glindex;
	u08 name[32];
	u16 codesize;
	u08 opcode;
	gcvarpt* gcpointer = NULL;
	int i;

	vmregister tmp;
	tmp.type = VAR_NULL;
	tmp.numval = 0;

	//vmstate* state = &vm->state[0];
	//set running state
	vm->status = RUN;

	//skip header
	vm->pc = 0x06;

	//get code size for top function
	codesize = platformReadWord(read, vm->pc); 
	vm->pc += 2;
	vm->state[vm->statept].constp = vm->pc + codesize * 4;
	vm->state[vm->statept].funcp = getFuncsPt(read, vm->state[vm->statept].constp);

	//for(u08 i=0; i<codesize; i++)
	while(vm->status == RUN)
	{
		vmstate* curstate = &vm->state[vm->statept];

		//get first instruction
		opcode = platformReadByte(read, vm->pc);
		a =		platformReadByte(read, vm->pc+1);
		b =		platformReadByte(read, vm->pc+2);
		c =		platformReadByte(read, vm->pc+3);
		sbx = (s16)((((u16)c)<<8 & 0xFF00) + b);
		//inst = platformReadDWord(vm->pc);
		//opcode = GET_OPCODE(inst);

		//go to next instruction
		vm->pc += 4;

		switch(opcode)
		{ 

		case OP_MOVE: //copy R(A) = R(B)
			clearRegister(&curstate->reg[a]);
			curstate->reg[a].type = curstate->reg[b].type;
			curstate->reg[a].pointer = curstate->reg[b].pointer;
			break;

		case OP_CLOSURE: //Create closure and put it into R(A)
			//create closure in GC
			gcpointer = gcNew(VAR_CLOSURE);
			GCVALUE(vmclosure, gcpointer).funcp = getFuncPt(read, curstate->funcp, sbx);
			GCVALUE(vmclosure, gcpointer).upvalcount = platformReadByte(read, GCVALUE(vmclosure, gcpointer).funcp + 3*4);

			//init upvalues
			for(i=0; i < GCVALUE(vmclosure, gcpointer).upvalcount; i++)
			{
				//TODO: implement upvalues
				//u32 nextint = platformReadDWord(read, vm->pc);
				//u08 nextopcode = GET_OPCODE(nextint);
				//u08 nextb = GETARG_B(nextint);

				//switch(nextopcode)
				//{
				////init upvalue from vm register
				//case OP_MOVE:
				//	GCVALUE(vmclosure, gcpointer).upval[i].type = curstate->reg[b].type;
				//	GCVALUE(vmclosure, gcpointer).upval[i].pointer = curstate->reg[b].pointer;
				//	break;

				////init upvalue from another upvalue
				//case OP_GETUPVAL:
				//	if(curstate->closure != NULL)
				//	{
				//		GCVALUE(vmclosure, gcpointer).upval[i].type = GCVALUE(vmclosure, curstate->closure).upval[b].type;
				//		GCVALUE(vmclosure, gcpointer).upval[i].pointer = GCVALUE(vmclosure, curstate->closure).upval[b].pointer;
				//	}
				//	break;

				//default:
				//	break;
				//}

				vm->pc += 4;
			}

			//save pointer to the closure into register
			GCREFINC(gcpointer);
			clearRegister(&curstate->reg[a]);
			curstate->reg[a].type = VAR_CLOSURE;
			curstate->reg[a].pointer = gcpointer;
			break;

		case OP_SETGLOBAL: //	A Bx	Gbl[Kst(Bx)] := R(A)
			//read global name
			constpt = getConstPt(read, curstate->constp, sbx);
			type = platformReadByte(read, constpt++);
			codesize = platformReadWord(read, constpt);
			constpt += 2;
			read(name, constpt, codesize);

			//search global
			glindex = getGlobalByName(vm, name);
			//copy name
			for(i=0; i<GLOBALNAMESIZE && name[i] != 0; i++)
			{
				vm->global[glindex].name[i] = name[i];
			}

			//set global value
			if(curstate->reg[a].type == VAR_CLOSURE)
			{
				GCREFINC((gcvarpt*)curstate->reg[a].pointer);
			}
			clearRegister(&vm->global[glindex].val);
			vm->global[glindex].val.type = curstate->reg[a].type;
			vm->global[glindex].val.pointer = curstate->reg[a].pointer;
			break;

		case OP_GETGLOBAL: // A Bx	R(A) := Gbl[Kst(Bx)]
			//read global name
			constpt = getConstPt(read, curstate->constp, sbx);
			type = platformReadByte(read, constpt++);
			codesize = platformReadWord(read, constpt);
			constpt += 2;
			read(name, constpt, codesize);

			//search global
			glindex = getGlobalByName(vm, name);


			clearRegister(&curstate->reg[a]);
			curstate->reg[a].type = vm->global[glindex].val.type;
			curstate->reg[a].pointer = vm->global[glindex].val.pointer;
			break;

		case OP_GETUPVAL: //R(A) := UpValue[B]
			clearRegister(&curstate->reg[a]);
			curstate->reg[a].type = GCVALUE(vmclosure, curstate->closure).upval[b].type;
			curstate->reg[a].pointer = GCVALUE(vmclosure, curstate->closure).upval[b].pointer;
			break;

		case OP_SETUPVAL: //UpValue[B] := R(A)
			clearRegister(&GCVALUE(vmclosure, curstate->closure).upval[b]);
			GCVALUE(vmclosure, curstate->closure).upval[b].type = curstate->reg[a].type;
			GCVALUE(vmclosure, curstate->closure).upval[b].pointer = curstate->reg[a].pointer;
			break;
		
		case OP_LOADK: //A Bx	R(A) := Kst(Bx)		
			constpt = getConstPt(read, curstate->constp, sbx);
			type = platformReadByte(read, constpt++);

			clearRegister(&curstate->reg[a]);

			switch(type)
			{
			case BOOL_TYPE:
				curstate->reg[a].type = VAR_BOOLEAN;
				break;

			case NUMBER_TYPE:
				curstate->reg[a].type = VAR_FLOAT;
				curstate->reg[a].floatval = platformReadNumber(read, constpt);
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
			for(i=a; i<=b; i++)
			{
				clearRegister(&curstate->reg[i]);
				curstate->reg[i].type = VAR_NULL;
				curstate->reg[i].numval = 0;
			}
			break;

		case OP_LOADBOOL:
			clearRegister(&curstate->reg[a]);
			curstate->reg[a].type = VAR_BOOLEAN;
			curstate->reg[a].floatval = (float)b;
			if(c > 0) //skip instruction
				vm->pc += 4;
			break;

		case OP_CALL: //A B C	R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))
			if(curstate->reg[a].type == VAR_NATIVE_FUNC)
			{//native function call
				nativeCall(vm, read, a, b, c);
			}
			else if(curstate->reg[a].type == VAR_CLOSURE)
			{//lua function call

				//save next pc instruction address to the stack
				vm->pcstack[vm->pcstackpt++] = vm->pc;
				//set pc = call function address
				//TODO: check reg type
				gcpointer = (gcvarpt*)curstate->reg[a].pointer;
				vm->pc = GCVALUE(vmclosure,gcpointer).funcp;
				vm->pc += 16; //skip functino header

				//prepare state
				vm->statept++;
				//get code size function
				codesize = platformReadWord(read, vm->pc); 
				vm->pc += 2;
				vm->state[vm->statept].constp = vm->pc + codesize * 4;
				vm->state[vm->statept].funcp = getFuncsPt(read, vm->state[vm->statept].constp);
				vm->state[vm->statept].retreg = a;
				vm->state[vm->statept].closure = gcpointer;
				//copy args to the new state
				//TODO: support copy all values on the top of the stack (b=0)
				for(i=0; i<b-1 && b!=0; i++)
				{
					vm->state[vm->statept].reg[i].type   = vm->state[vm->statept-1].reg[a+1+i].type;
					vm->state[vm->statept].reg[i].pointer = vm->state[vm->statept-1].reg[a+1+i].pointer;
				}

			} else {
#ifdef DEBUGVM
				printf("VM: Cant make CALL. Got non function pointer at %#x. Type: %d Numval: %d Floatval: %f\n", vm->pc-4, curstate->reg[a].type, curstate->reg[a].numval, curstate->reg[a].floatval);
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
				for(i=0; i<b-1 && b!=0; i++)
				{
					vm->state[vm->statept].reg[vm->state[vm->statept+1].retreg+i].type   = vm->state[vm->statept+1].reg[a+i].type;
					vm->state[vm->statept].reg[vm->state[vm->statept+1].retreg+i].pointer = vm->state[vm->statept+1].reg[a+i].pointer;
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

			if(b > CG_REG_COUNT)
			{
				constpt = getConstPt(read, curstate->constp, b - CG_REG_COUNT);
				type = platformReadByte(read, constpt++);
				tmp.floatval = platformReadNumber(read, constpt);
			}
			else
			{
				tmp.floatval = curstate->reg[b].floatval;
			}

			if(c > CG_REG_COUNT)
			{
				constpt = getConstPt(read, curstate->constp, c - CG_REG_COUNT);
				type = platformReadByte(read, constpt++);
				curstate->reg[a].floatval = tmp.floatval + platformReadNumber(read, constpt);
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

			if(b > CG_REG_COUNT)
			{
				constpt = getConstPt(read, curstate->constp, b - CG_REG_COUNT);
				type = platformReadByte(read, constpt++);
				tmp.floatval = platformReadNumber(read, constpt);
			}
			else
			{
				tmp.floatval = curstate->reg[b].floatval;
			}

			if(c > CG_REG_COUNT)
			{
				constpt = getConstPt(read, curstate->constp, c - CG_REG_COUNT);
				type = platformReadByte(read, constpt++);
				curstate->reg[a].floatval = tmp.floatval - platformReadNumber(read, constpt);
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
			if(b > CG_REG_COUNT)
			{
				constpt = getConstPt(read, curstate->constp, b - CG_REG_COUNT);
				type = platformReadByte(read, constpt++);
				tmp.floatval = platformReadNumber(read, constpt);
			}
			else
			{
				tmp.floatval = curstate->reg[b].floatval;
			}

			if(c > CG_REG_COUNT)
			{
				constpt = getConstPt(read, curstate->constp, c - CG_REG_COUNT);
				type = platformReadByte(read, constpt++);
				curstate->reg[a].floatval = tmp.floatval * platformReadNumber(read, constpt);
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
			if(b > CG_REG_COUNT)
			{
				constpt = getConstPt(read, curstate->constp, b - CG_REG_COUNT);
				type = platformReadByte(read, constpt++);
				tmp.floatval = platformReadNumber(read, constpt);
			}
			else
			{
				tmp.floatval = curstate->reg[b].floatval;
			}

			if(c > CG_REG_COUNT)
			{
				constpt = getConstPt(read, curstate->constp, c - CG_REG_COUNT);
				type = platformReadByte(read, constpt++);
				curstate->reg[a].floatval = tmp.floatval / platformReadNumber(read, constpt);
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
			if(b > CG_REG_COUNT)
			{
				constpt = getConstPt(read, curstate->constp, b - CG_REG_COUNT);
				type = platformReadByte(read, constpt++);
				tmp.floatval = platformReadNumber(read, constpt);
			}
			else
			{
				tmp.floatval = curstate->reg[b].floatval;
			}

			if(c > CG_REG_COUNT)
			{
				constpt = getConstPt(read, curstate->constp, c - CG_REG_COUNT);
				type = platformReadByte(read, constpt++);
				curstate->reg[a].floatval = fmod(tmp.floatval, platformReadNumber(read, constpt));
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
			if(b > CG_REG_COUNT)
			{
				constpt = getConstPt(read, curstate->constp, b - CG_REG_COUNT);
				type = platformReadByte(read, constpt++);
				tmp.floatval = platformReadNumber(read, constpt);
			}
			else
			{
				tmp.floatval = curstate->reg[b].floatval;
			}

			if(c > CG_REG_COUNT)
			{
				constpt = getConstPt(read, curstate->constp, c - CG_REG_COUNT);
				type = platformReadByte(read, constpt++);
				curstate->reg[a].floatval = pow(tmp.floatval, platformReadNumber(read, constpt));
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
			vm->pc += (sbx+1)*4; //TODO: why +1???
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
				vm->pc += (sbx+1)*4; //TODO: why +1???
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
			if((compare(&curstate->reg[b], &curstate->reg[c]) == -2 || compare(&curstate->reg[b], &curstate->reg[c]) == 1) != a)
			{
				vm->pc += 4;
			}
			break;
		//boolean checks
		case OP_TESTSET://	A B C	if (R(B) <=> C) then R(A) := R(B) else pc++ 
			if(curstate->reg[b].floatval != (float)c) {
				curstate->reg[a].floatval = curstate->reg[b].floatval;
				curstate->reg[a].type = curstate->reg[b].type;
			} else {
				vm->pc += 4;
			}
			break;
		case OP_TEST://	A C	if not (R(A) <=> C) then pc++
			if(curstate->reg[a].floatval == (float)c) {
				vm->pc += 4;
			}
			break;

		default:
#ifdef DEBUGVM
			printf("VM: Unknown opcode %d at %#x\n", opcode, vm->pc-4);
#endif
			break;
			//return 1;//TODO: unknown instruction
		}

	}

	return 0;
}
