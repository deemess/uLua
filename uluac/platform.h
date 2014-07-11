#ifndef PLATFORM_H
#define PLATFORM_H

#include "lcommon.h"
#include "basetypes.h"

#define luaS_new(L, s)	(luaS_newlstr(L, s, strlen(s)))
#define luaM_new(L,t)		cast(t *, luaM_malloc(sizeof(t)))



void initPlatform(const char* filename);
void * luaZ_resizebuffer(lua_State *L, void * block , size_t newsize);
TString* luaS_newlstr(lua_State *L, const char *str, size_t l);
void * luaM_malloc(size_t t);
int luaZ_fill (Zio *z);
int luaO_str2d (const char *s, double *result);

#endif
