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
	lu16 type;
	lu08 keyword[10];
} reserved;


typedef struct SString { //String structure - store only pointer to existing stream position and string lengh. Used to save RAM.
	lu08 bempty; //boolean if buff is emtry and not saved
	lu16 bp; //pointer in stream for buff
	lu16 bplen; //buff len
} SString;

typedef struct Token {
	lu16 token;
	SString semInfo;
	union {
		lu32 nvalue;
		float fvalue;
	} number;
} Token;

typedef struct LexState {
	lu08 current;
	lu08 linenumber;
	lu08 lastline;
	Token t;
	Token lookahead;
	lu08 decpoint;
	lu08* z; //input stream
	lu16 zp; //current pointer in z input stream
	SString buff;
} LexState;

//lexer function export
lu16 llex(LexState* ls, Token* t);
void next(LexState* ls);
void setInput(LexState* ls, lu08* stream);
ERROR_CODE getLastULexError();

#endif

