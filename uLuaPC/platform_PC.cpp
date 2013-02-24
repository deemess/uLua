#include "basetypes.h"
#include <stdio.h>

FILE* file;
u16 curpos;
//u08 buffer[8];
u08 buffer[1024*1024];

void platformInit()
{
	file = fopen("d:\\Lua\\src\\uLua\\test\\functest.luc", "r");
	fread(buffer, 1, 1024*1024, file);
	curpos = 0;
}

void checkPosition(u16 offset)
{
	if(offset != curpos)
	{
		fseek(file, offset, SEEK_SET);
	}
}

u08 platformReadByte(u16 offset)
{
	/*checkPosition(offset);
	fread(buffer, 1, 1, file);
	curpos++;*/
	return buffer[offset];
}

u16 platformReadWord(u16 offset)
{
	/*checkPosition(offset);
	fread(buffer, 2, 1, file);
	curpos += 2;*/
	return *(u16*)(&buffer[offset]);
}

u32 platformReadDWord(u16 offset)
{
	/*checkPosition(offset);
	fread(buffer, 4, 1, file);
	curpos += 4;*/
	return *(u32*)(&buffer[offset]);
}

float platformReadNumber(u16 offset)
{
	/*checkPosition(offset);
	fread(buffer, 8, 1, file);
	curpos += 8;*/
	return (float)( *(double*)(&buffer[offset]) );
}