#include <stdio.h>
#include "ulexer.h"
#include "uparser.c"

#define CODE_BUFFER_SIZE 64*1024

static u08 code[CODE_BUFFER_SIZE];

void printToken(Token* t) {
	int i;

	switch(t->token) {
		case TK_SET: printf("TK_SET "); break;
		case TK_MOD: printf("TK_MOD "); break;	
		case TK_POW: printf("TK_POW "); break;
		case TK_HASH: printf("TK_HASH "); break;	
		case TK_RBRACE: printf("TK_RBRACE "); break;
		case TK_LBRACE: printf("TK_LBRACE "); break;
		case TK_SLPAREN: printf("TK_SLPAREN "); break;
		case TK_SRPAREN: printf("TK_SRPAREN "); break;
		case TK_L: printf("TK_L "); break;
		case TK_G: printf("TK_G "); break;
		case TK_DOT: printf("TK_DOT "); break;
		case TK_COLON: printf("TK_COLON "); break;
		case TK_COMMA: printf("TK_COMMA "); break;
		case TK_SEMICOL: printf("TK_SEMICOL "); break;
		case TK_PLUS: printf("TK_PLUS "); break;
		case TK_MINUS: printf("TK_MINUS "); break;
		case TK_DIVIDE: printf("TK_DIVIDE "); break;
		case TK_TIMES: printf("TK_TIMES "); break;
		case TK_LPAREN: printf("TK_LPAREN "); break;
		case TK_RPAREN: printf("TK_RPAREN "); break;
		case TK_AND: printf("TK_AND "); break;
		case TK_BREAK: printf("TK_BREAK "); break;
		case TK_DO: printf("TK_DO "); break;
		case TK_ELSE: printf("TK_ELSE "); break;
		case TK_ELSEIF: printf("TK_ELSEIF "); break;
		case TK_END: printf("TK_END "); break;
		case TK_FALSE: printf("TK_FALSE "); break;
		case TK_FOR: printf("TK_FOR "); break;
		case TK_FUNCTION: printf("TK_FUNCTION "); break;
		case TK_GOTO: printf("TK_GOTO "); break;
		case TK_IF: printf("TK_IF "); break;
		case TK_IN: printf("TK_IN "); break;
		case TK_LOCAL: printf("TK_LOCAL "); break;
		case TK_NIL: printf("TK_NIL "); break;
		case TK_NOT: printf("TK_NOT "); break;
		case TK_OR: printf("TK_OR "); break;
		case TK_REPEAT: printf("TK_REPEAT "); break;
		case TK_RETURN: printf("TK_RETURN "); break;
		case TK_THEN: printf("TK_THEN "); break;
		case TK_TRUE: printf("TK_TRUE "); break;
		case TK_UNTIL: printf("TK_UNTIL "); break;
		case TK_WHILE: printf("TK_WHILE "); break;
		case TK_CONCAT: printf("TK_CONCAT "); break;
		case TK_DOTS: printf("TK_DOTS "); break;
		case TK_EQ: printf("TK_EQ "); break;
		case TK_GE: printf("TK_GE "); break;
		case TK_LE: printf("TK_LE "); break;
		case TK_NE: printf("TK_NE "); break;
		case TK_DBCOLON: printf("TK_DBCOLON "); break;
		case TK_EOS: printf("TK_EOS "); break;
		case TK_NUMBER: printf("TK_NUMBER "); printf("%f "/*, t->number.nvalue*/, t->number.fvalue); break;
		case TK_NAME: printf("TK_NAME "); break;
		case TK_STRING: printf("TK_STRING "); break;	
		default:
			printf("%c ", (char)t->token);
			break;
	}

	if(!t->semInfo.bempty) {// print token
		for(i=0; i<t->semInfo.bplen; i++) {
			printf("%c", code[t->semInfo.bp+i]);
		}
		printf(" ");
	}

	printf("\n");
}

int main(int argc, char **argv) {

	//local variable
	int i;
	FILE* file;
	u16 codelen;
	LexState ls;
	void *parser;

	//check args
	if(argc < 2) 
	{
		printf("Usage: \n");
		printf("uluac file.lua");
		return 1;
	}

	//read file
	for(i=0; i<CODE_BUFFER_SIZE; i++)
		code[i]=0;

	file = fopen(argv[1], "r");
	codelen = (u32)fread(code, 1, CODE_BUFFER_SIZE, file);

	//init Parser
	parser = ParseAlloc (malloc); 

	//init Lexer
	setInput(&ls, (u08*)code);

	//parse source code
	next(&ls);
	while(ls.t.token != TK_EOS) {
		if(getLastULexError() != E_NONE) {
			printf("Error: %d\n", getLastULexError());
			printf("Token:");
			printToken(&ls.t);
			return 1;
			break;
		}
		printToken(&ls.t);
		Parse(parser, ls.t.token, ls.t, &code[0]);
		next(&ls);
	}

	ParseFree(parser, free);
	return 0;
}
