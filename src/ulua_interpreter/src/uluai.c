#include "ulua_core/basetypes.h"
#include "ulua_core/vm.h"
#include <stdio.h>


#define CODE_BUFFER_SIZE (64*1024)-1

static lu08 ram[CODE_BUFFER_SIZE];


lu08 buffer[64*1024];
void readBytecode(lu08* buff, lu16 offset, lu16 size) {
	lu16 i;
	for(i=0; i<size; i++) {
		buff[i] = buffer[offset+i];
	}
}

int main(int argc, char **argv)
{
	FILE* file;

	if(argc < 2) 
	{
		printf("Usage: \n");
		printf("uluai file.clu\n");
		return 1;
	}
	//read file
	file = fopen(argv[1], "rb");
	fread(buffer, 1, 64*1024, file);

	ulua_memvar* vm = vmInit((lu08*)&ram, (lu16)CODE_BUFFER_SIZE);

	//tests
	//testGC();

	vmRun(vm, &readBytecode);

	//gcDump();

	return 0;
}

