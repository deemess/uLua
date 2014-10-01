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

%syntax_error {
  printf ("Syntax error!\n");
}

%start_symbol  chunk

chunk ::= block . {
	printf("P_CHUNK\n");
}

semi ::= SEMICOL . {
	printf("P_SEMI\n");
}
semi ::= . {
	printf("P_SEMI\n");
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
stat ::= setlist SET explist1 . {
	printf("P_STAT_SET\n");
}
stat ::= functioncall .  {
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
binding    ::= LOCAL namelist SET explist1 .{
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

namelist   ::= NAME .
namelist   ::= namelist COMMA NAME . 

explist1	::= exp . {
	printf("P_EXPLIST_EXP\n");
}
explist1	::= explist1 COMMA exp . {
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
exp        ::= NUMBER(B) . {
	printf("P_EXP_NUMBER\n");
	pushConstNumber(f, B.number.fvalue);
}
exp        ::= STRING(B) . {
	printf("P_EXP_STRING\n");
	pushConstString(f, &B.semInfo);
}
exp        ::= function . {
	printf("P_EXP_FUNCTION\n");
}
exp        ::= prefixexp . {
	printf("P_EXP_PREFIXEXP\n");
}
exp        ::= tableconstructor .
exp        ::= NOT|HASH|MINUS exp .
exp        ::= exp OR exp .
exp        ::= exp AND exp .
exp        ::= exp L|LE|G|GE|EQ|NE exp .
exp        ::= exp CONCAT exp .
exp		   ::= exp PLUS|MINUS|TIMES|DIVIDE|MOD|POW exp . {
	printf("P_EXP_MATH\n");
}

setlist ::= var . {
	printf("P_SETLIST_VAR\n");
}
setlist ::= setlist COMMA var . {
	printf("P_SETLIST_ADD_VAR\n");
}

var ::= NAME(B) . {
	printf("P_VAR_NAME\n");
	pushConstString(f, &B.semInfo);
}
var ::= prefixexp SLPAREN exp SRPAREN . {
	printf("P_PREFEXP_SPAREN_EXP\n");
}
var ::= prefixexp DOT NAME . {
	printf("P_PREFEXP_DOT_NAME\n");
}

prefixexp  ::= var . {
	printf("P_PREFEXP_VAR\n");
}
prefixexp  ::= functioncall .  {
	printf("P_PREFEXP_FCALL\n");
}
prefixexp  ::= OPEN exp RPAREN . {
	printf("P_PREFEXP_EXP\n");
}

functioncall ::= prefixexp args . {
	printf("P_FCALL_ARGS\n");
}
functioncall ::= prefixexp COLON NAME(B) args . {
	printf("P_FCALL_NAME_ARGS\n");
	pushConstString(f, &B.semInfo);
}

args        ::= LPAREN RPAREN . {
	printf("P_ARGS_EMPTY\n");
}
args        ::= LPAREN explist1 RPAREN . {
	printf("P_ARGS_EXPLIST\n");
}
args        ::= tableconstructor .
args        ::= STRING(B) . {
	printf("P_ARGS_STRING\n");
	pushConstString(f, &B.semInfo);
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
