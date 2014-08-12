/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>
#line 1 "uparser.y"

#include <stdio.h>
#include "basetypes.h"
#include "ulexer.h"
#include "uparser.h"
#line 14 "uparser.c"
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
#define YYNOCODE 83
#define YYACTIONTYPE unsigned short int
#define ParseTOKENTYPE Token
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  uExpression* yy48;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL  u08* code ;
#define ParseARG_PDECL , u08* code 
#define ParseARG_FETCH  u08* code  = yypParser->code 
#define ParseARG_STORE yypParser->code  = code 
#define YYNSTATE 166
#define YYNRULE 86
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
#define YY_ACTTAB_COUNT (695)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    46,   13,   31,   28,   27,   27,   27,   27,   27,   27,
 /*    10 */    26,   25,   24,   23,   22,   21,   84,   51,   20,   31,
 /*    20 */    28,   27,   27,   27,   27,   27,   27,   26,   25,   24,
 /*    30 */    23,   22,   21,   97,   96,   20,   23,   22,   21,   43,
 /*    40 */    11,   20,   31,   28,   27,   27,   27,   27,   27,   27,
 /*    50 */    26,   25,   24,   23,   22,   21,  117,  253,   20,   26,
 /*    60 */    25,   24,   23,   22,   21,   56,    4,   20,  151,   31,
 /*    70 */    28,   27,   27,   27,   27,   27,   27,   26,   25,   24,
 /*    80 */    23,   22,   21,  103,  104,   20,    4,  132,   89,   15,
 /*    90 */     5,  149,    2,  105,   31,   28,   27,   27,   27,   27,
 /*   100 */    27,   27,   26,   25,   24,   23,   22,   21,  132,   30,
 /*   110 */    20,    5,   42,    2,  166,  143,  135,  161,  139,   31,
 /*   120 */    28,   27,   27,   27,   27,   27,   27,   26,   25,   24,
 /*   130 */    23,   22,   21,  107,  108,   20,  160,  154,   20,  150,
 /*   140 */    12,   31,   28,   27,   27,   27,   27,   27,   27,   26,
 /*   150 */    25,   24,   23,   22,   21,   87,  146,   20,   31,   28,
 /*   160 */    27,   27,   27,   27,   27,   27,   26,   25,   24,   23,
 /*   170 */    22,   21,   52,   19,   20,  106,   28,   27,   27,   27,
 /*   180 */    27,   27,   27,   26,   25,   24,   23,   22,   21,  100,
 /*   190 */    10,   20,   52,  138,   18,  154,   29,   16,   56,  130,
 /*   200 */    29,   29,   33,  159,  159,  159,  159,  158,  157,   17,
 /*   210 */   225,   19,   52,    2,  142,  106,   29,   44,    6,  148,
 /*   220 */    29,   29,  225,  159,  159,  159,  159,  158,  157,    8,
 /*   230 */    92,   19,  134,    2,   37,  131,   29,   14,   92,  123,
 /*   240 */    29,   29,  129,  159,  159,  159,  159,  158,  157,   17,
 /*   250 */   147,   19,   52,    2,  144,  154,   27,   27,   27,   27,
 /*   260 */    27,   27,   26,   25,   24,   23,   22,   21,  116,  115,
 /*   270 */    20,  124,  122,  109,   15,   82,   29,   56,   56,  118,
 /*   280 */    29,   29,   56,  159,  159,  159,  159,  158,  157,   35,
 /*   290 */   152,   19,   47,    2,   32,   34,    1,   39,  148,   50,
 /*   300 */   154,   79,   83,  101,   53,  119,    7,   48,   59,  153,
 /*   310 */     9,  128,   56,  156,   55,   35,   75,  224,  165,  126,
 /*   320 */    56,   54,  102,   36,  152,  230,  230,   49,   56,  224,
 /*   330 */    98,   99,   88,   85,  125,  155,   19,   70,  153,   56,
 /*   340 */     3,   56,  156,  164,   59,  153,  136,  149,  163,  156,
 /*   350 */   230,  230,   36,  152,  162,  230,  133,   58,  153,   36,
 /*   360 */   152,   45,  156,   94,  155,   78,  140,   38,   41,   81,
 /*   370 */   127,  155,   36,  152,   67,  153,  137,   95,  145,  156,
 /*   380 */   121,  120,   40,   80,  155,  254,  133,  254,  254,   36,
 /*   390 */   152,   67,  153,  254,   93,  254,  156,   67,  153,  254,
 /*   400 */    91,  155,  156,  254,  254,  254,   36,  152,  254,  254,
 /*   410 */   254,  254,   36,  152,  254,  254,  254,  254,  155,  254,
 /*   420 */   254,  254,  254,  254,  155,   67,  153,  254,   90,  254,
 /*   430 */   156,  254,   67,  153,  254,   86,  254,  156,  254,  254,
 /*   440 */    36,  152,  254,  254,  254,   70,  153,   36,  152,  254,
 /*   450 */   156,  254,  155,   63,  153,  254,  254,  254,  156,  155,
 /*   460 */    36,  152,  254,  254,  254,  254,   71,  153,   36,  152,
 /*   470 */   254,  156,  155,  254,  141,  254,  254,  254,  254,  254,
 /*   480 */   155,   36,  152,   60,  153,  114,  153,  254,  156,  254,
 /*   490 */   156,  254,  254,  155,  254,  254,  254,  254,   36,  152,
 /*   500 */    36,  152,  254,   72,  153,  254,   74,  153,  156,  254,
 /*   510 */   155,  156,  155,  254,  254,  254,  254,  254,   36,  152,
 /*   520 */   254,   36,  152,  254,   73,  153,  254,   77,  153,  156,
 /*   530 */   155,  254,  156,  155,  254,  254,  254,  254,  254,   36,
 /*   540 */   152,  254,   36,  152,  254,  254,  254,  254,  254,  254,
 /*   550 */   254,  155,   76,  153,  155,  254,  254,  156,  254,  113,
 /*   560 */   153,  112,  153,  254,  156,  254,  156,   36,  152,  254,
 /*   570 */   254,  254,  254,  254,   36,  152,   36,  152,  254,  155,
 /*   580 */   111,  153,  110,  153,  254,  156,  155,  156,  155,  254,
 /*   590 */   254,  254,  254,  254,  254,   36,  152,   36,  152,  254,
 /*   600 */   254,  254,  254,  254,  254,   62,  153,  155,  254,  155,
 /*   610 */   156,  254,   69,  153,   61,  153,  254,  156,  254,  156,
 /*   620 */    36,  152,  254,  254,  254,  254,  254,   36,  152,   36,
 /*   630 */   152,  254,  155,   68,  153,   66,  153,  254,  156,  155,
 /*   640 */   156,  155,  254,  254,  254,  254,  254,  254,   36,  152,
 /*   650 */    36,  152,  254,  254,  254,  254,  254,  254,   65,  153,
 /*   660 */   155,  254,  155,  156,  254,   57,  153,   64,  153,  254,
 /*   670 */   156,  254,  156,   36,  152,  254,  254,  254,  254,  254,
 /*   680 */    36,  152,   36,  152,  254,  155,  254,  254,  254,  254,
 /*   690 */   254,  254,  155,  254,  155,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     3,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*    10 */    30,   31,   32,   33,   34,   35,   64,   65,   38,   22,
 /*    20 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*    30 */    33,   34,   35,   19,   20,   38,   33,   34,   35,   13,
 /*    40 */    14,   38,   22,   23,   24,   25,   26,   27,   28,   29,
 /*    50 */    30,   31,   32,   33,   34,   35,   62,   63,   38,   30,
 /*    60 */    31,   32,   33,   34,   35,   71,    1,   38,   48,   22,
 /*    70 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*    80 */    33,   34,   35,   19,   20,   38,   21,   44,    8,   21,
 /*    90 */    47,   11,   49,   46,   22,   23,   24,   25,   26,   27,
 /*   100 */    28,   29,   30,   31,   32,   33,   34,   35,   44,   45,
 /*   110 */    38,   47,   15,   49,    0,   50,   48,    4,   46,   22,
 /*   120 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*   130 */    33,   34,   35,   59,   60,   38,    4,   11,   38,    4,
 /*   140 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*   150 */    31,   32,   33,   34,   35,   21,   47,   38,   22,   23,
 /*   160 */    24,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*   170 */    34,   35,    8,   47,   38,   11,   23,   24,   25,   26,
 /*   180 */    27,   28,   29,   30,   31,   32,   33,   34,   35,   62,
 /*   190 */     9,   38,    8,   11,    9,   11,   32,    9,   71,   72,
 /*   200 */    36,   37,   21,   39,   40,   41,   42,   43,   44,   45,
 /*   210 */     9,   47,    8,   49,   50,   11,   32,    3,    9,   11,
 /*   220 */    36,   37,   21,   39,   40,   41,   42,   43,   44,   12,
 /*   230 */    21,   47,   48,   49,   11,    4,   32,    2,   21,   11,
 /*   240 */    36,   37,    4,   39,   40,   41,   42,   43,   44,   45,
 /*   250 */    42,   47,    8,   49,   50,   11,   24,   25,   26,   27,
 /*   260 */    28,   29,   30,   31,   32,   33,   34,   35,   62,   62,
 /*   270 */    38,    4,   11,   62,   21,   53,   32,   71,   71,    4,
 /*   280 */    36,   37,   71,   39,   40,   41,   42,   43,   44,   67,
 /*   290 */    68,   47,    3,   49,    5,    6,    7,    8,   11,   10,
 /*   300 */    11,   53,   54,   62,   56,   16,   17,   18,   52,   53,
 /*   310 */     9,   62,   71,   57,   66,   67,   68,    9,    1,   62,
 /*   320 */    71,   73,   74,   67,   68,   19,   20,   11,   71,   21,
 /*   330 */    62,   75,   62,   77,   78,   79,   47,   52,   53,   71,
 /*   340 */    61,   71,   57,   70,   52,   53,   69,   11,   70,   57,
 /*   350 */    44,   45,   67,   68,   70,   49,   79,   52,   53,   67,
 /*   360 */    68,   58,   57,   11,   79,   80,   81,   47,   58,   60,
 /*   370 */    78,   79,   67,   68,   52,   53,   69,   55,   42,   57,
 /*   380 */    11,   76,   58,   60,   79,   82,   79,   82,   82,   67,
 /*   390 */    68,   52,   53,   82,   55,   82,   57,   52,   53,   82,
 /*   400 */    55,   79,   57,   82,   82,   82,   67,   68,   82,   82,
 /*   410 */    82,   82,   67,   68,   82,   82,   82,   82,   79,   82,
 /*   420 */    82,   82,   82,   82,   79,   52,   53,   82,   55,   82,
 /*   430 */    57,   82,   52,   53,   82,   55,   82,   57,   82,   82,
 /*   440 */    67,   68,   82,   82,   82,   52,   53,   67,   68,   82,
 /*   450 */    57,   82,   79,   52,   53,   82,   82,   82,   57,   79,
 /*   460 */    67,   68,   82,   82,   82,   82,   52,   53,   67,   68,
 /*   470 */    82,   57,   79,   82,   81,   82,   82,   82,   82,   82,
 /*   480 */    79,   67,   68,   52,   53,   52,   53,   82,   57,   82,
 /*   490 */    57,   82,   82,   79,   82,   82,   82,   82,   67,   68,
 /*   500 */    67,   68,   82,   52,   53,   82,   52,   53,   57,   82,
 /*   510 */    79,   57,   79,   82,   82,   82,   82,   82,   67,   68,
 /*   520 */    82,   67,   68,   82,   52,   53,   82,   52,   53,   57,
 /*   530 */    79,   82,   57,   79,   82,   82,   82,   82,   82,   67,
 /*   540 */    68,   82,   67,   68,   82,   82,   82,   82,   82,   82,
 /*   550 */    82,   79,   52,   53,   79,   82,   82,   57,   82,   52,
 /*   560 */    53,   52,   53,   82,   57,   82,   57,   67,   68,   82,
 /*   570 */    82,   82,   82,   82,   67,   68,   67,   68,   82,   79,
 /*   580 */    52,   53,   52,   53,   82,   57,   79,   57,   79,   82,
 /*   590 */    82,   82,   82,   82,   82,   67,   68,   67,   68,   82,
 /*   600 */    82,   82,   82,   82,   82,   52,   53,   79,   82,   79,
 /*   610 */    57,   82,   52,   53,   52,   53,   82,   57,   82,   57,
 /*   620 */    67,   68,   82,   82,   82,   82,   82,   67,   68,   67,
 /*   630 */    68,   82,   79,   52,   53,   52,   53,   82,   57,   79,
 /*   640 */    57,   79,   82,   82,   82,   82,   82,   82,   67,   68,
 /*   650 */    67,   68,   82,   82,   82,   82,   82,   82,   52,   53,
 /*   660 */    79,   82,   79,   57,   82,   52,   53,   52,   53,   82,
 /*   670 */    57,   82,   57,   67,   68,   82,   82,   82,   82,   82,
 /*   680 */    67,   68,   67,   68,   82,   79,   82,   82,   82,   82,
 /*   690 */    82,   82,   79,   82,   79,
};
#define YY_SHIFT_USE_DFLT (-21)
#define YY_SHIFT_COUNT (117)
#define YY_SHIFT_MIN   (-20)
#define YY_SHIFT_MAX   (369)
static const short yy_shift_ofst[] = {
 /*     0 */   -21,  244,  204,  289,  164,  184,  244,  244,  244,  244,
 /*    10 */   244,  244,  244,  244,  244,  244,  244,  244,  244,  244,
 /*    20 */   244,  244,  244,  244,  244,  244,  244,  244,  244,  244,
 /*    30 */   244,  244,  244,  126,  -21,   64,   64,   43,  336,  369,
 /*    40 */   -21,  -21,  -21,  -21,  -21,  -21,  -21,  -21,   80,  320,
 /*    50 */   352,  320,  320,  317,  317,  317,  -21,  119,  -20,   97,
 /*    60 */    72,   47,   20,   -3,  136,  136,  136,  136,  136,  136,
 /*    70 */   136,  153,  232,   29,   29,  306,    3,    3,   65,  308,
 /*    80 */   209,  217,  201,  181,   14,   26,   68,  208,  275,  316,
 /*    90 */   253,  253,  287,  253,  301,  253,  261,  228,  267,  238,
 /*   100 */   235,  231,  214,  223,  182,  188,  185,  109,  134,  135,
 /*   110 */   100,  100,  100,  100,  100,  132,  113,  114,
};
#define YY_REDUCE_USE_DFLT (-49)
#define YY_REDUCE_COUNT (56)
#define YY_REDUCE_MIN   (-48)
#define YY_REDUCE_MAX   (615)
static const short yy_reduce_ofst[] = {
 /*     0 */    -6,  256,  285,  248,  393,  380,  373,  345,  339,  305,
 /*    10 */   322,  292,  615,  613,  606,  583,  581,  562,  560,  553,
 /*    20 */   530,  528,  509,  507,  500,  475,  472,  454,  451,  433,
 /*    30 */   431,  414,  401,  222,  127,  307,  307,  277,   74,  -48,
 /*    40 */   270,  268,  257,  249,  241,  211,  207,  206,  323,  324,
 /*    50 */   309,  310,  303,  284,  278,  273,  279,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   172,  252,  252,  169,  252,  252,  252,  192,  252,  252,
 /*    10 */   252,  252,  252,  252,  252,  252,  252,  252,  252,  252,
 /*    20 */   252,  252,  252,  252,  252,  252,  252,  252,  252,  252,
 /*    30 */   252,  252,  252,  252,  172,  252,  211,  252,  240,  252,
 /*    40 */   172,  172,  172,  172,  172,  172,  172,  172,  252,  252,
 /*    50 */   252,  252,  252,  168,  168,  168,  174,  205,  252,  252,
 /*    60 */   252,  252,  252,  252,  206,  171,  204,  203,  251,  250,
 /*    70 */   249,  214,  215,  217,  216,  183,  219,  218,  252,  229,
 /*    80 */   194,  252,  229,  252,  197,  186,  252,  252,  252,  252,
 /*    90 */   195,  193,  252,  185,  201,  182,  252,  252,  252,  252,
 /*   100 */   252,  252,  252,  252,  252,  252,  226,  252,  241,  252,
 /*   110 */   223,  222,  221,  220,  213,  252,  252,  252,  196,  191,
 /*   120 */   184,  199,  200,  198,  181,  188,  190,  189,  187,  180,
 /*   130 */   179,  178,  237,  236,  234,  235,  233,  232,  228,  227,
 /*   140 */   247,  248,  246,  245,  244,  242,  239,  243,  202,  201,
 /*   150 */   238,  231,  230,  229,  226,  212,  210,  209,  208,  207,
 /*   160 */   177,  176,  175,  173,  170,  167,
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
  "$",             "SEMICOL",       "UNTIL",         "DO",          
  "END",           "WHILE",         "REPEAT",        "IF",          
  "FUNCTION",      "SET",           "FOR",           "NAME",        
  "IN",            "ELSE",          "ELSEIF",        "THEN",        
  "BREAK",         "RETURN",        "LOCAL",         "COLON",       
  "DOT",           "COMMA",         "OR",            "AND",         
  "L",             "LE",            "G",             "GE",          
  "EQ",            "NE",            "CONCAT",        "PLUS",        
  "MINUS",         "TIMES",         "DIVIDE",        "MOD",         
  "NOT",           "HASH",          "POW",           "NIL",         
  "TRUE",          "FALSE",         "DOTS",          "NUMBER",      
  "STRING",        "SLPAREN",       "SRPAREN",       "LPAREN",      
  "RPAREN",        "LBRACE",        "RBRACE",        "error",       
  "exp",           "var",           "setlist",       "explist1",    
  "stat",          "function",      "params",        "parlist",     
  "namelist",      "statlist",      "block",         "chunk",       
  "dottedname",    "funcname",      "laststat",      "prefixexp",   
  "functioncall",  "args",          "semi",          "scope",       
  "ublock",        "binding",       "repetition",    "conds",       
  "explist23",     "condlist",      "cond",          "tableconstructor",
  "fieldlist",     "field",       
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
 /*  41 */ "exp ::= NIL|TRUE|FALSE|DOTS",
 /*  42 */ "exp ::= NUMBER",
 /*  43 */ "exp ::= STRING",
 /*  44 */ "exp ::= function",
 /*  45 */ "exp ::= prefixexp",
 /*  46 */ "exp ::= tableconstructor",
 /*  47 */ "exp ::= NOT|HASH|MINUS exp",
 /*  48 */ "exp ::= exp OR exp",
 /*  49 */ "exp ::= exp AND exp",
 /*  50 */ "exp ::= exp L|LE|G|GE|EQ|NE exp",
 /*  51 */ "exp ::= exp CONCAT exp",
 /*  52 */ "exp ::= exp PLUS exp",
 /*  53 */ "exp ::= exp MINUS exp",
 /*  54 */ "exp ::= exp TIMES exp",
 /*  55 */ "exp ::= exp DIVIDE exp",
 /*  56 */ "exp ::= exp MOD exp",
 /*  57 */ "exp ::= exp POW exp",
 /*  58 */ "setlist ::= var",
 /*  59 */ "setlist ::= setlist COMMA var",
 /*  60 */ "var ::= NAME",
 /*  61 */ "var ::= prefixexp SLPAREN exp SRPAREN",
 /*  62 */ "var ::= prefixexp DOT NAME",
 /*  63 */ "prefixexp ::= var",
 /*  64 */ "prefixexp ::= functioncall",
 /*  65 */ "prefixexp ::= LPAREN exp RPAREN",
 /*  66 */ "functioncall ::= prefixexp args",
 /*  67 */ "functioncall ::= prefixexp COLON NAME args",
 /*  68 */ "args ::= LPAREN RPAREN",
 /*  69 */ "args ::= LPAREN explist1 RPAREN",
 /*  70 */ "args ::= tableconstructor",
 /*  71 */ "args ::= STRING",
 /*  72 */ "function ::= FUNCTION params block END",
 /*  73 */ "params ::= LPAREN parlist LPAREN",
 /*  74 */ "parlist ::=",
 /*  75 */ "parlist ::= namelist",
 /*  76 */ "parlist ::= DOTS",
 /*  77 */ "parlist ::= namelist COMMA DOTS",
 /*  78 */ "tableconstructor ::= LBRACE RBRACE",
 /*  79 */ "tableconstructor ::= LBRACE fieldlist RBRACE",
 /*  80 */ "tableconstructor ::= LBRACE fieldlist COMMA|SEMICOL RBRACE",
 /*  81 */ "fieldlist ::= field",
 /*  82 */ "fieldlist ::= fieldlist COMMA|SEMICOL field",
 /*  83 */ "field ::= exp",
 /*  84 */ "field ::= NAME SET exp",
 /*  85 */ "field ::= SLPAREN exp SRPAREN SET exp",
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
static void *ParseAlloc(void *(*mallocProc)(size_t)){
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
static void ParseFree(
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
  { 63, 1 },
  { 70, 1 },
  { 70, 0 },
  { 62, 2 },
  { 62, 4 },
  { 72, 3 },
  { 71, 0 },
  { 71, 4 },
  { 61, 0 },
  { 61, 3 },
  { 56, 3 },
  { 56, 5 },
  { 56, 4 },
  { 56, 2 },
  { 56, 3 },
  { 56, 5 },
  { 56, 3 },
  { 56, 1 },
  { 74, 4 },
  { 74, 4 },
  { 75, 1 },
  { 75, 3 },
  { 77, 1 },
  { 77, 3 },
  { 78, 3 },
  { 66, 1 },
  { 66, 1 },
  { 66, 2 },
  { 73, 2 },
  { 73, 4 },
  { 73, 6 },
  { 65, 1 },
  { 65, 3 },
  { 64, 1 },
  { 64, 3 },
  { 60, 1 },
  { 60, 3 },
  { 55, 1 },
  { 55, 3 },
  { 76, 3 },
  { 76, 5 },
  { 52, 1 },
  { 52, 1 },
  { 52, 1 },
  { 52, 1 },
  { 52, 1 },
  { 52, 1 },
  { 52, 2 },
  { 52, 3 },
  { 52, 3 },
  { 52, 3 },
  { 52, 3 },
  { 52, 3 },
  { 52, 3 },
  { 52, 3 },
  { 52, 3 },
  { 52, 3 },
  { 52, 3 },
  { 54, 1 },
  { 54, 3 },
  { 53, 1 },
  { 53, 4 },
  { 53, 3 },
  { 67, 1 },
  { 67, 1 },
  { 67, 3 },
  { 68, 2 },
  { 68, 4 },
  { 69, 2 },
  { 69, 3 },
  { 69, 1 },
  { 69, 1 },
  { 57, 4 },
  { 58, 3 },
  { 59, 0 },
  { 59, 1 },
  { 59, 1 },
  { 59, 3 },
  { 79, 2 },
  { 79, 3 },
  { 79, 4 },
  { 80, 1 },
  { 80, 3 },
  { 81, 1 },
  { 81, 3 },
  { 81, 5 },
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
#line 38 "uparser.y"
{
	makeDump(yymsp[0].minor.yy48, code);
}
#line 1007 "uparser.c"
        break;
      case 3: /* block ::= scope statlist */
#line 45 "uparser.y"
{
	yygotominor.yy48 = makeExpr(EXP_CHUNK, yymsp[0].minor.yy48, NULL, NULL, code);
}
#line 1014 "uparser.c"
        break;
      case 4: /* block ::= scope statlist laststat semi */
#line 48 "uparser.y"
{
	if(yymsp[-1].minor.yy48 != NULL)
		yygotominor.yy48 = makeExpr(EXP_CHUNK, addList(yymsp[-2].minor.yy48,yymsp[-1].minor.yy48, code), NULL, NULL, code);
	else
		yygotominor.yy48 = makeExpr(EXP_CHUNK, yymsp[-2].minor.yy48, NULL, NULL, code);
}
#line 1024 "uparser.c"
        break;
      case 8: /* statlist ::= */
      case 74: /* parlist ::= */ yytestcase(yyruleno==74);
#line 59 "uparser.y"
{
	yygotominor.yy48 = makeList(NULL, code);
}
#line 1032 "uparser.c"
        break;
      case 9: /* statlist ::= statlist stat semi */
#line 62 "uparser.y"
{
	if(yymsp[-1].minor.yy48 != NULL) {
		yygotominor.yy48 = addList(yymsp[-2].minor.yy48, yymsp[-1].minor.yy48, code);
	} else {
		yygotominor.yy48 = yymsp[-2].minor.yy48;
	}
}
#line 1043 "uparser.c"
        break;
      case 15: /* stat ::= FUNCTION funcname params block END */
#line 75 "uparser.y"
{
	yygotominor.yy48 = makeExpr(EXP_FUNCTION, yymsp[-3].minor.yy48, yymsp[-2].minor.yy48, yymsp[-1].minor.yy48, code);
}
#line 1050 "uparser.c"
        break;
      case 16: /* stat ::= setlist SET explist1 */
#line 78 "uparser.y"
{
	yygotominor.yy48 = makeExpr(EXP_SET, yymsp[-2].minor.yy48, yymsp[0].minor.yy48, NULL, code);
}
#line 1057 "uparser.c"
        break;
      case 17: /* stat ::= functioncall */
      case 31: /* funcname ::= dottedname */ yytestcase(yyruleno==31);
      case 44: /* exp ::= function */ yytestcase(yyruleno==44);
      case 45: /* exp ::= prefixexp */ yytestcase(yyruleno==45);
      case 63: /* prefixexp ::= var */ yytestcase(yyruleno==63);
      case 64: /* prefixexp ::= functioncall */ yytestcase(yyruleno==64);
      case 75: /* parlist ::= namelist */ yytestcase(yyruleno==75);
#line 81 "uparser.y"
{
	yygotominor.yy48 = yymsp[0].minor.yy48;
}
#line 1070 "uparser.c"
        break;
      case 25: /* laststat ::= BREAK */
#line 94 "uparser.y"
{
	yygotominor.yy48 = makeExpr(EXP_BREAK, NULL, NULL, NULL, code);
}
#line 1077 "uparser.c"
        break;
      case 26: /* laststat ::= RETURN */
#line 97 "uparser.y"
{
	yygotominor.yy48 = makeExpr(EXP_RETURN, NULL, NULL, NULL, code);
}
#line 1084 "uparser.c"
        break;
      case 27: /* laststat ::= RETURN explist1 */
#line 100 "uparser.y"
{
	yygotominor.yy48 = makeExpr(EXP_RETURN, yymsp[0].minor.yy48, NULL, NULL, code);
}
#line 1091 "uparser.c"
        break;
      case 33: /* dottedname ::= NAME */
      case 43: /* exp ::= STRING */ yytestcase(yyruleno==43);
      case 71: /* args ::= STRING */ yytestcase(yyruleno==71);
#line 113 "uparser.y"
{
	yygotominor.yy48 = makeConst(EXP_STRING, 0, &yymsp[0].minor.yy0.semInfo, code);
}
#line 1100 "uparser.c"
        break;
      case 34: /* dottedname ::= dottedname DOT NAME */
#line 116 "uparser.y"
{
	yygotominor.yy48 = makeExpr(EXP_LIST, yymsp[-2].minor.yy48, makeConst(EXP_STRING, 0, &yymsp[0].minor.yy0.semInfo, code), NULL, code);
}
#line 1107 "uparser.c"
        break;
      case 35: /* namelist ::= NAME */
#line 120 "uparser.y"
{
	yygotominor.yy48 = makeList(makeVariable(EXP_VARIABLE, &yymsp[0].minor.yy0.semInfo, code), code);
}
#line 1114 "uparser.c"
        break;
      case 36: /* namelist ::= namelist COMMA NAME */
#line 123 "uparser.y"
{
	yygotominor.yy48 = addList(yymsp[-2].minor.yy48, makeVariable(EXP_VARIABLE, &yymsp[0].minor.yy0.semInfo, code), code);
}
#line 1121 "uparser.c"
        break;
      case 37: /* explist1 ::= exp */
      case 58: /* setlist ::= var */ yytestcase(yyruleno==58);
#line 127 "uparser.y"
{
	yygotominor.yy48 = makeList(yymsp[0].minor.yy48, code);
}
#line 1129 "uparser.c"
        break;
      case 38: /* explist1 ::= explist1 COMMA exp */
      case 59: /* setlist ::= setlist COMMA var */ yytestcase(yyruleno==59);
#line 130 "uparser.y"
{
	yygotominor.yy48 = addList(yymsp[-2].minor.yy48, yymsp[0].minor.yy48, code);
}
#line 1137 "uparser.c"
        break;
      case 42: /* exp ::= NUMBER */
#line 146 "uparser.y"
{
	yygotominor.yy48 = makeConst(EXP_NUMBER, yymsp[0].minor.yy0.number.fvalue, NULL, code);
}
#line 1144 "uparser.c"
        break;
      case 52: /* exp ::= exp PLUS exp */
#line 164 "uparser.y"
{
	yygotominor.yy48 = makeExpr(EXP_ADD, yymsp[-2].minor.yy48,yymsp[0].minor.yy48, NULL, code);
}
#line 1151 "uparser.c"
        break;
      case 53: /* exp ::= exp MINUS exp */
#line 167 "uparser.y"
{
	yygotominor.yy48 = makeExpr(EXP_SUB, yymsp[-2].minor.yy48,yymsp[0].minor.yy48, NULL, code);
}
#line 1158 "uparser.c"
        break;
      case 54: /* exp ::= exp TIMES exp */
#line 170 "uparser.y"
{
	yygotominor.yy48 = makeExpr(EXP_MUL, yymsp[-2].minor.yy48,yymsp[0].minor.yy48, NULL, code);
}
#line 1165 "uparser.c"
        break;
      case 55: /* exp ::= exp DIVIDE exp */
#line 173 "uparser.y"
{
	yygotominor.yy48 = makeExpr(EXP_DIV, yymsp[-2].minor.yy48,yymsp[0].minor.yy48, NULL, code);
}
#line 1172 "uparser.c"
        break;
      case 60: /* var ::= NAME */
#line 186 "uparser.y"
{
	yygotominor.yy48 = makeVariable(EXP_VARIABLE, &yymsp[0].minor.yy0.semInfo, code);
}
#line 1179 "uparser.c"
        break;
      case 66: /* functioncall ::= prefixexp args */
#line 200 "uparser.y"
{
	yygotominor.yy48 = makeExpr(EXP_FUNCTION_CALL, yymsp[-1].minor.yy48, yymsp[0].minor.yy48, NULL, code);
}
#line 1186 "uparser.c"
        break;
      case 68: /* args ::= LPAREN RPAREN */
#line 205 "uparser.y"
{
	yygotominor.yy48 = NULL;
}
#line 1193 "uparser.c"
        break;
      case 69: /* args ::= LPAREN explist1 RPAREN */
      case 73: /* params ::= LPAREN parlist LPAREN */ yytestcase(yyruleno==73);
#line 208 "uparser.y"
{
	yygotominor.yy48 = yymsp[-1].minor.yy48;
}
#line 1201 "uparser.c"
        break;
      case 72: /* function ::= FUNCTION params block END */
#line 216 "uparser.y"
{
	yygotominor.yy48 = makeExpr(EXP_FUNCTION, yymsp[-2].minor.yy48, yymsp[-1].minor.yy48, NULL, code);
}
#line 1208 "uparser.c"
        break;
      default:
      /* (1) semi ::= SEMICOL */ yytestcase(yyruleno==1);
      /* (2) semi ::= */ yytestcase(yyruleno==2);
      /* (5) ublock ::= block UNTIL exp */ yytestcase(yyruleno==5);
      /* (6) scope ::= */ yytestcase(yyruleno==6);
      /* (7) scope ::= scope statlist binding semi */ yytestcase(yyruleno==7);
      /* (10) stat ::= DO block END */ yytestcase(yyruleno==10);
      /* (11) stat ::= WHILE exp DO block END */ yytestcase(yyruleno==11);
      /* (12) stat ::= repetition DO block END */ yytestcase(yyruleno==12);
      /* (13) stat ::= REPEAT ublock */ yytestcase(yyruleno==13);
      /* (14) stat ::= IF conds END */ yytestcase(yyruleno==14);
      /* (18) repetition ::= FOR NAME SET explist23 */ yytestcase(yyruleno==18);
      /* (19) repetition ::= FOR namelist IN explist1 */ yytestcase(yyruleno==19);
      /* (20) conds ::= condlist */ yytestcase(yyruleno==20);
      /* (21) conds ::= condlist ELSE block */ yytestcase(yyruleno==21);
      /* (22) condlist ::= cond */ yytestcase(yyruleno==22);
      /* (23) condlist ::= condlist ELSEIF cond */ yytestcase(yyruleno==23);
      /* (24) cond ::= exp THEN block */ yytestcase(yyruleno==24);
      /* (28) binding ::= LOCAL namelist */ yytestcase(yyruleno==28);
      /* (29) binding ::= LOCAL namelist SET explist1 */ yytestcase(yyruleno==29);
      /* (30) binding ::= LOCAL FUNCTION NAME params block END */ yytestcase(yyruleno==30);
      /* (32) funcname ::= dottedname COLON NAME */ yytestcase(yyruleno==32);
      /* (39) explist23 ::= exp COMMA exp */ yytestcase(yyruleno==39);
      /* (40) explist23 ::= exp COMMA exp COMMA exp */ yytestcase(yyruleno==40);
      /* (41) exp ::= NIL|TRUE|FALSE|DOTS */ yytestcase(yyruleno==41);
      /* (46) exp ::= tableconstructor */ yytestcase(yyruleno==46);
      /* (47) exp ::= NOT|HASH|MINUS exp */ yytestcase(yyruleno==47);
      /* (48) exp ::= exp OR exp */ yytestcase(yyruleno==48);
      /* (49) exp ::= exp AND exp */ yytestcase(yyruleno==49);
      /* (50) exp ::= exp L|LE|G|GE|EQ|NE exp */ yytestcase(yyruleno==50);
      /* (51) exp ::= exp CONCAT exp */ yytestcase(yyruleno==51);
      /* (56) exp ::= exp MOD exp */ yytestcase(yyruleno==56);
      /* (57) exp ::= exp POW exp */ yytestcase(yyruleno==57);
      /* (61) var ::= prefixexp SLPAREN exp SRPAREN */ yytestcase(yyruleno==61);
      /* (62) var ::= prefixexp DOT NAME */ yytestcase(yyruleno==62);
      /* (65) prefixexp ::= LPAREN exp RPAREN */ yytestcase(yyruleno==65);
      /* (67) functioncall ::= prefixexp COLON NAME args */ yytestcase(yyruleno==67);
      /* (70) args ::= tableconstructor */ yytestcase(yyruleno==70);
      /* (76) parlist ::= DOTS */ yytestcase(yyruleno==76);
      /* (77) parlist ::= namelist COMMA DOTS */ yytestcase(yyruleno==77);
      /* (78) tableconstructor ::= LBRACE RBRACE */ yytestcase(yyruleno==78);
      /* (79) tableconstructor ::= LBRACE fieldlist RBRACE */ yytestcase(yyruleno==79);
      /* (80) tableconstructor ::= LBRACE fieldlist COMMA|SEMICOL RBRACE */ yytestcase(yyruleno==80);
      /* (81) fieldlist ::= field */ yytestcase(yyruleno==81);
      /* (82) fieldlist ::= fieldlist COMMA|SEMICOL field */ yytestcase(yyruleno==82);
      /* (83) field ::= exp */ yytestcase(yyruleno==83);
      /* (84) field ::= NAME SET exp */ yytestcase(yyruleno==84);
      /* (85) field ::= SLPAREN exp SRPAREN SET exp */ yytestcase(yyruleno==85);
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
#line 32 "uparser.y"

  printf ("Syntax error!\n");
#line 1320 "uparser.c"
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
static void Parse(
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
      **  * Set the error count to three.
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
