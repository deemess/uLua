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

void pushConst(Function* f, Constant* c) {
	Constant* last;

	if(f->c == NULL) {
		c->n = 1;
		f->c = c;
		return;
	}

	last = f->c;
	while(last->next != NULL) {
		last = last->next;
	}

	c->n = last->n + 1;
	last->next = c;
}

Constant* pushConstString(Function* f, SString* str) {
	Constant* c = (Constant*)malloc(sizeof(Constant));
	
	c->n = 0;
	c->next = NULL;
	c->type = CG_CONST_STRING;
	c->string.bempty = str->bempty;
	c->string.bp = str->bp;
	c->string.bplen = str->bplen;
	
	pushConst(f, c);

	return c;
}

Constant* pushConstNumber(Function* f, float number) {
	Constant* c = (Constant*)malloc(sizeof(Constant));
	
	c->n = 0;
	c->next = NULL;
	c->type = CG_CONST_NUMBER;
	c->number = number;
	
	pushConst(f, c);

	return c;
}
