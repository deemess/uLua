#include <stdio.h>
#include "platform.h"
#include "lcommon.h"

u08 buff[1024*1024];
u32 bufflen=0;
u32 buffpos=0;

void initPlatform(const char* filename) {
	FILE* file = fopen(filename, "r");
	bufflen = (u32)fread(buff, 1, 1024*1024, file);
	buffpos = 0;
}

void * luaZ_resizebuffer(lua_State *L, Mbuffer * block , size_t newsize) {
	//TODO: implement first (memory allocation)
	void * newmem = malloc(newsize);
	block->buffer = (char *)newmem;
	block->buffsize = newsize;
	return newmem;
}

TString* luaS_newlstr(lua_State *L, const char *str, size_t l) {
	//TODO: implement first (string allocation in memory)
	TString* newstr = (TString*)malloc(sizeof(TString));
	newstr->tsv.strpt = (char*)str;
	newstr->tsv.len = l;
	newstr->tsv.hash = 0;

	return newstr;
}


void * luaM_malloc(size_t t) {
	return malloc(t);
}

int luaZ_fill (Zio *z) {
//TODO: implement first (streaming from input (code string))
  //size_t size;
  //lua_State *L = z->L;
  //const char *buff;
  //lua_unlock(L);
  //buff = z->reader(L, z->data, &size);
  //lua_lock(L);
  if (buff == NULL || bufflen == 0) return EOZ;
  z->n = bufflen - 1;
  z->p = (const char*)&buff[0];
  return char2int(*(z->p++));
}



int luaO_str2d (const char *s, double *result) {
  char *endptr;
  *result = lua_str2number(s, &endptr);
  if (endptr == s) return 0;  /* conversion failed */
  if (*endptr == 'x' || *endptr == 'X')  /* maybe an hexadecimal constant? */
    *result = cast_num(strtoul(s, &endptr, 16));
  if (*endptr == '\0') return 1;  /* most common case */
  while (isspace(cast(unsigned char, *endptr))) endptr++;
  if (*endptr != '\0') return 0;  /* invalid trailing characters? */
  return 1;
}
