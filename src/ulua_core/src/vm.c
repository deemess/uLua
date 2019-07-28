#include <math.h>
#include <stdio.h>
#include "ulua_core/vm.h"
#include "ulua_core/vmconfig.h"
#include "ulua_core/opcodes.h"
#include "ulua_core/native.h"
#include "ulua_core/ucodegen.h"


//reset structs functions
static void initStructState(vmstate* state)
{
	int i=0;
	state->constp = 0;
	state->funcp = 0;
	state->closure = NULL;
	state->retreg = 0;

	for(i=0; i<REGISTERSIZE; i++) {
		state->reg[i].type = REGISTER_VAR_NULL;
		state->reg[i].numval = 0;
	}

}

void initStructVM(ulua_vm* vm)
{
	int i=0;
	int j=0;

	vm->status = STOP;
	vm->pc = 0;
	vm->pcstackpt = 0;
	vm->statept = 0;

	for(i=0; i<PCSTACKSIZE; i++)
		vm->pcstack[i] = 0;

	vm->globals_table = ulua_mem_table_new();

	for(i=0; i<STATEMAXCOUNT; i++)
		initStructState(&vm->state[i]);
}

//Initialize virtual machine
ulua_memvar* vmInit(lu08* memory, lu16 memory_size)
{
	//Init garbage collector and mamory management
    ulua_mem_init(memory, memory_size);
    //allocate memory for vm struct
    ulua_memvar* v = ulua_mem_new(ULUA_MEM_TYPE_VM, sizeof(ulua_vm));

    //reset structure
    initStructVM(GCVALUE(ulua_vm*, v));

    GCVALUE(ulua_vm*, v)->memory = memory;
    GCVALUE(ulua_vm*, v)->memory_size = memory_size;
	//pre load native functions in global namespace
	nativeInit(GCVALUE(ulua_vm*, v));

	return v;
}

//read from bytecode functions

lu08 buff[4];
lu08 platformReadByte(readBytes read, lu16 offset)
{
	read(buff, offset, 1);
	return buff[0];
}

lu16 platformReadWord(readBytes read, lu16 offset)
{
	read(buff, offset, 2);
	return *(lu16*)(&buff[0]);
}

float platformReadNumber(readBytes read, lu16 offset)
{
	read(buff, offset, sizeof(float));
	return  *(float*)(&buff[0]) ;
}

//Get pointer to sub functions
lu16 getFuncsPt(readBytes read, lu16 pt)
{
	lu08 i=0;
	lu16 csize = platformReadWord(read, pt); pt += 2;

	for(i=0; i<csize; i++)
	{
		lu08 type = platformReadByte(read, pt++);
		if(type == NUMBER_TYPE)
		{
			pt += sizeof(float);
		}
		else if(type == STRING_TYPE)
		{
			lu16 strsize = platformReadWord(read, pt); pt += 2;
			pt += strsize;
		}
	}
	return pt;
}

//skeep function which starts on pt
//return pointer to the next byte after function
lu16 skipFunction(readBytes read, lu16 pt)
{
	lu08 i=0;
	lu16 codesize;
	lu16 funcsize;

	//pt += 16; //function header;
	codesize = platformReadWord(read, pt); pt += 2;
	pt += codesize * 4;
	pt = getFuncsPt(read, pt);//subfunctions
	funcsize = platformReadWord(read, pt); pt += 2;
	for(i=0; i<funcsize; i++)
	{
		pt = skipFunction(read, pt);
	}
	//pt += 3 * 4; //TODO: for now ignore lines, locals and upvalues. we think that thay are all zero size.
	return pt;
}

//Get sub function number N
// pt - pointer to sub functions
lu16 getFuncPt(readBytes read, lu16 pt, lu16 N)
{
	lu08 i=0;
	//u32 funcsize = platformReadWord(read, pt); 
	pt += 2;
	for(i=0; N>i; i++)
	{
		pt = skipFunction(read, pt);
	}
	//pt += 16; //function header;
	return pt;
}

//Get potinter to constant number
lu16 getConstPt(readBytes read, lu16 constspt, lu16 N)
{
	lu08 i;
	lu16 strsize;
	lu16 csize = platformReadWord(read, constspt); constspt += 2;

	for(i=1; i<csize &&  i<N; i++)
	{
		lu08 type = platformReadByte(read, constspt++);
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

//compare values in register a and b. return:  0: a!=b   1: a=b  -1: a>b  -2: a<b 
ls08 compare(vmregister* a, vmregister* b)
{
	if(a->type != b->type)
		return 0;

	if(a->numval == b->numval)
		return 1;

	if(a->type == REGISTER_VAR_FLOAT)
	{ 
		if(a->floatval > b->floatval)
			return -1;
		if(a->floatval < b->floatval)
			return -2;
	}

	if(a->type == REGISTER_VAR_NUMBER)
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
//	if(reg->type == REGISTER_VAR_CLOSURE) {
//		GCREFDEC((gcvarpt*)reg->pointer);
//		GCCHECK((gcvarpt*)reg->pointer);
//	}
	//reg->type = REGISTER_VAR_NULL;
	//reg->numval = 0;
}


lu08 vmRun(ulua_memvar* memvm, readBytes read)
{
    ulua_vm* vm = GCVALUE(ulua_vm*, memvm);

	lu08 a = 0;
	lu08 b = 0;
	lu08 c = 0;
	ls16 sbx = 0;
	lu16 constpt = 0;
	lu08 type;
	lu08 glindex;
	lu08 name[32];
	lu16 codesize;
	lu08 opcode;
    ulua_memvar* gcpointer = NULL;
	int i;
    ulua_memvar* stringvar = ULUA_NULL;
    ulua_memvar* regvar = ULUA_NULL;
    vmregister* reg = ULUA_NULL;

	vmregister tmp;
	vmregister tmp2;
	tmp.type = REGISTER_VAR_NULL;
	tmp.numval = 0;
	tmp2.type = REGISTER_VAR_NULL;
	tmp2.numval = 0;

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
		sbx = (ls16)((((lu16)c)<<8 & 0xFF00) + b);
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
			gcpointer = ulua_mem_new(REGISTER_VAR_CLOSURE, sizeof(vmclosure));
			GCVALUE(vmclosure*, gcpointer)->funcp = getFuncPt(read, curstate->funcp, sbx);
			GCVALUE(vmclosure*, gcpointer)->upvalcount = platformReadByte(read, GCVALUE(vmclosure*, gcpointer)->funcp + 3*4);

			//init upvalues
			for(i=0; i < GCVALUE(vmclosure*, gcpointer)->upvalcount; i++)
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
			//GCREFINC(gcpointer);
			clearRegister(&curstate->reg[a]);
			curstate->reg[a].type = REGISTER_VAR_CLOSURE;
			curstate->reg[a].pointer = gcpointer;
			break;

		case OP_SETGLOBAL: //	A Bx	Gbl[Kst(Bx)] := R(A)
			//read global name
			constpt = getConstPt(read, curstate->constp, sbx);
			type = platformReadByte(read, constpt++);
			codesize = platformReadWord(read, constpt);
			constpt += 2;
			read(name, constpt, codesize);

			stringvar = ulua_mem_string_new(name);
			regvar = ulua_mem_new(ULUA_MEM_TYPE_VMREGISTER, sizeof(vmregister));
			reg = GCVALUE(vmregister*, regvar);
			reg->type = curstate->reg[a].type;
			reg->pointer = curstate->reg[a].pointer;

			ulua_mem_table_put(vm->globals_table, stringvar, regvar);
			break;

		case OP_GETGLOBAL: // A Bx	R(A) := Gbl[Kst(Bx)]
			//read global name
			constpt = getConstPt(read, curstate->constp, sbx);
			type = platformReadByte(read, constpt++);
			codesize = platformReadWord(read, constpt);
			constpt += 2;
			read(name, constpt, codesize);

			stringvar = ulua_mem_string_new(name);
			regvar = ulua_mem_table_get(vm->globals_table, stringvar);
			if(regvar != ULUA_NULL) {
                reg = GCVALUE(vmregister*, regvar);
                curstate->reg[a].type = reg->type;
                curstate->reg[a].pointer = reg->pointer;
            } else {
			    //TODO: raise an exception (Global not found)
			}
			break;

		case OP_GETUPVAL: //R(A) := UpValue[B]
			clearRegister(&curstate->reg[a]);
			curstate->reg[a].type = GCVALUE(vmclosure*, curstate->closure)->upval[b].type;
			curstate->reg[a].pointer = GCVALUE(vmclosure*, curstate->closure)->upval[b].pointer;
			break;

		case OP_SETUPVAL: //UpValue[B] := R(A)
			clearRegister(&GCVALUE(vmclosure*, curstate->closure)->upval[b]);
			GCVALUE(vmclosure*, curstate->closure)->upval[b].type = curstate->reg[a].type;
			GCVALUE(vmclosure*, curstate->closure)->upval[b].pointer = curstate->reg[a].pointer;
			break;
		
		case OP_LOADK: //A Bx	R(A) := Kst(Bx)		
			constpt = getConstPt(read, curstate->constp, sbx);
			type = platformReadByte(read, constpt++);

			clearRegister(&curstate->reg[a]);

			switch(type)
			{
			case BOOL_TYPE:
				curstate->reg[a].type = REGISTER_VAR_BOOLEAN;
				break;

			case NUMBER_TYPE:
				curstate->reg[a].type = REGISTER_VAR_FLOAT;
				curstate->reg[a].floatval = platformReadNumber(read, constpt);
				break;

			case STRING_TYPE:
                codesize = platformReadWord(read, constpt);
                constpt += 2;
                read(name, constpt, codesize);

			    stringvar = ulua_mem_string_new(name);
				curstate->reg[a].type = REGISTER_VAR_MEMVAR;
				curstate->reg[a].pointer = stringvar;
				break;

			default:
			case NULL_TYPE:
				curstate->reg[a].type = REGISTER_VAR_NULL;
				break;
			}
			break;

		
		case OP_LOADNIL: //R(A) := ... := R(B) := nil
			for(i=a; i<=b; i++)
			{
				clearRegister(&curstate->reg[i]);
				curstate->reg[i].type = REGISTER_VAR_NULL;
				curstate->reg[i].numval = 0;
			}
			break;

		case OP_LOADBOOL:
			clearRegister(&curstate->reg[a]);
			curstate->reg[a].type = REGISTER_VAR_BOOLEAN;
			curstate->reg[a].floatval = (float)b;
			if(c > 0) //skip instruction
				vm->pc += 4;
			break;

		case OP_CALL: //A B C	R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))
			if(curstate->reg[a].type == REGISTER_VAR_NATIVE_FUNC)
			{//native function call
				nativeCall(vm, read, a, b, c);
			}
			else if(curstate->reg[a].type == REGISTER_VAR_CLOSURE)
			{//lua function call

				//save next pc instruction address to the stack
				vm->pcstack[vm->pcstackpt++] = vm->pc;
				//set pc = call function address
				//TODO: check reg type
				gcpointer = (ulua_memvar*)curstate->reg[a].pointer;
				vm->pc = GCVALUE(vmclosure*,gcpointer)->funcp;
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
			curstate->reg[a].type = REGISTER_VAR_FLOAT;
			tmp.type = REGISTER_VAR_FLOAT;

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
			curstate->reg[a].type = REGISTER_VAR_FLOAT;
			tmp.type = REGISTER_VAR_FLOAT;

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
			curstate->reg[a].type = REGISTER_VAR_FLOAT;
			tmp.type = REGISTER_VAR_FLOAT;
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
			curstate->reg[a].type = REGISTER_VAR_FLOAT;
			tmp.type = REGISTER_VAR_FLOAT;
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
			curstate->reg[a].type = REGISTER_VAR_FLOAT;
			tmp.type = REGISTER_VAR_FLOAT;
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
			curstate->reg[a].type = REGISTER_VAR_FLOAT;
			tmp.type = REGISTER_VAR_FLOAT;
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
		
		case OP_NOT://	A B 	R(A) := not R(B)
			clearRegister(&curstate->reg[a]);
			curstate->reg[a].type = REGISTER_VAR_BOOLEAN;
			if(curstate->reg[b].type == REGISTER_VAR_NULL) {
				curstate->reg[a].floatval = 1;
			} else {
				curstate->reg[a].floatval = curstate->reg[b].floatval == 0 ? 1 : 0;
			}
			break;
		case OP_UNM://	A B		R(A) := -R(B)
			clearRegister(&curstate->reg[a]);
			curstate->reg[a].type = REGISTER_VAR_FLOAT;
			if(curstate->reg[b].type == REGISTER_VAR_NULL) {
				curstate->reg[a].type = REGISTER_VAR_NULL;
			} else {
				curstate->reg[a].floatval = -curstate->reg[b].floatval;
			}
			break;
		//execution logic functions
		//FOR LOOPS
		case OP_FORPREP:
			//TODO: add checking numeric types in for loops variables
			curstate->reg[a+3].type = REGISTER_VAR_FLOAT;
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
			if(b > CG_REG_COUNT) {
				constpt = getConstPt(read, curstate->constp, b - CG_REG_COUNT);
				tmp.type = (vartype)platformReadByte(read, constpt++);
				tmp.floatval = platformReadNumber(read, constpt);
			} else {
				tmp.floatval = curstate->reg[b].floatval;
				tmp.type = curstate->reg[b].type;;
			}
			if(c > CG_REG_COUNT) {
				constpt = getConstPt(read, curstate->constp, c - CG_REG_COUNT);
				tmp2.type = (vartype)platformReadByte(read, constpt++);
				tmp2.floatval = platformReadNumber(read, constpt);
			} else {
				tmp2.floatval = curstate->reg[c].floatval;
				tmp2.type = curstate->reg[c].type;
			}

			if((compare(&tmp, &tmp2) == 1) != a)
			{
				vm->pc += 4;
			}
			break;
		case OP_LT: //	A B C	if ((RK(B) <  RK(C)) ~= A) then pc++
			if(b > CG_REG_COUNT) {
				constpt = getConstPt(read, curstate->constp, b - CG_REG_COUNT);
				tmp.type = (vartype)platformReadByte(read, constpt++);
				tmp.floatval = platformReadNumber(read, constpt);
			} else {
				tmp.floatval = curstate->reg[b].floatval;
				tmp.type = curstate->reg[b].type;;
			}
			if(c > CG_REG_COUNT) {
				constpt = getConstPt(read, curstate->constp, c - CG_REG_COUNT);
				tmp2.type = (vartype)platformReadByte(read, constpt++);
				tmp2.floatval = platformReadNumber(read, constpt);
			} else {
				tmp2.floatval = curstate->reg[c].floatval;
				tmp2.type = curstate->reg[c].type;
			}

			if((compare(&tmp, &tmp2) == -2) != a)
			{
				vm->pc += 4;
			}
			break;
		case OP_LE: //	A B C	if ((RK(B) <= RK(C)) ~= A) then pc++
			if(b > CG_REG_COUNT) {
				constpt = getConstPt(read, curstate->constp, b - CG_REG_COUNT);
				tmp.type = (vartype)platformReadByte(read, constpt++);
				tmp.floatval = platformReadNumber(read, constpt);
			} else {
				tmp.floatval = curstate->reg[b].floatval;
				tmp.type = curstate->reg[b].type;;
			}
			if(c > CG_REG_COUNT) {
				constpt = getConstPt(read, curstate->constp, c - CG_REG_COUNT);
				tmp2.type = (vartype)platformReadByte(read, constpt++);
				tmp2.floatval = platformReadNumber(read, constpt);
			} else {
				tmp2.floatval = curstate->reg[c].floatval;
				tmp2.type = curstate->reg[c].type;
			}

			if((compare(&tmp, &tmp2) == -2 || compare(&tmp, &tmp2) == 1) != a)
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
