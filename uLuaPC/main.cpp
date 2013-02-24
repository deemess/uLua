#include "basetypes.h"
#include "platform.h"
#include "state.h"
#include "vm.h"
#include "gc.h"

void testGC()
{
	gcvarpt* num1 = gcNew(VAR_NUMBER);
	gcvarpt* num2 = gcNew(VAR_BOOLEAN);
	gcvarpt* num3 = gcNew(VAR_FLOAT);

	GCVALUE(s32,num1) = 10;
	GCVALUE(u08,num2) = TRUE;
	GCVALUE(float,num3) = 1.5f;

	/*(*num1)->var.number = 10;
	(*num2)->var.boolean = TRUE;
	(*num3)->var.fnumber = 1.5f;*/

	gcDelete(num2);

	gcvarpt* num4 = gcNew(VAR_STRING);
}

int main()
{
	vm thread;

	initvm(&thread);
	initState(&thread.state[0]);

	platformInit();

	vmInit();
	testGC();
	vmRun(&thread);
	return 0;
}