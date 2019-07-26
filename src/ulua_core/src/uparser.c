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
#define YY_ACTTAB_COUNT (592)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    41,   13,   26,   26,   23,   23,   23,   23,   23,   23,
 /*    10 */    21,   20,   20,   20,   20,   20,   38,   11,   20,   26,
 /*    20 */    26,   23,   23,   23,   23,   23,   23,   21,   20,   20,
 /*    30 */    20,   20,   20,   93,  158,   20,  143,   65,   31,   90,
 /*    40 */    89,   51,  120,  142,   26,   26,   23,   23,   23,   23,
 /*    50 */    23,   23,   21,   20,   20,   20,   20,   20,    8,   20,
 /*    60 */    20,  146,  145,   82,   71,  130,  139,   85,    5,  141,
 /*    70 */    26,   26,   23,   23,   23,   23,   23,   23,   21,   20,
 /*    80 */    20,   20,   20,   20,   15,  107,   20,   20,   20,   20,
 /*    90 */   153,  242,   20,   51,   98,   26,   26,   23,   23,   23,
 /*   100 */    23,   23,   23,   21,   20,   20,   20,   20,   20,  152,
 /*   110 */   125,   20,  122,   37,   46,  126,    2,   77,  140,  129,
 /*   120 */    26,   26,   23,   23,   23,   23,   23,   23,   21,   20,
 /*   130 */    20,   20,   20,   20,   80,  123,   20,   12,   26,   26,
 /*   140 */    23,   23,   23,   23,   23,   23,   21,   20,   20,   20,
 /*   150 */    20,   20,    4,  136,   20,   26,   26,   23,   23,   23,
 /*   160 */    23,   23,   23,   21,   20,   20,   20,   20,   20,   19,
 /*   170 */   138,   20,    4,  143,   54,   31,   10,  127,   47,   19,
 /*   180 */   142,   99,  101,   19,  115,   92,   78,   75,   28,   30,
 /*   190 */    18,  144,   47,  128,  142,   99,  214,  123,  146,  145,
 /*   200 */   133,  137,   22,   16,  100,   39,   22,   24,  214,  151,
 /*   210 */   150,  150,  149,  148,  147,   17,   22,   32,    2,  132,
 /*   220 */    22,   24,   19,  151,  150,  150,  149,  148,  147,   17,
 /*   230 */   121,   47,    2,  134,  144,   26,   23,   23,   23,   23,
 /*   240 */    23,   23,   21,   20,   20,   20,   20,   20,  139,   19,
 /*   250 */    20,   14,  113,   15,  112,   22,    6,  213,   47,   22,
 /*   260 */    24,  144,  151,  150,  150,  149,  148,  147,   85,  213,
 /*   270 */   124,    2,   21,   20,   20,   20,   20,   20,    9,  135,
 /*   280 */    20,    3,   22,  119,  114,  108,   22,   24,  106,  151,
 /*   290 */   150,  150,  149,  148,  147,   19,   51,  105,    2,   42,
 /*   300 */   102,   27,   29,    1,   34,   51,   45,  144,   51,  138,
 /*   310 */    44,  157,  109,    7,   43,   72,  243,   30,   76,  156,
 /*   320 */    94,  118,   69,   48,   49,  143,   54,   31,   51,   51,
 /*   330 */    50,  116,  142,  155,  154,   95,  117,   91,   81,   51,
 /*   340 */   143,   62,   31,   74,   88,   51,   51,  142,   40,   33,
 /*   350 */   146,  145,  143,   53,   31,   36,  143,   62,   31,  142,
 /*   360 */    86,   87,   73,  142,   35,  146,  145,  143,   62,   31,
 /*   370 */   219,   84,  111,  243,  142,  110,  243,  146,  145,  243,
 /*   380 */     5,  146,  145,  243,  143,   62,   31,  243,   83,  219,
 /*   390 */   219,  142,  146,  145,  143,   62,   31,  243,   79,   96,
 /*   400 */    97,  142,  243,  243,  143,   65,   31,  243,  243,  146,
 /*   410 */   145,  142,  243,  243,  219,  219,  243,  243,  219,  146,
 /*   420 */   145,  243,  243,  243,  122,   25,  243,  243,    2,  146,
 /*   430 */   145,  243,  243,  131,  143,   58,   31,  243,  143,   66,
 /*   440 */    31,  142,  143,   55,   31,  142,  143,  104,   31,  142,
 /*   450 */   143,   67,   31,  142,  143,  103,   31,  142,  243,  146,
 /*   460 */   145,  142,  243,  146,  145,  243,  243,  146,  145,  243,
 /*   470 */   243,  146,  145,  243,  243,  146,  145,  243,  243,  146,
 /*   480 */   145,  243,  143,   68,   31,  243,  243,  243,  243,  142,
 /*   490 */   143,   70,   31,  143,   57,   31,  243,  142,  243,  243,
 /*   500 */   142,  243,  143,   64,   31,  243,  243,  146,  145,  142,
 /*   510 */   143,   56,   31,  243,  243,  146,  145,  142,  146,  145,
 /*   520 */   243,  243,  143,   63,   31,  243,  243,  146,  145,  142,
 /*   530 */   143,   61,   31,  243,  243,  146,  145,  142,  143,   60,
 /*   540 */    31,  243,  243,  243,  243,  142,  243,  146,  145,  243,
 /*   550 */   243,  143,   52,   31,  243,  146,  145,  243,  142,  243,
 /*   560 */   243,  243,  243,  146,  145,  143,   59,   31,  243,  243,
 /*   570 */   243,  243,  142,  243,  243,  243,  146,  145,  243,  243,
 /*   580 */   243,  243,  243,  243,  243,  243,  243,  243,  243,  243,
 /*   590 */   146,  145,
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
 /*   560 */    83,   83,   83,   78,   79,   53,   54,   55,   83,   83,
 /*   570 */    83,   83,   60,   83,   83,   83,   78,   79,   83,   83,
 /*   580 */    83,   83,   83,   83,   83,   83,   83,   83,   83,   83,
 /*   590 */    78,   79,
};
#define YY_SHIFT_USE_DFLT (-23)
#define YY_SHIFT_COUNT (107)
#define YY_SHIFT_MIN   (-22)
#define YY_SHIFT_MAX   (378)
static const short yy_shift_ofst[] = {
 /*     0 */   -23,  248,  182,  294,  168,  221,  248,  248,  248,  248,
 /*    10 */   248,  248,  248,  248,  248,  248,  248,  248,  248,  248,
 /*    20 */   248,  248,  248,  248,  248,  248,  248,  248,  178,  -23,
 /*    30 */   378,  378,   66,  235,  359,  -23,  -23,  -23,  -23,  -23,
 /*    40 */   -23,  -23,  -23,   53,  347,  348,  347,  347,  308,  308,
 /*    50 */   308,  -23,  114,  -22,   96,   71,   46,   20,   -5,  131,
 /*    60 */   131,  131,  131,  131,  131,  131,  210,  240,  240,  368,
 /*    70 */    52,  149,  246,  245,   44,  185,  165,   18,    1,   61,
 /*    80 */   157,  279,  297,  230,  230,  296,  230,  267,  230,  241,
 /*    90 */   239,  278,  277,  247,  224,  200,  204,  180,  192,  179,
 /*   100 */   151,  111,  112,   19,   19,  103,   84,   34,
};
#define YY_REDUCE_USE_DFLT (-31)
#define YY_REDUCE_COUNT (51)
#define YY_REDUCE_MIN   (-30)
#define YY_REDUCE_MAX   (512)
static const short yy_reduce_ofst[] = {
 /*     0 */    22,  120,  -17,  262,  351,  341,  331,  314,  303,  299,
 /*    10 */   287,  272,  512,  498,  485,  477,  469,  457,  449,  440,
 /*    20 */   437,  429,  401,  397,  393,  389,  385,  381,  134,  -30,
 /*    30 */   118,  118,   56,  124,   40,  275,  274,  268,  258,  257,
 /*    40 */   237,  234,  225,  304,  289,  285,  280,  273,  264,  263,
 /*    50 */   249,  214,
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
#line 44 "src/uparser.y"

    f->error_code = E_STACKOVERFLOW;
#line 792 "src/uparser.c"
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
#line 50 "src/uparser.y"
{
    DPRINTF("P_CHUNK\n");
	doReturn(f);
}
#line 992 "src/uparser.c"
        break;
      case 1: /* semi ::= SEMICOL */
      case 2: /* semi ::= */ yytestcase(yyruleno==2);
#line 55 "src/uparser.y"
{
	DPRINTF("P_SEMI ------------------\n");
}
#line 1000 "src/uparser.c"
        break;
      case 3: /* block ::= scope statlist */
#line 62 "src/uparser.y"
{
    DPRINTF("P_BLOCK_STATLIST\n");
	yygotominor.yy15 = yymsp[0].minor.yy15;
    unloadRegisters(f);
}
#line 1009 "src/uparser.c"
        break;
      case 4: /* block ::= scope statlist laststat semi */
#line 67 "src/uparser.y"
{
    DPRINTF("P_BLOCK_STATLIST_LASTSTAT\n");
	yygotominor.yy15 = yymsp[-2].minor.yy15;
    unloadRegisters(f);
}
#line 1018 "src/uparser.c"
        break;
      case 5: /* ublock ::= block UNTIL exp */
#line 72 "src/uparser.y"
{
	DPRINTF("P_UBLOCK\n");
}
#line 1025 "src/uparser.c"
        break;
      case 6: /* scope ::= */
#line 76 "src/uparser.y"
{
	DPRINTF("P_SCOPE_EMPTY\n");
}
#line 1032 "src/uparser.c"
        break;
      case 7: /* scope ::= scope statlist binding semi */
#line 79 "src/uparser.y"
{
	DPRINTF("P_SCOPE_STATLIST\n");
}
#line 1039 "src/uparser.c"
        break;
      case 8: /* statlist ::= */
#line 83 "src/uparser.y"
{
    DPRINTF("P_STATLIST_EMPTY\n");
	yygotominor.yy15 = ULUA_NULL;
	f->currentStat = ULUA_NULL;
}
#line 1048 "src/uparser.c"
        break;
      case 9: /* statlist ::= statlist stat semi */
#line 88 "src/uparser.y"
{
    DPRINTF("P_STATLIST_ADD_STAT\n");
	if(yymsp[-2].minor.yy15 == ULUA_NULL) {
		yygotominor.yy15 = f->currentStat; //save only pointer to first statement
	} else {
		yygotominor.yy15 = yymsp[-2].minor.yy15;
	}
	f->currentStat = ULUA_NULL;
}
#line 1061 "src/uparser.c"
        break;
      case 10: /* stat ::= DO block END */
#line 98 "src/uparser.y"
{
    DPRINTF("P_STAT_BLOCK\n");
	yygotominor.yy15 = yymsp[-1].minor.yy15;
}
#line 1069 "src/uparser.c"
        break;
      case 11: /* stat ::= WHILE exp DO block END */
#line 102 "src/uparser.y"
{
    DPRINTF("P_STAT_WHILE\n");
	yygotominor.yy15 = statWHILE(f, yymsp[-3].minor.yy88, yymsp[-1].minor.yy15);
}
#line 1077 "src/uparser.c"
        break;
      case 14: /* stat ::= IF conds END */
#line 108 "src/uparser.y"
{
    DPRINTF("P_STAT_IF\n");
	yygotominor.yy15 = yymsp[-1].minor.yy15;
}
#line 1085 "src/uparser.c"
        break;
      case 16: /* stat ::= setlist SET explist1 */
#line 113 "src/uparser.y"
{
    DPRINTF("P_STAT_SET\n");
	yygotominor.yy15 = statSET(f, yymsp[-2].minor.yy88, yymsp[0].minor.yy88, ULUA_FALSE);
}
#line 1093 "src/uparser.c"
        break;
      case 17: /* stat ::= functioncall */
#line 117 "src/uparser.y"
{
    DPRINTF("P_STAT_FCALL\n");
	yygotominor.yy15 = yymsp[0].minor.yy15;
}
#line 1101 "src/uparser.c"
        break;
      case 20: /* conds ::= condlist */
#line 125 "src/uparser.y"
{
    DPRINTF("P_CONDS_CONDLIST\n");
	yygotominor.yy15 = yymsp[0].minor.yy15;
}
#line 1109 "src/uparser.c"
        break;
      case 21: /* conds ::= condlist ELSE block */
#line 129 "src/uparser.y"
{
    DPRINTF("P_CONDS_CONDLIST_ELSE_BLOCK\n");
	yygotominor.yy15 = statELSE(f, yymsp[-2].minor.yy15, yymsp[0].minor.yy15);
}
#line 1117 "src/uparser.c"
        break;
      case 22: /* condlist ::= cond */
#line 133 "src/uparser.y"
{
    DPRINTF("P_CONDLIST_COND\n");
	yygotominor.yy15 = yymsp[0].minor.yy15;
}
#line 1125 "src/uparser.c"
        break;
      case 23: /* condlist ::= condlist ELSEIF cond */
#line 137 "src/uparser.y"
{
    DPRINTF("P_CONDLIST_CONDLIST_ELSEIF_COND\n");
	yygotominor.yy15 = statELSEIF(f, yymsp[-2].minor.yy15, yymsp[0].minor.yy15);
}
#line 1133 "src/uparser.c"
        break;
      case 24: /* cond ::= exp THEN block */
#line 141 "src/uparser.y"
{
    DPRINTF("P_COND_EXP_THEN_BLOCK\n");
	yygotominor.yy15 = statTHEN(f, yymsp[-2].minor.yy88, yymsp[0].minor.yy15);
}
#line 1141 "src/uparser.c"
        break;
      case 28: /* binding ::= LOCAL namelist */
#line 150 "src/uparser.y"
{
	DPRINTF("P_LOCAL\n");
}
#line 1148 "src/uparser.c"
        break;
      case 29: /* binding ::= LOCAL namelist SET explist1 */
#line 153 "src/uparser.y"
{
    DPRINTF("P_LOCAL_SET\n");
	yygotominor.yy15 = statSET(f, yymsp[-2].minor.yy88, yymsp[0].minor.yy88, ULUA_TRUE);
}
#line 1156 "src/uparser.c"
        break;
      case 30: /* binding ::= LOCAL FUNCTION NAME params block END */
#line 157 "src/uparser.y"
{
	DPRINTF("P_LOCAL_FUNCTION\n");
	pushConstString(f, &yymsp[-3].minor.yy0.semInfo);
}
#line 1164 "src/uparser.c"
        break;
      case 35: /* namelist ::= NAME */
#line 168 "src/uparser.y"
{
	Constant* c;
    DPRINTF("P_NAMELIST_NAME\n");
	c = pushVarName(f, &yymsp[0].minor.yy0.semInfo);
	yygotominor.yy88 = getVarRegister(f,c);
}
#line 1174 "src/uparser.c"
        break;
      case 36: /* namelist ::= namelist COMMA NAME */
#line 174 "src/uparser.y"
{
	Constant* c;
    DPRINTF("P_NAMELIST_COMMA_NAME\n");
	c = pushVarName(f, &yymsp[0].minor.yy0.semInfo);
	yygotominor.yy88 = getVarRegister(f,c);
}
#line 1184 "src/uparser.c"
        break;
      case 37: /* explist1 ::= exp */
#line 181 "src/uparser.y"
{
    DPRINTF("P_EXPLIST_EXP\n");
	yygotominor.yy88 = yymsp[0].minor.yy88;
}
#line 1192 "src/uparser.c"
        break;
      case 38: /* explist1 ::= explist1 COMMA exp */
#line 185 "src/uparser.y"
{
    DPRINTF("P_EXPLIST_ADD_EXP\n");
	yygotominor.yy88 = yymsp[0].minor.yy88;
}
#line 1200 "src/uparser.c"
        break;
      case 41: /* exp ::= NIL */
#line 201 "src/uparser.y"
{
    DPRINTF("P_EXP_NIL\n");
	yygotominor.yy88 = doNil(f);
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1209 "src/uparser.c"
        break;
      case 42: /* exp ::= TRUE|FALSE */
#line 206 "src/uparser.y"
{
    DPRINTF("P_EXP_BOOLEAN\n");
	yygotominor.yy88 = doBoolean(f, &yymsp[0].minor.yy0);
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1218 "src/uparser.c"
        break;
      case 44: /* exp ::= NUMBER */
#line 212 "src/uparser.y"
{
	Constant* c;
	Register* r;
    DPRINTF("P_EXP_NUMBER\n");

	c = pushConstNumber(f, yymsp[0].minor.yy0.number.fvalue);
	r = getFreeRegister(f);
	r->consthold = ULUA_TRUE;
	r->constnum = c->num;
	yygotominor.yy88 = r;
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1234 "src/uparser.c"
        break;
      case 45: /* exp ::= STRING */
#line 224 "src/uparser.y"
{
	Constant* c;
	Register* r;
    DPRINTF("P_EXP_STRING\n");

	c = pushConstString(f, &yymsp[0].minor.yy0.semInfo);
	r = getFreeRegister(f);
	r->consthold = ULUA_TRUE;
	r->constnum = c->num;
	yygotominor.yy88 = r;
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1250 "src/uparser.c"
        break;
      case 46: /* exp ::= function */
#line 236 "src/uparser.y"
{
	DPRINTF("P_EXP_FUNCTION\n");
}
#line 1257 "src/uparser.c"
        break;
      case 47: /* exp ::= prefixexp */
#line 239 "src/uparser.y"
{
	DPRINTF("P_EXP_PREFIXEXP\n");
	yygotominor.yy88 = yymsp[0].minor.yy88;
}
#line 1265 "src/uparser.c"
        break;
      case 50: /* exp ::= NOT|MINUS exp */
#line 245 "src/uparser.y"
{
    DPRINTF("P_EXP_NOT_MINUS\n");
	yygotominor.yy88 = doNot(f, yymsp[0].minor.yy88, &yymsp[-1].minor.yy0);
}
#line 1273 "src/uparser.c"
        break;
      case 51: /* exp ::= exp OR|AND exp */
#line 249 "src/uparser.y"
{
    DPRINTF("P_EXP_LOGIC\n");
	yygotominor.yy88 = doLogic(f,yymsp[-2].minor.yy88,yymsp[0].minor.yy88,&yymsp[-1].minor.yy0);
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1282 "src/uparser.c"
        break;
      case 52: /* exp ::= exp L|LE|G|GE|EQ|NE exp */
#line 254 "src/uparser.y"
{
    DPRINTF("P_EXP_COMPARE\n");
	yygotominor.yy88 = doCompare(f,yymsp[-2].minor.yy88,yymsp[0].minor.yy88,&yymsp[-1].minor.yy0);
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1291 "src/uparser.c"
        break;
      case 54: /* exp ::= exp PLUS|MINUS|TIMES|DIVIDE|MOD|POW exp */
#line 260 "src/uparser.y"
{
    DPRINTF("P_EXP_MATH\n");
	yygotominor.yy88 = doMath(f,yymsp[-2].minor.yy88,yymsp[0].minor.yy88,&yymsp[-1].minor.yy0);
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1300 "src/uparser.c"
        break;
      case 55: /* setlist ::= var */
#line 266 "src/uparser.y"
{
    DPRINTF("P_SETLIST_VAR\n");
	yygotominor.yy88 = yymsp[0].minor.yy88;
}
#line 1308 "src/uparser.c"
        break;
      case 56: /* setlist ::= setlist COMMA var */
#line 270 "src/uparser.y"
{
    DPRINTF("P_SETLIST_ADD_VAR\n");
	yygotominor.yy88 = yymsp[0].minor.yy88;
}
#line 1316 "src/uparser.c"
        break;
      case 57: /* var ::= NAME */
#line 275 "src/uparser.y"
{
	Constant* c;
    DPRINTF("P_VAR_NAME\n");

	c = pushVarName(f, &yymsp[0].minor.yy0.semInfo);
	yygotominor.yy88 = getVarRegister(f,c);
	if(yygotominor.yy88->exprStart == ULUA_NULL) yygotominor.yy88->exprStart = f->currentStat;
}
#line 1328 "src/uparser.c"
        break;
      case 58: /* var ::= prefixexp SLPAREN exp SRPAREN */
#line 283 "src/uparser.y"
{
	DPRINTF("P_PREFEXP_SPAREN_EXP\n");
}
#line 1335 "src/uparser.c"
        break;
      case 59: /* var ::= prefixexp DOT NAME */
#line 286 "src/uparser.y"
{
	DPRINTF("P_PREFEXP_DOT_NAME\n");
}
#line 1342 "src/uparser.c"
        break;
      case 60: /* prefixexp ::= var */
#line 290 "src/uparser.y"
{
	DPRINTF("P_PREFEXP_VAR\n");
	yygotominor.yy88 = yymsp[0].minor.yy88;
}
#line 1350 "src/uparser.c"
        break;
      case 61: /* prefixexp ::= functioncall */
#line 294 "src/uparser.y"
{
	DPRINTF("P_PREFEXP_FCALL\n");
}
#line 1357 "src/uparser.c"
        break;
      case 62: /* prefixexp ::= OPEN exp RPAREN */
#line 297 "src/uparser.y"
{
    DPRINTF("P_PREFEXP_EXP\n");
	yygotominor.yy88 = yymsp[-1].minor.yy88;
}
#line 1365 "src/uparser.c"
        break;
      case 63: /* functioncall ::= prefixexp args */
#line 302 "src/uparser.y"
{
    DPRINTF("P_FCALL_ARGS\n");
	yygotominor.yy15 = functionCALL(f, yymsp[-1].minor.yy88, yymsp[0].minor.yy88);
}
#line 1373 "src/uparser.c"
        break;
      case 64: /* functioncall ::= prefixexp COLON NAME args */
#line 306 "src/uparser.y"
{
	DPRINTF("P_FCALL_NAME_ARGS\n");
}
#line 1380 "src/uparser.c"
        break;
      case 65: /* args ::= LPAREN RPAREN */
#line 310 "src/uparser.y"
{
    DPRINTF("P_ARGS_EMPTY\n");
	yygotominor.yy88 = ULUA_NULL;
}
#line 1388 "src/uparser.c"
        break;
      case 66: /* args ::= LPAREN explist1 RPAREN */
#line 314 "src/uparser.y"
{
    DPRINTF("P_ARGS_EXPLIST\n");
	yygotominor.yy88 = yymsp[-1].minor.yy88;
}
#line 1396 "src/uparser.c"
        break;
      case 68: /* args ::= STRING */
#line 319 "src/uparser.y"
{
	Constant* c;
	Register* r;
    DPRINTF("P_ARGS_STRING\n");

	c = pushConstString(f, &yymsp[0].minor.yy0.semInfo);
	r = getFreeRegister(f);
	r->consthold = ULUA_TRUE;
	r->constnum = c->num;
	yygotominor.yy88 = r;
}
#line 1411 "src/uparser.c"
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
      /* (49) exp ::= HASH exp */ yytestcase(yyruleno==49);
      /* (53) exp ::= exp CONCAT exp */ yytestcase(yyruleno==53);
      /* (67) args ::= tableconstructor */ yytestcase(yyruleno==67);
      /* (69) function ::= FUNCTION params block END */ yytestcase(yyruleno==69);
      /* (70) params ::= LPAREN parlist LPAREN */ yytestcase(yyruleno==70);
      /* (71) parlist ::= */ yytestcase(yyruleno==71);
      /* (72) parlist ::= namelist */ yytestcase(yyruleno==72);
      /* (73) parlist ::= DOTS */ yytestcase(yyruleno==73);
      /* (74) parlist ::= namelist COMMA DOTS */ yytestcase(yyruleno==74);
      /* (75) tableconstructor ::= LBRACE RBRACE */ yytestcase(yyruleno==75);
      /* (76) tableconstructor ::= LBRACE fieldlist RBRACE */ yytestcase(yyruleno==76);
      /* (77) tableconstructor ::= LBRACE fieldlist COMMA|SEMICOL RBRACE */ yytestcase(yyruleno==77);
      /* (78) fieldlist ::= field */ yytestcase(yyruleno==78);
      /* (79) fieldlist ::= fieldlist COMMA|SEMICOL field */ yytestcase(yyruleno==79);
      /* (80) field ::= exp */ yytestcase(yyruleno==80);
      /* (81) field ::= NAME SET exp */ yytestcase(yyruleno==81);
      /* (82) field ::= SLPAREN exp SRPAREN SET exp */ yytestcase(yyruleno==82);
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
#line 40 "src/uparser.y"

  f->error_code = E_SYNTAX_ERROR;
#line 1509 "src/uparser.c"
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
#line 36 "src/uparser.y"

	f->parsed = ULUA_TRUE;
#line 1531 "src/uparser.c"
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
