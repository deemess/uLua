#ifndef UCODEGEN_H
#define UCODEGEN_H

#include "ulua_core/vmconfig.h"
#include "ulua_core/basetypes.h"
#include "ulua_core/ulexer.h"

#define CG_REG_COUNT 30

#define NULL_TYPE	0x00
#define BOOL_TYPE	0x01
#define NUMBER_TYPE 0x03
#define STRING_TYPE 0x04

typedef struct Function Function;
typedef struct Instruction Instruction;
typedef struct Constant Constant;
typedef struct Register Register;

struct Register {
	lu08	num;//register number (0 - CG_REG_COUNT)
	BOOL	isfree;//is register used or not
	BOOL	isload;//is register holds any value
	BOOL	islocal;//is register local variable
	lu08	constnum;//constant number (0 for defauld - no constant)
	lu08	varnum;//if register refer to variable - holds var number in constant pool
	BOOL	consthold;//is register refer to constant
	BOOL	constpreloaded;//is register refer to constant and load
	Instruction* exprStart;
	Register* next;//next register in register list
} ;

struct Function {
	lu08*			code;
	Register		reg[CG_REG_COUNT];
	lu16			instrSize;
	Instruction*	instr;
	Instruction*	currentStat;
	Constant*		vars;
	Constant*		consts;
	lu16			constsSize;
	Function*		subfuncs;
	lu16			subfuncsSize;
    Function*       next;
	ERROR_CODE		error_code;
	BOOL			parsed;
};

struct Constant {
	lu08		num;
	lu08		type;
	BOOL		isglobal;
	SString		val_string;
	float		val_number;
	Constant*	next;
};


struct Instruction {
	union i {
		lu32	packed;
		struct unpacked {
			lu08	opc;
			lu08	a;
			union bx {
				ls16	bx;
				struct	l {
					lu08	b;
					lu08	c;
				} l;
			} bx;
		} unpacked;
	} i;
	Instruction* next;
	Instruction* prev;
};

typedef void (*writeBytes)(lu08* buff, lu16 size);

void initFunction(Function* f, lu08* code);
void freeFunction(Function* f); //free all resources used by function and its subfunctions
void dump(Function* f, writeBytes callback); // make binary dump using given callback function
Constant* pushConstString(Function* f, SString* str); //save string in constant pool
Constant* pushConstNumber(Function* f, float number); //save number in constant pool
Constant* pushVarName(Function* f, SString* str); //save var name in var name pool
Register* getFreeRegister(Function* f); //get non used register
Register* getVarRegister(Function* f, Constant* var); //get register for var name
Register* addRegisterList(Register* a, Register* b); //add register b to the registerlist a
void unloadRegisters(Function* f);//unload/mark registers
Register* doNot(Function* f, Register* a, Token* t); //do not\minus logic
Register* doMath(Function* f, Register* a, Register* b, Token* t); //make math
Register* doLogic(Function* f, Register* a, Register* b, Token* t); //make logic
Register* doCompare(Function* f, Register* a, Register* b, Token* t); //make register comparison. return boolean result
Register* doNil(Function* f); //allocate register and load nil in it
Register* doBoolean(Function* f, Token* t); //allocate register and load bool value in it
Register* doTable(Function* f); //allocate register and load new table in it
Instruction* doReturn(Function* f); //generate return instruction
Instruction* statSET(Function* f, Register* a, Register* b, BOOL islocal); //set statement (a = b). return last instruction
Instruction* statSETList(Function* f, Register* a, Register* b, BOOL islocal); //set statement with list assigment support
Instruction* statTHEN(Function* f, Register* a, Instruction* block); // make then block if register a is true else skip it
Instruction* statELSE(Function* f, Instruction* condlist, Instruction* block); //make else block
Instruction* statELSEIF(Function* f, Instruction* condlist, Instruction* cond); //make elseif block
Instruction* statWHILE(Function* f, Register* a, Instruction* block); //make while block
Instruction* functionCALL(Function* f, Register* a, Register* b); //call function

#endif
