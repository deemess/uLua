#include "udebug.h"

#ifdef DEBUGVM

void printToken(Token* t, lu08* code) {
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

void printSString(SString* s, lu08* c) {
	lu08 i;

	if(!s->bempty) {// print token
		for(i=0; i<s->bplen; i++) {
			printf("%c", c[s->bp+i]);
		}
	}
}

void printConst(Function *f, lu08 num) {
		Constant* c = f->consts;
		while(c->next != NULL && c->num != num) {
			c = c->next;
		}
		if(c->type == NUMBER_TYPE) {
			printf("(%.0f)\t", c->val_number);
		} else	if(c->type == STRING_TYPE) {
			printf("(");
			printSString(&c->val_string, f->code);
			printf(")\t");
		}
}

void printRK(Function *f, lu08 reg) {
	if(reg<CG_REG_COUNT) {
		printf("%d\t", reg);
	} else {
		printf("%d", reg);
		printConst(f, reg - CG_REG_COUNT);
	}
}

void printIntruction(Function *f, Instruction* i) {
		switch(i->i.unpacked.opc) {
			case OP_MOVE:/*	A B	R(A) := R(B)					*/
				printf("OP_MOVE ");
				break;
			case OP_LOADK:/*	A Bx	R(A) := Kst(Bx)					*/
				printf("OP_LOADK ");
				break;
			case OP_LOADBOOL:/*	A B C	R(A) := (Bool)B; if (C) pc++			*/
				printf("OP_LOADBOOL ");
				break;
			case OP_LOADNIL:/*	A B	R(A) := ... := R(B) := nil			*/
				printf("OP_LOADNIL ");
				break;
			case OP_GETUPVAL:/*	A B	R(A) := UpValue[B]				*/
				printf("OP_GETUPVAL ");
				break;
			case OP_GETGLOBAL:/*	A Bx	R(A) := Gbl[Kst(Bx)]				*/
				printf("OP_GETGLOBAL ");
				break;
			case OP_GETTABLE:/*	A B C	R(A) := R(B)[RK(C)]				*/
				printf("OP_GETTABLE ");
				break;
			case OP_SETGLOBAL:/*	A Bx	Gbl[Kst(Bx)] := R(A)				*/
				printf("OP_SETGLOBAL ");
				break;
			case OP_SETUPVAL:/*	A B	UpValue[B] := R(A)				*/
				printf("OP_SETUPVAL ");
				break;
			case OP_SETTABLE:/*	A B C	R(A)[RK(B)] := RK(C)				*/
				printf("OP_SETTABLE ");
				break;
			case OP_NEWTABLE:/*	A B C	R(A) := {} (size = B,C)				*/
				printf("OP_NEWTABLE ");
				break;
			case OP_SELF:/*	A B C	R(A+1) := R(B); R(A) := R(B)[RK(C)]		*/
				printf("OP_SELF ");
				break;
			case OP_ADD:/*	A B C	R(A) := RK(B) + RK(C)				*/
				printf("OP_ADD\t");
				break;
			case OP_SUB:/*	A B C	R(A) := RK(B) - RK(C)				*/
				printf("OP_SUB\t");
				break;
			case OP_MUL:/*	A B C	R(A) := RK(B) * RK(C)				*/
				printf("OP_MUL\t");
				break;
			case OP_DIV:/*	A B C	R(A) := RK(B) / RK(C)				*/
				printf("OP_DIV\t");
				break;
			case OP_MOD:/*	A B C	R(A) := RK(B) % RK(C)				*/
				printf("OP_MOD\t");
				break;
			case OP_POW:/*	A B C	R(A) := RK(B) ^ RK(C)				*/
				printf("OP_POW\t");
				break;
			case OP_UNM:/*	A B	R(A) := -R(B)					*/
				printf("OP_UNM\t");
				break;
			case OP_NOT:/*	A B	R(A) := not R(B)				*/
				printf("OP_NOT\t");
				break;
			case OP_LEN:/*	A B	R(A) := length of R(B)				*/
				printf("OP_LEN\t");
				break;
			case OP_CONCAT:/*	A B C	R(A) := R(B).. ... ..R(C)			*/
				printf("OP_CONCAT ");
				break;
			case OP_JMP:/*	sBx	pc+=sBx					*/
				printf("OP_JMP\t");
				break;
			case OP_EQ:/*	A B C	if ((RK(B) == RK(C)) ~= A) then pc++		*/
				printf("OP_EQ\t");
				break;
			case OP_LT:/*	A B C	if ((RK(B) <  RK(C)) ~= A) then pc++  		*/
				printf("OP_LT\t");
				break;
			case OP_LE:/*	A B C	if ((RK(B) <= RK(C)) ~= A) then pc++  		*/
				printf("OP_LE\t");
				break;
			case OP_TEST:/*	A C	if not (R(A) <=> C) then pc++			*/ 
				printf("OP_TEST ");
				break;
			case OP_TESTSET:/*	A B C	if (R(B) <=> C) then R(A) := R(B) else pc++	*/ 
				printf("OP_TESTSET ");
				break;
			case OP_CALL:/*	A B C	R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1)) */
				printf("OP_CALL ");
				break;
			case OP_TAILCALL:/*	A B C	return R(A)(R(A+1), ... ,R(A+B-1))		*/
				printf("OP_TAILCALL ");
				break;
			case OP_RETURN:/*	A B	return R(A), ... ,R(A+B-2)	(see note)	*/
				printf("OP_RETURN ");
				break;
			case OP_FORLOOP:/*	A sBx	R(A)+=R(A+2);			if R(A) <?= R(A+1) then { pc+=sBx; R(A+3)=R(A) }*/
				printf("OP_FORLOOP ");
				break;
			case OP_FORPREP:/*	A sBx	R(A)-=R(A+2); pc+=sBx				*/
				printf("OP_FORPREP ");
				break;
			case OP_TFORLOOP:/*	A C	R(A+3), ... ,R(A+2+C) := R(A)(R(A+1), R(A+2)); if R(A+3) ~= nil then R(A+2)=R(A+3) else pc++	*/ 
				printf("OP_TFORLOOP ");
				break;
			case OP_SETLIST:/*	A B C	R(A)[(C-1)*FPF+i] := R(A+i), 1 <= i <= B	*/
				printf("OP_SETLIST ");
				break;
			case OP_CLOSE:/*	A 	close all variables in the stack up to (>=) R(A)*/
				printf("OP_CLOSE ");
				break;
			case OP_CLOSURE:/*	A Bx	R(A) := closure(KPROTO[Bx], R(A), ... ,R(A+n))	*/
				printf("OP_CLOSURE ");
				break;
			case OP_VARARG:/*	A B	R(A), R(A+1), ..., R(A+B-1) = vararg		*/
				printf("OP_VARARG ");
				break;
		}
		printf("\t");
		switch(i->i.unpacked.opc) {
			case OP_MOVE:/*	A B	R(A) := R(B)					*/
			case OP_UNM:/*	A B	R(A) := -R(B)					*/
			case OP_NOT:/*	A B	R(A) := not R(B)				*/
			case OP_LOADNIL:/*	A B	R(A) := ... := R(B) := nil			*/
			case OP_GETUPVAL:/*	A B	R(A) := UpValue[B]				*/
			case OP_SETUPVAL:/*	A B	UpValue[B] := R(A)				*/
			case OP_LEN:/*	A B	R(A) := length of R(B)				*/
			case OP_RETURN:/*	A B	return R(A), ... ,R(A+B-2)	(see note)	*/
				printf("%d\t%d\t", i->i.unpacked.a, i->i.unpacked.bx.l.b);
				break;
			case OP_LOADK:/*	A Bx	R(A) := Kst(Bx)					*/
			case OP_GETGLOBAL:/*	A Bx	R(A) := Gbl[Kst(Bx)]				*/
			case OP_SETGLOBAL:/*	A Bx	Gbl[Kst(Bx)] := R(A)				*/
				printf("%d\t%d", i->i.unpacked.a, i->i.unpacked.bx.bx);
				printConst(f, i->i.unpacked.bx.bx);
				break;
			case OP_LOADBOOL:/*	A B C	R(A) := (Bool)B; if (C) pc++			*/
			case OP_NEWTABLE:/*	A B C	R(A) := {} (size = B,C)				*/
				printf("%d\t%d\t%d", i->i.unpacked.a, i->i.unpacked.bx.l.b, i->i.unpacked.bx.l.c);
				break;
			case OP_GETTABLE:/*	A B C	R(A) := R(B)[RK(C)]				*/
				break;
			case OP_SETTABLE:/*	A B C	R(A)[RK(B)] := RK(C)				*/
				break;
			case OP_SELF:/*	A B C	R(A+1) := R(B); R(A) := R(B)[RK(C)]		*/
				break;
			case OP_ADD:/*	A B C	R(A) := RK(B) + RK(C)				*/
			case OP_SUB:/*	A B C	R(A) := RK(B) - RK(C)				*/
			case OP_MUL:/*	A B C	R(A) := RK(B) * RK(C)				*/
			case OP_DIV:/*	A B C	R(A) := RK(B) / RK(C)				*/
			case OP_MOD:/*	A B C	R(A) := RK(B) % RK(C)				*/
			case OP_POW:/*	A B C	R(A) := RK(B) ^ RK(C)				*/
			case OP_EQ:/*	A B C	if ((RK(B) == RK(C)) ~= A) then pc++		*/
			case OP_LT:/*	A B C	if ((RK(B) <  RK(C)) ~= A) then pc++  		*/
			case OP_LE:/*	A B C	if ((RK(B) <= RK(C)) ~= A) then pc++  		*/
			case OP_CALL:/*	A B C	R(A), ... ,R(A+C-2) := R(A)(R(A+1), ... ,R(A+B-1)) */
			case OP_TEST:/*	A C	if not (R(A) <=> C) then pc++			*/ 
			case OP_TESTSET:/*	A B C	if (R(B) <=> C) then R(A) := R(B) else pc++	*/ 
			case OP_TAILCALL:/*	A B C	return R(A)(R(A+1), ... ,R(A+B-1))		*/
				printRK(f, i->i.unpacked.a);
				printRK(f, i->i.unpacked.bx.l.b);
				printRK(f, i->i.unpacked.bx.l.c);
				break;
			case OP_CONCAT:/*	A B C	R(A) := R(B).. ... ..R(C)			*/
				break;
			case OP_JMP:/*	sBx	pc+=sBx					*/
				printf("%d", i->i.unpacked.bx.bx);
				break;
				break;
			case OP_FORLOOP:/*	A sBx	R(A)+=R(A+2);			if R(A) <?= R(A+1) then { pc+=sBx; R(A+3)=R(A) }*/
				break;
			case OP_FORPREP:/*	A sBx	R(A)-=R(A+2); pc+=sBx				*/
				break;
			case OP_TFORLOOP:/*	A C	R(A+3), ... ,R(A+2+C) := R(A)(R(A+1), R(A+2)); if R(A+3) ~= nil then R(A+2)=R(A+3) else pc++	*/ 
				break;
			case OP_SETLIST:/*	A B C	R(A)[(C-1)*FPF+i] := R(A+i), 1 <= i <= B	*/
				break;
			case OP_CLOSE:/*	A 	close all variables in the stack up to (>=) R(A)*/
				break;
			case OP_CLOSURE:/*	A Bx	R(A) := closure(KPROTO[Bx], R(A), ... ,R(A+n))	*/
				break;
			case OP_VARARG:/*	A B	R(A), R(A+1), ..., R(A+B-1) = vararg		*/
				break;
		}
		printf("\n");
}

void printFunction(Function *f) {
	Instruction* i;
	Constant* c = f->consts;
	printf("\nConstants:\n");
	while(c != NULL) {
		if(c->type == STRING_TYPE) {
			printf("%d: ", c->num);
			printSString(&c->val_string, f->code);
			printf("\n");
		} else if (c->type == NUMBER_TYPE) {
			printf("%d: %.0f\n", c->num, c->val_number);
		} else {
			printf("%d: unknown type\n", c->num);
		}
		c = c->next;
	}
	c = f->vars;
	printf("\nVars:\n");
	while(c != NULL) {
		if(c->type == STRING_TYPE) {
			printf("%d: ", c->num);
			printSString(&c->val_string, f->code);
			printf("\n");
		} else if (c->type == NUMBER_TYPE) {
			printf("%d: %f\n", c->num, c->val_number);
		} else {
			printf("%d: unknown type\n", c->num);
		}
		c = c->next;
	}
	i = f->instr;
	printf("\nInstructions:\n");
	while(i != NULL) {
		printIntruction(f, i);
		i = i->next;
	}

}

#endif
