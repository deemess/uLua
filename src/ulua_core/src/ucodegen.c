#include <stdlib.h>
#include <stdio.h>
#include "ulua_core/ucodegen.h"
#include "ulua_core/udebug.h"
#include "ulua_core/opcodes.h"
#include "ulua_core/uparser.h"

void initFunction(Function* f, lu08* code) {
	lu08 i;

	f->code = code;
	f->consts = ULUA_NULL;
	f->constsSize = 0;
	f->vars = ULUA_NULL;
	f->subfuncs = ULUA_NULL;
	f->subfuncsSize = 0;
    f->next = ULUA_NULL;
	f->instr = ULUA_NULL;
	f->instrSize = 0;
	f->currentStat = ULUA_NULL;
	f->error_code = E_NONE;
	f->parsed = ULUA_FALSE;
	for(i=0; i<CG_REG_COUNT; i++) {
		f->reg[i].num = i;
		f->reg[i].consthold = ULUA_FALSE;
		f->reg[i].constnum = 0;
		f->reg[i].varnum = 0;
		f->reg[i].isfree = ULUA_TRUE;
		f->reg[i].isload = ULUA_FALSE;
		f->reg[i].islocal = ULUA_FALSE;
		f->reg[i].exprStart = ULUA_NULL;
	}
}

void freeFunction(Function* f) {
    Instruction* i;
    Instruction* li;
    Constant* c;
    Constant* lc;
    Function* sf;
    Function* lsf;
    
    //free all instructions
    i = f->instr;
    while(i != ULUA_NULL) {
        li = i;
        i = i->next;
        free(li);
    }
	f->instr = ULUA_NULL;
    //free all constants
    c = f->consts;
    while(c != ULUA_NULL) {
        lc = c;
        c = c->next;
        free(lc);
    }
	f->consts = ULUA_NULL;
    //free all subfunctions
    sf = f->subfuncs;
    while (sf != ULUA_NULL) {
        freeFunction(sf);
        lsf = sf;
        sf = sf->next;
        free(lsf);
    }
	f->subfuncs = ULUA_NULL;
}

BOOL constEqueals(Function* f, Constant* a, Constant* b) {
	lu08 i;
	if(a->type == b->type) {
		if(a->type == NUMBER_TYPE) {
			if(a->val_number == b->val_number)
				return ULUA_TRUE;
		} else if(a->type == STRING_TYPE) {
			if(a->val_string.bplen == b->val_string.bplen){
				for(i=0; i<a->val_string.bplen; i++) {
					if(f->code[a->val_string.bp + i] != f->code[b->val_string.bp + i]) 
						return ULUA_FALSE;
				}
				return ULUA_TRUE;
			}
		}
	}
	return ULUA_FALSE;
}


Constant* pushConst(Function* f, Constant* c) {
	Constant* last;

	if(f->consts == ULUA_NULL) {
		c->num = 1;
		f->consts = c;
		f->constsSize++;
		return c;
	}

	last = f->consts;
	if(constEqueals(f, last, c)) {
		free(c);
		return last;
	}
	while(last->next != ULUA_NULL) {
		last = last->next;
		if(constEqueals(f, last, c)) {
			free(c);
			return last;
		}
	}

	last->next = c;
	c->num = last->num + 1;
	f->constsSize++;

	return c;
}

Constant* pushVar(Function* f, Constant* c) {
	Constant* last;

	if(f->vars == ULUA_NULL) {
		c->num = 1;
		f->vars = c;
		return c;
	}

	last = f->vars;
	if(constEqueals(f, last, c)) {
		free(c);
		return last;
	}
	while(last->next != ULUA_NULL) {
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
	c->isglobal = ULUA_TRUE;
	c->next = ULUA_NULL;
	c->type = STRING_TYPE;
	c->val_string.bp = str->bp;
	c->val_string.bplen = str->bplen;
	c->val_string.bempty = str->bempty;
	
	c = pushVar(f, c);
	return c;
}

Constant* getVarByNum(Function* f, lu08 num) {
	Constant* v = f->vars;
	while(v != ULUA_NULL && v->num != num) {
		v = v->next;
	}
	return v;
}

Instruction* insertInstruction(Function* f, Instruction* i, Instruction* before) {
	Instruction* tmp;

	if(i == ULUA_NULL) {
		f->error_code = E_NULL_INSTRUCTION;
		return ULUA_NULL;
	}

	if(before == ULUA_NULL) {
		f->error_code = E_NULL_INSTRUCTION;
		return ULUA_NULL;
	}

	tmp = before->prev;
    if(tmp != ULUA_NULL) {
        tmp->next = i;
    } else {
        f->instr = i;
    }
	i->prev = tmp;
	i->next = before;
	before->prev = i;
	f->instrSize++;

	if(f->currentStat == ULUA_NULL)
		f->currentStat = i;

#ifdef DEBUGVM
	printf("insert\t");
	printIntruction(f, i);
#endif

	return before;
}

Instruction* pushInstruction(Function* f, Instruction* i) {
	Instruction* last;

	if(f->instr == ULUA_NULL) {
		f->instr = i;
		i->next = ULUA_NULL;
		i->prev = ULUA_NULL;
		f->instrSize++;

		if(f->currentStat == ULUA_NULL)
			f->currentStat = i;

#ifdef DEBUGVM
	printf("push\t");
	printIntruction(f, i);
#endif
		return i;
	}
	last = f->instr;
	while(last->next != ULUA_NULL) {
		last = last->next;
	}
	last->next = i;
	i->next = ULUA_NULL;
	i->prev = last;
	f->instrSize++;

	if(f->currentStat == ULUA_NULL)
		f->currentStat = i;

#ifdef DEBUGVM
	printf("push\t");
	printIntruction(f, i);
#endif
	return i;
}

Instruction* addInstruction(Function* f, Instruction* i, Instruction* after) {
	Instruction* last;

	if(i == ULUA_NULL) {
		f->error_code = E_NULL_INSTRUCTION;
		return ULUA_NULL;
	}

	if(after == ULUA_NULL) {
		f->error_code = E_NULL_INSTRUCTION;
		return ULUA_NULL;
	}	

	last = after->next;
	after->next = i;
	i->prev = after;
	i->next = last;
	if(last != ULUA_NULL) {
		last->prev = i;
	}

	f->instrSize++;

	if(f->currentStat == ULUA_NULL)
		f->currentStat = i;

#ifdef DEBUGVM
	printf("add\t");
	printIntruction(f, i);
#endif

	return after;
}

Instruction* checkLoad(Function* f, Register* a, Register* ta, BOOL isloadK, Instruction* before) {
	Instruction* i = ULUA_NULL;
	Constant* c;

	if(a == ULUA_NULL) //nothing to load
        return i;

	if(!a->isload) { //make pre loading function
		if(a->consthold) {//constant
			if(isloadK) {//make preload
				i = (Instruction*)malloc(sizeof(Instruction));
				i->i.unpacked.opc = OP_LOADK;
				i->i.unpacked.a = ta->num;
				i->i.unpacked.bx.bx = a->constnum;
				if(before != ULUA_NULL) {
					insertInstruction(f,i,before);
				} else {
					pushInstruction(f,i);
				}
				ta->isload = ULUA_TRUE;
			} else {//just copy constant
				if(ta->num != a->num) {
					ta->consthold = ULUA_TRUE;
					ta->constnum = a->constnum;
				}
			}
		} else {
			if(!a->islocal) {//global variable
				//put global name to constant pool
				c = getVarByNum(f, a->varnum);
				c = pushConstString(f, &c->val_string);
				i = (Instruction*)malloc(sizeof(Instruction));
				i->i.unpacked.opc = OP_GETGLOBAL;
				i->i.unpacked.bx.bx = c->num;
				i->i.unpacked.a = ta->num;
				if(before != ULUA_NULL) {
					insertInstruction(f,i,before);
				} else {
					pushInstruction(f,i);
				}
				ta->varnum = a->varnum;
				ta->isload = ULUA_TRUE;
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
			if(before != ULUA_NULL) {
				insertInstruction(f,i,before);
			} else {
				pushInstruction(f,i);
			}
			ta->isload = ULUA_TRUE;
		}
	}
	ta->islocal = a->islocal;
	return i;
}


void freeRegister(Register* r) {
	r->consthold = ULUA_FALSE;
	r->constnum = 0;
	r->varnum = 0;
	r->isfree = ULUA_TRUE;
	r->isload = ULUA_FALSE;
	r->islocal = ULUA_FALSE;
	r->exprStart = ULUA_NULL;
}

void tryFreeRegister(Register* r) {
    if(r == ULUA_NULL)
        return;
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
	c->isglobal = ULUA_TRUE;
	c->next = ULUA_NULL;
	c->type = STRING_TYPE;
	c->val_string.bempty = str->bempty;
	c->val_string.bp = str->bp;
	c->val_string.bplen = str->bplen;
	
	c = pushConst(f, c);
	return c;
}

Constant* pushConstNumber(Function* f, float number) {
	Constant* c = (Constant*)malloc(sizeof(Constant));
	
	c->num = 0;
	c->isglobal = ULUA_TRUE;
	c->next = ULUA_NULL;
	c->type = NUMBER_TYPE;
	c->val_number = number;
	
	c = pushConst(f, c);

	return c;
}

Register* getFreeRegister(Function* f) {
	lu08 i;
	for(i=0; i<CG_REG_COUNT; i++) {
		if(f->reg[i].isfree) {
			f->reg[i].isfree = ULUA_FALSE;
			return &f->reg[i];
		}
	}
	f->error_code = E_NO_FREE_REGITER;
	return ULUA_NULL;
}

Register* getFreeRegisters(Function* f, lu08 count) {
	lu08 i;
	lu08 j;
	BOOL found;

	for(i=0; i<CG_REG_COUNT; i++) {
		found = ULUA_TRUE;
		if(f->reg[i].isfree) {
			for(j=i+1; j<CG_REG_COUNT && j<i+1+count; j++) {
				if(!f->reg[j].isfree) {
					found = ULUA_FALSE;
					break;
				}
			}
			if(found) {
				for(j=0; j<count+1; j++) {
					f->reg[i+j].isfree = ULUA_FALSE;
				}
				return &f->reg[i];
			}
		}
	}
	f->error_code = E_NO_FREE_REGITER;
	return ULUA_NULL;
}

Register* getVarRegister(Function* f, Constant* var) {
	lu08 i;
	Register* r;

	for(i=0; i<CG_REG_COUNT; i++){
		if(f->reg[i].varnum == var->num)
			return &f->reg[i];
	}

	r = getFreeRegister(f);
	r->isfree = ULUA_FALSE;
	r->varnum = var->num;

	return r;
}

void unloadRegisters(Function* f) {//unload/mark registers
    lu08 i;
    for(i=0; i<CG_REG_COUNT; i++){
        if(f->reg[i].varnum > 0) //unload only variable registers
            f->reg[i].isload = ULUA_FALSE;
    }
}

Register* doNot(Function* f, Register* a, Token* t) { //do not\minus logic
	Instruction* i;
	Register* res;

	checkLoad(f, a, a, ULUA_TRUE, ULUA_NULL);
	res = getFreeRegister(f);

	i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = t->token == TK_NOT ? OP_NOT : OP_UNM;
	i->i.unpacked.a = res->num;
	i->i.unpacked.bx.l.b = a->num;
	i->i.unpacked.bx.l.c = 0;
	pushInstruction(f,i);

	tryFreeRegister(a);

	res->isload = ULUA_TRUE;
	return res;
}

Register* doLogic(Function* f, Register* a, Register* b, Token* t) {
	Register* res;
	Instruction* i;

	checkLoad(f, a, a, ULUA_TRUE, ULUA_NULL);
	checkLoad(f, b, b, ULUA_TRUE, ULUA_NULL);
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

	res->isload = ULUA_TRUE;
	return res;
}

Register* doCompare(Function* f, Register* a, Register* b, Token* t) {
	Register* res;
	Instruction* i = (Instruction*)malloc(sizeof(Instruction));
	res = getFreeRegister(f);
	
	checkLoad(f, a, a, ULUA_FALSE, ULUA_NULL);
	checkLoad(f, b, b, ULUA_FALSE, ULUA_NULL);

	//generate skip next instruction if true
	i->i.unpacked.a = 1; //do not skip next instruction if comparison valid
	i->i.unpacked.bx.l.b = a->consthold ? a->constnum + CG_REG_COUNT : a->num;
	i->i.unpacked.bx.l.c = b->consthold ? b->constnum + CG_REG_COUNT : b->num;
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
			i->i.unpacked.bx.l.b = b->consthold ? b->constnum + CG_REG_COUNT : b->num;
			i->i.unpacked.bx.l.c = a->consthold ? a->constnum + CG_REG_COUNT : a->num;
			break;
		case TK_GE:
			i->i.unpacked.opc = OP_LE;
			i->i.unpacked.bx.l.b = b->consthold ? b->constnum + CG_REG_COUNT : b->num;
			i->i.unpacked.bx.l.c = a->consthold ? a->constnum + CG_REG_COUNT : a->num;
			break;
		case TK_NE:
			i->i.unpacked.opc = OP_EQ;
			i->i.unpacked.bx.l.b = b->consthold ? b->constnum + CG_REG_COUNT : b->num;
			i->i.unpacked.bx.l.c = a->consthold ? a->constnum + CG_REG_COUNT : a->num;
			i->i.unpacked.a = 0;
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
	i->i.unpacked.bx.l.b = 0; //false
	i->i.unpacked.bx.l.c = 0; //do not skip next instruction
	pushInstruction(f, i);
	res->isload = ULUA_TRUE;

	tryFreeRegister(a);
	tryFreeRegister(b);

	return res;
}

Register* doMath(Function* f, Register* a, Register* b, Token* t) {
	Register* r;
	Instruction* i = (Instruction*)malloc(sizeof(Instruction));
	
	checkLoad(f, a, a, ULUA_FALSE, ULUA_NULL);
	checkLoad(f, b, b, ULUA_FALSE, ULUA_NULL);

	//r = a->islocal ? getFreeRegister(f) : a;
	r = getFreeRegister(f);

	i->i.unpacked.a = r->num;
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
	tryFreeRegister(a);
	r->consthold = ULUA_FALSE;
	r->constnum = 0;
	r->varnum = 0;
	r->islocal = ULUA_FALSE;
	r->isload = ULUA_TRUE;
	
	return r;
}

Register* doNil(Function* f) { //allocate register and load nil in it
	Register* res;
	Instruction* i = (Instruction*)malloc(sizeof(Instruction));

	res = getFreeRegister(f);
	i->i.unpacked.opc = OP_LOADNIL;
	i->i.unpacked.a = res->num; //from register
	i->i.unpacked.bx.l.b = res->num; //to register
	i->i.unpacked.bx.l.c = 0; //nothing
	pushInstruction(f, i);
	res->isload = ULUA_TRUE;
	return res;
}

Register* doBoolean(Function* f, Token* t) { //allocate register and load bool value in it
	Register* res;
	Instruction* i = (Instruction*)malloc(sizeof(Instruction));

	res = getFreeRegister(f);
	i->i.unpacked.opc = OP_LOADBOOL;
	i->i.unpacked.a = res->num;
	i->i.unpacked.bx.l.b = t->token == TK_TRUE ? 1 : 0; //boolean value
	i->i.unpacked.bx.l.c = 0; //do not skip next instruction
	pushInstruction(f, i);
	res->isload = ULUA_TRUE;
	return res;
}

Instruction* statWHILE(Function* f, Register* a, Instruction* block) { //make while block
	Instruction* i;
	Instruction* tmp;
	Instruction* last;
	Instruction* result;
	lu16 count = 0;

	checkLoad(f, a, a, ULUA_TRUE, block);

	//make register test and skip WHILE block if true
	i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = OP_TEST;//load false in result register
	i->i.unpacked.a = a->num;
	i->i.unpacked.bx.l.b = 0;
	i->i.unpacked.bx.l.c = 1; // if false for OR instruction and true for AND instruction
	insertInstruction(f, i, block);

	i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = OP_JMP;//skip while block block
	i->i.unpacked.bx.bx = 1;
	insertInstruction(f, i, block);

	if(a->exprStart == ULUA_NULL)
		a->exprStart = f->currentStat;
	//check if given block is not null. If NULL - we have a problem in parser
	if(block == ULUA_NULL || a->exprStart == ULUA_NULL) {
		f->error_code = E_NULL_INSTRUCTION;
		return ULUA_NULL;
	}
	//size instructions to skip
	tmp = block;
	count++;
	while(tmp->next != ULUA_NULL) {
		count++; 
		tmp = tmp->next;
	}
	i->i.unpacked.bx.bx = ++count; // +1 for final jump instruction
	last = tmp;

	//size expression block
	tmp = a->exprStart;
	count++;
	while(tmp->next != ULUA_NULL && tmp->next != block) {
		count++; 
		tmp = tmp->next;
	}
	i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = OP_JMP;//repeat while iteration from expression start
	i->i.unpacked.bx.bx = -count; //repeat while block + expressions

	//add 1 jump to the end of the "then" to use it in future to jump over "else" or "elseif"
	addInstruction(f, i, last);

	result = a->exprStart;
	tryFreeRegister(a);
	return result;
}

Instruction* statTHEN(Function* f, Register* a, Instruction* block) {
	Instruction* i;
	Instruction* tmp;
	Instruction* first;
	lu16 count = 1;

	checkLoad(f, a, a, ULUA_TRUE, block);

	//make register test and skip THEN block if false
	i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = OP_TEST;//load false in result register
	i->i.unpacked.a = a->num;
	i->i.unpacked.bx.l.b = 0;
	i->i.unpacked.bx.l.c = 1; // if false for OR instruction and true for AND instruction
	insertInstruction(f, i, block);
	first = i;

	i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = OP_JMP;//skip THEN block
	i->i.unpacked.bx.bx = 1;
	insertInstruction(f, i, block);

	//check if given block is not null. If NULL - we have a problem in parser
	if(block == ULUA_NULL) {
		f->error_code = E_NULL_INSTRUCTION;
		return ULUA_NULL;
	}
	//size instructions to skip
	tmp = block;
	while(tmp->next != ULUA_NULL) {
		count++; 
		tmp = tmp->next;
	}
	i->i.unpacked.bx.bx = count+1;

	i = (Instruction*)malloc(sizeof(Instruction));
	i->i.unpacked.opc = OP_JMP;//exit from if - will be amended in the next
	i->i.unpacked.bx.bx = 0;

	//add 1 jump to the end of the "then" to use it in future to jump over "else" or "elseif"
	addInstruction(f, i, tmp);

	tryFreeRegister(a);
	return first;
}

Instruction* statELSE(Function* f, Instruction* condlist, Instruction* block) { //make else block
	Instruction* tmp;
	Instruction* first;
	Instruction* jmp;
	lu16 count = 1;
	lu16 countprejump = 0;

	first = condlist;
	//find last instruction in condlist and make jump over "else" block
	while(first->next != block && first->next != ULUA_NULL)
		first = first->next;

	//size instructions to skip
	if(block != ULUA_NULL) {
		tmp = block;
		while(tmp->next != ULUA_NULL) {
			count++; 
			tmp = tmp->next;
		}
	}

	//find last jump and amend it
	while(first->i.unpacked.opc != OP_JMP && first != condlist) {
		first = first->prev;
	}

	//check if we found jump
	if(first->i.unpacked.opc == OP_JMP) {
		jmp = first;
		while(first->next != block && first->next != ULUA_NULL) {
			first = first->next;
			countprejump++;
		}
		jmp->i.unpacked.bx.bx = count + countprejump;
	}

	//i = (Instruction*)malloc(sizeof(Instruction));
	//i->i.unpacked.opc = OP_JMP;//skip THEN block
	//i->i.unpacked.bx.bx = size;
	//insertInstruction(f, i, tmp);

	return condlist;
}

Instruction* statELSEIF(Function* f, Instruction* condlist, Instruction* cond){ //make elseif block
	Instruction* tmp;
	Instruction* first;
	Instruction* jmp;
	lu16 count = 0;
	lu16 countprejump = 0;

	first = condlist;
	//find last instruction in condlist and make jump over "else" block
	while(first->next != cond && first->next != ULUA_NULL)
		first = first->next;

	//size instructions to skip
	if(cond != ULUA_NULL) {
		tmp = cond;
		while(tmp->next != ULUA_NULL) {
			count++; 
			tmp = tmp->next;
		}
	}

	//find last jump and amend it
	while(first->i.unpacked.opc != OP_JMP && first != condlist) {
		first = first->prev;
	}

	//check if we found jump
	if(first->i.unpacked.opc == OP_JMP) {
		jmp = first;
		while(first->next != cond && first->next != ULUA_NULL) {
			first = first->next;
			countprejump++;
		}
		jmp->i.unpacked.bx.bx = count + countprejump;
	}

	//i = (Instruction*)malloc(sizeof(Instruction));
	//i->i.unpacked.opc = OP_JMP;//skip THEN block
	//i->i.unpacked.bx.bx = size;
	//insertInstruction(f, i, tmp);

	return condlist;
}

Instruction* statSET(Function* f, Register* a, Register* b, BOOL islocal) {
	Instruction* i;
	Constant* c;

	if(islocal) { //local variable - just load
		i = checkLoad(f, b, a, ULUA_TRUE, ULUA_NULL);
		a->islocal = ULUA_TRUE;
		tryFreeRegister(b);
	} else {//global variable
		checkLoad(f, b, a, ULUA_TRUE, ULUA_NULL);
		i = (Instruction*)malloc(sizeof(Instruction));
		c = getVarByNum(f, a->varnum);
		c = pushConstString(f, &c->val_string);
		//register or preloaded constant
		i->i.unpacked.opc = OP_SETGLOBAL;
		i->i.unpacked.a = a->num;//register number
		i->i.unpacked.bx.bx = c->num;//global const name number

		tryFreeRegister(b);
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
	checkLoad(f,a,ta,ULUA_TRUE, ULUA_NULL);

	//ARGUMENTS B reg
	checkLoad(f,b,tb,ULUA_TRUE, ULUA_NULL);

	i->i.unpacked.opc = OP_CALL;
	i->i.unpacked.a = ta->num;
	i->i.unpacked.bx.l.b = 2;
	i->i.unpacked.bx.l.c = 1; //TODO: support function return result
	pushInstruction(f, i);

	//free all registers
	freeRegister(ta);
	freeRegister(tb);
	tryFreeRegister(a);
	tryFreeRegister(b);

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


void dumpFunction(Function* f, writeBytes write) {
	lu08 buff[6];
	Instruction *i;
	Constant* c;
	Function* fp;
	lu08 j;

	//dump instructions
	((lu16*)buff)[0] = f->instrSize; write(buff, 2);
	i = f->instr;
	while(i != ULUA_NULL) {
		((lu32*)buff)[0] = i->i.packed; write(buff, 4);
		i = i->next;
	}

	//dump constants
	((lu16*)buff)[0] = f->constsSize; write(buff, 2);
	c = f->consts;
	while(c != ULUA_NULL) {
		buff[0] = c->type; write(buff, 1); //const type
		switch(c->type) {
			case NUMBER_TYPE: //number
				((float*)buff)[0] = c->val_number ; write(buff, sizeof(float));
				break;
			case STRING_TYPE: //string
				((lu16*)buff)[0] = c->val_string.bplen + 1; write(buff, 2); //string len
				for(j=0; j < c->val_string.bplen; j++) {
					buff[0] = f->code[c->val_string.bp+j]; write(buff, 1); //string char
				}
				buff[0] = 0; write(buff, 1); //end of ANSI string
				break;
		}
		c = c->next;
	}

	//dump functions
	((lu16*)buff)[0] = f->subfuncsSize; write(buff, 2);
	fp = f->subfuncs;
	while(fp != ULUA_NULL) {
		dumpFunction(fp, write);
	}
	
}

void dump(Function* f, writeBytes callback) {
	lu08 buff[6];

	//write header
	buff[0] = 0x1B; //"Lua"
	buff[1] = 0x4C; 
	buff[2] = 0x75; 
	buff[3] = 0x61; 
	buff[4] = 0x51; //version Lua 5.1
	buff[5] = 0x10; //format 0x10 means ulua_repl v.1.0
	callback(buff, 6); //write header

	//dump top level function
	dumpFunction(f, callback);
}

