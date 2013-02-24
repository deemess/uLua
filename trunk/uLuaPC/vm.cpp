#include "vm.h"
#include <math.h>

//Initialize virtual machine
void vmInit()
{
	//Init garbage collector and mamory management
	gcInit();
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

u08 vmRun(vmstate* state)
{
	//set running state
	state->thstate = RUN;

	//skip header
	state->pc = 0x1c;

	//get code size for top function
	u32 codesize = platformReadDWord(state->pc); state->pc += 4;
	state->constp = state->pc + codesize * 4;
	state->funcp = getFuncsPt(state->constp);


	u08 a = 0;
	u08 b = 0;
	u08 c = 0;
	u16 bx = 0;
	u16 constpt = 0;
	u08 type;

	//for(u08 i=0; i<codesize; i++)
	while(state->thstate == RUN)
	{
		//get first instruction
		u32 inst = platformReadDWord(state->pc);
		u08 opcode = GET_OPCODE(inst);

		//go to next instruction
		state->pc += 4;

		//get operands
		a = GETARG_A(inst);
		b = GETARG_B(inst);
		c = GETARG_C(inst);
		bx = GETARG_Bx(inst);

		switch(opcode)
		{ 
		case OP_CLOSURE: //Create closure and put it into R(A)
			state->reg[a].type = VAR_FILE_POINTER;
			state->reg[a].numval = getFuncPt(state->funcp, bx);
			break;

		case OP_SETGLOBAL: //	A Bx	Gbl[Kst(Bx)] := R(A)
			state->global[bx].type = state->reg[a].type;
			state->global[bx].numval = state->reg[a].numval;
			break;

		case OP_GETGLOBAL: // A Bx	R(A) := Gbl[Kst(Bx)]
			state->reg[a].type = state->global[bx].type;
			state->reg[a].numval = state->global[bx].numval;
			break;

		case OP_LOADK: //A Bx	R(A) := Kst(Bx)		
			constpt = getConstPt(state->constp, bx);
			type = platformReadByte(constpt++);

			switch(type)
			{
			case BOOL_TYPE:
				state->reg[a].type = VAR_BOOLEAN;
				break;

			case NUMBER_TYPE:
				state->reg[a].type = VAR_FLOAT;
				state->reg[a].floatval = platformReadNumber(constpt);
				break;

			case STRING_TYPE:
				state->reg[a].type = VAR_FILE_POINTER;
				state->reg[a].numval = constpt;
				break;

			default:
			case NULL_TYPE:
				state->reg[a].type = VAR_NULL;
				break;
			}
			break;

		case OP_CALL: //A B C	R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))
			//save next pc instruction address to the stack
			state->pcstack[state->pcstackpt++] = state->pc;
			//set pc = call function address
			//TODO: check reg type
			state->pc = state->reg[a].numval;
			//get code size function
			codesize = platformReadDWord(state->pc); 
			state->pc += 4;
			break;

		case OP_RETURN: //A B	return R(A), ... ,R(A+B-2)	(see note)
			//check if return from main function
			if(state->pcstackpt == 0)
			{
				state->thstate = STOP;
			}
			else
			{
				//restore pc stack address
				state->pc = state->pcstack[--state->pcstackpt];
			}
			break;

		//mathematic function
		//TODO: string support
		//TODO: check types and nulls
		case OP_ADD://	A B C	R(A) := RK(B) + RK(C)
			state->reg[a].floatval = state->reg[b].floatval + state->reg[c].floatval;
			break;
		case OP_SUB://	A B C	R(A) := RK(B) - RK(C)
			state->reg[a].floatval = state->reg[b].floatval - state->reg[c].floatval;
			break;
		case OP_MUL://	A B C	R(A) := RK(B) * RK(C)
			state->reg[a].floatval = state->reg[b].floatval * state->reg[c].floatval;
			break;
		case OP_DIV://	A B C	R(A) := RK(B) / RK(C)
			state->reg[a].floatval = state->reg[b].floatval / state->reg[c].floatval;
			break;
		case OP_MOD://	A B C	R(A) := RK(B) % RK(C)
			state->reg[a].floatval = fmod(state->reg[b].floatval, state->reg[c].floatval);
			break;
		case OP_POW://	A B C	R(A) := RK(B) ^ RK(C)
			state->reg[a].floatval = pow(state->reg[b].floatval , state->reg[c].floatval);
			break;

		default:
			return 1;//TODO: unknown instruction
		}

	}

	return 0;
}
