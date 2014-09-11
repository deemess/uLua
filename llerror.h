#ifndef LLERROR_H
#define LLERROR_H

typedef enum {
	E_NONE, //no errors
	E_LONGSTR, //unfinished long string or unfinished long comment
	E_LONGSTRDELIM, //invalid long string delimiter
	E_UNFINISHSTR, //unfinished string
	E_NUMBERFORMAT, //malformed number
} ERROR_CODE;

#endif