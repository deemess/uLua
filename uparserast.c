#include "ulexer.h"
#include <stdio.h>

extern u08 code[];

void debugExprType(uExpression* e, u08* code) {
	u08 i;
	if(e == NULL) {
		printf("NULL");
		return;
	}

	switch(e->type) {
		case EXP_VARIABLE: printf("EXP_VARIABLE "); 
			if(e->name.bempty == FALSE) {// print token
				for(i=0; i<e->name.bplen; i++) {
					printf("%c", code[e->name.bp+i]);
				}
			}
			break;
		case EXP_NUMBER: printf("EXP_NUMBER "); 
			printf("%f", e->fvalue);
			break;
		case EXP_STRING: printf("EXP_STRING "); 
			if(e->name.bempty == FALSE) {// print token
				for(i=0; i<e->name.bplen; i++) {
					printf("%c", code[e->name.bp+i]);
				}
			}
			break;
		case EXP_BOOLEAN: printf("EXP_BOOLEAN"); break;
		case EXP_FUNCTION: printf("EXP_FUNCTION"); 
			if(e->name.bempty == FALSE) {// print token
				for(i=0; i<e->name.bplen; i++) {
					printf("%c", code[e->name.bp+i]);
				}
				printf(" ");
			}
			break;
		case EXP_ADD: printf("EXP_ADD"); break;
		case EXP_SUB: printf("EXP_SUB"); break;
		case EXP_DIV: printf("EXP_DIV"); break;
		case EXP_MUL: printf("EXP_MUL"); break;
		case EXP_LIST: printf("EXP_LIST"); break;
		case EXP_SET: printf("EXP_SET"); break;
		case EXP_CHUNK: printf("EXP_CHUNK"); break;
		case EXP_RETURN: printf("EXP_RETURN"); break;
		case EXP_BREAK: printf("EXP_BREAK"); break;
		case EXP_FUNCTION_CALL: printf("EXP_FUNCTION_CALL"); break;
		default:
			printf("UNKNOWN");
	}
}

void debugExpr(uExpression* e, u08* code) {
	debugExprType(e, code);
	printf(" arg0=");
	debugExprType((uExpression*)e->arg0, code);
	printf(" arg1=");
	debugExprType((uExpression*)e->arg1, code);
	printf(" arg2=");
	debugExprType((uExpression*)e->arg2, code);
	printf("\n");
}

uExpression* makeExpr(EXPR_TYPE type, uExpression* arg0, uExpression* arg1, uExpression* arg2, u08* code) {
	uExpression* result = NULL;

	//perform math calculation for constants
	if( 
		(arg0 != NULL && arg0->type == EXP_NUMBER)
		&&
		(arg1 != NULL && arg1->type == EXP_NUMBER)
		)
	{
		switch(type) {
		case EXP_ADD:
			arg0->fvalue += arg1->fvalue;
			free(arg1);
			return arg0;
		case EXP_SUB:
			arg0->fvalue -= arg1->fvalue;
			free(arg1);
			return arg0;
		case EXP_MUL:
			arg0->fvalue *= arg1->fvalue;
			free(arg1);
			return arg0;
		case EXP_DIV:
			arg0->fvalue = arg0->fvalue / arg1->fvalue;
			free(arg1);
			return arg0;
		}
	}

	result = (uExpression*)malloc(sizeof(uExpression));

	result->type = type;
	result->arg0 = arg0;
	result->arg1 = arg1;
	result->arg2 = arg2;
	result->first = NULL;

	debugExpr(result, code);

	return result;
}

uExpression* makeConst(EXPR_TYPE type, float fval, SString* strval, u08* code) {
	uExpression* result = (uExpression*)malloc(sizeof(uExpression));

	result->type = type;
	result->arg0 = NULL;
	result->arg1 = NULL;
	result->arg2 = NULL;
	result->first = NULL;

	switch(type) {
	case EXP_STRING:
		result->name.bempty = strval->bempty;
		result->name.bp = strval->bp;
		result->name.bplen = strval->bplen;
		break;
	default:
		result->fvalue = fval;
	}

	debugExpr(result, code);

	return result;
}

uExpression* makeVariable(EXPR_TYPE type, SString* strval, u08* code) {
	uExpression* result = (uExpression*)malloc(sizeof(uExpression));

	result->type = type;
	result->name.bempty = strval->bempty;
	result->name.bp = strval->bp;
	result->name.bplen = strval->bplen;
	result->arg0 = NULL;
	result->arg1 = NULL;
	result->arg2 = NULL;
	result->first = NULL;

	debugExpr(result, code);

	return result;
}

uExpression* addList(uExpression* list, uExpression* expr, u08* code) {
	uNode* newnode = NULL;
	uNode* node = list->first;
	if(node == NULL) {
		newnode = (uNode*)malloc(sizeof(uNode));
		list->first = newnode;
		newnode->expr = expr;
		newnode->next = NULL;
	} else {
		while(node->next != NULL) {
			node = node->next;
		}
		newnode = (uNode*)malloc(sizeof(uNode));
		node->next = newnode;
		newnode->expr = expr;
		newnode->next = NULL;
	}

	return list;
}

uExpression* makeList(uExpression* firstExpr, u08* code) {
	uExpression* result = (uExpression*)malloc(sizeof(uExpression));
	uNode* node = NULL;

	if(firstExpr != NULL) { //if first expr is not null, add it to the list
		node = (uNode*)malloc(sizeof(uNode));
		node->expr = firstExpr;
		node->next = NULL;
	}

	result->type = EXP_LIST;
	result->arg0 = NULL;
	result->arg1 = NULL;
	result->arg2 = NULL;
	result->first = node;

	debugExpr(result, code);

	return result;
}

void makeDump(uExpression* chunk, u08* code) {
	uExpression *a, *b, *c;

	if(chunk->type == EXP_CHUNK) {
		uExpression* list = chunk->arg0;
		uNode* node = list->first;
		while (node != NULL) {
			//for each statement
			// EXP_LIST current A(EXP_LIST) next B(EXP)

			uExpression* expr = node->expr;
			switch(expr->type) {
			case EXP_SET: // variable assigment
				a = expr->arg0; //varname
				b = expr->arg1; //expression
				break;
			case EXP_FUNCTION_CALL:
				a = expr->arg0; //funcname
				b = expr->arg1; //arguments
				break;
			}

			node = node->next;
		}
	}
}


uBlock* createBlock() {
	uBlock* result = (uBlock*)malloc(sizeof(uBlock));

	result->globals = NULL;
	result->locals = NULL;
	result->functions = NULL;
	result->constants = NULL;
	result->regN = 0;
	result->regMaxN = 0;
	result->instructions = NULL;
	result->instructionCount = 0;

	return result;
}

uBlock* mergeBlock(uBlock* a, uBlock* b) {
	//merge stack and free b block (right side)
	u08 i = 0;
	for(i=0; i < b->regN; i++) {
		a->regs[a->regN++] = b->regs[i];
	}
	if(a->regN > a->regMaxN)  a->regMaxN = a->regN;

	free(b);

	return a;
}

uBlock* pushConstant(uBlock* b, Token* t, VAL_TYPE type) {
	uValNode* newnode = (uValNode*)malloc(sizeof(uValNode));
	
	newnode->type = type;
	newnode->isLocal = FALSE;
	newnode->function = NULL;
	newnode->next = NULL;

	switch(type) {
	case VAL_NUMBER:
		newnode->value = t->number.fvalue;
		break;
	case VAL_STRING:
		newnode->name.bempty = t->semInfo.bempty;
		newnode->name.bp = t->semInfo.bp;
		newnode->name.bplen = t->semInfo.bplen;
		break;
	case VAL_BOOLEAN:
		if(t->token == TK_TRUE) 
			newnode->boolean = TRUE;
		else
			newnode->boolean = FALSE;
		break;
	}

	b->regs[b->regN++] = newnode;
	if(b->regN > b->regMaxN)  b->regMaxN = b->regN;

	return b;
}

uBlock* mathOp(uBlock* a, Token* t) {
	uValNode* newnode;
	if(a->regs[a->regN]->type == VAL_NUMBER) {
		switch(t->token) {
		case TK_PLUS:
		case TK_MINUS:
		case TK_DIVIDE:
		case TK_TIMES:
			if(a->regs[a->regN - 1]->type == VAL_NUMBER) { //perform math operation on constant in stack without creating instruction
			}
			break;
		}
	}
	newnode = (uValNode*)malloc(sizeof(uValNode));
}
