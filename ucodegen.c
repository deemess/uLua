#include "ucodegen.h"
#include <stdio.h>

void initFunction(Function* f, u08* code) {
	u08 i;

	f->code = code;
	f->consts = NULL;
	f->vars = NULL;
	f->subfuncs = NULL;
	f->instr = NULL;
	f->error_code = E_NONE;
	for(i=0; i<CG_REG_COUNT; i++) {
		f->reg[i].num = i;
		f->reg[i].consthold = FALSE;
		f->reg[i].constnum = 0;
		f->reg[i].varnum = 0;
		f->reg[i].isfree = TRUE;
		f->reg[i].isload = FALSE;
		f->reg[i].islocal = FALSE;
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

Instruction* insertInstruction(Function* f, Instruction* i, Instruction* before) {
	Instruction* tmp;

	if(i == NULL) {
		f->error_code = E_NULL_INSTRUCTION;
		return NULL;
	}

	if(before == NULL) {
		f->error_code = E_NULL_INSTRUCTION;
		return NULL;
	}

	tmp = before->prev;
	tmp->next = i;
	i->prev = tmp;
	i->next = before;
	before->prev = i;

	return before;
}

Instruction* pushInstruction(Function* f, Instruction* i) {
	Instruction* last;

	if(f->instr == NULL) {
		f->instr = i;
		i->next = NULL;
		i->prev = NULL;
		return i;
	}
	last = f->instr;
	while(last->next != NULL) {
		last = last->next;
	}
	last->next = i;
	i->next = NULL;
	i->prev = last;

	return i;
}

Instruction* checkLoad(Function* f, Register* a, Register* ta, BOOL isloadK) {
	Instruction* i = NULL;
	Constant* c;

	if(!a->isload) { //make pre loading function
		if(a->consthold) {//constant
			if(isloadK) {//make preload
				i = (Instruction*)malloc(sizeof(Instruction));
				i->i.unpacked.opc = OP_LOADK;
				i->i.unpacked.a = ta->num;
				i->i.unpacked.bx.l.b = a->constnum;
				pushInstruction(f,i);
				ta->isload = TRUE;
			} else {//just copy constant
				if(ta->num != a->num) {
					ta->consthold = TRUE;
					ta->constnum = a->constnum;
					ta->constpreloaded = FALSE;
				}
			}
		} else {
			if(!a->islocal) {//global variable
				//put global name to constant pool
				c = getVarByNum(f, a->varnum);
				c = pushConstString(f, &c->val_string);
				i = (Instruction*)malloc(sizeof(Instruction));
				i->i.unpacked.opc = OP_GETGLOBAL;
				i->i.unpacked.bx.l.b = c->num;
				i->i.unpacked.a = ta->num;
				pushInstruction(f, i);
				ta->varnum = a->varnum;
				ta->isload = TRUE;
			} else {//uninitialized local variable - error
				f->error_code = E_NOTINIT_LOCAL;
			}
		}
	} else {
		if(a->num != ta->num) {//move function to target register
			i = (Instruction*)malloc(sizeof(Instruction));
			i->i.unpacked.opc = OP_MOVE;
			i->i.unpacked.a = ta->num;
			i->i.unpacked.bx.l.b = a->num;
			pushInstruction(f, i);
			ta->isload = TRUE;
		}
	}
	ta->islocal = a->islocal;
	return i;
}


void freeRegister(Register* r) {
	r->consthold = FALSE;
	r->constnum = 0;
	r->varnum = 0;
	r->isfree = TRUE;
	r->isload = FALSE;
	r->islocal = FALSE;
	r->constpreloaded = FALSE;
}

void tryFreeRegister(Register* r) {
	if(r->isload) { //if register holds some value
		if(r->varnum > 0) //and refer to variable
			return; //then keep it
	}
	freeRegister(r);//else free register
}
//public functions ---------------------------------------------------------------------

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
	f->error_code = E_NO_FREE_REGITER;
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
	f->error_code = E_NO_FREE_REGITER;
	return NULL;
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
	Register* res;
	Instruction* i;

	checkLoad(f, a, a, TRUE);
	checkLoad(f, b, b, TRUE);
	res = getFreeRegister(f);
	
	i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = OP_LOADBOOL;
	i->i.unpacked.a = res->num;
	i->i.unpacked.bx.l.b = t->token == TK_OR ? 0 : 1;//load false in result register for OR else true for AND
	i->i.unpacked.bx.l.c = 0;
	pushInstruction(f,i);

	i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = OP_TESTSET;//load false in result register
	i->i.unpacked.a = res->num;
	i->i.unpacked.bx.l.b = a->num;
	i->i.unpacked.bx.l.c = t->token == TK_OR ? 0 : 1; // false for OR instruction and true for AND instruction
	pushInstruction(f,i);

	i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = OP_JMP;//skip next check
	i->i.unpacked.bx.bx = 1;
	pushInstruction(f,i);

	i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = OP_MOVE;//load false in result register
	i->i.unpacked.a = res->num;
	i->i.unpacked.bx.l.b = b->num;
	pushInstruction(f,i);

	tryFreeRegister(a);
	tryFreeRegister(b);

	res->isload = TRUE;
	return res;
}

Register* doCompare(Function* f, Register* a, Register* b, Token* t) {
	Register* res;
	Instruction* i = (Instruction*)malloc(sizeof(Instruction));
	res = getFreeRegister(f);
	
	checkLoad(f, a, a, TRUE);
	checkLoad(f, b, b, TRUE);

	//generate skip next instruction if true
	i->i.unpacked.a = 0; //do not skip next instruction if comparison valid
	i->i.unpacked.bx.l.b = a->num;
	i->i.unpacked.bx.l.c = b->num;
	switch(t->token)
	{
		case TK_L:
			i->i.unpacked.opc = OP_LT;
			break;
		case TK_LE:
			i->i.unpacked.opc = OP_LE;
			break;
		case TK_EQ:
			i->i.unpacked.opc = OP_EQ;
			break;
		case TK_G:
			i->i.unpacked.opc = OP_LT;
			i->i.unpacked.bx.l.b = b->num;
			i->i.unpacked.bx.l.c = a->num;
			break;
		case TK_GE:
			i->i.unpacked.opc = OP_LE;
			i->i.unpacked.bx.l.b = b->num;
			i->i.unpacked.bx.l.c = a->num;
			break;
		case TK_NE:
			i->i.unpacked.opc = OP_EQ;
			i->i.unpacked.bx.l.b = b->num;
			i->i.unpacked.bx.l.c = a->num;
			break;
	}
	pushInstruction(f, i);

	//generate instructions to load true and false
	i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = OP_LOADBOOL;
	i->i.unpacked.a = res->num;
	i->i.unpacked.bx.l.b = 1; //true
	i->i.unpacked.bx.l.c = 1; //skip next instruction
	pushInstruction(f, i);
	i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = OP_LOADBOOL;
	i->i.unpacked.a = res->num;
	i->i.unpacked.bx.l.b = 0; //true
	i->i.unpacked.bx.l.c = 0; //do not skip next instruction
	pushInstruction(f, i);
	res->isload = TRUE;

	tryFreeRegister(a);
	tryFreeRegister(b);

	return res;
}

Register* doMath(Function* f, Register* a, Register* b, Token* t) {
	Instruction* i = (Instruction*)malloc(sizeof(Instruction));
	
	checkLoad(f, a, a, FALSE);
	checkLoad(f, b, b, FALSE);

	i->i.unpacked.a = a->num;
	i->i.unpacked.bx.l.b = a->consthold ? a->constnum + CG_REG_COUNT : a->num;
	i->i.unpacked.bx.l.c = b->consthold ? b->constnum + CG_REG_COUNT : b->num;

	switch(t->token) {
		case TK_PLUS:
			i->i.unpacked.opc = OP_ADD;
			break;
		case TK_MINUS:
			i->i.unpacked.opc = OP_SUB;
			break;
		case TK_TIMES:
			i->i.unpacked.opc = OP_MUL;
			break;
		case TK_DIVIDE:
			i->i.unpacked.opc = OP_DIV;
			break;
		case TK_MOD:
			i->i.unpacked.opc = OP_MOD;
			break;
		case TK_POW:
			i->i.unpacked.opc = OP_POW;
			break;
	}
	pushInstruction(f,i);

	tryFreeRegister(b);
	a->consthold = FALSE;
	a->constpreloaded = FALSE;
	a->constnum = 0;
	a->varnum = 0;
	a->islocal = FALSE;
	a->isload = TRUE;
	
	return a;
}

Instruction* statTHEN(Function* f, Register* a, Instruction* block) {
	Instruction* i;
	Instruction* tmp;
	u16 count = 0;

	//make register test and skip THEN block if false
	i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = OP_TEST;//load false in result register
	i->i.unpacked.a = a->num;
	i->i.unpacked.bx.l.b = 0;
	i->i.unpacked.bx.l.c = 0; // if false for OR instruction and true for AND instruction
	tmp = insertInstruction(f, i, block);

	i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = OP_JMP;//skip THEN block
	i->i.unpacked.bx.bx = 1;
	insertInstruction(f, i, tmp);

	//count instructions to skip
	tmp = block->next;
	while(tmp != NULL) {
		count++;
		tmp = tmp->next;
	}
	i->i.unpacked.bx.bx = count+1;

	tryFreeRegister(a);
	return i;
}

Instruction* statSET(Function* f, Register* a, Register* b, BOOL islocal) {
	Instruction* i;
	Constant* c;

	if(islocal) { //local variable - just load
		i = checkLoad(f, b, a, TRUE);
		a->islocal = TRUE;
		tryFreeRegister(b);
	} else {//global variable
		checkLoad(f, b, b, TRUE);
		i = (Instruction*)malloc(sizeof(Instruction));
		c = getVarByNum(f, a->varnum);
		c = pushConstString(f, &c->val_string);
		//register or preloaded constant
		i->i.unpacked.opc = OP_SETGLOBAL;
		i->i.unpacked.a = b->num;//register number
		i->i.unpacked.bx.l.b = c->num;//global const name number
		b->isload = TRUE;
		b->varnum = a->varnum;
		b->consthold = FALSE;
		b->constpreloaded = FALSE;
		b->constnum = 0;
		tryFreeRegister(a);
		pushInstruction(f,i);
	}
	return i;
}

Instruction* functionCALL(Function* f, Register* a, Register* b) {
	//TODO: support more than 1 arg function call
	Register* ta;
	Register* tb;
	Instruction* i = (Instruction*)malloc(sizeof(Instruction));
	
	//allocate registers for call
	ta = getFreeRegisters(f, 1);
	tb = &f->reg[ta->num + 1];

	//FUNCTION A reg
	checkLoad(f,a,ta,TRUE);

	//ARGUMENTS B reg
	checkLoad(f,b,tb,TRUE);

	i->i.unpacked.opc = OP_CALL;
	i->i.unpacked.a = ta->num;
	i->i.unpacked.bx.l.b = 1;
	i->i.unpacked.bx.l.c = 1; //TODO: support function return result
	pushInstruction(f, i);

	//free all registers
	freeRegister(ta);
	freeRegister(tb);

	return i;
}

Instruction*  doReturn(Function* f)  {
	Instruction* i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = OP_RETURN;
	i->i.unpacked.a = 0;
	i->i.unpacked.bx.l.b = 1;//TODO: support multiple return result
	pushInstruction(f, i);

	return i;
}