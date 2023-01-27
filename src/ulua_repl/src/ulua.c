#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ulua_core/ulexer.h"
#include "ulua_core/uparser_static.h"
#include "ulua_core/vm.h"
#include "ulua_core/udebug.h"

#define GETLINE_OK       0
#define GETLINE_NO_INPUT 1
#define GETLINE_TOO_LONG 2

#define CODE_BUFFER_SIZE (64*1024)-1

static lu08 ram[CODE_BUFFER_SIZE];
static lu08 code[CODE_BUFFER_SIZE];
static lu08 bdump[CODE_BUFFER_SIZE];

static int getLine (char *prmpt, lu08 *buff, int sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != ULUA_NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
	int readline;
	lu08* p = buff;
	do {
		readline = 0;
		if (fgets((char*)p, sz, stdin) == ULUA_NULL)
			return GETLINE_NO_INPUT;
		if (p[strlen((char*)p) - 2] == '\\') {
			p[strlen((char*)p) - 2] = ' ';
			p += strlen((char*)p);
			readline = 1;
		}
	} while (readline);
	

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen((char*)buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? GETLINE_TOO_LONG : GETLINE_OK;
    }

    // Otherwise remove newline and give string back to caller.
    //buff[strlen((char*)buff)-1] = '\0';
    return GETLINE_OK;
}

lu16 dp = 0;
void writeBytecode(const lu08* buff, lu16 size) {
	lu16 i;
	for(i=0; i<size; i++) {
		bdump[dp + i] = buff[i];
	}
	dp += size;
}

void readBytecode(lu08* buff, lu16 offset, lu16 size) {
	lu16 i;
	for(i=0; i<size; i++) {
		buff[i] = bdump[offset+i];
	}
}

int main(int argc, char **argv) {
	LexState ls;
	Function top;
	ulua_memvar* thread;
	void *parser;
	lu32 i;

	//clear buffers
	for(i=0; i<CODE_BUFFER_SIZE; i++) {
		code[i] = 0;
		bdump[i] = 0;
	}

	//init vm stucture
    thread = vmInit((lu08*)&ram, CODE_BUFFER_SIZE);

	//main cycle
	while(ULUA_TRUE) {
		//read code from command line
		getLine(">> ", code, CODE_BUFFER_SIZE);
		//exit on exit() command
		if(strcmp("exit()", (char*)code) == 0)
			break;
		//init Parser
		//ulua_mem_dump();
		parser = ParseAlloc(ulua_mem_new_block);
		//ulua_mem_dump();
		//init top level function
		initFunction(&top, (lu08*)code);
        //init Lexer
        setInput(&ls, (lu08*)code);
        //parse code
		next(&ls);
		while(ls.t.token != TK_EOS) {
			if(getLastULexError() != E_NONE) {
				printf("Error: %d\n", getLastULexError());
				printf("Token:");
				printToken(&ls.t, code);
				break;
			}
			Parse(parser, ls.t.token, ls.t, &top);
			if(top.error_code != 0) {
				printf("Syntax error: %d on line: %d token ", top.error_code, ls.linenumber);
				printToken(&ls.t, code);
				printf("\n");
				break;
			}
			next(&ls);
		}
		if(top.error_code == 0 && getLastULexError() == E_NONE) {
			//parse last token (should be EOS)
			Parse(parser, ls.t.token, ls.t, &top);
		}

		//check if parsing complete
		if(!top.parsed) {
			printf("Unfinished code!\n");
		}

		//execude code only if there were no errors
		if(top.parsed == ULUA_TRUE && top.error_code == 0 && getLastULexError() == E_NONE) {
			//dump function
#ifdef DEBUGVM
			printFunction(&top);
#endif
			//get binary dump
			dp = 0;
			dump(&top, (writeBytes) &writeBytecode);
			//free resources
			//ulua_mem_dump();
			freeFunction(&top);
			//ulua_mem_dump();
			ParseFree(parser, ulua_mem_free_block);
			//ulua_mem_dump();
			//run dump on vm
			vmRun(thread, &readBytecode);
		}
		else {
			//free resources
			freeFunction(&top);
			ParseFree(parser, ulua_mem_free_block);
		}
	}
	return 0;
}
