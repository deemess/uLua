%include {
#include <stdio.h>
#include "basetypes.h"
#include "ulexer.h"
#include "uparser.h"
}

%extra_argument { u08* code }

%token_type {Token}
%token_prefix TK_

%type exp		{uExpression*}
%type var		{uExpression*}
%type setlist	{uExpression*}
%type explist1	{uExpression*}
%type stat		{uExpression*}
%type function	{uExpression*}
%type params	{uExpression*}
%type parlist	{uExpression*}
%type namelist	{uExpression*}
%type statlist	{uExpression*}
%type block		{uExpression*}
%type chunk		{uExpression*}
%type dottedname {uExpression*}
%type funcname {uExpression*}
%type laststat	{uExpression*}
%type prefixexp {uExpression*}
%type functioncall {uExpression*}
%type args		{uExpression*}

%syntax_error {
  printf ("Syntax error!\n");
}

%start_symbol  chunk

chunk ::= block(B) . {
	makeDump(B, code);
}

semi ::= SEMICOL .
semi ::= .

block(A) ::= scope statlist(B) . {
	A = makeExpr(EXP_CHUNK, B, NULL, NULL, code);
}
block(A) ::= scope statlist(B) laststat(C) semi . {
	if(C != NULL)
		A = makeExpr(EXP_CHUNK, addList(B,C, code), NULL, NULL, code);
	else
		A = makeExpr(EXP_CHUNK, B, NULL, NULL, code);
}
ublock ::= block UNTIL exp .

scope ::= .
scope ::= scope statlist binding semi.
           
statlist(A)   ::= . {
	A = makeList(NULL, code);
}
statlist(A)   ::= statlist(B) stat(C) semi . {
	if(C != NULL) {
		A = addList(B, C, code);
	} else {
		A = B;
	}
}

stat ::= DO block END .
stat ::= WHILE exp DO block END .
stat ::= repetition DO block END .
stat ::= REPEAT ublock .
stat ::= IF conds END .
stat(A) ::= FUNCTION funcname(B) params(C) block(D) END . {
	A = makeExpr(EXP_FUNCTION, B, C, D, code);
}
stat(A) ::= setlist(B) SET explist1(C) . {
	A = makeExpr(EXP_SET, B, C, NULL, code);
}
stat(A) ::= functioncall(B) . {
	A = B;
}

repetition ::= FOR NAME SET explist23 .
repetition ::= FOR namelist IN explist1 .

conds      ::= condlist .
conds      ::= condlist ELSE block .
condlist   ::= cond .
condlist   ::= condlist ELSEIF cond .
cond       ::= exp THEN block .

laststat(A) ::= BREAK . {
	A = makeExpr(EXP_BREAK, NULL, NULL, NULL, code);
}
laststat(A) ::= RETURN . {
	A = makeExpr(EXP_RETURN, NULL, NULL, NULL, code);
}
laststat(A) ::= RETURN explist1(B) . {
	A = makeExpr(EXP_RETURN, B, NULL, NULL, code);
}

binding    ::= LOCAL namelist .
binding    ::= LOCAL namelist SET explist1 .
binding    ::= LOCAL FUNCTION NAME params block END .

funcname(A)   ::= dottedname(B) . {
	A = B;
}
funcname   ::= dottedname COLON NAME .

dottedname(A) ::= NAME(B) . {
	A = makeConst(EXP_STRING, 0, &B.semInfo, code);
}
dottedname(A) ::= dottedname(B) DOT NAME(C) . {
	A = makeExpr(EXP_LIST, B, makeConst(EXP_STRING, 0, &C.semInfo, code), NULL, code);
}

namelist(A)   ::= NAME(B) .{
	A = makeList(makeVariable(EXP_VARIABLE, &B.semInfo, code), code);
}
namelist(A)   ::= namelist(B) COMMA NAME(C) . {
	A = addList(B, makeVariable(EXP_VARIABLE, &C.semInfo, code), code);
}

explist1(A) ::= exp(B) . {
	A = makeList(B, code);
}
explist1(A)   ::= explist1(B) COMMA exp(C) . {
	A = addList(B, C, code);
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
	A = makeConst(EXP_NUMBER, B.number.fvalue, NULL, code);
}
exp(A)        ::= STRING(B) . {
	A = makeConst(EXP_STRING, 0, &B.semInfo, code);
}
exp(A)        ::= function(B) . {
	A = B;
}
exp(A)        ::= prefixexp(B) . {
	A = B;
}
exp        ::= tableconstructor .
exp        ::= NOT|HASH|MINUS exp .
exp        ::= exp OR exp .
exp        ::= exp AND exp .
exp        ::= exp L|LE|G|GE|EQ|NE exp .
exp        ::= exp CONCAT exp .
exp(A) ::= exp(B) PLUS exp(C) .{
	A = makeExpr(EXP_ADD, B,C, NULL, code);
}
exp(A) ::= exp(B) MINUS exp(C) .{
	A = makeExpr(EXP_SUB, B,C, NULL, code);
}
exp(A) ::= exp(B) TIMES exp(C) .{
	A = makeExpr(EXP_MUL, B,C, NULL, code);
}
exp(A) ::= exp(B) DIVIDE exp(C) .{
	A = makeExpr(EXP_DIV, B,C, NULL, code);
}
exp        ::= exp MOD exp .
exp        ::= exp POW exp .

setlist(A) ::= var(B) . {
	A = makeList(B, code);
}
setlist(A) ::= setlist(B) COMMA var(C) . {
	A = addList(B, C, code);
}

var(A) ::= NAME(B) . {
	A = makeVariable(EXP_VARIABLE, &B.semInfo, code);
}
var        ::= prefixexp SLPAREN exp SRPAREN .
var        ::= prefixexp DOT NAME .

prefixexp(A)  ::= var(B) . {
	A = B;
}
prefixexp(A)  ::= functioncall(B) . {
	A = B;
}
prefixexp  ::= LPAREN exp RPAREN .

functioncall(A) ::= prefixexp(B) args(C) . {
	A = makeExpr(EXP_FUNCTION_CALL, B, C, NULL, code);
}
functioncall ::= prefixexp COLON NAME args .

args(A)        ::= LPAREN RPAREN . {
	A = NULL;
}
args(A)        ::= LPAREN explist1(B) RPAREN . {
	A = B;
}
args        ::= tableconstructor .
args(A)        ::= STRING(B) . {
	A = makeConst(EXP_STRING, 0, &B.semInfo, code);
}

function(A)    ::= FUNCTION params(B) block(C) END . {
	A = makeExpr(EXP_FUNCTION, B, C, NULL, code);
}

params(A)      ::= LPAREN parlist(B) LPAREN . {
	A = B;
}

parlist(A)     ::= . {
	A = makeList(NULL, code);
}
parlist(A)     ::= namelist(B) . {
	A = B;
}
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