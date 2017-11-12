#ifndef UPARSER_STATIC_H
#define UPARSER_STATIC_H

#include <stddef.h>
#include "ulua_core/ulexer.h"
#include "ulua_core/ucodegen.h"

void *ParseAlloc(void *(*mallocProc)(size_t));
void ParseFree(void *p, void (*freeProc)(void*));
void Parse(void *yyp, int yymajor, Token yyminor, Function* f);

#endif