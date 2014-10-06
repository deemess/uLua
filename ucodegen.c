#include "ucodegen.h"
#include <stdio.h>

void initFunction(Function* f, u08* code) {
	u08 i;

	f->code = code;
	f->consts = NULL;
	f->vars = NULL;
	f->subfuncs = NULL;
	f->instr = NULL;
	f->error_code = 0;
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

Instruction* checkLoad(Function* f, Register* a, Register* ta, BOOL isloadK) {
	Instruction* i = NULL;
	Constant* c;

	if(!a->isload) { //make pre loading function
		if(a->consthold) {//constant
			if(isloadK) {//make preload
				i = (Instruction*)malloc(sizeof(Instruction));
				i->opc = OP_LOADK;
				i->a = ta->num;
				i->b = a->constnum;
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
				i->opc = OP_GETGLOBAL;
				i->b = c->num;
				i->a = ta->num;
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
			i->opc = OP_MOVE;
			i->a = ta->num;
			i->b = a->num;
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
	i->opc = OP_LOADBOOL;
	i->a = res->num;
	i->b = t->token == TK_OR ? 0 : 1;//load false in result register for OR else true for AND
	i->c = 0;
	pushInstruction(f,i);

	i = (Instruction*)malloc(sizeof(Instruction));
	i->opc = OP_TESTSET;//load false in result register
	i->a = res->num;
	i->b = a->num;
	i->c = t->token == TK_OR ? 0 : 1; // false for OR instruction and true for AND instruction
	pushInstruction(f,i);

	i = (Instruction*)malloc(sizeof(Instruction));
	i->opc = OP_JMP;//skip next check
	i->bx = 1;
	pushInstruction(f,i);

	i = (Instruction*)malloc(sizeof(Instruction));
	i->opc = OP_MOVE;//load false in result register
	i->a = res->num;
	i->b = b->num;
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
	i->a = 0; //do not skip next instruction if comparison valid
	i->b = a->num;
	i->c = b->num;
	switch(t->token)
	{
		case TK_L:
			i->opc = OP_LT;
			break;
		case TK_LE:
			i->opc = OP_LE;
			break;
		case TK_EQ:
			i->opc = OP_EQ;
			break;
		case TK_G:
			i->opc = OP_LT;
			i->b = b->num;
			i->c = a->num;
			break;
		case TK_GE:
			i->opc = OP_LE;
			i->b = b->num;
			i->c = a->num;
			break;
		case TK_NE:
			i->opc = OP_EQ;
			i->b = b->num;
			i->c = a->num;
			break;
	}
	pushInstruction(f, i);

	//generate instructions to load true and false
	i = (Instruction*)malloc(sizeof(Instruction));
	i->opc = OP_LOADBOOL;
	i->a = res->num;
	i->b = 1; //true
	i->c = 1; //skip next instruction
	pushInstruction(f, i);
	i = (Instruction*)malloc(sizeof(Instruction));
	i->opc = OP_LOADBOOL;
	i->a = res->num;
	i->b = 0; //true
	i->c = 0; //do not skip next instruction
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

	i->a = a->num;
	i->b = a->consthold ? a->constnum + CG_REG_COUNT : a->num;
	i->c = b->consthold ? b->constnum + CG_REG_COUNT : b->num;

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

	tryFreeRegister(b);
	a->consthold = FALSE;
	a->constpreloaded = FALSE;
	a->constnum = 0;
	a->varnum = 0;
	a->islocal = FALSE;
	a->isload = TRUE;
	
	return a;
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
		i->opc = OP_SETGLOBAL;
		i->a = b->num;//register number
		i->b = c->num;//global const name number
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

Register* functionCALL(Function* f, Register* a, Register* b) {
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

Instruction*  doReturn(Function* f)  {
	Instruction* i = (Instruction*)malloc(sizeof(Instruction));
	i->opc = OP_RETURN;
	i->a = 0;
	i->b = 1;//TODO: support multiple return result
	pushInstruction(f, i);

	return i;
}