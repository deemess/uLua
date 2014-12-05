#ifndef UCODEGEN_H
#define UCODEGEN_H

#include <stdlib.h>
#include <stdio.h>
#include "basetypes.h"
#include "opcodes.h"
#include "ulexer.h"
#include "llerror.h"

#define CG_REG_COUNT 30

#define NULL_TYPE	0x00
#define BOOL_TYPE	0x01
#define NUMBER_TYPE 0x03
#define STRING_TYPE 0x04

typedef struct Function Function;
typedef struct Instruction Instruction;
typedef struct Constant Constant;

typedef struct Register {
	u08		num;//register number (0 - CG_REG_COUNT)
	BOOL	isfree;//is register used or not
	BOOL	isload;//is register holds any value
	BOOL	islocal;//is register local variable
	u08		constnum;//constant number (0 for defauld - no constant)
	u08		varnum;//if register refer to variable - holds var number in constant pool
	BOOL	consthold;//is register refer to constant
	BOOL	constpreloaded;//is register refer to constant and load
} Register;

struct Function {
	u08*			code;
	Register		reg[CG_REG_COUNT];
	u16				instrSize;
	Instruction*	instr;
	Constant*		vars;
	Constant*		consts;
	u16				constsSize;
	Function*		subfuncs;
	u16				subfuncsSize;
    Function*       next;
	ERROR_CODE		error_code;
	BOOL			parsed;
};

struct Constant {
	u08			num;
	u08			type;
	BOOL		isglobal;
	SString		val_string;
	float		val_number;
	Constant*	next;
};


struct Instruction {
	union i {
		u32		packed;
		struct unpacked {
			u08		opc;
			u08		a;
			union bx {
				s16		bx;
				struct	l {
					u08		b;
					u08		c;
				} l;
			} bx;
		} unpacked;
	} i;
	Instruction* next;
	Instruction* prev;
};

typedef void (*writeBytes)(u08* buff, u16 size);

void initFunction(Function* f, u08* code);
void freeFunction(Function* f); //free all resources used by function and its subfunctions
void dump(Function* f, writeBytes callback); // make binary dump using given callback function
Constant* pushConstString(Function* f, SString* str); //save string in constant pool
Constant* pushConstNumber(Function* f, float number); //save number in constant pool
Constant* pushVarName(Function* f, SString* str); //save var name in var name pool
Register* getFreeRegister(Function* f); //get non used register
Register* getVarRegister(Function* f, Constant* var); //get register for var name
Register* doMath(Function* f, Register* a, Register* b, Token* t); //make math
Register* doLogic(Function* f, Register* a, Register* b, Token* t); //make logic
Register* doCompare(Function* f, Register* a, Register* b, Token* t); //make register comparison. return boolean result
Instruction*  doReturn(Function* f); //generate return instruction
Instruction* statSET(Function* f, Register* a, Register* b, BOOL islocal); //set statement (a = b). return last instruction
Instruction* statTHEN(Function* f, Register* a, Instruction* block); // make then block if register a is true else skip it
Instruction* statELSE(Function* f, Instruction* condlist, Instruction* block); //make else block
Instruction* statELSEIF(Function* f, Instruction* condlist, Instruction* cond); //make elseif block
Instruction* functionCALL(Function* f, Register* a, Register* b); //call function

#endif
