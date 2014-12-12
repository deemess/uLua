%fallback  OPEN LPAREN .

%include {
#include <stdio.h>
#include "basetypes.h"
#include "ucodegen.h"
#include "uparser.h"
#include "ulexer.h"
#include "llerror.h"
#include "vmconfig.h"
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
	f->parsed = TRUE;
}

%syntax_error {
  f->error_code = E_SYNTAX_ERROR;
}

%stack_overflow {
    f->error_code = E_STACKOVERFLOW;
}

%start_symbol  chunk

chunk ::= block . {
	doReturn(f);
	DPRINTF("P_CHUNK\n");
}

semi ::= SEMICOL . {
	DPRINTF("P_SEMI ------------------\n");
}
semi ::= . {
	DPRINTF("P_SEMI ------------------\n");
}

block(A) ::= scope statlist(B) . {
	A = B;
	DPRINTF("P_BLOCK_STATLIST\n");
}
block(A) ::= scope statlist(B) laststat semi . {
	A = B;
	DPRINTF("P_BLOCK_STATLIST_LASTSTAT\n");
}
ublock ::= block UNTIL exp . {
	//doReturn(f);
	DPRINTF("P_UBLOCK\n");
}

scope ::= . {
	DPRINTF("P_SCOPE_EMPTY\n");
}
scope ::= scope statlist binding semi. {
	DPRINTF("P_SCOPE_STATLIST\n");
}

statlist(A) ::= . {
	A = NULL;
	f->currentStat = NULL;
	DPRINTF("P_STATLIST_EMPTY\n");
}
statlist(A) ::= statlist(B) stat semi . {
	if(B == NULL) { 
		A = f->currentStat; //save only pointer to first statement
	} else {
		A = B;
	}
	f->currentStat = NULL;
	DPRINTF("P_STATLIST_ADD_STAT\n");
}

stat(A) ::= DO block(B) END . {
	A = B;
	DPRINTF("P_STAT_BLOCK\n");
}
stat(A) ::= WHILE exp(B) DO block(C) END . {
	A = statWHILE(f, B, C);
	DPRINTF("P_STAT_WHILE\n");
}
stat ::= repetition DO block END .
stat ::= REPEAT ublock .
stat(A) ::= IF conds(B) END . {
	A = B;
	DPRINTF("P_STAT_IF\n");
}
stat ::= FUNCTION funcname params block END . 
stat(A) ::= setlist(B) SET explist1(C) . {
	A = statSET(f, B, C, FALSE);
	DPRINTF("P_STAT_SET\n");
}
stat(A) ::= functioncall(B) .  {
	A = B;
	DPRINTF("P_STAT_FCALL\n");
}

repetition ::= FOR NAME SET explist23 .
repetition ::= FOR namelist IN explist1 .

conds(A)      ::= condlist(B) . {
	A = B;
	DPRINTF("P_CONDS_CONDLIST\n");
}
conds(A)      ::= condlist(B) ELSE block(C) . {
	A = statELSE(f, B, C);
	DPRINTF("P_CONDS_CONDLIST_ELSE_BLOCK\n");
}
condlist(A)   ::= cond(B) . {
	A = B;
	DPRINTF("P_CONDLIST_COND\n");
}
condlist(A)   ::= condlist(B) ELSEIF cond(C) . {
	A = statELSEIF(f, B, C);
	DPRINTF("P_CONDLIST_CONDLIST_ELSEIF_COND\n");
}
cond(A)    ::= exp(B) THEN block(C) . {
	A = statTHEN(f, B, C);
	DPRINTF("P_COND_EXP_THEN_BLOCK\n");
}

laststat ::= BREAK . 
laststat ::= RETURN . 
laststat ::= RETURN explist1 . 

binding    ::= LOCAL namelist . {
	DPRINTF("P_LOCAL\n");
}
binding(A)    ::= LOCAL namelist(B) SET explist1(C) .{
	A = statSET(f, B, C, TRUE);
	DPRINTF("P_LOCAL_SET\n");
}
binding    ::= LOCAL FUNCTION NAME(B) params block END .{
	DPRINTF("P_LOCAL_FUNCTION\n");
	pushConstString(f, &B.semInfo);
}

funcname   ::= dottedname . 
funcname   ::= dottedname COLON NAME .

dottedname ::= NAME . 
dottedname ::= dottedname DOT NAME . 

namelist(A)   ::= NAME(B) . {
	Constant* c = pushVarName(f, &B.semInfo);
	A = getVarRegister(f,c);
	DPRINTF("P_NAMELIST_NAME\n");
}
namelist(A)   ::= namelist COMMA NAME(B) . {
	Constant* c = pushVarName(f, &B.semInfo);
	A = getVarRegister(f,c);
	DPRINTF("P_NAMELIST_COMMA_NAME\n");
}

explist1(A)	::= exp(B) . {
	A = B;
	DPRINTF("P_EXPLIST_EXP\n");
}
explist1(A)	::= explist1 COMMA exp(B) . {
	A = B;
	DPRINTF("P_EXPLIST_ADD_EXP\n");
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
	A = doNil(f);
	if(A->exprStart == NULL) A->exprStart = f->currentStat;
}
exp(A)        ::= TRUE(B)|FALSE . {
	A = doBoolean(f, &B);
	if(A->exprStart == NULL) A->exprStart = f->currentStat;
}
exp        ::= DOTS .
exp(A)        ::= NUMBER(B) . {
	Constant* c;
	Register* r;

	DPRINTF("P_EXP_NUMBER\n");
	c = pushConstNumber(f, B.number.fvalue);
	r = getFreeRegister(f);
	r->consthold = TRUE;
	r->constpreloaded = FALSE;
	r->constnum = c->num;
	A = r;
	if(A->exprStart == NULL) A->exprStart = f->currentStat;
}
exp(A)        ::= STRING(B) . {
	Constant* c;
	Register* r;

	DPRINTF("P_EXP_STRING\n");
	c = pushConstString(f, &B.semInfo);
	r = getFreeRegister(f);
	r->consthold = TRUE;
	r->constpreloaded = FALSE;
	r->constnum = c->num;
	A = r;
	if(A->exprStart == NULL) A->exprStart = f->currentStat;
}
exp        ::= function . {
	DPRINTF("P_EXP_FUNCTION\n");
}
exp(A)        ::= prefixexp(B) . {
	DPRINTF("P_EXP_PREFIXEXP\n");
	A = B;
}
exp        ::= tableconstructor .
exp        ::= NOT|HASH|MINUS exp .
exp(A)        ::= exp(B) OR(D)|AND exp(C) . {
	A = doLogic(f,B,C,&D);
	if(A->exprStart == NULL) A->exprStart = f->currentStat;
	DPRINTF("P_EXP_LOGIC\n");
}
exp(A)     ::= exp(B) L(D)|LE|G|GE|EQ|NE exp(C) . {
	A = doCompare(f,B,C,&D);
	if(A->exprStart == NULL) A->exprStart = f->currentStat;
	DPRINTF("P_EXP_COMPARE\n");
}
exp        ::= exp CONCAT exp .
exp(A)	   ::= exp(B) PLUS(E)|MINUS|TIMES|DIVIDE|MOD|POW exp(C) . {
	A = doMath(f,B,C,&E);
	if(A->exprStart == NULL) A->exprStart = f->currentStat;
	DPRINTF("P_EXP_MATH\n");
}

setlist(A) ::= var(B) . {
	A = B;
	DPRINTF("P_SETLIST_VAR\n");
}
setlist(A) ::= setlist COMMA var(B) . {
	A = B;
	DPRINTF("P_SETLIST_ADD_VAR\n");
}

var(A) ::= NAME(B) . {
	Constant* c;

	DPRINTF("P_VAR_NAME\n");
	c = pushVarName(f, &B.semInfo);
	A = getVarRegister(f,c);
}
var ::= prefixexp SLPAREN exp SRPAREN . {
	DPRINTF("P_PREFEXP_SPAREN_EXP\n");
}
var ::= prefixexp DOT NAME . {
	DPRINTF("P_PREFEXP_DOT_NAME\n");
}

prefixexp(A)  ::= var(B) . {
	DPRINTF("P_PREFEXP_VAR\n");
	A = B;
}
prefixexp  ::= functioncall .  {
	DPRINTF("P_PREFEXP_FCALL\n");
}
prefixexp(A) ::= OPEN exp(B) RPAREN . {
	A = B;
	DPRINTF("P_PREFEXP_EXP\n");
}

functioncall(A) ::= prefixexp(B) args(C) . {
	A = functionCALL(f, B, C);
	DPRINTF("P_FCALL_ARGS\n");
}
functioncall ::= prefixexp COLON NAME args . {
	DPRINTF("P_FCALL_NAME_ARGS\n");
}

args(A)        ::= LPAREN RPAREN . {
	A = NULL;
	DPRINTF("P_ARGS_EMPTY\n");
}
args(A)        ::= LPAREN explist1(B) RPAREN . {
	A = B;
	DPRINTF("P_ARGS_EXPLIST\n");
}
args        ::= tableconstructor .
args(A)        ::= STRING(B) . {
	Constant* c;
	Register* r;

	c = pushConstString(f, &B.semInfo);
	r = getFreeRegister(f);
	r->consthold = TRUE;
	r->constpreloaded = FALSE;
	r->constnum = c->num;
	A = r;
	DPRINTF("P_ARGS_STRING\n");
}
function    ::= FUNCTION params block END . 
params      ::= LPAREN parlist LPAREN . 
parlist     ::= . 
parlist     ::= namelist . 
parlist     ::= DOTS .
parlist     ::= namelist COMMA DOTS .

tableconstructor ::= LBRACE RBRACE .
tableconstructor ::= LBRACE fieldlist RBRACE .
tableconstructor ::= LBRACE fieldlist COMMA|SEMICOL RBRACE .

fieldlist   ::= field .
fieldlist   ::= fieldlist COMMA|SEMICOL field .
            
field       ::= exp .
field       ::= NAME SET exp .
field       ::= SLPAREN exp SRPAREN SET exp .
