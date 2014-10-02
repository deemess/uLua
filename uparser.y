%fallback  OPEN LPAREN .

%include {
#include <stdio.h>
#include "basetypes.h"
#include "ucodegen.h"
#include "uparser.h"
#include "ulexer.h"
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
%type functioncall {Register*}
%type stat		{Instruction*}
%type binding	{Instruction*}

%syntax_error {
  printf ("Syntax error!\n");
}

%start_symbol  chunk

chunk ::= block . {
	printf("P_CHUNK\n");
}

semi ::= SEMICOL . {
	printf("P_SEMI ------------------\n");
}
semi ::= . {
	printf("P_SEMI ------------------\n");
}

block ::= scope statlist . {
	printf("P_BLOCK_STATLIST\n");
}
block ::= scope statlist laststat semi . {
	printf("P_BLOCK_STATLIST_LASTSTAT\n");
}
ublock ::= block UNTIL exp . {
	printf("P_UBLOCK\n");
}

scope ::= . {
	printf("P_SCOPE_EMPTY\n");
}
scope ::= scope statlist binding semi. {
	printf("P_SCOPE_STATLIST\n");
}
           
statlist   ::= . {
	printf("P_STATLIST_EMPTY\n");
}
statlist   ::= statlist stat semi . {
	printf("P_STATLIST_ADD_STAT\n");
}

stat ::= DO block END .
stat ::= WHILE exp DO block END .
stat ::= repetition DO block END .
stat ::= REPEAT ublock .
stat ::= IF conds END .
stat ::= FUNCTION funcname params block END . 
stat(A) ::= setlist(B) SET explist1(C) . {
	A = statSET(f, B, C, FALSE);
	printf("P_STAT_SET\n");
}
stat ::= functioncall(B) .  {
	freeRegister(B);
	printf("P_STAT_FCALL\n");
}

repetition ::= FOR NAME SET explist23 .
repetition ::= FOR namelist IN explist1 .

conds      ::= condlist .
conds      ::= condlist ELSE block .
condlist   ::= cond .
condlist   ::= condlist ELSEIF cond .
cond       ::= exp THEN block .

laststat ::= BREAK . 
laststat ::= RETURN . 
laststat ::= RETURN explist1 . 

binding    ::= LOCAL namelist . {
	printf("P_LOCAL\n");
}
binding(A)    ::= LOCAL namelist(B) SET explist1(C) .{
	A = statSET(f, B, C, TRUE);
	printf("P_LOCAL_SET\n");
}
binding    ::= LOCAL FUNCTION NAME(B) params block END .{
	printf("P_LOCAL_FUNCTION\n");
	pushConstString(f, &B.semInfo);
}

funcname   ::= dottedname . 
funcname   ::= dottedname COLON NAME .

dottedname ::= NAME . 
dottedname ::= dottedname DOT NAME . 

namelist(A)   ::= NAME(B) . {
	Constant* c = pushVarName(f, &B.semInfo);
	A = getVarRegister(f,c);
	printf("P_NAMELIST_NAME\n");
}
namelist(A)   ::= namelist COMMA NAME(B) . {
	Constant* c = pushVarName(f, &B.semInfo);
	A = getVarRegister(f,c);
	printf("P_NAMELIST_COMMA_NAME\n");
}

explist1(A)	::= exp(B) . {
	A = B;
	printf("P_EXPLIST_EXP\n");
}
explist1(A)	::= explist1 COMMA exp(B) . {
	A = B;
	printf("P_EXPLIST_ADD_EXP\n");
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

exp        ::= NIL|TRUE|FALSE|DOTS .
exp(A)        ::= NUMBER(B) . {
	Constant* c;
	Register* r;

	printf("P_EXP_NUMBER\n");
	c = pushConstNumber(f, B.number.fvalue);
	r = getFreeRegister(f);
	r->consthold = TRUE;
	r->constpreloaded = FALSE;
	r->constnum = c->num;
	A = r;
}
exp(A)        ::= STRING(B) . {
	Constant* c;
	Register* r;

	printf("P_EXP_STRING\n");
	c = pushConstString(f, &B.semInfo);
	r = getFreeRegister(f);
	r->consthold = TRUE;
	r->constpreloaded = FALSE;
	r->constnum = c->num;
	A = r;
}
exp        ::= function . {
	printf("P_EXP_FUNCTION\n");
}
exp(A)        ::= prefixexp(B) . {
	printf("P_EXP_PREFIXEXP\n");
	A = B;
}
exp        ::= tableconstructor .
exp        ::= NOT|HASH|MINUS exp .
exp        ::= exp OR|AND exp .
exp(A)     ::= exp(B) L(D)|LE|G|GE|EQ|NE exp(C) . {
	A = doLogic(f,B,C,D);
	printf("P_EXP_LOGIC\n");
}
exp        ::= exp CONCAT exp .
exp(A)	   ::= exp(B) PLUS(E)|MINUS|TIMES|DIVIDE|MOD|POW exp(C) . {
	A = doMath(f,B,C,&E);
	printf("P_EXP_MATH\n");
}

setlist(A) ::= var(B) . {
	printf("P_SETLIST_VAR\n");
	A = B;
}
setlist(A) ::= setlist COMMA var(B) . {
	printf("P_SETLIST_ADD_VAR\n");
	A = B;
}

var(A) ::= NAME(B) . {
	Constant* c;

	printf("P_VAR_NAME\n");
	c = pushVarName(f, &B.semInfo);
	A = getVarRegister(f,c);
}
var ::= prefixexp SLPAREN exp SRPAREN . {
	printf("P_PREFEXP_SPAREN_EXP\n");
}
var ::= prefixexp DOT NAME . {
	printf("P_PREFEXP_DOT_NAME\n");
}

prefixexp(A)  ::= var(B) . {
	printf("P_PREFEXP_VAR\n");
	A = B;
}
prefixexp  ::= functioncall .  {
	printf("P_PREFEXP_FCALL\n");
}
prefixexp  ::= OPEN exp RPAREN . {
	printf("P_PREFEXP_EXP\n");
}

functioncall(A) ::= prefixexp(B) args(C) . {
	A = functionCALL(f, B, C);
	printf("P_FCALL_ARGS\n");
}
functioncall ::= prefixexp COLON NAME args . {
	printf("P_FCALL_NAME_ARGS\n");
}

args(A)        ::= LPAREN RPAREN . {
	A = NULL;
	printf("P_ARGS_EMPTY\n");
}
args(A)        ::= LPAREN explist1(B) RPAREN . {
	A = B;
	printf("P_ARGS_EXPLIST\n");
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
	printf("P_ARGS_STRING\n");
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
