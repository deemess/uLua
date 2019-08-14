%fallback  OPEN LPAREN .

%include {
#include <stdio.h>
#include "ulua_core/basetypes.h"
#include "ulua_core/ucodegen.h"
#include "ulua_core/uparser.h"
#include "ulua_core/ulexer.h"
#include "ulua_core/llerror.h"
#include "ulua_core/vmconfig.h"
}

%stack_size 100
%extra_argument { Function* f }

%token_type {Token}
%token_prefix TK_

%type var		{Register*}
%type exp		{Register*}
%type prefixexp {Register*}
%type setlist	{Register*}
%type explist1	{Register*}
%type namelist	{Register*}
%type args		{Register*}
%type tableconstructor  {Register*}
%type functioncall {Instruction*}
%type stat		{Instruction*}
%type binding	{Instruction*}
%type block		{Instruction*}
%type cond		{Instruction*}
%type conds		{Instruction*}
%type condlist	{Instruction*}
%type statlist	{Instruction*}
%type laststat	{Instruction*}

%parse_accept {
	f->parsed = ULUA_TRUE;
}

%syntax_error {
  f->error_code = E_SYNTAX_ERROR;
}

%stack_overflow {
    f->error_code = E_STACKOVERFLOW;
}

%start_symbol  chunk

chunk ::= block . {
    DPRINTF("P_CHUNK\n");
	doReturn(f);
}

semi ::= SEMICOL . {
	DPRINTF("P_SEMI ------------------\n");
}
semi ::= . {
	DPRINTF("P_SEMI ------------------\n");
}

block(A) ::= scope statlist(B) . {
    DPRINTF("P_BLOCK_STATLIST\n");
	A = B;
    unloadRegisters(f);
}
block(A) ::= scope statlist(B) laststat semi . {
    DPRINTF("P_BLOCK_STATLIST_LASTSTAT\n");
	A = B;
    unloadRegisters(f);
}
ublock ::= block UNTIL exp . {
	DPRINTF("P_UBLOCK\n");
}

scope ::= . {
	DPRINTF("P_SCOPE_EMPTY\n");
}
scope ::= scope statlist binding semi. {
	DPRINTF("P_SCOPE_STATLIST\n");
}

statlist(A) ::= . {
    DPRINTF("P_STATLIST_EMPTY\n");
	A = ULUA_NULL;
	f->currentStat = ULUA_NULL;
}
statlist(A) ::= statlist(B) stat semi . {
    DPRINTF("P_STATLIST_ADD_STAT\n");
	if(B == ULUA_NULL) {
		A = f->currentStat; //save only pointer to first statement
	} else {
		A = B;
	}
	f->currentStat = ULUA_NULL;
}

stat(A) ::= DO block(B) END . {
    DPRINTF("P_STAT_BLOCK\n");
	A = B;
}
stat(A) ::= WHILE exp(B) DO block(C) END . {
    DPRINTF("P_STAT_WHILE\n");
	A = statWHILE(f, B, C);
}
stat ::= repetition DO block END .
stat ::= REPEAT ublock .
stat(A) ::= IF conds(B) END . {
    DPRINTF("P_STAT_IF\n");
	A = B;
}
stat ::= FUNCTION funcname params block END . {
    DPRINTF("P_STAT_FUNCTION\n");
}
stat(A) ::= setlist(B) SET explist1(C) . {
	DPRINTF("P_STAT_SET\n");
	A = statSETList(f, B, C, ULUA_FALSE);
}
stat(A) ::= functioncall(B) .  {
	DPRINTF("P_STAT_FCALL\n");
	A = B;
}

repetition ::= FOR NAME SET explist23 .
repetition ::= FOR namelist IN explist1 .

conds(A)      ::= condlist(B) . {
    DPRINTF("P_CONDS_CONDLIST\n");
	A = B;
}
conds(A)      ::= condlist(B) ELSE block(C) . {
    DPRINTF("P_CONDS_CONDLIST_ELSE_BLOCK\n");
	A = statELSE(f, B, C);
}
condlist(A)   ::= cond(B) . {
    DPRINTF("P_CONDLIST_COND\n");
	A = B;
}
condlist(A)   ::= condlist(B) ELSEIF cond(C) . {
    DPRINTF("P_CONDLIST_CONDLIST_ELSEIF_COND\n");
	A = statELSEIF(f, B, C);
}
cond(A)    ::= exp(B) THEN block(C) . {
    DPRINTF("P_COND_EXP_THEN_BLOCK\n");
	A = statTHEN(f, B, C);
}

laststat ::= BREAK . 
laststat ::= RETURN . 
laststat ::= RETURN explist1 . 

binding    ::= LOCAL namelist . {
	DPRINTF("P_LOCAL\n");
}
binding(A)    ::= LOCAL namelist(B) SET explist1(C) .{
    DPRINTF("P_LOCAL_SET\n");
	A = statSET(f, B, C, ULUA_TRUE);
}
binding    ::= LOCAL FUNCTION NAME(B) params block END .{
	DPRINTF("P_LOCAL_FUNCTION\n");
	pushConstString(f, &B.semInfo);
}

funcname   ::= dottedname . {
	DPRINTF("P_FUNCNAME_DOTTEDNAME\n");
}
funcname   ::= dottedname COLON NAME . {
	DPRINTF("P_FUNCNAME_DOTTEDNAME_COLON_NAME\n");
}

dottedname ::= NAME . {
	DPRINTF("P_DOTTEDNAME_NAME\n");
}
dottedname ::= dottedname DOT NAME . {
	DPRINTF("P_DOTTEDNAME_DOTTEDNAME_DOT_NAME\n");
}

namelist(A)   ::= NAME(B) . {
	Constant* c;
    	DPRINTF("P_NAMELIST_NAME\n");
	c = pushVarName(f, &B.semInfo);
	A = getVarRegister(f,c);
}
namelist(A)   ::= namelist COMMA NAME(B) . {
	Constant* c;
    	DPRINTF("P_NAMELIST_COMMA_NAME\n");
	c = pushVarName(f, &B.semInfo);
	A = getVarRegister(f,c);
}

explist1(A)	::= exp(B) . {
    DPRINTF("P_EXPLIST_EXP\n");
	A = B;
}
explist1(A)	::= explist1(B) COMMA exp(C) . {
	DPRINTF("P_EXPLIST_ADD_EXP\n");
	A = addRegisterList(B, C);
}
explist23  ::= exp COMMA exp .
explist23  ::= exp COMMA exp COMMA exp .

%left      OR .
%left      AND .
%left      L LE G GE EQ NE .
%right     CONCAT .
%left      PLUS MINUS .
%left      TIMES DIVIDE MOD .
%right     NOT HASH .
%right     POW .

exp(A)        ::= NIL . {
    DPRINTF("P_EXP_NIL\n");
	A = doNil(f);
	if(A->exprStart == ULUA_NULL) A->exprStart = f->currentStat;
}
exp(A)        ::= TRUE(B)|FALSE . {
	DPRINTF("P_EXP_BOOLEAN %.*s\n", B.semInfo.bplen, &f->code[B.semInfo.bp]);
	A = doBoolean(f, &B);
	if(A->exprStart == ULUA_NULL) A->exprStart = f->currentStat;
}
exp        ::= DOTS .
exp(A)        ::= NUMBER(B) . {
	Constant* c;
	Register* r;
	DPRINTF("P_EXP_NUMBER %.*s\n", B.semInfo.bplen, &f->code[B.semInfo.bp]);

	c = pushConstNumber(f, B.number.fvalue);
	r = getFreeRegister(f);
	r->consthold = ULUA_TRUE;
	r->constnum = c->num;
	A = r;
	if(A->exprStart == ULUA_NULL) A->exprStart = f->currentStat;
}
exp(A)        ::= STRING(B) . {
	Constant* c;
	Register* r;
	DPRINTF("P_EXP_STRING %.*s\n", B.semInfo.bplen, &f->code[B.semInfo.bp]);

	c = pushConstString(f, &B.semInfo);
	r = getFreeRegister(f);
	r->consthold = ULUA_TRUE;
	r->constnum = c->num;
	A = r;
	if(A->exprStart == ULUA_NULL) A->exprStart = f->currentStat;
}
exp        ::= function . {
	DPRINTF("P_EXP_FUNCTION\n");
}
exp(A)        ::= prefixexp(B) . {
	DPRINTF("P_EXP_PREFIXEXP\n");
	A = B;
}
exp(A)        ::= tableconstructor(B) . {
	DPRINTF("P_EXP_TABLECONSTRUCTOR\n");
	A = B;
}
exp        ::= HASH exp .
exp(A)     ::= NOT(B)|MINUS exp(C) . {
    DPRINTF("P_EXP_NOT_MINUS\n");
	A = doNot(f, C, &B);
}
exp(A)        ::= exp(B) OR(D)|AND exp(C) . {
	DPRINTF("P_EXP_LOGIC %.*s\n", D.semInfo.bplen, &f->code[D.semInfo.bp]);
	A = doLogic(f,B,C,&D);
	if(A->exprStart == ULUA_NULL) A->exprStart = f->currentStat;
}
exp(A)     ::= exp(B) L(D)|LE|G|GE|EQ|NE exp(C) . {
	DPRINTF("P_EXP_COMPARE %.*s\n", D.semInfo.bplen, &f->code[D.semInfo.bp]);
	A = doCompare(f,B,C,&D);
	if(A->exprStart == ULUA_NULL) A->exprStart = f->currentStat;
}
exp        ::= exp CONCAT exp .
exp(A)	   ::= exp(B) PLUS(E)|MINUS|TIMES|DIVIDE|MOD|POW exp(C) . {
	DPRINTF("P_EXP_MATH %.*s\n", E.semInfo.bplen, &f->code[E.semInfo.bp]);
	A = doMath(f,B,C,&E);
	if(A->exprStart == ULUA_NULL) A->exprStart = f->currentStat;
}

setlist(A) ::= var(B) . {
    DPRINTF("P_SETLIST_VAR\n");
	A = B;
}
setlist(A) ::= setlist(B) COMMA var(C) . {
	DPRINTF("P_SETLIST_COMMA_VAR\n");
	A = addRegisterList(B, C);
}

var(A) ::= NAME(B) . {
	Constant* c;
	DPRINTF("P_VAR_NAME %.*s\n", B.semInfo.bplen, &f->code[B.semInfo.bp]);

	c = pushVarName(f, &B.semInfo);
	A = getVarRegister(f,c);
	if(A->exprStart == ULUA_NULL) A->exprStart = f->currentStat;
}
var ::= prefixexp SLPAREN exp SRPAREN . {
	DPRINTF("P_PREFEXP_SLPAREN_EXP_SRPAREN\n");
}
var ::= prefixexp DOT NAME(C) . {
	DPRINTF("P_PREFEXP_DOT_NAME %.*s\n", C.semInfo.bplen, &f->code[C.semInfo.bp]);

}

prefixexp(A)  ::= var(B) . {
	DPRINTF("P_PREFEXP_VAR\n");
	A = B;
}
prefixexp  ::= functioncall .  {
	DPRINTF("P_PREFEXP_FCALL\n");
}
prefixexp(A) ::= OPEN exp(B) RPAREN . {
    DPRINTF("P_PREFEXP_EXP\n");
	A = B;
}

functioncall(A) ::= prefixexp(B) args(C) . {
    DPRINTF("P_FCALL_ARGS\n");
	A = functionCALL(f, B, C);
}
functioncall ::= prefixexp COLON NAME(C) args . {
	DPRINTF("P_FCALL_NAME_ARGS %.*s\n", C.semInfo.bplen, &f->code[C.semInfo.bp]);
}

args(A)        ::= LPAREN RPAREN . {
    DPRINTF("P_ARGS_EMPTY\n");
	A = ULUA_NULL;
}
args(A)        ::= LPAREN explist1(B) RPAREN . {
    DPRINTF("P_ARGS_EXPLIST\n");
	A = B;
}
args(A)        ::= tableconstructor(B) . {
    DPRINTF("P_ARGS_TABLECONSTRUCTOR\n");
    A = B;
}
args(A)        ::= STRING(B) . {
	Constant* c;
	Register* r;
	DPRINTF("P_ARGS_STRING %.*s\n", B.semInfo.bplen, &f->code[B.semInfo.bp]);

	c = pushConstString(f, &B.semInfo);
	r = getFreeRegister(f);
	r->consthold = ULUA_TRUE;
	r->constnum = c->num;
	A = r;
}
function    ::= FUNCTION params block END . 
params      ::= LPAREN parlist LPAREN . 
parlist     ::= . 
parlist     ::= namelist . 
parlist     ::= DOTS .
parlist     ::= namelist COMMA DOTS .

tableconstructor(A) ::= LBRACE RBRACE . {
    	DPRINTF("P_TABLECONSTRUCTOR_LBRACE_RBRACE\n");

	A = doTable(f);
}
tableconstructor ::= LBRACE fieldlist RBRACE .
tableconstructor ::= LBRACE fieldlist COMMA|SEMICOL RBRACE .

fieldlist   ::= field .
fieldlist   ::= fieldlist COMMA|SEMICOL field .
            
field       ::= exp .
field       ::= NAME SET exp .
field       ::= SLPAREN exp SRPAREN SET exp .
