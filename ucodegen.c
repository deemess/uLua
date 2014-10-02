#include "ucodegen.h"
#include <stdio.h>

void initFunction(Function* f, u08* code) {
	u08 i;

	f->code = code;
	f->consts = NULL;
	f->vars = NULL;
	f->subfuncs = NULL;
	f->instr = NULL;
	for(i=0; i<CG_REG_COUNT; i++) {
		f->reg[i].num = i;
		f->reg[i].consthold = FALSE;
		f->reg[i].constnum = 0;
		f->reg[i].varnum = 0;
		f->reg[i].isfree = TRUE;
		f->reg[i].constpreloaded = FALSE;
	}
}

BOOL constEqueals(Function* f, Constant* a, Constant* b) {
	u08 i;
	if(a->type == b->type) {
		if(a->type == CG_CONST_NUMBER) {
			if(a->val_number == b->val_number)
				return TRUE;
		} else if(a->type == CG_CONST_STRING) {
			if(a->val_string.bplen == b->val_string.bplen){
				for(i=0; i<a->val_string.bplen; i++) {
					if(f->code[a->val_string.bp + i] != f->code[b->val_string.bp + i]) 
						return FALSE;
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}


Constant* pushConst(Function* f, Constant* c) {
	Constant* last;

	if(f->consts == NULL) {
		c->num = 1;
		f->consts = c;
		return c;
	}

	last = f->consts;
	if(constEqueals(f, last, c)) {
		free(c);
		return last;
	}
	while(last->next != NULL) {
		last = last->next;
		if(constEqueals(f, last, c)) {
			free(c);
			return last;
		}
	}

	last->next = c;
	c->num = last->num + 1;
	return c;
}

Constant* pushVar(Function* f, Constant* c) {
	Constant* last;

	if(f->vars == NULL) {
		c->num = 1;
		f->vars = c;
		return c;
	}

	last = f->vars;
	if(constEqueals(f, last, c)) {
		free(c);
		return last;
	}
	while(last->next != NULL) {
		last = last->next;
		if(constEqueals(f, last, c)) {
			free(c);
			return last;
		}
	}

	last->next = c;
	c->num = last->num + 1;
	return c;
}

Constant* pushVarName(Function* f, SString* str) {
	Constant* c = (Constant*)malloc(sizeof(Constant));

	c->num = 0;
	c->next = NULL;
	c->type = CG_CONST_STRING;
	c->val_string.bempty = str->bempty;
	c->val_string.bp = str->bp;
	c->val_string.bplen = str->bplen;
	
	c = pushVar(f, c);
	return c;
}

Instruction* pushInstruction(Function* f, Instruction* i) {
	Instruction* last;

	if(f->instr == NULL) {
		f->instr = i;
		i->next = NULL;
		return i;
	}
	last = f->instr;
	while(last->next != NULL) {
		last = last->next;
	}
	last->next = i;
	i->next = NULL;

	return i;
}

//public functions

Constant* pushConstString(Function* f, SString* str) {
	Constant* c = (Constant*)malloc(sizeof(Constant));
	
	c->num = 0;
	c->next = NULL;
	c->type = CG_CONST_STRING;
	c->val_string.bempty = str->bempty;
	c->val_string.bp = str->bp;
	c->val_string.bplen = str->bplen;
	
	c = pushConst(f, c);
	return c;
}

Constant* pushConstNumber(Function* f, float number) {
	Constant* c = (Constant*)malloc(sizeof(Constant));
	
	c->num = 0;
	c->next = NULL;
	c->type = CG_CONST_NUMBER;
	c->val_number = number;
	
	c = pushConst(f, c);

	return c;
}

Register* getFreeRegister(Function* f) {
	u08 i;
	for(i=0; i<CG_REG_COUNT; i++) {
		if(f->reg[i].isfree) {
			f->reg[i].isfree = FALSE;
			return &f->reg[i];
		}
	}

	return NULL;
}

void freeRegister(Register* r) {
	r->consthold = FALSE;
	r->constnum = 0;
	r->varnum = 0;
	r->isfree = TRUE;
	r->constpreloaded = FALSE;
}

Register* getVarRegister(Function* f, Constant* var) {
	u08 i;
	Register* r;

	for(i=0; i<CG_REG_COUNT; i++){
		if(f->reg[i].varnum == var->num)
			return &f->reg[i];
	}

	r = getFreeRegister(f);
	r->isfree = FALSE;
	r->varnum = var->num;

	return r;
}

Register* doMath(Function* f, Register* a, Register* b, Token* t) {
	Instruction* i = (Instruction*)malloc(sizeof(Instruction));
	i->a = a->num;
	if(a->consthold)
		i->b = a->constnum + CG_REG_COUNT;
	else
		i->b = a->num;

	if(b->consthold)
		i->c = b->constnum + CG_REG_COUNT;
	else
		i->c = b->num;

	switch(t->token) {
		case TK_PLUS:
			i->opc = OP_ADD;
			break;
		case TK_MINUS:
			i->opc = OP_SUB;
			break;
		case TK_TIMES:
			i->opc = OP_MUL;
			break;
		case TK_DIVIDE:
			i->opc = OP_DIV;
			break;
		case TK_MOD:
			i->opc = OP_MOD;
			break;
		case TK_POW:
			i->opc = OP_POW;
			break;
	}
	pushInstruction(f,i);

	freeRegister(b);
	a->consthold = FALSE;
	a->constpreloaded = FALSE;
	a->constnum = 0;
	
	return a;
}