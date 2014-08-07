#include <stdio.h>
#include "lcommon.h"
#include "platform.h"
#include "lparser.h"

int main(int argc, char **argv) {
	lua_State L;
	Zio z;
	Mbuffer buff;
	Proto * proto;

	if(argc < 2) 
	{
		printf("Usage: \n");
		printf("uluac file.lua");
		return 1;
	}

	initPlatform(argv[1]);

	luaZ_fill(&z);

	buff.buffer = (char*)malloc(1024*1024);
	buff.buffsize = 1024*1024;
	buff.n=0;

	proto = luaY_parser (&L, &z, &buff, argv[1]);

}