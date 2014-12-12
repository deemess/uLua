/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>
#line 3 "uparser.y"

#include <stdio.h>
#include "basetypes.h"
#include "ucodegen.h"
#include "uparser.h"
#include "ulexer.h"
#include "llerror.h"
#include "vmconfig.h"
#line 17 "uparser.c"
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
#define YYNSTATE 156
#define YYNRULE 82
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
#define YY_ACTTAB_COUNT (578)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    40,   13,   25,   25,   22,   22,   22,   22,   22,   22,
 /*    10 */    21,   20,   20,   20,   20,   20,   37,   11,   20,   25,
 /*    20 */    25,   22,   22,   22,   22,   22,   22,   21,   20,   20,
 /*    30 */    20,   20,   20,   92,  156,   20,  141,   64,   30,   89,
 /*    40 */    88,   50,  118,  140,   25,   25,   22,   22,   22,   22,
 /*    50 */    22,   22,   21,   20,   20,   20,   20,   20,    8,   20,
 /*    60 */    20,  144,  143,   81,   70,  128,  137,   84,    5,  139,
 /*    70 */    25,   25,   22,   22,   22,   22,   22,   22,   21,   20,
 /*    80 */    20,   20,   20,   20,   15,  105,   20,   20,   20,   20,
 /*    90 */   151,  239,   20,   50,   97,   25,   25,   22,   22,   22,
 /*   100 */    22,   22,   22,   21,   20,   20,   20,   20,   20,  150,
 /*   110 */   123,   20,  120,   36,   45,  124,    2,   76,  138,  127,
 /*   120 */    25,   25,   22,   22,   22,   22,   22,   22,   21,   20,
 /*   130 */    20,   20,   20,   20,   79,  121,   20,   12,   25,   25,
 /*   140 */    22,   22,   22,   22,   22,   22,   21,   20,   20,   20,
 /*   150 */    20,   20,    4,  134,   20,   25,   25,   22,   22,   22,
 /*   160 */    22,   22,   22,   21,   20,   20,   20,   20,   20,   19,
 /*   170 */   136,   20,    4,  141,   53,   30,   10,  125,   46,   19,
 /*   180 */   140,   98,  100,   19,  113,   91,   77,   74,   27,   29,
 /*   190 */    18,  142,   46,  126,  140,   98,  211,  121,  144,  143,
 /*   200 */   131,  135,   23,   16,   99,   38,   23,   23,  211,  149,
 /*   210 */   148,  148,  147,  146,  145,   17,   23,   31,    2,  130,
 /*   220 */    23,   23,   19,  149,  148,  148,  147,  146,  145,   17,
 /*   230 */   119,   46,    2,  132,  142,   25,   22,   22,   22,   22,
 /*   240 */    22,   22,   21,   20,   20,   20,   20,   20,  137,   19,
 /*   250 */    20,   14,  111,   15,  110,   23,    6,  210,   46,   23,
 /*   260 */    23,  142,  149,  148,  148,  147,  146,  145,   84,  210,
 /*   270 */   122,    2,   21,   20,   20,   20,   20,   20,    9,  133,
 /*   280 */    20,    3,   23,  117,  112,  106,   23,   23,  104,  149,
 /*   290 */   148,  148,  147,  146,  145,   19,   50,  103,    2,   41,
 /*   300 */   101,   26,   28,    1,   33,   50,   44,  142,   50,  136,
 /*   310 */    43,  155,  107,    7,   42,   71,  240,   29,   75,  154,
 /*   320 */    93,  116,   68,   47,   48,  141,   53,   30,   50,   50,
 /*   330 */    49,  114,  140,  153,  152,   94,  115,   90,   80,   50,
 /*   340 */   141,   61,   30,   73,   87,   50,   50,  140,   39,   32,
 /*   350 */   144,  143,  141,   52,   30,   35,  141,   61,   30,  140,
 /*   360 */    85,   86,   72,  140,   34,  144,  143,  141,   61,   30,
 /*   370 */   216,   83,  109,  240,  140,  108,  240,  144,  143,  240,
 /*   380 */     5,  144,  143,  240,  141,   61,   30,  240,   82,  216,
 /*   390 */   216,  140,  144,  143,  141,   61,   30,  240,   78,   95,
 /*   400 */    96,  140,  240,  240,  141,   64,   30,  240,  240,  144,
 /*   410 */   143,  140,  240,  240,  216,  216,  240,  240,  216,  144,
 /*   420 */   143,  240,  240,  240,  120,   24,  240,  240,    2,  144,
 /*   430 */   143,  240,  240,  129,  141,   57,   30,  240,  141,   65,
 /*   440 */    30,  140,  141,   54,   30,  140,  141,  102,   30,  140,
 /*   450 */   141,   67,   30,  140,  141,   66,   30,  140,  240,  144,
 /*   460 */   143,  140,  240,  144,  143,  240,  240,  144,  143,  240,
 /*   470 */   240,  144,  143,  240,  240,  144,  143,  240,  240,  144,
 /*   480 */   143,  240,  141,   69,   30,  240,  240,  240,  240,  140,
 /*   490 */   141,   56,   30,  141,   63,   30,  240,  140,  240,  240,
 /*   500 */   140,  240,  141,   55,   30,  240,  240,  144,  143,  140,
 /*   510 */   141,   62,   30,  240,  240,  144,  143,  140,  144,  143,
 /*   520 */   240,  240,  141,   60,   30,  240,  240,  144,  143,  140,
 /*   530 */   141,   59,   30,  240,  240,  144,  143,  140,  141,   51,
 /*   540 */    30,  240,  240,  240,  240,  140,  240,  144,  143,  240,
 /*   550 */   240,  141,   58,   30,  240,  144,  143,  240,  140,  240,
 /*   560 */   240,  240,  240,  144,  143,  240,  240,  240,  240,  240,
 /*   570 */   240,  240,  240,  240,  240,  240,  144,  143,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     5,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*    10 */    32,   33,   34,   35,   36,   37,   15,   16,   40,   24,
 /*    20 */    25,   26,   27,   28,   29,   30,   31,   32,   33,   34,
 /*    30 */    35,   36,   37,   63,    0,   40,   53,   54,   55,   21,
 /*    40 */    22,   71,   72,   60,   24,   25,   26,   27,   28,   29,
 /*    50 */    30,   31,   32,   33,   34,   35,   36,   37,   14,   40,
 /*    60 */    40,   78,   79,   10,   81,   82,   13,   23,    2,   49,
 /*    70 */    24,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*    80 */    34,   35,   36,   37,   23,   63,   40,   35,   36,   37,
 /*    90 */     6,   69,   40,   71,   48,   24,   25,   26,   27,   28,
 /*   100 */    29,   30,   31,   32,   33,   34,   35,   36,   37,    6,
 /*   110 */    49,   40,   46,   17,   74,   59,   50,   77,    6,   48,
 /*   120 */    24,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*   130 */    34,   35,   36,   37,   23,   79,   40,   23,   24,   25,
 /*   140 */    26,   27,   28,   29,   30,   31,   32,   33,   34,   35,
 /*   150 */    36,   37,    3,    2,   40,   24,   25,   26,   27,   28,
 /*   160 */    29,   30,   31,   32,   33,   34,   35,   36,   37,    1,
 /*   170 */    13,   40,   23,   53,   54,   55,   11,   59,   10,    1,
 /*   180 */    60,   13,   58,    1,   64,   65,   66,   53,   23,   55,
 /*   190 */    11,   13,   10,   13,   60,   13,   11,   79,   78,   79,
 /*   200 */    51,   44,   34,   11,   80,    5,   38,   39,   23,   41,
 /*   210 */    42,   43,   44,   45,   46,   47,   34,   13,   50,   51,
 /*   220 */    38,   39,    1,   41,   42,   43,   44,   45,   46,   47,
 /*   230 */     6,   10,   50,   51,   13,   25,   26,   27,   28,   29,
 /*   240 */    30,   31,   32,   33,   34,   35,   36,   37,   13,    1,
 /*   250 */    40,    4,   13,   23,   13,   34,   11,   11,   10,   38,
 /*   260 */    39,   13,   41,   42,   43,   44,   45,   46,   23,   23,
 /*   270 */    49,   50,   32,   33,   34,   35,   36,   37,   11,   44,
 /*   280 */    40,   67,   34,    6,    6,    6,   38,   39,   63,   41,
 /*   290 */    42,   43,   44,   45,   46,    1,   71,   63,   50,    5,
 /*   300 */    63,    7,    8,    9,   10,   71,   12,   13,   71,   13,
 /*   310 */    13,    3,   18,   19,   20,   53,   83,   55,   56,   70,
 /*   320 */    63,   63,   60,   61,   62,   53,   54,   55,   71,   71,
 /*   330 */    68,   63,   60,   70,   70,   73,   64,   63,   63,   71,
 /*   340 */    53,   54,   55,   58,   57,   71,   71,   60,   75,    2,
 /*   350 */    78,   79,   53,   54,   55,   75,   53,   54,   55,   60,
 /*   360 */    57,   13,   58,   60,   75,   78,   79,   53,   54,   55,
 /*   370 */     2,   57,   13,   83,   60,   76,   83,   78,   79,   83,
 /*   380 */     2,   78,   79,   83,   53,   54,   55,   83,   57,   21,
 /*   390 */    22,   60,   78,   79,   53,   54,   55,   83,   57,   21,
 /*   400 */    22,   60,   83,   83,   53,   54,   55,   83,   83,   78,
 /*   410 */    79,   60,   83,   83,   46,   47,   83,   83,   50,   78,
 /*   420 */    79,   83,   83,   83,   46,   47,   83,   83,   50,   78,
 /*   430 */    79,   83,   83,   82,   53,   54,   55,   83,   53,   54,
 /*   440 */    55,   60,   53,   54,   55,   60,   53,   54,   55,   60,
 /*   450 */    53,   54,   55,   60,   53,   54,   55,   60,   83,   78,
 /*   460 */    79,   60,   83,   78,   79,   83,   83,   78,   79,   83,
 /*   470 */    83,   78,   79,   83,   83,   78,   79,   83,   83,   78,
 /*   480 */    79,   83,   53,   54,   55,   83,   83,   83,   83,   60,
 /*   490 */    53,   54,   55,   53,   54,   55,   83,   60,   83,   83,
 /*   500 */    60,   83,   53,   54,   55,   83,   83,   78,   79,   60,
 /*   510 */    53,   54,   55,   83,   83,   78,   79,   60,   78,   79,
 /*   520 */    83,   83,   53,   54,   55,   83,   83,   78,   79,   60,
 /*   530 */    53,   54,   55,   83,   83,   78,   79,   60,   53,   54,
 /*   540 */    55,   83,   83,   83,   83,   60,   83,   78,   79,   83,
 /*   550 */    83,   53,   54,   55,   83,   78,   79,   83,   60,   83,
 /*   560 */    83,   83,   83,   78,   79,   83,   83,   83,   83,   83,
 /*   570 */    83,   83,   83,   83,   83,   83,   78,   79,
};
#define YY_SHIFT_USE_DFLT (-23)
#define YY_SHIFT_COUNT (105)
#define YY_SHIFT_MIN   (-22)
#define YY_SHIFT_MAX   (378)
static const short yy_shift_ofst[] = {
 /*     0 */   -23,  248,  182,  294,  168,  221,  248,  248,  248,  248,
 /*    10 */   248,  248,  248,  248,  248,  248,  248,  248,  248,  248,
 /*    20 */   248,  248,  248,  248,  248,  248,  248,  178,  -23,  378,
 /*    30 */   378,   66,  235,  359,  -23,  -23,  -23,  -23,  -23,  -23,
 /*    40 */   -23,  -23,   53,  347,  348,  347,  347,  308,  308,  308,
 /*    50 */   -23,  114,  -22,   96,   71,   46,   20,   -5,  131,  131,
 /*    60 */   131,  131,  131,  131,  131,  210,  240,  240,  368,   52,
 /*    70 */   149,  246,  245,   44,  185,  165,   18,    1,   61,  157,
 /*    80 */   279,  297,  230,  230,  296,  230,  267,  230,  241,  239,
 /*    90 */   278,  277,  247,  224,  200,  204,  180,  192,  179,  151,
 /*   100 */   111,  112,   19,  103,   84,   34,
};
#define YY_REDUCE_USE_DFLT (-31)
#define YY_REDUCE_COUNT (50)
#define YY_REDUCE_MIN   (-30)
#define YY_REDUCE_MAX   (498)
static const short yy_reduce_ofst[] = {
 /*     0 */    22,  120,  -17,  262,  351,  341,  331,  314,  303,  299,
 /*    10 */   287,  272,  498,  485,  477,  469,  457,  449,  440,  437,
 /*    20 */   429,  401,  397,  393,  389,  385,  381,  134,  -30,  118,
 /*    30 */   118,   56,  124,   40,  275,  274,  268,  258,  257,  237,
 /*    40 */   234,  225,  304,  289,  285,  280,  273,  264,  263,  249,
 /*    50 */   214,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   162,  238,  238,  159,  238,  238,  238,  182,  238,  238,
 /*    10 */   238,  238,  238,  238,  238,  238,  238,  238,  238,  238,
 /*    20 */   238,  238,  238,  238,  238,  238,  238,  238,  162,  238,
 /*    30 */   203,  238,  226,  238,  162,  162,  162,  162,  162,  162,
 /*    40 */   162,  162,  238,  238,  238,  238,  238,  158,  158,  158,
 /*    50 */   164,  195,  238,  238,  238,  238,  238,  238,  196,  161,
 /*    60 */   194,  193,  237,  236,  235,  206,  208,  207,  173,  209,
 /*    70 */   238,  215,  184,  238,  215,  238,  187,  176,  238,  238,
 /*    80 */   238,  238,  185,  183,  238,  175,  191,  172,  238,  238,
 /*    90 */   238,  238,  238,  238,  238,  238,  238,  238,  212,  238,
 /*   100 */   227,  238,  205,  238,  238,  238,  186,  181,  174,  189,
 /*   110 */   190,  188,  171,  178,  180,  179,  177,  170,  169,  168,
 /*   120 */   223,  222,  220,  221,  219,  218,  214,  213,  233,  234,
 /*   130 */   232,  231,  230,  228,  225,  229,  192,  191,  224,  217,
 /*   140 */   216,  215,  212,  204,  202,  201,  200,  199,  198,  197,
 /*   150 */   167,  166,  165,  163,  160,  157,
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
  "functioncall",  "stat",          "binding",       "block",       
  "cond",          "conds",         "condlist",      "statlist",    
  "laststat",      "chunk",         "semi",          "scope",       
  "ublock",        "repetition",    "funcname",      "params",      
  "explist23",     "dottedname",    "function",      "tableconstructor",
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
 /*  49 */ "exp ::= NOT|HASH|MINUS exp",
 /*  50 */ "exp ::= exp OR|AND exp",
 /*  51 */ "exp ::= exp L|LE|G|GE|EQ|NE exp",
 /*  52 */ "exp ::= exp CONCAT exp",
 /*  53 */ "exp ::= exp PLUS|MINUS|TIMES|DIVIDE|MOD|POW exp",
 /*  54 */ "setlist ::= var",
 /*  55 */ "setlist ::= setlist COMMA var",
 /*  56 */ "var ::= NAME",
 /*  57 */ "var ::= prefixexp SLPAREN exp SRPAREN",
 /*  58 */ "var ::= prefixexp DOT NAME",
 /*  59 */ "prefixexp ::= var",
 /*  60 */ "prefixexp ::= functioncall",
 /*  61 */ "prefixexp ::= OPEN exp RPAREN",
 /*  62 */ "functioncall ::= prefixexp args",
 /*  63 */ "functioncall ::= prefixexp COLON NAME args",
 /*  64 */ "args ::= LPAREN RPAREN",
 /*  65 */ "args ::= LPAREN explist1 RPAREN",
 /*  66 */ "args ::= tableconstructor",
 /*  67 */ "args ::= STRING",
 /*  68 */ "function ::= FUNCTION params block END",
 /*  69 */ "params ::= LPAREN parlist LPAREN",
 /*  70 */ "parlist ::=",
 /*  71 */ "parlist ::= namelist",
 /*  72 */ "parlist ::= DOTS",
 /*  73 */ "parlist ::= namelist COMMA DOTS",
 /*  74 */ "tableconstructor ::= LBRACE RBRACE",
 /*  75 */ "tableconstructor ::= LBRACE fieldlist RBRACE",
 /*  76 */ "tableconstructor ::= LBRACE fieldlist COMMA|SEMICOL RBRACE",
 /*  77 */ "fieldlist ::= field",
 /*  78 */ "fieldlist ::= fieldlist COMMA|SEMICOL field",
 /*  79 */ "field ::= exp",
 /*  80 */ "field ::= NAME SET exp",
 /*  81 */ "field ::= SLPAREN exp SRPAREN SET exp",
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
#line 44 "uparser.y"

    f->error_code = E_STACKOVERFLOW;
#line 787 "uparser.c"
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
  { 69, 1 },
  { 70, 1 },
  { 70, 0 },
  { 63, 2 },
  { 63, 4 },
  { 72, 3 },
  { 71, 0 },
  { 71, 4 },
  { 67, 0 },
  { 67, 3 },
  { 61, 3 },
  { 61, 5 },
  { 61, 4 },
  { 61, 2 },
  { 61, 3 },
  { 61, 5 },
  { 61, 3 },
  { 61, 1 },
  { 73, 4 },
  { 73, 4 },
  { 65, 1 },
  { 65, 3 },
  { 66, 1 },
  { 66, 3 },
  { 64, 3 },
  { 68, 1 },
  { 68, 1 },
  { 68, 2 },
  { 62, 2 },
  { 62, 4 },
  { 62, 6 },
  { 74, 1 },
  { 74, 3 },
  { 77, 1 },
  { 77, 3 },
  { 58, 1 },
  { 58, 3 },
  { 57, 1 },
  { 57, 3 },
  { 76, 3 },
  { 76, 5 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
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
  { 60, 2 },
  { 60, 4 },
  { 59, 2 },
  { 59, 3 },
  { 59, 1 },
  { 59, 1 },
  { 78, 4 },
  { 75, 3 },
  { 80, 0 },
  { 80, 1 },
  { 80, 1 },
  { 80, 3 },
  { 79, 2 },
  { 79, 3 },
  { 79, 4 },
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
#line 50 "uparser.y"
{
	doReturn(f);
	DPRINTF("P_CHUNK\n");
}
#line 986 "uparser.c"
        break;
      case 1: /* semi ::= SEMICOL */
      case 2: /* semi ::= */ yytestcase(yyruleno==2);
#line 55 "uparser.y"
{
	DPRINTF("P_SEMI ------------------\n");
}
#line 994 "uparser.c"
        break;
      case 3: /* block ::= scope statlist */
#line 62 "uparser.y"
{
	yygotominor.yy15 = yymsp[0].minor.yy15;
	DPRINTF("P_BLOCK_STATLIST\n");
}
#line 1002 "uparser.c"
        break;
      case 4: /* block ::= scope statlist laststat semi */
#line 66 "uparser.y"
{
	yygotominor.yy15 = yymsp[-2].minor.yy15;
	DPRINTF("P_BLOCK_STATLIST_LASTSTAT\n");
}
#line 1010 "uparser.c"
        break;
      case 5: /* ublock ::= block UNTIL exp */
#line 70 "uparser.y"
{
	//doReturn(f);
	DPRINTF("P_UBLOCK\n");
}
#line 1018 "uparser.c"
        break;
      case 6: /* scope ::= */
#line 75 "uparser.y"
{
	DPRINTF("P_SCOPE_EMPTY\n");
}
#line 1025 "uparser.c"
        break;
      case 7: /* scope ::= scope statlist binding semi */
#line 78 "uparser.y"
{
	DPRINTF("P_SCOPE_STATLIST\n");
}
#line 1032 "uparser.c"
        break;
      case 8: /* statlist ::= */
#line 82 "uparser.y"
{
	yygotominor.yy15 = NULL;
	f->currentStat = NULL;
	DPRINTF("P_STATLIST_EMPTY\n");
}
#line 1041 "uparser.c"
        break;
      case 9: /* statlist ::= statlist stat semi */
#line 87 "uparser.y"
{
	if(yymsp[-2].minor.yy15 == NULL) { 
		yygotominor.yy15 = f->currentStat; //save only pointer to first statement
	} else {
		yygotominor.yy15 = yymsp[-2].minor.yy15;
	}
	f->currentStat = NULL;
	DPRINTF("P_STATLIST_ADD_STAT\n");
}
#line 1054 "uparser.c"
        break;
      case 10: /* stat ::= DO block END */
#line 97 "uparser.y"
{
	yygotominor.yy15 = yymsp[-1].minor.yy15;
	DPRINTF("P_STAT_BLOCK\n");
}
#line 1062 "uparser.c"
        break;
      case 11: /* stat ::= WHILE exp DO block END */
#line 101 "uparser.y"
{
	yygotominor.yy15 = statWHILE(f, yymsp[-3].minor.yy88, yymsp[-1].minor.yy15);
	DPRINTF("P_STAT_WHILE\n");
}
#line 1070 "uparser.c"
        break;
      case 14: /* stat ::= IF conds END */
#line 107 "uparser.y"
{
	yygotominor.yy15 = yymsp[-1].minor.yy15;
	DPRINTF("P_STAT_IF\n");
}
#line 1078 "uparser.c"
        break;
      case 16: /* stat ::= setlist SET explist1 */
#line 112 "uparser.y"
{
	yygotominor.yy15 = statSET(f, yymsp[-2].minor.yy88, yymsp[0].minor.yy88, FALSE);
	DPRINTF("P_STAT_SET\n");
}
#line 1086 "uparser.c"
        break;
      case 17: /* stat ::= functioncall */
#line 116 "uparser.y"
{
	yygotominor.yy15 = yymsp[0].minor.yy15;
	DPRINTF("P_STAT_FCALL\n");
}
#line 1094 "uparser.c"
        break;
      case 20: /* conds ::= condlist */
#line 124 "uparser.y"
{
	yygotominor.yy15 = yymsp[0].minor.yy15;
	DPRINTF("P_CONDS_CONDLIST\n");
}
#line 1102 "uparser.c"
        break;
      case 21: /* conds ::= condlist ELSE block */
#line 128 "uparser.y"
{
	yygotominor.yy15 = statELSE(f, yymsp[-2].minor.yy15, yymsp[0].minor.yy15);
	DPRINTF("P_CONDS_CONDLIST_ELSE_BLOCK\n");
}
#line 1110 "uparser.c"
        break;
      case 22: /* condlist ::= cond */
#line 132 "uparser.y"
{
	yygotominor.yy15 = yymsp[0].minor.yy15;
	DPRINTF("P_CONDLIST_COND\n");
}
#line 1118 "uparser.c"
        break;
      case 23: /* condlist ::= condlist ELSEIF cond */
#line 136 "uparser.y"
{
	yygotominor.yy15 = statELSEIF(f, yymsp[-2].minor.yy15, yymsp[0].minor.yy15);
	DPRINTF("P_CONDLIST_CONDLIST_ELSEIF_COND\n");
}
#line 1126 "uparser.c"
        break;
      case 24: /* cond ::= exp THEN block */
#line 140 "uparser.y"
{
	yygotominor.yy15 = statTHEN(f, yymsp[-2].minor.yy88, yymsp[0].minor.yy15);
	DPRINTF("P_COND_EXP_THEN_BLOCK\n");
}
#line 1134 "uparser.c"
        break;
      case 28: /* binding ::= LOCAL namelist */
#line 149 "uparser.y"
{
	DPRINTF("P_LOCAL\n");
}
#line 1141 "uparser.c"
        break;
      case 29: /* binding ::= LOCAL namelist SET explist1 */
#line 152 "uparser.y"
{
	yygotominor.yy15 = statSET(f, yymsp[-2].minor.yy88, yymsp[0].minor.yy88, TRUE);
	DPRINTF("P_LOCAL_SET\n");
}
#line 1149 "uparser.c"
        break;
      case 30: /* binding ::= LOCAL FUNCTION NAME params block END */
#line 156 "uparser.y"
{
	DPRINTF("P_LOCAL_FUNCTION\n");
	pushConstString(f, &yymsp[-3].minor.yy0.semInfo);
}
#line 1157 "uparser.c"
        break;
      case 35: /* namelist ::= NAME */
#line 167 "uparser.y"
{
	Constant* c = pushVarName(f, &yymsp[0].minor.yy0.semInfo);
	yygotominor.yy88 = getVarRegister(f,c);
	DPRINTF("P_NAMELIST_NAME\n");
}
#line 1166 "uparser.c"
        break;
      case 36: /* namelist ::= namelist COMMA NAME */
#line 172 "uparser.y"
{
	Constant* c = pushVarName(f, &yymsp[0].minor.yy0.semInfo);
	yygotominor.yy88 = getVarRegister(f,c);
	DPRINTF("P_NAMELIST_COMMA_NAME\n");
}
#line 1175 "uparser.c"
        break;
      case 37: /* explist1 ::= exp */
#line 178 "uparser.y"
{
	yygotominor.yy88 = yymsp[0].minor.yy88;
	DPRINTF("P_EXPLIST_EXP\n");
}
#line 1183 "uparser.c"
        break;
      case 38: /* explist1 ::= explist1 COMMA exp */
#line 182 "uparser.y"
{
	yygotominor.yy88 = yymsp[0].minor.yy88;
	DPRINTF("P_EXPLIST_ADD_EXP\n");
}
#line 1191 "uparser.c"
        break;
      case 41: /* exp ::= NIL */
#line 198 "uparser.y"
{
	yygotominor.yy88 = doNil(f);
	if(yygotominor.yy88->exprStart == NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1199 "uparser.c"
        break;
      case 42: /* exp ::= TRUE|FALSE */
#line 202 "uparser.y"
{
	yygotominor.yy88 = doBoolean(f, &yymsp[0].minor.yy0);
	if(yygotominor.yy88->exprStart == NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1207 "uparser.c"
        break;
      case 44: /* exp ::= NUMBER */
#line 207 "uparser.y"
{
	Constant* c;
	Register* r;

	DPRINTF("P_EXP_NUMBER\n");
	c = pushConstNumber(f, yymsp[0].minor.yy0.number.fvalue);
	r = getFreeRegister(f);
	r->consthold = TRUE;
	r->constpreloaded = FALSE;
	r->constnum = c->num;
	yygotominor.yy88 = r;
	if(yygotominor.yy88->exprStart == NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1224 "uparser.c"
        break;
      case 45: /* exp ::= STRING */
#line 220 "uparser.y"
{
	Constant* c;
	Register* r;

	DPRINTF("P_EXP_STRING\n");
	c = pushConstString(f, &yymsp[0].minor.yy0.semInfo);
	r = getFreeRegister(f);
	r->consthold = TRUE;
	r->constpreloaded = FALSE;
	r->constnum = c->num;
	yygotominor.yy88 = r;
	if(yygotominor.yy88->exprStart == NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1241 "uparser.c"
        break;
      case 46: /* exp ::= function */
#line 233 "uparser.y"
{
	DPRINTF("P_EXP_FUNCTION\n");
}
#line 1248 "uparser.c"
        break;
      case 47: /* exp ::= prefixexp */
#line 236 "uparser.y"
{
	DPRINTF("P_EXP_PREFIXEXP\n");
	yygotominor.yy88 = yymsp[0].minor.yy88;
}
#line 1256 "uparser.c"
        break;
      case 50: /* exp ::= exp OR|AND exp */
#line 242 "uparser.y"
{
	yygotominor.yy88 = doLogic(f,yymsp[-2].minor.yy88,yymsp[0].minor.yy88,&yymsp[-1].minor.yy0);
	if(yygotominor.yy88->exprStart == NULL) yygotominor.yy88->exprStart = f->currentStat;
	DPRINTF("P_EXP_LOGIC\n");
}
#line 1265 "uparser.c"
        break;
      case 51: /* exp ::= exp L|LE|G|GE|EQ|NE exp */
#line 247 "uparser.y"
{
	yygotominor.yy88 = doCompare(f,yymsp[-2].minor.yy88,yymsp[0].minor.yy88,&yymsp[-1].minor.yy0);
	if(yygotominor.yy88->exprStart == NULL) yygotominor.yy88->exprStart = f->currentStat;
	DPRINTF("P_EXP_COMPARE\n");
}
#line 1274 "uparser.c"
        break;
      case 53: /* exp ::= exp PLUS|MINUS|TIMES|DIVIDE|MOD|POW exp */
#line 253 "uparser.y"
{
	yygotominor.yy88 = doMath(f,yymsp[-2].minor.yy88,yymsp[0].minor.yy88,&yymsp[-1].minor.yy0);
	if(yygotominor.yy88->exprStart == NULL) yygotominor.yy88->exprStart = f->currentStat;
	DPRINTF("P_EXP_MATH\n");
}
#line 1283 "uparser.c"
        break;
      case 54: /* setlist ::= var */
#line 259 "uparser.y"
{
	yygotominor.yy88 = yymsp[0].minor.yy88;
	DPRINTF("P_SETLIST_VAR\n");
}
#line 1291 "uparser.c"
        break;
      case 55: /* setlist ::= setlist COMMA var */
#line 263 "uparser.y"
{
	yygotominor.yy88 = yymsp[0].minor.yy88;
	DPRINTF("P_SETLIST_ADD_VAR\n");
}
#line 1299 "uparser.c"
        break;
      case 56: /* var ::= NAME */
#line 268 "uparser.y"
{
	Constant* c;

	DPRINTF("P_VAR_NAME\n");
	c = pushVarName(f, &yymsp[0].minor.yy0.semInfo);
	yygotominor.yy88 = getVarRegister(f,c);
}
#line 1310 "uparser.c"
        break;
      case 57: /* var ::= prefixexp SLPAREN exp SRPAREN */
#line 275 "uparser.y"
{
	DPRINTF("P_PREFEXP_SPAREN_EXP\n");
}
#line 1317 "uparser.c"
        break;
      case 58: /* var ::= prefixexp DOT NAME */
#line 278 "uparser.y"
{
	DPRINTF("P_PREFEXP_DOT_NAME\n");
}
#line 1324 "uparser.c"
        break;
      case 59: /* prefixexp ::= var */
#line 282 "uparser.y"
{
	DPRINTF("P_PREFEXP_VAR\n");
	yygotominor.yy88 = yymsp[0].minor.yy88;
}
#line 1332 "uparser.c"
        break;
      case 60: /* prefixexp ::= functioncall */
#line 286 "uparser.y"
{
	DPRINTF("P_PREFEXP_FCALL\n");
}
#line 1339 "uparser.c"
        break;
      case 61: /* prefixexp ::= OPEN exp RPAREN */
#line 289 "uparser.y"
{
	yygotominor.yy88 = yymsp[-1].minor.yy88;
	DPRINTF("P_PREFEXP_EXP\n");
}
#line 1347 "uparser.c"
        break;
      case 62: /* functioncall ::= prefixexp args */
#line 294 "uparser.y"
{
	yygotominor.yy15 = functionCALL(f, yymsp[-1].minor.yy88, yymsp[0].minor.yy88);
	DPRINTF("P_FCALL_ARGS\n");
}
#line 1355 "uparser.c"
        break;
      case 63: /* functioncall ::= prefixexp COLON NAME args */
#line 298 "uparser.y"
{
	DPRINTF("P_FCALL_NAME_ARGS\n");
}
#line 1362 "uparser.c"
        break;
      case 64: /* args ::= LPAREN RPAREN */
#line 302 "uparser.y"
{
	yygotominor.yy88 = NULL;
	DPRINTF("P_ARGS_EMPTY\n");
}
#line 1370 "uparser.c"
        break;
      case 65: /* args ::= LPAREN explist1 RPAREN */
#line 306 "uparser.y"
{
	yygotominor.yy88 = yymsp[-1].minor.yy88;
	DPRINTF("P_ARGS_EXPLIST\n");
}
#line 1378 "uparser.c"
        break;
      case 67: /* args ::= STRING */
#line 311 "uparser.y"
{
	Constant* c;
	Register* r;

	c = pushConstString(f, &yymsp[0].minor.yy0.semInfo);
	r = getFreeRegister(f);
	r->consthold = TRUE;
	r->constpreloaded = FALSE;
	r->constnum = c->num;
	yygotominor.yy88 = r;
	DPRINTF("P_ARGS_STRING\n");
}
#line 1394 "uparser.c"
        break;
      default:
      /* (12) stat ::= repetition DO block END */ yytestcase(yyruleno==12);
      /* (13) stat ::= REPEAT ublock */ yytestcase(yyruleno==13);
      /* (15) stat ::= FUNCTION funcname params block END */ yytestcase(yyruleno==15);
      /* (18) repetition ::= FOR NAME SET explist23 */ yytestcase(yyruleno==18);
      /* (19) repetition ::= FOR namelist IN explist1 */ yytestcase(yyruleno==19);
      /* (25) laststat ::= BREAK */ yytestcase(yyruleno==25);
      /* (26) laststat ::= RETURN */ yytestcase(yyruleno==26);
      /* (27) laststat ::= RETURN explist1 */ yytestcase(yyruleno==27);
      /* (31) funcname ::= dottedname */ yytestcase(yyruleno==31);
      /* (32) funcname ::= dottedname COLON NAME */ yytestcase(yyruleno==32);
      /* (33) dottedname ::= NAME */ yytestcase(yyruleno==33);
      /* (34) dottedname ::= dottedname DOT NAME */ yytestcase(yyruleno==34);
      /* (39) explist23 ::= exp COMMA exp */ yytestcase(yyruleno==39);
      /* (40) explist23 ::= exp COMMA exp COMMA exp */ yytestcase(yyruleno==40);
      /* (43) exp ::= DOTS */ yytestcase(yyruleno==43);
      /* (48) exp ::= tableconstructor */ yytestcase(yyruleno==48);
      /* (49) exp ::= NOT|HASH|MINUS exp */ yytestcase(yyruleno==49);
      /* (52) exp ::= exp CONCAT exp */ yytestcase(yyruleno==52);
      /* (66) args ::= tableconstructor */ yytestcase(yyruleno==66);
      /* (68) function ::= FUNCTION params block END */ yytestcase(yyruleno==68);
      /* (69) params ::= LPAREN parlist LPAREN */ yytestcase(yyruleno==69);
      /* (70) parlist ::= */ yytestcase(yyruleno==70);
      /* (71) parlist ::= namelist */ yytestcase(yyruleno==71);
      /* (72) parlist ::= DOTS */ yytestcase(yyruleno==72);
      /* (73) parlist ::= namelist COMMA DOTS */ yytestcase(yyruleno==73);
      /* (74) tableconstructor ::= LBRACE RBRACE */ yytestcase(yyruleno==74);
      /* (75) tableconstructor ::= LBRACE fieldlist RBRACE */ yytestcase(yyruleno==75);
      /* (76) tableconstructor ::= LBRACE fieldlist COMMA|SEMICOL RBRACE */ yytestcase(yyruleno==76);
      /* (77) fieldlist ::= field */ yytestcase(yyruleno==77);
      /* (78) fieldlist ::= fieldlist COMMA|SEMICOL field */ yytestcase(yyruleno==78);
      /* (79) field ::= exp */ yytestcase(yyruleno==79);
      /* (80) field ::= NAME SET exp */ yytestcase(yyruleno==80);
      /* (81) field ::= SLPAREN exp SRPAREN SET exp */ yytestcase(yyruleno==81);
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
#line 40 "uparser.y"

  f->error_code = E_SYNTAX_ERROR;
#line 1492 "uparser.c"
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
#line 36 "uparser.y"

	f->parsed = TRUE;
#line 1514 "uparser.c"
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
