#include <stdio.h>
#include "ulexer.h"

char code[] = 
"a = 1\n\
b = 2.5\n\
c = false\n\
d = {}\n\
\n\
for i = 1, 10, 1 do\n\
	print(i)\n\
end\n\
";

void printToken(Token* t) {
	int i;

	if(!t->semInfo.bempty) {// print token
		for(i=0; i<t->semInfo.bplen; i++) {
			printf("%c", code[t->semInfo.bp+i]);
		}
		printf(" ");
	}


	if(t->token > 255) {
		switch(t->token) {
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
		}
	} else {
		printf("%c ", (char)t->token);
	}

	printf("\n");
}

int main(int argc, char **argv) {
	LexState ls;

	setInput(&ls, (u08*)code);
	while(ls.t.token != TK_EOS) {
		next(&ls);
		if(getLastULexError() != E_NONE) {
			printf("Error: %d\n", getLastULexError());
			printf("Token:");
			printToken(&ls.t);
			return 1;
			break;
		}
		printToken(&ls.t);
	}

	return 0;
}
