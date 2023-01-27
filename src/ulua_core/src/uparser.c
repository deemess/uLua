/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>
#line 3 "src/uparser.y"

#include <stdio.h>
#include "ulua_core/basetypes.h"
#include "ulua_core/ucodegen.h"
#include "ulua_core/uparser.h"
#include "ulua_core/ulexer.h"
#include "ulua_core/llerror.h"
#include "ulua_core/vmconfig.h"
#line 17 "src/uparser.c"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    ParseTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 84
#define YYACTIONTYPE unsigned char
#define ParseTOKENTYPE Token
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  Instruction* yy15;
  Register* yy88;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL  Function* f ;
#define ParseARG_PDECL , Function* f 
#define ParseARG_FETCH  Function* f  = yypParser->f 
#define ParseARG_STORE yypParser->f  = f 
#define YYNSTATE 158
#define YYNRULE 83
#define YYFALLBACK 1
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

/* The yyzerominor constant is used to initialize instances of
** YYMINORTYPE objects to zero. */
static const YYMINORTYPE yyzerominor = { 0 };

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
*/
#define YY_ACTTAB_COUNT (665)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    41,   13,   26,   26,   23,   23,   23,   23,   23,   23,
 /*    10 */    21,   20,   20,   20,   20,   20,   15,  219,   20,   26,
 /*    20 */    26,   23,   23,   23,   23,   23,   23,   21,   20,   20,
 /*    30 */    20,   20,   20,   38,   11,   20,  219,  219,   20,   20,
 /*    40 */    20,    5,  125,   20,   26,   26,   23,   23,   23,   23,
 /*    50 */    23,   23,   21,   20,   20,   20,   20,   20,   93,  158,
 /*    60 */    20,  219,  219,  153,    4,  219,   51,  120,  152,  141,
 /*    70 */    26,   26,   23,   23,   23,   23,   23,   23,   21,   20,
 /*    80 */    20,   20,   20,   20,    4,  122,   20,  138,  101,    2,
 /*    90 */    82,   90,   89,  139,   98,   26,   26,   23,   23,   23,
 /*   100 */    23,   23,   23,   21,   20,   20,   20,   20,   20,  107,
 /*   110 */   100,   20,  133,   37,  140,  242,   20,   51,  137,  129,
 /*   120 */    26,   26,   23,   23,   23,   23,   23,   23,   21,   20,
 /*   130 */    20,   20,   20,   20,  126,  123,   20,   12,   26,   26,
 /*   140 */    23,   23,   23,   23,   23,   23,   21,   20,   20,   20,
 /*   150 */    20,   20,  127,  123,   20,   26,   26,   23,   23,   23,
 /*   160 */    23,   23,   23,   21,   20,   20,   20,   20,   20,   19,
 /*   170 */    46,   20,   10,   77,   75,   80,   30,   72,   47,   30,
 /*   180 */    76,   99,  142,   19,   28,   69,   48,   49,  214,    6,
 /*   190 */   136,  106,   47,   50,    8,   99,  213,  105,   95,   51,
 /*   200 */   214,   85,   22,   85,   18,   51,   22,   24,  213,  151,
 /*   210 */   150,  150,  149,  148,  147,   17,   22,   16,    2,  132,
 /*   220 */    22,   24,   19,  151,  150,  150,  149,  148,  147,   17,
 /*   230 */   128,   47,    2,  134,  144,   26,   23,   23,   23,   23,
 /*   240 */    23,   23,   21,   20,   20,   20,   20,   20,  102,   19,
 /*   250 */    20,   32,  139,  121,   39,   22,   51,   14,   47,   22,
 /*   260 */    24,  144,  151,  150,  150,  149,  148,  147,  119,   19,
 /*   270 */   124,    2,   21,   20,   20,   20,   20,   20,  114,  113,
 /*   280 */    20,  144,   22,  135,  112,    9,   22,   24,   94,  151,
 /*   290 */   150,  150,  149,  148,  147,   19,   51,  118,    2,   42,
 /*   300 */   116,   27,   29,    1,   34,   51,   45,  144,   51,  143,
 /*   310 */    54,   31,  109,    7,   43,   15,  145,  142,  143,   65,
 /*   320 */    31,  115,   92,   78,   91,  145,  142,  138,   44,  143,
 /*   330 */    54,   31,   51,  108,  156,  146,  145,  142,    3,  157,
 /*   340 */   155,  117,   33,   40,  146,   74,   71,  130,  154,  143,
 /*   350 */    62,   31,   87,   88,   81,  146,  145,  142,  143,   53,
 /*   360 */    31,   36,   51,  111,   35,  145,  142,   73,  243,  243,
 /*   370 */   143,   62,   31,  243,   86,  146,  243,  145,  142,  143,
 /*   380 */    62,   31,  110,   84,  146,  243,  145,  142,  243,  243,
 /*   390 */   143,   62,   31,  243,   83,  243,  146,  145,  142,  243,
 /*   400 */   143,   62,   31,    5,   79,  146,  243,  145,  142,  243,
 /*   410 */   143,   65,   31,  243,  243,  243,  146,  145,  142,  243,
 /*   420 */   243,  243,   96,   97,  243,  243,  146,  243,  243,  243,
 /*   430 */   243,  243,  243,  143,   58,   31,  146,  243,  243,  131,
 /*   440 */   145,  142,  243,  243,  243,  243,  243,  122,   25,  243,
 /*   450 */   243,    2,  243,  243,  243,  143,   66,   31,  243,  146,
 /*   460 */   243,  243,  145,  142,  143,   55,   31,  243,  243,  243,
 /*   470 */   243,  145,  142,  243,  243,  243,  143,  104,   31,  243,
 /*   480 */   243,  146,  243,  145,  142,  243,  243,  143,   67,   31,
 /*   490 */   146,  243,  243,  243,  145,  142,  243,  243,  243,  243,
 /*   500 */   243,  243,  146,  243,  243,  243,  243,  243,  243,  143,
 /*   510 */   103,   31,  243,  146,  243,  243,  145,  142,  143,   68,
 /*   520 */    31,  243,  243,  243,  243,  145,  142,  243,  243,  243,
 /*   530 */   143,   70,   31,  243,  243,  146,  243,  145,  142,  243,
 /*   540 */   243,  143,   57,   31,  146,  243,  243,  243,  145,  142,
 /*   550 */   243,  243,  243,  243,  243,  243,  146,  243,  243,  243,
 /*   560 */   243,  243,  243,  143,   64,   31,  243,  146,  243,  243,
 /*   570 */   145,  142,  143,   56,   31,  243,  243,  243,  243,  145,
 /*   580 */   142,  243,  243,  243,  143,   63,   31,  243,  243,  146,
 /*   590 */   243,  145,  142,  243,  243,  143,   61,   31,  146,  243,
 /*   600 */   243,  243,  145,  142,  243,  243,  243,  243,  243,  243,
 /*   610 */   146,  243,  243,  243,  243,  243,  243,  143,   60,   31,
 /*   620 */   243,  146,  243,  243,  145,  142,  143,   52,   31,  243,
 /*   630 */   243,  243,  243,  145,  142,  243,  243,  243,  143,   59,
 /*   640 */    31,  243,  243,  146,  243,  145,  142,  243,  243,  243,
 /*   650 */   243,  243,  146,  243,  243,  243,  243,  243,  243,  243,
 /*   660 */   243,  243,  243,  243,  146,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     5,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*    10 */    32,   33,   34,   35,   36,   37,   23,    2,   40,   24,
 /*    20 */    25,   26,   27,   28,   29,   30,   31,   32,   33,   34,
 /*    30 */    35,   36,   37,   15,   16,   40,   21,   22,   35,   36,
 /*    40 */    37,    2,   49,   40,   24,   25,   26,   27,   28,   29,
 /*    50 */    30,   31,   32,   33,   34,   35,   36,   37,   64,    0,
 /*    60 */    40,   46,   47,    6,    3,   50,   72,   73,    6,   49,
 /*    70 */    24,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*    80 */    34,   35,   36,   37,   23,   46,   40,   13,   58,   50,
 /*    90 */    10,   21,   22,   13,   48,   24,   25,   26,   27,   28,
 /*   100 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   64,
 /*   110 */    80,   40,   51,   17,    6,   70,   40,   72,   44,   48,
 /*   120 */    24,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*   130 */    34,   35,   36,   37,   59,   60,   40,   23,   24,   25,
 /*   140 */    26,   27,   28,   29,   30,   31,   32,   33,   34,   35,
 /*   150 */    36,   37,   59,   60,   40,   24,   25,   26,   27,   28,
 /*   160 */    29,   30,   31,   32,   33,   34,   35,   36,   37,    1,
 /*   170 */    75,   40,   11,   78,   53,   23,   55,   53,   10,   55,
 /*   180 */    56,   13,   61,    1,   23,   61,   62,   63,   11,   11,
 /*   190 */     2,   64,   10,   69,   14,   13,   11,   64,   74,   72,
 /*   200 */    23,   23,   34,   23,   11,   72,   38,   39,   23,   41,
 /*   210 */    42,   43,   44,   45,   46,   47,   34,   11,   50,   51,
 /*   220 */    38,   39,    1,   41,   42,   43,   44,   45,   46,   47,
 /*   230 */    13,   10,   50,   51,   13,   25,   26,   27,   28,   29,
 /*   240 */    30,   31,   32,   33,   34,   35,   36,   37,   64,    1,
 /*   250 */    40,   13,   13,    6,    5,   34,   72,    4,   10,   38,
 /*   260 */    39,   13,   41,   42,   43,   44,   45,   46,    6,    1,
 /*   270 */    49,   50,   32,   33,   34,   35,   36,   37,    6,   13,
 /*   280 */    40,   13,   34,   44,   13,   11,   38,   39,   64,   41,
 /*   290 */    42,   43,   44,   45,   46,    1,   72,   64,   50,    5,
 /*   300 */    64,    7,    8,    9,   10,   72,   12,   13,   72,   53,
 /*   310 */    54,   55,   18,   19,   20,   23,   60,   61,   53,   54,
 /*   320 */    55,   65,   66,   67,   64,   60,   61,   13,   13,   53,
 /*   330 */    54,   55,   72,    6,   71,   79,   60,   61,   68,    3,
 /*   340 */    71,   65,    2,   76,   79,   58,   81,   82,   71,   53,
 /*   350 */    54,   55,   13,   57,   64,   79,   60,   61,   53,   54,
 /*   360 */    55,   76,   72,   13,   76,   60,   61,   58,   83,   83,
 /*   370 */    53,   54,   55,   83,   57,   79,   83,   60,   61,   53,
 /*   380 */    54,   55,   77,   57,   79,   83,   60,   61,   83,   83,
 /*   390 */    53,   54,   55,   83,   57,   83,   79,   60,   61,   83,
 /*   400 */    53,   54,   55,    2,   57,   79,   83,   60,   61,   83,
 /*   410 */    53,   54,   55,   83,   83,   83,   79,   60,   61,   83,
 /*   420 */    83,   83,   21,   22,   83,   83,   79,   83,   83,   83,
 /*   430 */    83,   83,   83,   53,   54,   55,   79,   83,   83,   82,
 /*   440 */    60,   61,   83,   83,   83,   83,   83,   46,   47,   83,
 /*   450 */    83,   50,   83,   83,   83,   53,   54,   55,   83,   79,
 /*   460 */    83,   83,   60,   61,   53,   54,   55,   83,   83,   83,
 /*   470 */    83,   60,   61,   83,   83,   83,   53,   54,   55,   83,
 /*   480 */    83,   79,   83,   60,   61,   83,   83,   53,   54,   55,
 /*   490 */    79,   83,   83,   83,   60,   61,   83,   83,   83,   83,
 /*   500 */    83,   83,   79,   83,   83,   83,   83,   83,   83,   53,
 /*   510 */    54,   55,   83,   79,   83,   83,   60,   61,   53,   54,
 /*   520 */    55,   83,   83,   83,   83,   60,   61,   83,   83,   83,
 /*   530 */    53,   54,   55,   83,   83,   79,   83,   60,   61,   83,
 /*   540 */    83,   53,   54,   55,   79,   83,   83,   83,   60,   61,
 /*   550 */    83,   83,   83,   83,   83,   83,   79,   83,   83,   83,
 /*   560 */    83,   83,   83,   53,   54,   55,   83,   79,   83,   83,
 /*   570 */    60,   61,   53,   54,   55,   83,   83,   83,   83,   60,
 /*   580 */    61,   83,   83,   83,   53,   54,   55,   83,   83,   79,
 /*   590 */    83,   60,   61,   83,   83,   53,   54,   55,   79,   83,
 /*   600 */    83,   83,   60,   61,   83,   83,   83,   83,   83,   83,
 /*   610 */    79,   83,   83,   83,   83,   83,   83,   53,   54,   55,
 /*   620 */    83,   79,   83,   83,   60,   61,   53,   54,   55,   83,
 /*   630 */    83,   83,   83,   60,   61,   83,   83,   83,   53,   54,
 /*   640 */    55,   83,   83,   79,   83,   60,   61,   83,   83,   83,
 /*   650 */    83,   83,   79,   83,   83,   83,   83,   83,   83,   83,
 /*   660 */    83,   83,   83,   83,   79,
};
#define YY_SHIFT_USE_DFLT (-23)
#define YY_SHIFT_COUNT (107)
#define YY_SHIFT_MIN   (-22)
#define YY_SHIFT_MAX   (401)
static const short yy_shift_ofst[] = {
 /*     0 */   -23,  248,  182,  294,  168,  221,  248,  248,  248,  248,
 /*    10 */   248,  248,  248,  248,  248,  248,  248,  248,  248,  248,
 /*    20 */   248,  248,  248,  248,  248,  248,  248,  248,  268,  -23,
 /*    30 */   401,  401,   39,  239,  350,  -23,  -23,  -23,  -23,  -23,
 /*    40 */   -23,  -23,  -23,   80,  340,  339,  340,  340,  336,  336,
 /*    50 */   336,  -23,  114,  -22,   96,   71,   46,   20,   -5,  131,
 /*    60 */   131,  131,  131,  131,  131,  131,  210,  240,  240,   15,
 /*    70 */     3,   61,  185,  178,  180,  177,  161,   70,   18,   -7,
 /*    80 */    74,  327,  315,  292,  292,  314,  292,  274,  292,  271,
 /*    90 */   266,  272,  262,  253,  247,  249,  238,  217,  206,  193,
 /*   100 */   188,  152,  108,   76,   76,   62,   57,   59,
};
#define YY_REDUCE_USE_DFLT (-7)
#define YY_REDUCE_COUNT (51)
#define YY_REDUCE_MIN   (-6)
#define YY_REDUCE_MAX   (585)
static const short yy_reduce_ofst[] = {
 /*     0 */    45,  256,  265,  124,  357,  347,  337,  326,  317,  305,
 /*    10 */   296,  276,  585,  573,  564,  542,  531,  519,  510,  488,
 /*    20 */   477,  465,  456,  434,  423,  411,  402,  380,  121,   -6,
 /*    30 */    93,   93,   75,   30,   95,  290,  260,  236,  233,  224,
 /*    40 */   184,  133,  127,  309,  288,  287,  285,  267,  277,  269,
 /*    50 */   263,  270,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   164,  241,  241,  161,  241,  241,  241,  184,  241,  241,
 /*    10 */   241,  241,  241,  241,  241,  241,  241,  241,  241,  241,
 /*    20 */   241,  241,  241,  241,  241,  241,  241,  241,  241,  164,
 /*    30 */   241,  205,  241,  229,  241,  164,  164,  164,  164,  164,
 /*    40 */   164,  164,  164,  241,  241,  241,  241,  241,  160,  160,
 /*    50 */   160,  166,  197,  241,  241,  241,  241,  241,  241,  198,
 /*    60 */   163,  196,  195,  240,  239,  238,  209,  210,  211,  175,
 /*    70 */   212,  241,  218,  186,  241,  218,  241,  189,  178,  241,
 /*    80 */   241,  241,  241,  187,  185,  241,  177,  193,  174,  241,
 /*    90 */   241,  241,  241,  241,  241,  241,  241,  241,  241,  215,
 /*   100 */   241,  230,  241,  208,  207,  241,  241,  241,  188,  183,
 /*   110 */   176,  191,  192,  190,  173,  180,  182,  181,  179,  172,
 /*   120 */   171,  170,  226,  225,  223,  224,  222,  221,  217,  216,
 /*   130 */   236,  237,  235,  234,  233,  231,  228,  232,  194,  193,
 /*   140 */   227,  220,  219,  218,  215,  206,  204,  203,  202,  201,
 /*   150 */   200,  199,  169,  168,  167,  165,  162,  159,
};

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
    0,  /*          $ => nothing */
    0,  /*       OPEN => nothing */
    1,  /*     LPAREN => OPEN */
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
  int yyerrcnt;                 /* Shifts left before out of the error */
  ParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
static void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "OPEN",          "LPAREN",        "SEMICOL",     
  "UNTIL",         "DO",            "END",           "WHILE",       
  "REPEAT",        "IF",            "FUNCTION",      "SET",         
  "FOR",           "NAME",          "IN",            "ELSE",        
  "ELSEIF",        "THEN",          "BREAK",         "RETURN",      
  "LOCAL",         "COLON",         "DOT",           "COMMA",       
  "OR",            "AND",           "L",             "LE",          
  "G",             "GE",            "EQ",            "NE",          
  "CONCAT",        "PLUS",          "MINUS",         "TIMES",       
  "DIVIDE",        "MOD",           "NOT",           "HASH",        
  "POW",           "NIL",           "TRUE",          "FALSE",       
  "DOTS",          "NUMBER",        "STRING",        "SLPAREN",     
  "SRPAREN",       "RPAREN",        "LBRACE",        "RBRACE",      
  "error",         "var",           "exp",           "prefixexp",   
  "setlist",       "explist1",      "namelist",      "args",        
  "tableconstructor",  "functioncall",  "stat",          "binding",     
  "block",         "cond",          "conds",         "condlist",    
  "statlist",      "laststat",      "chunk",         "semi",        
  "scope",         "ublock",        "repetition",    "funcname",    
  "params",        "explist23",     "dottedname",    "function",    
  "parlist",       "fieldlist",     "field",       
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "chunk ::= block",
 /*   1 */ "semi ::= SEMICOL",
 /*   2 */ "semi ::=",
 /*   3 */ "block ::= scope statlist",
 /*   4 */ "block ::= scope statlist laststat semi",
 /*   5 */ "ublock ::= block UNTIL exp",
 /*   6 */ "scope ::=",
 /*   7 */ "scope ::= scope statlist binding semi",
 /*   8 */ "statlist ::=",
 /*   9 */ "statlist ::= statlist stat semi",
 /*  10 */ "stat ::= DO block END",
 /*  11 */ "stat ::= WHILE exp DO block END",
 /*  12 */ "stat ::= repetition DO block END",
 /*  13 */ "stat ::= REPEAT ublock",
 /*  14 */ "stat ::= IF conds END",
 /*  15 */ "stat ::= FUNCTION funcname params block END",
 /*  16 */ "stat ::= setlist SET explist1",
 /*  17 */ "stat ::= functioncall",
 /*  18 */ "repetition ::= FOR NAME SET explist23",
 /*  19 */ "repetition ::= FOR namelist IN explist1",
 /*  20 */ "conds ::= condlist",
 /*  21 */ "conds ::= condlist ELSE block",
 /*  22 */ "condlist ::= cond",
 /*  23 */ "condlist ::= condlist ELSEIF cond",
 /*  24 */ "cond ::= exp THEN block",
 /*  25 */ "laststat ::= BREAK",
 /*  26 */ "laststat ::= RETURN",
 /*  27 */ "laststat ::= RETURN explist1",
 /*  28 */ "binding ::= LOCAL namelist",
 /*  29 */ "binding ::= LOCAL namelist SET explist1",
 /*  30 */ "binding ::= LOCAL FUNCTION NAME params block END",
 /*  31 */ "funcname ::= dottedname",
 /*  32 */ "funcname ::= dottedname COLON NAME",
 /*  33 */ "dottedname ::= NAME",
 /*  34 */ "dottedname ::= dottedname DOT NAME",
 /*  35 */ "namelist ::= NAME",
 /*  36 */ "namelist ::= namelist COMMA NAME",
 /*  37 */ "explist1 ::= exp",
 /*  38 */ "explist1 ::= explist1 COMMA exp",
 /*  39 */ "explist23 ::= exp COMMA exp",
 /*  40 */ "explist23 ::= exp COMMA exp COMMA exp",
 /*  41 */ "exp ::= NIL",
 /*  42 */ "exp ::= TRUE|FALSE",
 /*  43 */ "exp ::= DOTS",
 /*  44 */ "exp ::= NUMBER",
 /*  45 */ "exp ::= STRING",
 /*  46 */ "exp ::= function",
 /*  47 */ "exp ::= prefixexp",
 /*  48 */ "exp ::= tableconstructor",
 /*  49 */ "exp ::= HASH exp",
 /*  50 */ "exp ::= NOT|MINUS exp",
 /*  51 */ "exp ::= exp OR|AND exp",
 /*  52 */ "exp ::= exp L|LE|G|GE|EQ|NE exp",
 /*  53 */ "exp ::= exp CONCAT exp",
 /*  54 */ "exp ::= exp PLUS|MINUS|TIMES|DIVIDE|MOD|POW exp",
 /*  55 */ "setlist ::= var",
 /*  56 */ "setlist ::= setlist COMMA var",
 /*  57 */ "var ::= NAME",
 /*  58 */ "var ::= prefixexp SLPAREN exp SRPAREN",
 /*  59 */ "var ::= prefixexp DOT NAME",
 /*  60 */ "prefixexp ::= var",
 /*  61 */ "prefixexp ::= functioncall",
 /*  62 */ "prefixexp ::= OPEN exp RPAREN",
 /*  63 */ "functioncall ::= prefixexp args",
 /*  64 */ "functioncall ::= prefixexp COLON NAME args",
 /*  65 */ "args ::= LPAREN RPAREN",
 /*  66 */ "args ::= LPAREN explist1 RPAREN",
 /*  67 */ "args ::= tableconstructor",
 /*  68 */ "args ::= STRING",
 /*  69 */ "function ::= FUNCTION params block END",
 /*  70 */ "params ::= LPAREN parlist LPAREN",
 /*  71 */ "parlist ::=",
 /*  72 */ "parlist ::= namelist",
 /*  73 */ "parlist ::= DOTS",
 /*  74 */ "parlist ::= namelist COMMA DOTS",
 /*  75 */ "tableconstructor ::= LBRACE RBRACE",
 /*  76 */ "tableconstructor ::= LBRACE fieldlist RBRACE",
 /*  77 */ "tableconstructor ::= LBRACE fieldlist COMMA|SEMICOL RBRACE",
 /*  78 */ "fieldlist ::= field",
 /*  79 */ "fieldlist ::= fieldlist COMMA|SEMICOL field",
 /*  80 */ "field ::= exp",
 /*  81 */ "field ::= NAME SET exp",
 /*  82 */ "field ::= SLPAREN exp SRPAREN SET exp",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor(pParser, yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from ParseAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>YY_SHIFT_COUNT
   || (i = yy_shift_ofst[stateno])==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
  //assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    if( iLookAhead>0 ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        return yy_find_shift_action(pParser, iFallback);
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
    }
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  //assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  //assert( i!=YY_REDUCE_USE_DFLT );
  //assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  //assert( i>=0 && i<YY_ACTTAB_COUNT );
  //assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser, YYMINORTYPE *yypMinor){
   ParseARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
#line 45 "src/uparser.y"

    f->error_code = E_STACKOVERFLOW;
#line 806 "src/uparser.c"
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer to the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser, yypMinor);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser, yypMinor);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 70, 1 },
  { 71, 1 },
  { 71, 0 },
  { 64, 2 },
  { 64, 4 },
  { 73, 3 },
  { 72, 0 },
  { 72, 4 },
  { 68, 0 },
  { 68, 3 },
  { 62, 3 },
  { 62, 5 },
  { 62, 4 },
  { 62, 2 },
  { 62, 3 },
  { 62, 5 },
  { 62, 3 },
  { 62, 1 },
  { 74, 4 },
  { 74, 4 },
  { 66, 1 },
  { 66, 3 },
  { 67, 1 },
  { 67, 3 },
  { 65, 3 },
  { 69, 1 },
  { 69, 1 },
  { 69, 2 },
  { 63, 2 },
  { 63, 4 },
  { 63, 6 },
  { 75, 1 },
  { 75, 3 },
  { 78, 1 },
  { 78, 3 },
  { 58, 1 },
  { 58, 3 },
  { 57, 1 },
  { 57, 3 },
  { 77, 3 },
  { 77, 5 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 2 },
  { 54, 2 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 56, 1 },
  { 56, 3 },
  { 53, 1 },
  { 53, 4 },
  { 53, 3 },
  { 55, 1 },
  { 55, 1 },
  { 55, 3 },
  { 61, 2 },
  { 61, 4 },
  { 59, 2 },
  { 59, 3 },
  { 59, 1 },
  { 59, 1 },
  { 79, 4 },
  { 76, 3 },
  { 80, 0 },
  { 80, 1 },
  { 80, 1 },
  { 80, 3 },
  { 60, 2 },
  { 60, 3 },
  { 60, 4 },
  { 81, 1 },
  { 81, 3 },
  { 82, 1 },
  { 82, 3 },
  { 82, 5 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  /* Silence complaints from purify about yygotominor being uninitialized
  ** in some cases when it is copied into the stack after the following
  ** switch.  yygotominor is uninitialized when a rule reduces that does
  ** not set the value of its left-hand side nonterminal.  Leaving the
  ** value of the nonterminal uninitialized is utterly harmless as long
  ** as the value is never used.  So really the only thing this code
  ** accomplishes is to quieten purify.  
  **
  ** 2007-01-16:  The wireshark project (www.wireshark.org) reports that
  ** without this code, their parser segfaults.  I'm not sure what there
  ** parser is doing to make this happen.  This is the second bug report
  ** from wireshark this week.  Clearly they are stressing Lemon in ways
  ** that it has not been previously stressed...  (SQLite ticket #2172)
  */
  /*memset(&yygotominor, 0, sizeof(yygotominor));*/
  yygotominor = yyzerominor;


  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
      case 0: /* chunk ::= block */
#line 51 "src/uparser.y"
{
    DPRINTF("P_CHUNK\n");
	doReturn(f);
}
#line 1006 "src/uparser.c"
        break;
      case 1: /* semi ::= SEMICOL */
      case 2: /* semi ::= */ yytestcase(yyruleno==2);
#line 56 "src/uparser.y"
{
	DPRINTF("P_SEMI ------------------\n");
}
#line 1014 "src/uparser.c"
        break;
      case 3: /* block ::= scope statlist */
#line 63 "src/uparser.y"
{
    DPRINTF("P_BLOCK_STATLIST\n");
	yygotominor.yy15 = yymsp[0].minor.yy15;
    unloadRegisters(f);
}
#line 1023 "src/uparser.c"
        break;
      case 4: /* block ::= scope statlist laststat semi */
#line 68 "src/uparser.y"
{
    DPRINTF("P_BLOCK_STATLIST_LASTSTAT\n");
	yygotominor.yy15 = yymsp[-2].minor.yy15;
    unloadRegisters(f);
}
#line 1032 "src/uparser.c"
        break;
      case 5: /* ublock ::= block UNTIL exp */
#line 73 "src/uparser.y"
{
	DPRINTF("P_UBLOCK\n");
}
#line 1039 "src/uparser.c"
        break;
      case 6: /* scope ::= */
#line 77 "src/uparser.y"
{
	DPRINTF("P_SCOPE_EMPTY\n");
}
#line 1046 "src/uparser.c"
        break;
      case 7: /* scope ::= scope statlist binding semi */
#line 80 "src/uparser.y"
{
	DPRINTF("P_SCOPE_STATLIST\n");
}
#line 1053 "src/uparser.c"
        break;
      case 8: /* statlist ::= */
#line 84 "src/uparser.y"
{
    DPRINTF("P_STATLIST_EMPTY\n");
	yygotominor.yy15 = ULUA_NULL;
	f->currentStat = ULUA_NULL;
}
#line 1062 "src/uparser.c"
        break;
      case 9: /* statlist ::= statlist stat semi */
#line 89 "src/uparser.y"
{
    DPRINTF("P_STATLIST_ADD_STAT\n");
	if(yymsp[-2].minor.yy15 == ULUA_NULL) {
		yygotominor.yy15 = f->currentStat; //save only pointer to first statement
	} else {
		yygotominor.yy15 = yymsp[-2].minor.yy15;
	}
	f->currentStat = ULUA_NULL;
}
#line 1075 "src/uparser.c"
        break;
      case 10: /* stat ::= DO block END */
#line 99 "src/uparser.y"
{
    DPRINTF("P_STAT_BLOCK\n");
	yygotominor.yy15 = yymsp[-1].minor.yy15;
}
#line 1083 "src/uparser.c"
        break;
      case 11: /* stat ::= WHILE exp DO block END */
#line 103 "src/uparser.y"
{
    DPRINTF("P_STAT_WHILE\n");
	yygotominor.yy15 = statWHILE(f, yymsp[-3].minor.yy88, yymsp[-1].minor.yy15);
}
#line 1091 "src/uparser.c"
        break;
      case 14: /* stat ::= IF conds END */
#line 109 "src/uparser.y"
{
    DPRINTF("P_STAT_IF\n");
	yygotominor.yy15 = yymsp[-1].minor.yy15;
}
#line 1099 "src/uparser.c"
        break;
      case 15: /* stat ::= FUNCTION funcname params block END */
#line 113 "src/uparser.y"
{
    DPRINTF("P_STAT_FUNCTION\n");
}
#line 1106 "src/uparser.c"
        break;
      case 16: /* stat ::= setlist SET explist1 */
#line 116 "src/uparser.y"
{
	DPRINTF("P_STAT_SET\n");
	yygotominor.yy15 = statSETList(f, yymsp[-2].minor.yy88, yymsp[0].minor.yy88, ULUA_FALSE);
}
#line 1114 "src/uparser.c"
        break;
      case 17: /* stat ::= functioncall */
#line 120 "src/uparser.y"
{
	DPRINTF("P_STAT_FCALL\n");
	yygotominor.yy15 = yymsp[0].minor.yy15;
}
#line 1122 "src/uparser.c"
        break;
      case 20: /* conds ::= condlist */
#line 128 "src/uparser.y"
{
    DPRINTF("P_CONDS_CONDLIST\n");
	yygotominor.yy15 = yymsp[0].minor.yy15;
}
#line 1130 "src/uparser.c"
        break;
      case 21: /* conds ::= condlist ELSE block */
#line 132 "src/uparser.y"
{
    DPRINTF("P_CONDS_CONDLIST_ELSE_BLOCK\n");
	yygotominor.yy15 = statELSE(f, yymsp[-2].minor.yy15, yymsp[0].minor.yy15);
}
#line 1138 "src/uparser.c"
        break;
      case 22: /* condlist ::= cond */
#line 136 "src/uparser.y"
{
    DPRINTF("P_CONDLIST_COND\n");
	yygotominor.yy15 = yymsp[0].minor.yy15;
}
#line 1146 "src/uparser.c"
        break;
      case 23: /* condlist ::= condlist ELSEIF cond */
#line 140 "src/uparser.y"
{
    DPRINTF("P_CONDLIST_CONDLIST_ELSEIF_COND\n");
	yygotominor.yy15 = statELSEIF(f, yymsp[-2].minor.yy15, yymsp[0].minor.yy15);
}
#line 1154 "src/uparser.c"
        break;
      case 24: /* cond ::= exp THEN block */
#line 144 "src/uparser.y"
{
    DPRINTF("P_COND_EXP_THEN_BLOCK\n");
	yygotominor.yy15 = statTHEN(f, yymsp[-2].minor.yy88, yymsp[0].minor.yy15);
}
#line 1162 "src/uparser.c"
        break;
      case 28: /* binding ::= LOCAL namelist */
#line 153 "src/uparser.y"
{
	DPRINTF("P_LOCAL\n");
}
#line 1169 "src/uparser.c"
        break;
      case 29: /* binding ::= LOCAL namelist SET explist1 */
#line 156 "src/uparser.y"
{
    DPRINTF("P_LOCAL_SET\n");
	yygotominor.yy15 = statSET(f, yymsp[-2].minor.yy88, yymsp[0].minor.yy88, ULUA_TRUE);
}
#line 1177 "src/uparser.c"
        break;
      case 30: /* binding ::= LOCAL FUNCTION NAME params block END */
#line 160 "src/uparser.y"
{
	DPRINTF("P_LOCAL_FUNCTION\n");
	pushConstString(f, &yymsp[-3].minor.yy0.semInfo);
}
#line 1185 "src/uparser.c"
        break;
      case 31: /* funcname ::= dottedname */
#line 165 "src/uparser.y"
{
	DPRINTF("P_FUNCNAME_DOTTEDNAME\n");
}
#line 1192 "src/uparser.c"
        break;
      case 32: /* funcname ::= dottedname COLON NAME */
#line 168 "src/uparser.y"
{
	DPRINTF("P_FUNCNAME_DOTTEDNAME_COLON_NAME\n");
}
#line 1199 "src/uparser.c"
        break;
      case 33: /* dottedname ::= NAME */
#line 172 "src/uparser.y"
{
	DPRINTF("P_DOTTEDNAME_NAME\n");
}
#line 1206 "src/uparser.c"
        break;
      case 34: /* dottedname ::= dottedname DOT NAME */
#line 175 "src/uparser.y"
{
	DPRINTF("P_DOTTEDNAME_DOTTEDNAME_DOT_NAME\n");
}
#line 1213 "src/uparser.c"
        break;
      case 35: /* namelist ::= NAME */
#line 179 "src/uparser.y"
{
	Constant* c;
    	DPRINTF("P_NAMELIST_NAME\n");
	c = pushVarName(f, &yymsp[0].minor.yy0.semInfo);
	yygotominor.yy88 = getVarRegister(f,c);
}
#line 1223 "src/uparser.c"
        break;
      case 36: /* namelist ::= namelist COMMA NAME */
#line 185 "src/uparser.y"
{
	Constant* c;
    	DPRINTF("P_NAMELIST_COMMA_NAME\n");
	c = pushVarName(f, &yymsp[0].minor.yy0.semInfo);
	yygotominor.yy88 = getVarRegister(f,c);
}
#line 1233 "src/uparser.c"
        break;
      case 37: /* explist1 ::= exp */
#line 192 "src/uparser.y"
{
    DPRINTF("P_EXPLIST_EXP\n");
	yygotominor.yy88 = yymsp[0].minor.yy88;
}
#line 1241 "src/uparser.c"
        break;
      case 38: /* explist1 ::= explist1 COMMA exp */
#line 196 "src/uparser.y"
{
	DPRINTF("P_EXPLIST_ADD_EXP\n");
	yygotominor.yy88 = addRegisterList(yymsp[-2].minor.yy88, yymsp[0].minor.yy88);
}
#line 1249 "src/uparser.c"
        break;
      case 41: /* exp ::= NIL */
#line 212 "src/uparser.y"
{
    DPRINTF("P_EXP_NIL\n");
	yygotominor.yy88 = doNil(f);
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1258 "src/uparser.c"
        break;
      case 42: /* exp ::= TRUE|FALSE */
#line 217 "src/uparser.y"
{
	DPRINTF("P_EXP_BOOLEAN %.*s\n", yymsp[0].minor.yy0.semInfo.bplen, &f->code[yymsp[0].minor.yy0.semInfo.bp]);
	yygotominor.yy88 = doBoolean(f, &yymsp[0].minor.yy0);
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1267 "src/uparser.c"
        break;
      case 44: /* exp ::= NUMBER */
#line 223 "src/uparser.y"
{
	Constant* c;
	Register* r;
	DPRINTF("P_EXP_NUMBER %.*s\n", yymsp[0].minor.yy0.semInfo.bplen, &f->code[yymsp[0].minor.yy0.semInfo.bp]);

	c = pushConstNumber(f, yymsp[0].minor.yy0.number.fvalue);
	r = getFreeRegister(f);
	r->consthold = ULUA_TRUE;
	r->constnum = c->num;
	yygotominor.yy88 = r;
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1283 "src/uparser.c"
        break;
      case 45: /* exp ::= STRING */
#line 235 "src/uparser.y"
{
	Constant* c;
	Register* r;
	DPRINTF("P_EXP_STRING %.*s\n", yymsp[0].minor.yy0.semInfo.bplen, &f->code[yymsp[0].minor.yy0.semInfo.bp]);

	c = pushConstString(f, &yymsp[0].minor.yy0.semInfo);
	r = getFreeRegister(f);
	r->consthold = ULUA_TRUE;
	r->constnum = c->num;
	yygotominor.yy88 = r;
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1299 "src/uparser.c"
        break;
      case 46: /* exp ::= function */
#line 247 "src/uparser.y"
{
	DPRINTF("P_EXP_FUNCTION\n");
}
#line 1306 "src/uparser.c"
        break;
      case 47: /* exp ::= prefixexp */
#line 250 "src/uparser.y"
{
	DPRINTF("P_EXP_PREFIXEXP\n");
	yygotominor.yy88 = yymsp[0].minor.yy88;
}
#line 1314 "src/uparser.c"
        break;
      case 48: /* exp ::= tableconstructor */
#line 254 "src/uparser.y"
{
	DPRINTF("P_EXP_TABLECONSTRUCTOR\n");
	yygotominor.yy88 = yymsp[0].minor.yy88;
}
#line 1322 "src/uparser.c"
        break;
      case 50: /* exp ::= NOT|MINUS exp */
#line 259 "src/uparser.y"
{
    DPRINTF("P_EXP_NOT_MINUS\n");
	yygotominor.yy88 = doNot(f, yymsp[0].minor.yy88, &yymsp[-1].minor.yy0);
}
#line 1330 "src/uparser.c"
        break;
      case 51: /* exp ::= exp OR|AND exp */
#line 263 "src/uparser.y"
{
	DPRINTF("P_EXP_LOGIC %.*s\n", yymsp[-1].minor.yy0.semInfo.bplen, &f->code[yymsp[-1].minor.yy0.semInfo.bp]);
	yygotominor.yy88 = doLogic(f,yymsp[-2].minor.yy88,yymsp[0].minor.yy88,&yymsp[-1].minor.yy0);
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1339 "src/uparser.c"
        break;
      case 52: /* exp ::= exp L|LE|G|GE|EQ|NE exp */
#line 268 "src/uparser.y"
{
	DPRINTF("P_EXP_COMPARE %.*s\n", yymsp[-1].minor.yy0.semInfo.bplen, &f->code[yymsp[-1].minor.yy0.semInfo.bp]);
	yygotominor.yy88 = doCompare(f,yymsp[-2].minor.yy88,yymsp[0].minor.yy88,&yymsp[-1].minor.yy0);
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1348 "src/uparser.c"
        break;
      case 54: /* exp ::= exp PLUS|MINUS|TIMES|DIVIDE|MOD|POW exp */
#line 274 "src/uparser.y"
{
	DPRINTF("P_EXP_MATH %.*s\n", yymsp[-1].minor.yy0.semInfo.bplen, &f->code[yymsp[-1].minor.yy0.semInfo.bp]);
	yygotominor.yy88 = doMath(f,yymsp[-2].minor.yy88,yymsp[0].minor.yy88,&yymsp[-1].minor.yy0);
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1357 "src/uparser.c"
        break;
      case 55: /* setlist ::= var */
#line 280 "src/uparser.y"
{
    DPRINTF("P_SETLIST_VAR\n");
	yygotominor.yy88 = yymsp[0].minor.yy88;
}
#line 1365 "src/uparser.c"
        break;
      case 56: /* setlist ::= setlist COMMA var */
#line 284 "src/uparser.y"
{
	DPRINTF("P_SETLIST_COMMA_VAR\n");
	yygotominor.yy88 = addRegisterList(yymsp[-2].minor.yy88, yymsp[0].minor.yy88);
}
#line 1373 "src/uparser.c"
        break;
      case 57: /* var ::= NAME */
#line 289 "src/uparser.y"
{
	Constant* c;
	DPRINTF("P_VAR_NAME %.*s\n", yymsp[0].minor.yy0.semInfo.bplen, &f->code[yymsp[0].minor.yy0.semInfo.bp]);

	c = pushVarName(f, &yymsp[0].minor.yy0.semInfo);
	yygotominor.yy88 = getVarRegister(f,c);
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1385 "src/uparser.c"
        break;
      case 58: /* var ::= prefixexp SLPAREN exp SRPAREN */
#line 297 "src/uparser.y"
{
	DPRINTF("P_PREFEXP_SLPAREN_EXP_SRPAREN\n");

	yymsp[-3].minor.yy88->istable = ULUA_TRUE;
	yymsp[-3].minor.yy88->tablekey = yymsp[-1].minor.yy88;
	yygotominor.yy88 = yymsp[-3].minor.yy88;

	//if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1398 "src/uparser.c"
        break;
      case 59: /* var ::= prefixexp DOT NAME */
#line 306 "src/uparser.y"
{
	Constant* c;
	Register* r;
	DPRINTF("P_PREFEXP_DOT_NAME %.*s\n", yymsp[0].minor.yy0.semInfo.bplen, &f->code[yymsp[0].minor.yy0.semInfo.bp]);

	c = pushVarName(f, &yymsp[0].minor.yy0.semInfo);
	r = getVarRegister(f,c);
	
	yymsp[-2].minor.yy88->istable = ULUA_TRUE;
	yymsp[-2].minor.yy88->tablekey = r;
	yygotominor.yy88 = yymsp[-2].minor.yy88;

	//if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1416 "src/uparser.c"
        break;
      case 60: /* prefixexp ::= var */
#line 321 "src/uparser.y"
{
	DPRINTF("P_PREFEXP_VAR\n");
	yygotominor.yy88 = yymsp[0].minor.yy88;
}
#line 1424 "src/uparser.c"
        break;
      case 61: /* prefixexp ::= functioncall */
#line 325 "src/uparser.y"
{
	DPRINTF("P_PREFEXP_FCALL\n");
}
#line 1431 "src/uparser.c"
        break;
      case 62: /* prefixexp ::= OPEN exp RPAREN */
#line 328 "src/uparser.y"
{
    DPRINTF("P_PREFEXP_EXP\n");
	yygotominor.yy88 = yymsp[-1].minor.yy88;
}
#line 1439 "src/uparser.c"
        break;
      case 63: /* functioncall ::= prefixexp args */
#line 333 "src/uparser.y"
{
    DPRINTF("P_FCALL_ARGS\n");
	yygotominor.yy15 = functionCALL(f, yymsp[-1].minor.yy88, yymsp[0].minor.yy88);
}
#line 1447 "src/uparser.c"
        break;
      case 64: /* functioncall ::= prefixexp COLON NAME args */
#line 337 "src/uparser.y"
{
	DPRINTF("P_FCALL_NAME_ARGS %.*s\n", yymsp[-1].minor.yy0.semInfo.bplen, &f->code[yymsp[-1].minor.yy0.semInfo.bp]);
}
#line 1454 "src/uparser.c"
        break;
      case 65: /* args ::= LPAREN RPAREN */
#line 341 "src/uparser.y"
{
    DPRINTF("P_ARGS_EMPTY\n");
	yygotominor.yy88 = ULUA_NULL;
}
#line 1462 "src/uparser.c"
        break;
      case 66: /* args ::= LPAREN explist1 RPAREN */
#line 345 "src/uparser.y"
{
    DPRINTF("P_ARGS_EXPLIST\n");
	yygotominor.yy88 = yymsp[-1].minor.yy88;
}
#line 1470 "src/uparser.c"
        break;
      case 67: /* args ::= tableconstructor */
#line 349 "src/uparser.y"
{
    DPRINTF("P_ARGS_TABLECONSTRUCTOR\n");
    yygotominor.yy88 = yymsp[0].minor.yy88;
}
#line 1478 "src/uparser.c"
        break;
      case 68: /* args ::= STRING */
#line 353 "src/uparser.y"
{
	Constant* c;
	Register* r;
	DPRINTF("P_ARGS_STRING %.*s\n", yymsp[0].minor.yy0.semInfo.bplen, &f->code[yymsp[0].minor.yy0.semInfo.bp]);

	c = pushConstString(f, &yymsp[0].minor.yy0.semInfo);
	r = getFreeRegister(f);
	r->consthold = ULUA_TRUE;
	r->constnum = c->num;
	yygotominor.yy88 = r;
}
#line 1493 "src/uparser.c"
        break;
      case 75: /* tableconstructor ::= LBRACE RBRACE */
#line 371 "src/uparser.y"
{
    	DPRINTF("P_TABLECONSTRUCTOR_LBRACE_RBRACE\n");

	yygotominor.yy88 = doTable(f);
}
#line 1502 "src/uparser.c"
        break;
      case 76: /* tableconstructor ::= LBRACE fieldlist RBRACE */
#line 376 "src/uparser.y"
{
    DPRINTF("P_TABLECONSTRUCTOR_LBRACE_FIELDLIST_RBRACE\n");
}
#line 1509 "src/uparser.c"
        break;
      case 77: /* tableconstructor ::= LBRACE fieldlist COMMA|SEMICOL RBRACE */
#line 379 "src/uparser.y"
{
    DPRINTF("P_TABLECONSTRUCTOR_LBRACE_FIELDLIST_COMMA_SEMICOL_RBRACE\n");
}
#line 1516 "src/uparser.c"
        break;
      case 78: /* fieldlist ::= field */
#line 383 "src/uparser.y"
{
    DPRINTF("P_FIELDLIST_FIELD\n");
}
#line 1523 "src/uparser.c"
        break;
      case 79: /* fieldlist ::= fieldlist COMMA|SEMICOL field */
#line 386 "src/uparser.y"
{
    DPRINTF("P_FIELDLIST_FIELDLIST_COMMA_SEMICOL_FIELD\n");
}
#line 1530 "src/uparser.c"
        break;
      case 80: /* field ::= exp */
#line 390 "src/uparser.y"
{
    DPRINTF("P_FIELD_EXP\n");
}
#line 1537 "src/uparser.c"
        break;
      case 81: /* field ::= NAME SET exp */
#line 393 "src/uparser.y"
{
    DPRINTF("P_FIELD_NAME_SET_EXP\n");
}
#line 1544 "src/uparser.c"
        break;
      case 82: /* field ::= SLPAREN exp SRPAREN SET exp */
#line 396 "src/uparser.y"
{
    DPRINTF("P_FIELD_SLPAREN_EXP_SRPAREN_SET_EXP\n");
}
#line 1551 "src/uparser.c"
        break;
      default:
      /* (12) stat ::= repetition DO block END */ yytestcase(yyruleno==12);
      /* (13) stat ::= REPEAT ublock */ yytestcase(yyruleno==13);
      /* (18) repetition ::= FOR NAME SET explist23 */ yytestcase(yyruleno==18);
      /* (19) repetition ::= FOR namelist IN explist1 */ yytestcase(yyruleno==19);
      /* (25) laststat ::= BREAK */ yytestcase(yyruleno==25);
      /* (26) laststat ::= RETURN */ yytestcase(yyruleno==26);
      /* (27) laststat ::= RETURN explist1 */ yytestcase(yyruleno==27);
      /* (39) explist23 ::= exp COMMA exp */ yytestcase(yyruleno==39);
      /* (40) explist23 ::= exp COMMA exp COMMA exp */ yytestcase(yyruleno==40);
      /* (43) exp ::= DOTS */ yytestcase(yyruleno==43);
      /* (49) exp ::= HASH exp */ yytestcase(yyruleno==49);
      /* (53) exp ::= exp CONCAT exp */ yytestcase(yyruleno==53);
      /* (69) function ::= FUNCTION params block END */ yytestcase(yyruleno==69);
      /* (70) params ::= LPAREN parlist LPAREN */ yytestcase(yyruleno==70);
      /* (71) parlist ::= */ yytestcase(yyruleno==71);
      /* (72) parlist ::= namelist */ yytestcase(yyruleno==72);
      /* (73) parlist ::= DOTS */ yytestcase(yyruleno==73);
      /* (74) parlist ::= namelist COMMA DOTS */ yytestcase(yyruleno==74);
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact < YYNSTATE ){
#ifdef NDEBUG
    /* If we are not debugging and the reduce action popped at least
    ** one element off the stack, then we can push the new element back
    ** onto the stack here, and skip the stack overflow test in yy_shift().
    ** That gives a significant speed improvement. */
    if( yysize ){
      yypParser->yyidx++;
      yymsp -= yysize-1;
      yymsp->stateno = (YYACTIONTYPE)yyact;
      yymsp->major = (YYCODETYPE)yygoto;
      yymsp->minor = yygotominor;
    }else
#endif
    {
      yy_shift(yypParser,yyact,yygoto,&yygotominor);
    }
  }else{
    //assert( yyact == YYNSTATE + YYNRULE + 1 );
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 41 "src/uparser.y"

  f->error_code = E_SYNTAX_ERROR;
#line 1634 "src/uparser.c"
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
#line 37 "src/uparser.y"

	f->parsed = ULUA_TRUE;
#line 1656 "src/uparser.c"
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      /*memset(&yyminorunion, 0, sizeof(yyminorunion));*/
      yyminorunion = yyzerominor;
      yyStackOverflow(yypParser, &yyminorunion);
      return;
    }
#endif
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact<YYNSTATE ){
      //assert( !yyendofinput );  /* Impossible to shift the $ token */
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      yymajor = YYNOCODE;
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
    }else{
      //assert( yyact == YY_ERROR_ACTION );
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error size to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor,yyminorunion);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}
