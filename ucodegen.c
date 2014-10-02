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
		f->reg[i].isload = FALSE;
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
	c->isglobal = TRUE;
	c->next = NULL;
	c->type = CG_CONST_STRING;
	c->val_string.bempty = str->bempty;
	c->val_string.bp = str->bp;
	c->val_string.bplen = str->bplen;
	
	c = pushVar(f, c);
	return c;
}

Constant* getVarByNum(Function* f, u08 num) {
	Constant* v = f->vars;
	while(v != NULL && v->num != num) {
		v = v->next;
	}
	return v;
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
	c->isglobal = TRUE;
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
	c->isglobal = TRUE;
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

Register* getFreeRegisters(Function* f, u08 count) {
	u08 i;
	u08 j;
	BOOL found;

	for(i=0; i<CG_REG_COUNT; i++) {
		found = TRUE;
		if(f->reg[i].isfree) {
			for(j=i+1; j<CG_REG_COUNT && j<i+1+count; j++) {
				if(!f->reg[j].isfree) {
					found = FALSE;
					break;
				}
			}
			if(found) {
				for(j=0; j<count+1; j++) {
					f->reg[i+j].isfree = FALSE;
				}
				return &f->reg[i];
			}
		}
	}
	return NULL;
}

void freeRegister(Register* r) {
	r->consthold = FALSE;
	r->constnum = 0;
	r->varnum = 0;
	r->isfree = TRUE;
	r->isload = FALSE;
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

Register* doLogic(Function* f, Register* a, Register* b, Token* t) {

	return a;
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
	a->isload = TRUE;
	
	return a;
}

Instruction* statSET(Function* f, Register* a, Register* b, BOOL islocal) {
	Constant* c;
	Instruction* i = (Instruction*)malloc(sizeof(Instruction));

	if(islocal) { //local var
		if(b->consthold && !b->constpreloaded) {
			//constant
			i->opc = OP_LOADK;
			i->a = a->num;
			i->b = b->constnum;
			pushInstruction(f,i);
			b->constpreloaded = TRUE;
		} else {
			//register
			i->opc = OP_MOVE;
			i->a = a->num;
			i->b = b->num;
			pushInstruction(f,i);
		}
		
	} else {//global var
		//put global name to constant pool
		c = getVarByNum(f, a->varnum);
		c = pushConstString(f, &c->val_string);
		if(b->consthold && !b->constpreloaded) {
			//constant
			i->opc = OP_LOADK;
			i->a = b->num;
			i->b = b->constnum;
			pushInstruction(f,i);
			i = (Instruction*)malloc(sizeof(Instruction));
		} 
		//register or preloaded constant
		i->opc = OP_SETGLOBAL;
		i->a = c->num;//global const name number
		i->b = b->num;//register number
		pushInstruction(f,i);
	}

	a->isload = TRUE;
	freeRegister(b);
	return i;
}

Register* functionCALL(Function* f, Register* a, Register* b) {
	//TODO: support more than 1 arg function call
	Constant* c;
	Register* ta;
	Register* tb;
	Instruction* i = (Instruction*)malloc(sizeof(Instruction));
	
	//allocate registers for call
	ta = getFreeRegisters(f, 1);
	tb = &f->reg[ta->num + 1];

	//FUNCTION A reg
	if(!a->isload) { //make pre loading function
		//put global name to constant pool
		c = getVarByNum(f, a->varnum);
		c = pushConstString(f, &c->val_string);
		i->opc = OP_GETGLOBAL;
		i->a = ta->num;
		i->b = c->num;
		pushInstruction(f, i);
		a->isload = TRUE;
		i = (Instruction*)malloc(sizeof(Instruction));
	} else {
		//move function to target register
		i->opc = OP_MOVE;
		i->a = ta->num;
		i->b = a->num;
		pushInstruction(f, i);
		i = (Instruction*)malloc(sizeof(Instruction));
	}

	//ARGUMENTS B reg
	if(!b->isload) {//make arg pre loading
		if(b->consthold) {//constant
			//constant
			i->opc = OP_LOADK;
			i->a = tb->num;
			i->b = b->constnum;
			pushInstruction(f,i);
			b->isload = TRUE;
			i = (Instruction*)malloc(sizeof(Instruction));
		} else {//global variable
			c = getVarByNum(f, b->varnum);
			c = pushConstString(f, &c->val_string);
			i->opc = OP_GETGLOBAL;
			i->a = tb->num;
			i->b = c->num;
			b->isload = TRUE;
			pushInstruction(f, i);
			i = (Instruction*)malloc(sizeof(Instruction));
		}
	} else {
		//move argument to target register
		i->opc = OP_MOVE;
		i->a = tb->num;
		i->b = b->num;
		pushInstruction(f, i);
		i = (Instruction*)malloc(sizeof(Instruction));
	}

	ta->isload = TRUE;
	tb->isload = TRUE;

	i->opc = OP_CALL;
	i->a = ta->num;
	i->b = 1;
	i->c = 1; //TODO: support function return result
	pushInstruction(f, i);
	//free all registers
	freeRegister(ta);
	freeRegister(tb);

	return a;
}

