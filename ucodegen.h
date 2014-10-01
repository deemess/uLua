#ifndef UCODEGEN_H
#define UCODEGEN_H

#include "basetypes.h"
#include "opcodes.h"
#include "ulexer.h"

#define CG_REG_COUNT 32

#define CG_CONST_STRING 1
#define CG_CONST_NUMBER 2

typedef struct Function Function;
typedef struct Instruction Instruction;
typedef struct Constant Constant;

typedef struct Register {
	u08		number;
	u08		constnum;
	BOOL	consthold;
	BOOL	constpreloaded;
	BOOL	isfree;
} Register;

struct Function {
	u08*			code;
	Register		r[CG_REG_COUNT];
	Instruction*	i;
	Constant*		c;
	Function*		f;
};

struct Constant {
	u08			n;
	u08			type;
	SString		string;
	float		number;
	Constant*	next;
};


struct Instruction {
	OpCode	opc;
	u08		a;
	union s16 {
		u08 b;
		u08 c;
	} bx;
	Instruction* next;
};

void initFunction(Function* f, u08* code);
Constant* pushConstString(Function* f, SString* str);
Constant* pushConstNumber(Function* f, float number);
Register* getFreeRegister(Function* f);

#endif