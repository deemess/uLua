#ifndef UCODEGEN_H
#define UCODEGEN_H

#include "basetypes.h"
#include "opcodes.h"
#include "ulexer.h"

#define CG_REG_COUNT 30

#define CG_CONST_STRING 1
#define CG_CONST_NUMBER 2

typedef struct Function Function;
typedef struct Instruction Instruction;
typedef struct Constant Constant;

typedef struct Register {
	u08		num;
	BOOL	isfree;
	BOOL	isload;
	u08		constnum;
	u08		varnum;
	BOOL	consthold;
	BOOL	constpreloaded;
} Register;

struct Function {
	u08*			code;
	Register		reg[CG_REG_COUNT];
	Instruction*	instr;
	Constant*		vars;
	Constant*		consts;
	Function*		subfuncs;
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
void freeRegister(Register* r); //free register for future usage
Register* doMath(Function* f, Register* a, Register* b, Token* t); //make math
Register* doLogic(Function* f, Register* a, Register* b, Token* t); //make logic
Instruction* statSET(Function* f, Register* a, Register* b, BOOL islocal); //set statement. return last instruction
Register* functionCALL(Function* f, Register* a, Register* b); //call function

#endif
