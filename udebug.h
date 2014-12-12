#ifndef UDEBUG_H
#define UDEBUG_H

#include "vmconfig.h"

#ifdef DEBUGVM

#include <stdio.h>
#include "basetypes.h"
#include "ucodegen.h"


void printFunction(Function *f);
void printToken(Token* t, lu08* code);

#endif

#endif
