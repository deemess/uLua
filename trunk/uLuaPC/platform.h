#ifndef PLATFORM_H
#define PLATFORM_H

void platformInit();
u08 platformReadByte(u16 offset);
u16 platformReadWord(u16 offset);
u32 platformReadDWord(u16 offset);
float platformReadNumber(u16 offset);
u08* platformReadBuffer(u16 offset, u08 size);

#endif
