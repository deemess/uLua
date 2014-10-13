#ifndef ULEXER_H
#define ULEXER_H

#include <ctype.h>
#include <stdlib.h>
#include "basetypes.h"
#include "llerror.h"
#include "uparser.h"

//#define TK_AND 257
//#define TK_BREAK 258
//#define TK_DO 259
//#define TK_ELSE 260
//#define TK_ELSEIF 261
//#define TK_END 262
//#define TK_FALSE 263
//#define TK_FOR 264
//#define TK_FUNCTION 265
#define TK_GOTO 266
//#define TK_IF 267
//#define TK_IN 268
//#define TK_LOCAL 269
//#define TK_NIL 270
//#define TK_NOT 271
//#define TK_OR 272
//#define TK_REPEAT 273
//#define TK_RETURN 274
//#define TK_THEN 275
//#define TK_TRUE 276
//#define TK_UNTIL 277
//#define TK_WHILE 278
// other terminal symbols
//#define TK_CONCAT 279
//#define TK_DOTS 280
//#define TK_EQ 281
//#define TK_GE 282
//#define TK_LE 283
//#define TK_NE 284
#define TK_DBCOLON 285
#define TK_EOS 0
//#define TK_NUMBER 287
//#define TK_NAME 288
//#define TK_STRING 289



typedef struct {
	u16 type;
	u08 keyword[10];
} reserved;


typedef struct SString { //String structure - store only pointer to existing stream position and string lengh. Used to save RAM.
	u08 bempty; //boolean if buff is emtry and not saved
	u16 bp; //pointer in stream for buff
	u16 bplen; //buff len
} SString;

typedef struct Token {
	u16 token;
	SString semInfo;
	union {
		u32 nvalue;
		float fvalue;
	} number;
} Token;

typedef struct LexState {
	u08 current;
	u08 linenumber;
	u08 lastline;
	Token t;
	Token lookahead;
	u08 decpoint;
	u08* z; //input stream
	u16 zp; //current pointer in z input stream
	SString buff;
} LexState;

//lexer function export
u16 llex(LexState* ls, Token* t);
void next(LexState* ls);
void setInput(LexState* ls, u08* stream);
ERROR_CODE getLastULexError();

//parser structures definition
typedef enum {
	EXP_VARIABLE,
	EXP_NUMBER,
	EXP_STRING,
	EXP_BOOLEAN,
	EXP_FUNCTION,
	EXP_FUNCTION_CALL,
	EXP_ADD,
	EXP_SUB,
	EXP_DIV,
	EXP_MUL,
	EXP_LIST,
	EXP_SET,
	EXP_CHUNK,
	EXP_RETURN,
	EXP_BREAK,
} EXPR_TYPE;



typedef struct uExpression uExpression;
typedef struct uNode uNode;

struct uNode {
	uExpression* expr;
	uNode* next;
};

struct uExpression {
	EXPR_TYPE type;
	float fvalue;
	SString name;
	uExpression* arg0;
	uExpression* arg1;
	uExpression* arg2;
	uNode* first;
};

//---------------- new structures
typedef struct uBlock uBlock;
typedef struct uValNode uValNode;
typedef struct uInstruction uInstruction;

typedef enum VAL_TYPE {
	VAL_STRING,
	VAL_NUMBER,
	VAL_BOOLEAN,
	VAL_NIL,
	VAL_CLOSURE
} VAL_TYPE ;

struct uInstruction {
	u08 opCode;
	u08 A;
	u16 B;
	u16 C;
	uInstruction* next;
};

struct uValNode {
	VAL_TYPE type;
	BOOL isLocal;
	BOOL boolean;
	SString name;
	float value;
	uBlock* function;
	uValNode* next;
};

struct uBlock {
	uValNode* globals;
	uValNode* locals;
	uValNode* functions;
	uValNode* constants;
	uValNode* regs[32];
	u08 regN;
	u08 regMaxN;
	uInstruction* instructions;
	u16 instructionCount;
};

//parser function for AST building
uExpression* makeExpr(EXPR_TYPE type, uExpression* arg0, uExpression* arg1, uExpression* arg2, u08* code);
uExpression* makeConst(EXPR_TYPE type, float fval, SString* strval, u08* code);
uExpression* makeVariable(EXPR_TYPE type, SString* strval, u08* code);
uExpression* addList(uExpression* list, uExpression* expr, u08* code);
uExpression* makeList(uExpression* firstExpr, u08* code);
void makeDump(uExpression* chunk, u08* code);

/*
//parser function export
void *ParseAlloc(void *(*mallocProc)(size_t));
void Parse(
  void *yyp,                   // The parser 
  int yymajor,                 // The major token code number 
  ParseTOKENTYPE yyminor       // The value for the token 
  ParseARG_PDECL               // Optional %extra_argument parameter 
);
void ParseFree(
  void *p,                    // The parser to be deleted 
  void (*freeProc)(void*)     // Function used to reclaim memory 
);
*/
#endif