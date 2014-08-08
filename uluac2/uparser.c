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
#define YYNOCODE 84
#define YYACTIONTYPE unsigned char
#define ParseTOKENTYPE Token*
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  uExpression yy144;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL
#define ParseARG_PDECL
#define ParseARG_FETCH
#define ParseARG_STORE
#define YYNSTATE 163
#define YYNRULE 87
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
#define YY_ACTTAB_COUNT (612)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    47,   13,   31,   28,   27,   27,   27,   27,   27,   27,
 /*    10 */    26,   25,   24,   23,   22,   21,   57,  150,   20,   31,
 /*    20 */    28,   27,   27,   27,   27,   27,   27,   26,   25,   24,
 /*    30 */    23,   22,   21,  149,  144,   20,  112,  123,   54,  153,
 /*    40 */    36,  152,   31,   28,   27,   27,   27,   27,   27,   27,
 /*    50 */    26,   25,   24,   23,   22,   21,   44,   11,   20,   26,
 /*    60 */    25,   24,   23,   22,   21,  143,  163,   20,  148,   31,
 /*    70 */    28,   27,   27,   27,   27,   27,   27,   26,   25,   24,
 /*    80 */    23,   22,   21,   99,  100,   20,   23,   22,   21,  251,
 /*    90 */   113,   20,   54,  101,   31,   28,   27,   27,   27,   27,
 /*   100 */    27,   27,   26,   25,   24,   23,   22,   21,  128,   30,
 /*   110 */    20,    5,   43,    2,   94,   93,  129,  132,  135,   31,
 /*   120 */    28,   27,   27,   27,   27,   27,   27,   26,   25,   24,
 /*   130 */    23,   22,   21,  128,   86,   20,    5,  145,    2,  158,
 /*   140 */    12,   31,   28,   27,   27,   27,   27,   27,   27,   26,
 /*   150 */    25,   24,   23,   22,   21,  129,  133,   20,   31,   28,
 /*   160 */    27,   27,   27,   27,   27,   27,   26,   25,   24,   23,
 /*   170 */    22,   21,   42,    4,   20,  102,   28,   27,   27,   27,
 /*   180 */    27,   27,   27,   26,   25,   24,   23,   22,   21,    8,
 /*   190 */   157,   20,   42,    4,   80,  151,   29,   96,   89,   54,
 /*   200 */    29,   29,  126,  156,  156,  156,  156,  155,  154,   17,
 /*   210 */   149,   19,   42,    2,  138,  102,   29,   35,   10,   20,
 /*   220 */    29,   29,  139,  156,  156,  156,  156,  155,  154,  146,
 /*   230 */    33,   19,  130,    2,  142,   18,   29,  111,  222,   54,
 /*   240 */    29,   29,   15,  156,  156,  156,  156,  155,  154,   17,
 /*   250 */   222,   19,   42,    2,  140,  151,   27,   27,   27,   27,
 /*   260 */    27,   27,   26,   25,   24,   23,   22,   21,   40,  131,
 /*   270 */    20,  105,   97,   54,   54,   82,   29,    6,  227,  227,
 /*   280 */    29,   29,  145,  156,  156,  156,  156,  155,  154,   89,
 /*   290 */    85,   19,   48,    2,   32,   34,    1,   41,  134,   50,
 /*   300 */   151,   57,  150,  227,  227,  115,    7,   49,  227,  147,
 /*   310 */   124,  221,   54,  141,  122,   95,   54,   16,  149,  104,
 /*   320 */    46,   83,  121,  221,  153,   36,  152,   37,  103,  127,
 /*   330 */    77,   81,  120,   51,   68,  150,   19,   65,  150,   53,
 /*   340 */    92,   52,   98,   46,  114,   45,   73,   14,   56,  150,
 /*   350 */   125,  149,  119,   35,  149,   46,    3,  153,   36,  152,
 /*   360 */   153,   36,  152,   76,  136,  149,  116,  118,   15,  151,
 /*   370 */   144,  153,   36,  152,   65,  150,    9,   90,   65,  150,
 /*   380 */    39,   88,  161,   65,  150,  162,   87,   65,  150,  160,
 /*   390 */    84,  149,   68,  150,   91,  149,  159,  153,   36,  152,
 /*   400 */   149,  153,   36,  152,  149,   19,  153,   36,  152,  149,
 /*   410 */   153,   36,  152,   61,  150,  153,   36,  152,   69,  150,
 /*   420 */    79,   78,  137,   38,  117,  252,  252,   58,  150,  252,
 /*   430 */   149,  110,  150,  252,  252,  149,  153,   36,  152,  252,
 /*   440 */   252,  153,   36,  152,  149,   70,  150,  252,  149,  252,
 /*   450 */   153,   36,  152,  252,  153,   36,  152,   72,  150,  252,
 /*   460 */    71,  150,  149,  252,  252,  252,  252,  252,  153,   36,
 /*   470 */   152,   75,  150,  252,  149,   74,  150,  149,  252,  252,
 /*   480 */   153,   36,  152,  153,   36,  152,  252,  252,  149,  109,
 /*   490 */   150,  252,  149,  252,  153,   36,  152,  252,  153,   36,
 /*   500 */   152,  108,  150,  252,  107,  150,  149,  252,  252,  252,
 /*   510 */   252,  252,  153,   36,  152,  106,  150,  252,  149,  252,
 /*   520 */   252,  149,  252,  252,  153,   36,  152,  153,   36,  152,
 /*   530 */    60,  150,  149,   67,  150,  252,  252,  252,  153,   36,
 /*   540 */   152,  252,   59,  150,  252,   66,  150,  149,  252,  252,
 /*   550 */   149,  252,  252,  153,   36,  152,  153,   36,  152,  149,
 /*   560 */   252,  252,  149,  252,  252,  153,   36,  152,  153,   36,
 /*   570 */   152,   64,  150,  252,   63,  150,  252,  252,  252,  252,
 /*   580 */   252,  252,  252,   55,  150,  252,   62,  150,  149,  252,
 /*   590 */   252,  149,  252,  252,  153,   36,  152,  153,   36,  152,
 /*   600 */   149,  252,  252,  149,  252,  252,  153,   36,  152,  153,
 /*   610 */    36,  152,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     3,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*    10 */    30,   31,   32,   33,   34,   35,   52,   53,   38,   22,
 /*    20 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*    30 */    33,   34,   35,   69,   11,   38,   58,   73,   60,   75,
 /*    40 */    76,   77,   22,   23,   24,   25,   26,   27,   28,   29,
 /*    50 */    30,   31,   32,   33,   34,   35,   13,   14,   38,   30,
 /*    60 */    31,   32,   33,   34,   35,   42,    0,   38,   48,   22,
 /*    70 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*    80 */    33,   34,   35,   19,   20,   38,   33,   34,   35,   57,
 /*    90 */    58,   38,   60,   46,   22,   23,   24,   25,   26,   27,
 /*   100 */    28,   29,   30,   31,   32,   33,   34,   35,   44,   45,
 /*   110 */    38,   47,   15,   49,   19,   20,   77,   78,   46,   22,
 /*   120 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*   130 */    33,   34,   35,   44,    8,   38,   47,   11,   49,    4,
 /*   140 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*   150 */    31,   32,   33,   34,   35,   77,   78,   38,   22,   23,
 /*   160 */    24,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*   170 */    34,   35,    8,    1,   38,   11,   23,   24,   25,   26,
 /*   180 */    27,   28,   29,   30,   31,   32,   33,   34,   35,   12,
 /*   190 */     4,   38,    8,   21,   53,   11,   32,   58,   21,   60,
 /*   200 */    36,   37,   63,   39,   40,   41,   42,   43,   44,   45,
 /*   210 */    69,   47,    8,   49,   50,   11,   32,   76,    9,   38,
 /*   220 */    36,   37,   50,   39,   40,   41,   42,   43,   44,    4,
 /*   230 */    21,   47,   48,   49,   47,    9,   32,   58,    9,   60,
 /*   240 */    36,   37,   21,   39,   40,   41,   42,   43,   44,   45,
 /*   250 */    21,   47,    8,   49,   50,   11,   24,   25,   26,   27,
 /*   260 */    28,   29,   30,   31,   32,   33,   34,   35,   67,   48,
 /*   270 */    38,   58,   58,   60,   60,   74,   32,    9,   19,   20,
 /*   280 */    36,   37,   11,   39,   40,   41,   42,   43,   44,   21,
 /*   290 */    21,   47,    3,   49,    5,    6,    7,    8,   11,   10,
 /*   300 */    11,   52,   53,   44,   45,   16,   17,   18,   49,   68,
 /*   310 */    58,    9,   60,   42,   58,   66,   60,    9,   69,   71,
 /*   320 */    79,   72,   73,   21,   75,   76,   77,   11,   80,    4,
 /*   330 */    53,   54,   68,   56,   52,   53,   47,   52,   53,   62,
 /*   340 */    55,   64,   65,   79,   68,    3,   69,    2,   52,   53,
 /*   350 */     4,   69,   11,   76,   69,   79,   61,   75,   76,   77,
 /*   360 */    75,   76,   77,   81,   82,   69,   70,   11,   21,   11,
 /*   370 */    11,   75,   76,   77,   52,   53,    9,   55,   52,   53,
 /*   380 */    11,   55,   59,   52,   53,    1,   55,   52,   53,   59,
 /*   390 */    55,   69,   52,   53,   11,   69,   59,   75,   76,   77,
 /*   400 */    69,   75,   76,   77,   69,   47,   75,   76,   77,   69,
 /*   410 */    75,   76,   77,   52,   53,   75,   76,   77,   52,   53,
 /*   420 */    71,   71,   82,   47,   11,   83,   83,   52,   53,   83,
 /*   430 */    69,   52,   53,   83,   83,   69,   75,   76,   77,   83,
 /*   440 */    83,   75,   76,   77,   69,   52,   53,   83,   69,   83,
 /*   450 */    75,   76,   77,   83,   75,   76,   77,   52,   53,   83,
 /*   460 */    52,   53,   69,   83,   83,   83,   83,   83,   75,   76,
 /*   470 */    77,   52,   53,   83,   69,   52,   53,   69,   83,   83,
 /*   480 */    75,   76,   77,   75,   76,   77,   83,   83,   69,   52,
 /*   490 */    53,   83,   69,   83,   75,   76,   77,   83,   75,   76,
 /*   500 */    77,   52,   53,   83,   52,   53,   69,   83,   83,   83,
 /*   510 */    83,   83,   75,   76,   77,   52,   53,   83,   69,   83,
 /*   520 */    83,   69,   83,   83,   75,   76,   77,   75,   76,   77,
 /*   530 */    52,   53,   69,   52,   53,   83,   83,   83,   75,   76,
 /*   540 */    77,   83,   52,   53,   83,   52,   53,   69,   83,   83,
 /*   550 */    69,   83,   83,   75,   76,   77,   75,   76,   77,   69,
 /*   560 */    83,   83,   69,   83,   83,   75,   76,   77,   75,   76,
 /*   570 */    77,   52,   53,   83,   52,   53,   83,   83,   83,   83,
 /*   580 */    83,   83,   83,   52,   53,   83,   52,   53,   69,   83,
 /*   590 */    83,   69,   83,   83,   75,   76,   77,   75,   76,   77,
 /*   600 */    69,   83,   83,   69,   83,   83,   75,   76,   77,   75,
 /*   610 */    76,   77,
};
#define YY_SHIFT_USE_DFLT (-21)
#define YY_SHIFT_COUNT (113)
#define YY_SHIFT_MIN   (-20)
#define YY_SHIFT_MAX   (413)
static const short yy_shift_ofst[] = {
 /*     0 */   -21,  244,  204,  289,  164,  184,  244,  244,  244,  244,
 /*    10 */   244,  244,  244,  244,  244,  244,  244,  244,  244,  244,
 /*    20 */   244,  244,  244,  244,  244,  244,  244,  244,  244,  244,
 /*    30 */   244,  244,  244,  358,  -21,   64,   64,   89,  271,  376,
 /*    40 */   376,  413,  376,  -21,  -21,  -21,  -21,  -21,  -21,  126,
 /*    50 */   383,  384,  384,  384,  -21,  119,  -20,   97,   72,   47,
 /*    60 */    20,   -3,  136,  136,  136,  136,  136,  136,  136,  153,
 /*    70 */   232,   29,   29,  259,   53,   53,  172,  302,  268,  177,
 /*    80 */   229,  209,   95,   43,  221,   23,  369,  347,  347,  359,
 /*    90 */   347,  367,  347,  356,  341,  346,  345,  325,  342,  316,
 /*   100 */   287,  308,  226,  187,  269,  225,  181,  181,  181,  181,
 /*   110 */   181,  186,  135,   66,
};
#define YY_REDUCE_USE_DFLT (-37)
#define YY_REDUCE_COUNT (54)
#define YY_REDUCE_MIN   (-36)
#define YY_REDUCE_MAX   (534)
static const short yy_reduce_ofst[] = {
 /*     0 */    32,  249,  282,  277,  340,  335,  331,  326,  322,  296,
 /*    10 */   285,  -36,  534,  531,  522,  519,  493,  490,  481,  478,
 /*    20 */   463,  452,  449,  437,  423,  419,  408,  405,  393,  379,
 /*    30 */   375,  366,  361,  141,  139,   78,   78,   39,  248,  276,
 /*    40 */   264,  201,  241,  256,  252,  214,  213,  179,  -22,  350,
 /*    50 */   349,  337,  330,  323,  295,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   169,  250,  250,  166,  250,  250,  250,  189,  250,  250,
 /*    10 */   250,  250,  250,  250,  250,  250,  250,  250,  250,  250,
 /*    20 */   250,  250,  250,  250,  250,  250,  250,  250,  250,  250,
 /*    30 */   250,  250,  250,  250,  169,  250,  208,  250,  238,  250,
 /*    40 */   250,  250,  250,  169,  169,  169,  169,  169,  169,  250,
 /*    50 */   250,  165,  165,  165,  171,  202,  250,  250,  250,  250,
 /*    60 */   250,  250,  203,  168,  201,  200,  249,  248,  247,  211,
 /*    70 */   212,  214,  213,  180,  216,  215,  250,  226,  191,  250,
 /*    80 */   226,  250,  194,  183,  250,  250,  250,  192,  190,  250,
 /*    90 */   182,  198,  179,  250,  250,  250,  250,  250,  250,  250,
 /*   100 */   250,  250,  223,  250,  239,  250,  220,  219,  218,  217,
 /*   110 */   210,  250,  250,  250,  193,  188,  181,  196,  197,  195,
 /*   120 */   178,  185,  187,  186,  184,  177,  176,  175,  234,  233,
 /*   130 */   231,  232,  230,  229,  225,  224,  245,  246,  244,  243,
 /*   140 */   242,  240,  237,  241,  199,  198,  236,  235,  228,  227,
 /*   150 */   226,  223,  209,  207,  206,  205,  204,  174,  173,  172,
 /*   160 */   170,  167,  164,
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
  "stat",          "chunk",         "block",         "semi",        
  "scope",         "statlist",      "laststat",      "ublock",      
  "binding",       "repetition",    "conds",         "funcname",    
  "funcbody",      "functioncall",  "explist23",     "namelist",    
  "condlist",      "cond",          "dottedname",    "function",    
  "prefixexp",     "tableconstructor",  "args",          "params",      
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
 /*  15 */ "stat ::= FUNCTION funcname funcbody",
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
 /*  30 */ "binding ::= LOCAL FUNCTION NAME funcbody",
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
 /*  72 */ "function ::= FUNCTION funcbody",
 /*  73 */ "funcbody ::= params block END",
 /*  74 */ "params ::= LPAREN parlist LPAREN",
 /*  75 */ "parlist ::=",
 /*  76 */ "parlist ::= namelist",
 /*  77 */ "parlist ::= DOTS",
 /*  78 */ "parlist ::= namelist COMMA DOTS",
 /*  79 */ "tableconstructor ::= LBRACE RBRACE",
 /*  80 */ "tableconstructor ::= LBRACE fieldlist RBRACE",
 /*  81 */ "tableconstructor ::= LBRACE fieldlist COMMA|SEMICOL RBRACE",
 /*  82 */ "fieldlist ::= field",
 /*  83 */ "fieldlist ::= fieldlist COMMA|SEMICOL field",
 /*  84 */ "field ::= exp",
 /*  85 */ "field ::= NAME SET exp",
 /*  86 */ "field ::= SLPAREN exp SRPAREN SET exp",
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
  { 57, 1 },
  { 59, 1 },
  { 59, 0 },
  { 58, 2 },
  { 58, 4 },
  { 63, 3 },
  { 60, 0 },
  { 60, 4 },
  { 61, 0 },
  { 61, 3 },
  { 56, 3 },
  { 56, 5 },
  { 56, 4 },
  { 56, 2 },
  { 56, 3 },
  { 56, 3 },
  { 56, 3 },
  { 56, 1 },
  { 65, 4 },
  { 65, 4 },
  { 66, 1 },
  { 66, 3 },
  { 72, 1 },
  { 72, 3 },
  { 73, 3 },
  { 62, 1 },
  { 62, 1 },
  { 62, 2 },
  { 64, 2 },
  { 64, 4 },
  { 64, 4 },
  { 67, 1 },
  { 67, 3 },
  { 74, 1 },
  { 74, 3 },
  { 71, 1 },
  { 71, 3 },
  { 55, 1 },
  { 55, 3 },
  { 70, 3 },
  { 70, 5 },
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
  { 76, 1 },
  { 76, 1 },
  { 76, 3 },
  { 69, 2 },
  { 69, 4 },
  { 78, 2 },
  { 78, 3 },
  { 78, 1 },
  { 78, 1 },
  { 75, 2 },
  { 68, 3 },
  { 79, 3 },
  { 80, 0 },
  { 80, 1 },
  { 80, 1 },
  { 80, 3 },
  { 77, 2 },
  { 77, 3 },
  { 77, 4 },
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
      case 16: /* stat ::= setlist SET explist1 */
#line 44 "uparser.y"
{
	yygotominor.yy144.fvalue = yymsp[0].minor.yy144.fvalue;
}
#line 993 "uparser.c"
        break;
      case 37: /* explist1 ::= exp */
#line 75 "uparser.y"
{
	yygotominor.yy144.fvalue = yymsp[0].minor.yy144.fvalue;
	yygotominor.yy144.name.bempty = yymsp[0].minor.yy144.name.bempty;
	yygotominor.yy144.name.bp = yymsp[0].minor.yy144.name.bp;
	yygotominor.yy144.name.bplen = yymsp[0].minor.yy144.name.bplen;
}
#line 1003 "uparser.c"
        break;
      case 42: /* exp ::= NUMBER */
#line 95 "uparser.y"
{
	yygotominor.yy144.fvalue = yymsp[0].minor.yy0->number.fvalue;
}
#line 1010 "uparser.c"
        break;
      case 43: /* exp ::= STRING */
      case 60: /* var ::= NAME */ yytestcase(yyruleno==60);
#line 98 "uparser.y"
{
	yygotominor.yy144.name.bempty = yymsp[0].minor.yy0->semInfo.bempty;
	yygotominor.yy144.name.bp = yymsp[0].minor.yy0->semInfo.bp;
	yygotominor.yy144.name.bplen = yymsp[0].minor.yy0->semInfo.bplen;
}
#line 1020 "uparser.c"
        break;
      case 52: /* exp ::= exp PLUS exp */
#line 111 "uparser.y"
{
	yygotominor.yy144.fvalue = yymsp[-2].minor.yy144.fvalue + yymsp[0].minor.yy144.fvalue;
}
#line 1027 "uparser.c"
        break;
      case 53: /* exp ::= exp MINUS exp */
#line 114 "uparser.y"
{
	yygotominor.yy144.fvalue = yymsp[-2].minor.yy144.fvalue - yymsp[0].minor.yy144.fvalue;
}
#line 1034 "uparser.c"
        break;
      case 54: /* exp ::= exp TIMES exp */
#line 117 "uparser.y"
{
	yygotominor.yy144.fvalue = yymsp[-2].minor.yy144.fvalue * yymsp[0].minor.yy144.fvalue;
}
#line 1041 "uparser.c"
        break;
      case 55: /* exp ::= exp DIVIDE exp */
#line 120 "uparser.y"
{
	yygotominor.yy144.fvalue = yymsp[-2].minor.yy144.fvalue / yymsp[0].minor.yy144.fvalue;
}
#line 1048 "uparser.c"
        break;
      case 58: /* setlist ::= var */
#line 126 "uparser.y"
{
	yygotominor.yy144.name.bempty = yymsp[0].minor.yy144.name.bempty;
	yygotominor.yy144.name.bp = yymsp[0].minor.yy144.name.bp;
	yygotominor.yy144.name.bplen = yymsp[0].minor.yy144.name.bplen;
}
#line 1057 "uparser.c"
        break;
      default:
      /* (0) chunk ::= block */ yytestcase(yyruleno==0);
      /* (1) semi ::= SEMICOL */ yytestcase(yyruleno==1);
      /* (2) semi ::= */ yytestcase(yyruleno==2);
      /* (3) block ::= scope statlist */ yytestcase(yyruleno==3);
      /* (4) block ::= scope statlist laststat semi */ yytestcase(yyruleno==4);
      /* (5) ublock ::= block UNTIL exp */ yytestcase(yyruleno==5);
      /* (6) scope ::= */ yytestcase(yyruleno==6);
      /* (7) scope ::= scope statlist binding semi */ yytestcase(yyruleno==7);
      /* (8) statlist ::= */ yytestcase(yyruleno==8);
      /* (9) statlist ::= statlist stat semi */ yytestcase(yyruleno==9);
      /* (10) stat ::= DO block END */ yytestcase(yyruleno==10);
      /* (11) stat ::= WHILE exp DO block END */ yytestcase(yyruleno==11);
      /* (12) stat ::= repetition DO block END */ yytestcase(yyruleno==12);
      /* (13) stat ::= REPEAT ublock */ yytestcase(yyruleno==13);
      /* (14) stat ::= IF conds END */ yytestcase(yyruleno==14);
      /* (15) stat ::= FUNCTION funcname funcbody */ yytestcase(yyruleno==15);
      /* (17) stat ::= functioncall */ yytestcase(yyruleno==17);
      /* (18) repetition ::= FOR NAME SET explist23 */ yytestcase(yyruleno==18);
      /* (19) repetition ::= FOR namelist IN explist1 */ yytestcase(yyruleno==19);
      /* (20) conds ::= condlist */ yytestcase(yyruleno==20);
      /* (21) conds ::= condlist ELSE block */ yytestcase(yyruleno==21);
      /* (22) condlist ::= cond */ yytestcase(yyruleno==22);
      /* (23) condlist ::= condlist ELSEIF cond */ yytestcase(yyruleno==23);
      /* (24) cond ::= exp THEN block */ yytestcase(yyruleno==24);
      /* (25) laststat ::= BREAK */ yytestcase(yyruleno==25);
      /* (26) laststat ::= RETURN */ yytestcase(yyruleno==26);
      /* (27) laststat ::= RETURN explist1 */ yytestcase(yyruleno==27);
      /* (28) binding ::= LOCAL namelist */ yytestcase(yyruleno==28);
      /* (29) binding ::= LOCAL namelist SET explist1 */ yytestcase(yyruleno==29);
      /* (30) binding ::= LOCAL FUNCTION NAME funcbody */ yytestcase(yyruleno==30);
      /* (31) funcname ::= dottedname */ yytestcase(yyruleno==31);
      /* (32) funcname ::= dottedname COLON NAME */ yytestcase(yyruleno==32);
      /* (33) dottedname ::= NAME */ yytestcase(yyruleno==33);
      /* (34) dottedname ::= dottedname DOT NAME */ yytestcase(yyruleno==34);
      /* (35) namelist ::= NAME */ yytestcase(yyruleno==35);
      /* (36) namelist ::= namelist COMMA NAME */ yytestcase(yyruleno==36);
      /* (38) explist1 ::= explist1 COMMA exp */ yytestcase(yyruleno==38);
      /* (39) explist23 ::= exp COMMA exp */ yytestcase(yyruleno==39);
      /* (40) explist23 ::= exp COMMA exp COMMA exp */ yytestcase(yyruleno==40);
      /* (41) exp ::= NIL|TRUE|FALSE|DOTS */ yytestcase(yyruleno==41);
      /* (44) exp ::= function */ yytestcase(yyruleno==44);
      /* (45) exp ::= prefixexp */ yytestcase(yyruleno==45);
      /* (46) exp ::= tableconstructor */ yytestcase(yyruleno==46);
      /* (47) exp ::= NOT|HASH|MINUS exp */ yytestcase(yyruleno==47);
      /* (48) exp ::= exp OR exp */ yytestcase(yyruleno==48);
      /* (49) exp ::= exp AND exp */ yytestcase(yyruleno==49);
      /* (50) exp ::= exp L|LE|G|GE|EQ|NE exp */ yytestcase(yyruleno==50);
      /* (51) exp ::= exp CONCAT exp */ yytestcase(yyruleno==51);
      /* (56) exp ::= exp MOD exp */ yytestcase(yyruleno==56);
      /* (57) exp ::= exp POW exp */ yytestcase(yyruleno==57);
      /* (59) setlist ::= setlist COMMA var */ yytestcase(yyruleno==59);
      /* (61) var ::= prefixexp SLPAREN exp SRPAREN */ yytestcase(yyruleno==61);
      /* (62) var ::= prefixexp DOT NAME */ yytestcase(yyruleno==62);
      /* (63) prefixexp ::= var */ yytestcase(yyruleno==63);
      /* (64) prefixexp ::= functioncall */ yytestcase(yyruleno==64);
      /* (65) prefixexp ::= LPAREN exp RPAREN */ yytestcase(yyruleno==65);
      /* (66) functioncall ::= prefixexp args */ yytestcase(yyruleno==66);
      /* (67) functioncall ::= prefixexp COLON NAME args */ yytestcase(yyruleno==67);
      /* (68) args ::= LPAREN RPAREN */ yytestcase(yyruleno==68);
      /* (69) args ::= LPAREN explist1 RPAREN */ yytestcase(yyruleno==69);
      /* (70) args ::= tableconstructor */ yytestcase(yyruleno==70);
      /* (71) args ::= STRING */ yytestcase(yyruleno==71);
      /* (72) function ::= FUNCTION funcbody */ yytestcase(yyruleno==72);
      /* (73) funcbody ::= params block END */ yytestcase(yyruleno==73);
      /* (74) params ::= LPAREN parlist LPAREN */ yytestcase(yyruleno==74);
      /* (75) parlist ::= */ yytestcase(yyruleno==75);
      /* (76) parlist ::= namelist */ yytestcase(yyruleno==76);
      /* (77) parlist ::= DOTS */ yytestcase(yyruleno==77);
      /* (78) parlist ::= namelist COMMA DOTS */ yytestcase(yyruleno==78);
      /* (79) tableconstructor ::= LBRACE RBRACE */ yytestcase(yyruleno==79);
      /* (80) tableconstructor ::= LBRACE fieldlist RBRACE */ yytestcase(yyruleno==80);
      /* (81) tableconstructor ::= LBRACE fieldlist COMMA|SEMICOL RBRACE */ yytestcase(yyruleno==81);
      /* (82) fieldlist ::= field */ yytestcase(yyruleno==82);
      /* (83) fieldlist ::= fieldlist COMMA|SEMICOL field */ yytestcase(yyruleno==83);
      /* (84) field ::= exp */ yytestcase(yyruleno==84);
      /* (85) field ::= NAME SET exp */ yytestcase(yyruleno==85);
      /* (86) field ::= SLPAREN exp SRPAREN SET exp */ yytestcase(yyruleno==86);
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
#line 17 "uparser.y"

  printf ("Syntax error!\n");
#line 1199 "uparser.c"
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
