#include "ucodegen.h"
#include <stdio.h>

void initFunction(Function* f, u08* code) {
	u08 i;

	f->code = code;
	f->c = NULL;
	f->f = NULL;
	f->i = NULL;
	for(i=0; i<CG_REG_COUNT; i++) {
		f->r[i].number = i;
		f->r[i].consthold = FALSE;
		f->r[i].constnum = 0;
		f->r[i].isfree = TRUE;
		f->r[i].constpreloaded = FALSE;
	}
}

BOOL constEqueals(Function* f, Constant* a, Constant* b) {
	u08 i;
	if(a->type == b->type) {
		if(a->type == CG_CONST_NUMBER) {
			if(a->number == b->number)
				return TRUE;
		} else if(a->type == CG_CONST_STRING) {
			if(a->string.bplen == b->string.bplen){
				for(i=0; i<a->string.bplen; i++) {
					if(f->code[a->string.bp + i] != f->code[b->string.bp + i]) 
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

	if(f->c == NULL) {
		c->n = 1;
		f->c = c;
		return;
	}

	last = f->c;
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
	c->n = last->n + 1;
}

Constant* pushConstString(Function* f, SString* str) {
	Constant* c = (Constant*)malloc(sizeof(Constant));
	
	c->n = 0;
	c->next = NULL;
	c->type = CG_CONST_STRING;
	c->string.bempty = str->bempty;
	c->string.bp = str->bp;
	c->string.bplen = str->bplen;
	
	c = pushConst(f, c);

	return c;
}

Constant* pushConstNumber(Function* f, float number) {
	Constant* c = (Constant*)malloc(sizeof(Constant));
	
	c->n = 0;
	c->next = NULL;
	c->type = CG_CONST_NUMBER;
	c->number = number;
	
	c = pushConst(f, c);

	return c;
}

Register* getFreeRegister(Function* f) {
	u08 i;
	for(i=0; i<CG_REG_COUNT; i++) {
		if(f->r[i].isfree)
			return &f->r[i];
	}

	return NULL;
}