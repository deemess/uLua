%fallback  OPEN LPAREN .

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
stat ::= FUNCTION funcname params block END . 
stat ::= setlist SET explist1 . 
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
binding    ::= LOCAL FUNCTION NAME params block END .

funcname   ::= dottedname . 
funcname   ::= dottedname COLON NAME .

dottedname ::= NAME . 
dottedname ::= dottedname DOT NAME . 

namelist   ::= NAME .
namelist   ::= namelist COMMA NAME . 

explist1	::= exp . 
explist1	::= explist1 COMMA exp . 
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
exp        ::= NUMBER . 
exp        ::= STRING . 
exp        ::= function . 
exp        ::= prefixexp . 
exp        ::= tableconstructor .
exp        ::= NOT|HASH|MINUS exp .
exp        ::= exp OR exp .
exp        ::= exp AND exp .
exp        ::= exp L|LE|G|GE|EQ|NE exp .
exp        ::= exp CONCAT exp .
exp			::= exp PLUS|MINUS|TIMES|DIVIDE|MOD|POW exp .

setlist ::= var . 
setlist ::= setlist COMMA var . 

var ::= NAME . 
var ::= prefixexp SLPAREN exp SRPAREN .
var ::= prefixexp DOT NAME .

prefixexp  ::= var . 
prefixexp  ::= functioncall . 
prefixexp  ::= OPEN exp RPAREN .

functioncall ::= prefixexp args . 
functioncall ::= prefixexp COLON NAME args .

args        ::= LPAREN RPAREN . 
args        ::= LPAREN explist1 RPAREN . 
args        ::= tableconstructor .
args        ::= STRING . 
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