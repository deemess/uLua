#include "basetypes.h"
#include "vm.h"
#include "gc.h"
#include <stdio.h>

void testGC()
{
	gcvarpt* num1;
	gcvarpt* num2; 
	gcvarpt* num3; 
	gcvarpt* num4;

	num1 = gcNew(VAR_NUMBER);
	num2 = gcNew(VAR_BOOLEAN);
	num3 = gcNew(VAR_FLOAT);

	GCVALUE(ls32,num1) = 10;
	GCVALUE(lu08,num2) = TRUE;
	GCVALUE(float,num3) = 1.5f;
	GCVALUE(lu08,num2) = FALSE;

	gcDelete(num2);

	num4 = gcNew(VAR_FLOAT);
	GCVALUE(float,num4) = 9.99f;

	if(GCVALUE(ls32,num1) != 10 || GCVALUE(float,num3) != 1.5f || GCVALUE(float,num4) != 9.99f)
	{
		printf("GC test failed!");
	}
	//gcDump();
}

lu08 buffer[64*1024];
void readBytecode(lu08* buff, lu16 offset, lu16 size) {
	lu16 i;
	for(i=0; i<size; i++) {
		buff[i] = buffer[offset+i];
	}
}

int main(int argc, char **argv)
{
	vm thread;
	FILE* file;

	if(argc < 2) 
	{
		printf("Usage: \n");
		printf("uLua file.luc");
		return 1;
	}
	//read file
	file = fopen(argv[1], "rb");
	fread(buffer, 1, 64*1024, file);

	vmInit(&thread);

	//tests
	//testGC();

	vmRun(&thread, &readBytecode);

	//gcDump();

	return 0;
}

