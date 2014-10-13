#ifndef LLERROR_H
#define LLERROR_H

typedef enum {
	E_NONE, //no errors
	E_LONGSTR, //unfinished long string or unfinished long comment
	E_LONGSTRDELIM, //invalid long string delimiter
	E_UNFINISHSTR, //unfinished string
	E_NUMBERFORMAT, //malformed number
	E_NOTINIT_LOCAL, //local variable not initialized
	E_NO_FREE_REGITER, //all registers are being used
	E_NULL_INSTRUCTION, //tryied to insert NULL instruction
	E_STACKOVERFLOW, //tryied to insert NULL instruction
} ERROR_CODE;

#endif