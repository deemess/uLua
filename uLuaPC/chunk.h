#ifndef CHUNK_H
#define CHUNK_H

#include "basetypes.h"

#define HEADERSIZE 12

struct chunkHeader {
	u32 sign; //4 bytes Header signature: ESC, “Lua” or 0x1B4C7561
	u08 version; //1 byte Version number, 0x51 (81 decimal) for Lua 5.1
	u08 format; //1 byte Format version, 0=official version
	u08 endianness; //1 byte Endianness flag (default 1) 0=big endian, 1=little endian
	u08 intsize; //1 byte Size of int (in bytes) (default 4)
	u08 sizetsize; //1 byte Size of size_t (in bytes) (default 4)
	u08 instructionsize; //1 byte Size of Instruction (in bytes) (default 4)
	u08 luanumbersize; //1 byte Size of lua_Number (in bytes) (default 8)
	u08 integralflag; //1 byte Integral flag (default 0) 0=floating-point, 1=integral number type
};



#endif