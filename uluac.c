#include <stdio.h>
#include "ulexer.h"
#include "uparser.c"
#include "ucodegen.h"
#include "udebug.h"

#define CODE_BUFFER_SIZE 64*1024

static lu08 code[CODE_BUFFER_SIZE];

FILE* ofile;

void writeToFile(lu08* buff, lu16 size) {
	fwrite(buff, 1, size, ofile);
}

int main(int argc, char **argv) {

	//local variable
	//int i;
	FILE* file;
	lu16 codelen;
	LexState ls;
	Function top;
	void *parser;
	int i;

	//check args
	if(argc < 3) 
	{
		printf("Usage: \n");
		printf("uluac file.lua file.clu");
		return 1;
	}

	//read file
	for(i=0; i<CODE_BUFFER_SIZE; i++)
		code[i]=0;

	file = fopen(argv[1], "r");
	codelen = (lu32)fread(code, 1, CODE_BUFFER_SIZE, file);
	code[codelen] = 0;
	fclose(file);

	//init Parser
	parser = ParseAlloc (malloc); 

	//init Lexer
	setInput(&ls, (lu08*)code);

	//init top level function
	initFunction(&top, (lu08*)code);

	//parse source code
	next(&ls);
	while(ls.t.token != TK_EOS) {
		if(getLastULexError() != E_NONE) {
			printf("Error: %d\n", getLastULexError());
			printf("Token:");
			printToken(&ls.t, code);
			return 1;
			break;
		}
		printToken(&ls.t, code);
		Parse(parser, ls.t.token, ls.t, &top);
		if(top.error_code != 0) {
			printf("Syntax error:%d on line: %d token ", top.error_code, ls.linenumber);
			printToken(&ls.t, code);
			printf("\n");
			return 1;
		}
		next(&ls);
	}
	printToken(&ls.t, code);
	Parse(parser, ls.t.token, ls.t, &top);

	printFunction(&top);
	
	//save dump to file
	ofile = fopen(argv[2], "wb");
	dump(&top, &writeToFile);
	fclose(ofile);

	ParseFree(parser, free);
	return 0;
}
