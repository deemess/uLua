#ifndef LCOMMON_H
#define LCOMMON_H

#include <stdlib.h>
#include <ctype.h>

typedef unsigned int   size_t;
//typedef struct Zio ZIO;
/* chars used as small naturals (so that `char' is reserved for characters) */
typedef unsigned char lu_byte;

#ifndef cast
#define cast(t, exp)	((t)(exp))
#endif

/*
** Marks the end of a patch list. It is an invalid value both as an absolute
** address, and as a list link (would link an element to itself).
*/
#define NO_JUMP (-1)

/* masks for new-style vararg */
#define VARARG_HASARG		1
#define VARARG_ISVARARG		2
#define VARARG_NEEDSARG		4

#define UCHAR_MAX     0xff      /* maximum unsigned char value */
#define EOZ	(-1)			/* end of stream */
#define lua_assert(c)		((void)0)
#define next(ls) (ls->current = zgetc(ls->z))
#define INT_MAX       2147483647    /* maximum (signed) int value */
#define MAX_INT (INT_MAX-2)  /* maximum value of an int (-2 for safety) */
#define MAX_SIZET	((size_t)(~(size_t)0)-2)
#define char2int(c)	cast(int, cast(unsigned char, (c)))
/*
@@ LUAI_MAXUPVALUES is the maximum number of upvalues per function
@* (must be smaller than 250).
*/
#define LUAI_MAXUPVALUES	60
/*
@@ LUAI_MAXVARS is the maximum number of local variables per function
@* (must be smaller than 250).
*/
#define LUAI_MAXVARS		200
/*
@@ LUAI_MAXCCALLS is the maximum depth for nested C calls (short) and
@* syntactical nested non-terminals in a program.
*/
#define LUAI_MAXCCALLS		200
#define LUA_MINBUFFER	32
#define lua_str2number(s,p)	strtod((s), (p))
#define cast_num(i)	cast(double, (i))

#define getstr(ts)	cast(const char *, (ts->tsv.strpt))

/*
** type for virtual-machine instructions
** must be an unsigned with (at least) 4 bytes (see details in lopcodes.h)
*/
typedef unsigned int Instruction;

//TODO: impoement
typedef struct Zio {
  unsigned int n;			/* bytes still unread */
  const char *p;		/* current position in buffer */
  //lua_Reader reader;
  void* data;			/* additional data */
  //lua_State *L;			/* Lua state (for reader) */
} Zio;

/*
** String headers for string table
*/
typedef union TString {
  //L_Umaxalign dummy;  /* ensures maximum alignment for strings */
  struct {
    //CommonHeader;
    lu_byte reserved;
    unsigned int hash;
    size_t len;
	char* strpt;//new for ulua
  } tsv;
} TString;

typedef struct Mbuffer {
  char *buffer;
  size_t n;
  size_t buffsize;
} Mbuffer;


/*
** Function Prototypes
*/
typedef struct Proto {
  //CommonHeader;
  //TValue *k;  /* constants used by the function */
  Instruction code[256];
  struct Proto **p;  /* functions defined inside the function */
  //int *lineinfo;  /* map from opcodes to source lines */
  struct LocVar *locvars;  /* information about local variables */
  //TString **upvalues;  /* upvalue names */
  //TString  *source;
  int sizeupvalues;
  int sizek;  /* size of `k' */
  int sizecode;
  int sizelineinfo;
  int sizep;  /* size of `p' */
  int sizelocvars;
  int linedefined;
  int lastlinedefined;
  //GCObject *gclist;
  lu_byte nups;  /* number of upvalues */
  lu_byte numparams;
  lu_byte is_vararg;
  lu_byte maxstacksize;
} Proto;

/*
** `per thread' state
*/
typedef struct lua_State {
  //CommonHeader;
  lu_byte status;
  //StkId top;  /* first free slot in the stack */
  //StkId base;  /* base of current function */
  //global_State *l_G;
  //CallInfo *ci;  /* call info for current function */
  //const Instruction *savedpc;  /* `savedpc' of current function */
  //StkId stack_last;  /* last free slot in the stack */
  //StkId stack;  /* stack base */
  //CallInfo *end_ci;  /* points after end of ci array*/
  //CallInfo *base_ci;  /* array of CallInfo's */
  int stacksize;
  int size_ci;  /* size of array `base_ci' */
  unsigned short nCcalls;  /* number of nested C calls */
  unsigned short baseCcalls;  /* nested C calls when resuming coroutine */
  lu_byte hookmask;
  lu_byte allowhook;
  int basehookcount;
  int hookcount;
  //lua_Hook hook;
  //TValue l_gt;  /* table of globals */
  //TValue env;  /* temporary place for environments */
  //GCObject *openupval;  /* list of open upvalues in this stack */
  //GCObject *gclist;
  //struct lua_longjmp *errorJmp;  /* current error recover point */
  //ptrdiff_t errfunc;  /* current error handling function (stack index) */
} lua_State;

#define luaZ_initbuffer(L, buff) ((buff)->buffer = NULL, (buff)->buffsize = 0)
#define luaZ_buffer(buff)	((buff)->buffer)
#define luaZ_sizebuffer(buff)	((buff)->buffsize)
#define luaZ_bufflen(buff)	((buff)->n)
#define luaZ_resetbuffer(buff) ((buff)->n = 0)



#define zgetc(z)  (((z)->n--)>0 ?  char2int(*(z)->p++) : luaZ_fill(z))



#endif