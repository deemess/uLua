#ifndef UCODEGEN_H
#define UCODEGEN_H

#include "basetypes.h"
#include "opcodes.h"
#include "ulexer.h"
#include "llerror.h"

#define CG_REG_COUNT 30

#define CG_CONST_STRING 1
#define CG_CONST_NUMBER 2

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
	Instruction*	instr;
	Constant*		vars;
	Constant*		consts;
	Function*		subfuncs;
	ERROR_CODE		error_code;
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
	OpCode	opc;
	u08		a;
	u08		b;
	u08		c;
	s16		bx;
	Instruction* next;
};

void initFunction(Function* f, u08* code);
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
Register* functionCALL(Function* f, Register* a, Register* b); //call function

#endif
