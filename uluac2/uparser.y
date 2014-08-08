%include {
#include <stdio.h>
#include "basetypes.h"
#include "ulexer.h"
#include "uparser.h"
}

%token_type {Token*}
%token_prefix TK_

%type exp		{uExpression}
%type var		{uExpression}
%type setlist	{uExpression}
%type explist1	{uExpression}
%type stat		{uExpression}

%syntax_error {
  printf ("Syntax error!\n");
}

%start_symbol  chunk

chunk ::= block .

semi ::= SEMICOL .
semi ::= .

block ::= scope statlist .
block ::= scope statlist laststat semi .
ublock ::= block UNTIL exp .

scope ::= .
scope ::= scope statlist binding semi.
           
statlist   ::= .
statlist   ::= statlist stat semi .

stat ::= DO block END .
stat ::= WHILE exp DO block END .
stat ::= repetition DO block END .
stat ::= REPEAT ublock .
stat ::= IF conds END .
stat ::= FUNCTION funcname funcbody .
stat(A) ::= setlist(B) SET explist1(C) . {
	A.fvalue = C.fvalue;
}
stat ::= functioncall .

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

binding    ::= LOCAL namelist .
binding    ::= LOCAL namelist SET explist1 .
binding    ::= LOCAL FUNCTION NAME funcbody .

funcname   ::= dottedname .
funcname   ::= dottedname COLON NAME .

dottedname ::= NAME .
dottedname ::= dottedname DOT NAME .

namelist   ::= NAME .
namelist   ::= namelist COMMA NAME .

explist1(A) ::= exp(B) . {
	A.fvalue = B.fvalue;
	A.name.bempty = B.name.bempty;
	A.name.bp = B.name.bp;
	A.name.bplen = B.name.bplen;
}
explist1   ::= explist1 COMMA exp .
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
	A.fvalue = B->number.fvalue;
}
exp(A)        ::= STRING(B) . {
	A.name.bempty = B->semInfo.bempty;
	A.name.bp = B->semInfo.bp;
	A.name.bplen = B->semInfo.bplen;
}
exp        ::= function .
exp        ::= prefixexp .
exp        ::= tableconstructor .
exp        ::= NOT|HASH|MINUS exp .
exp        ::= exp OR exp .
exp        ::= exp AND exp .
exp        ::= exp L|LE|G|GE|EQ|NE exp .
exp        ::= exp CONCAT exp .
exp(A) ::= exp(B) PLUS exp(C) .{
	A.fvalue = B.fvalue + C.fvalue;
}
exp(A) ::= exp(B) MINUS exp(C) .{
	A.fvalue = B.fvalue - C.fvalue;
}
exp(A) ::= exp(B) TIMES exp(C) .{
	A.fvalue = B.fvalue * C.fvalue;
}
exp(A) ::= exp(B) DIVIDE exp(C) .{
	A.fvalue = B.fvalue / C.fvalue;
}
exp        ::= exp MOD exp .
exp        ::= exp POW exp .

setlist(A) ::= var(B) . {
	A.name.bempty = B.name.bempty;
	A.name.bp = B.name.bp;
	A.name.bplen = B.name.bplen;
}
setlist ::= setlist COMMA var .

var(A) ::= NAME(B) . {
	A.name.bempty = B->semInfo.bempty;
	A.name.bp = B->semInfo.bp;
	A.name.bplen = B->semInfo.bplen;
}
var        ::= prefixexp SLPAREN exp SRPAREN .
var        ::= prefixexp DOT NAME .

prefixexp  ::= var .
prefixexp  ::= functioncall .
prefixexp  ::= LPAREN exp RPAREN .

functioncall ::= prefixexp args .
functioncall ::= prefixexp COLON NAME args .

args        ::= LPAREN RPAREN .
args        ::= LPAREN explist1 RPAREN .
args        ::= tableconstructor .
args        ::= STRING .

function    ::= FUNCTION funcbody .

funcbody    ::= params block END .

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