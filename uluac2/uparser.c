/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>
#line 1 "uparser.y"

#include <stdio.h>
#include "basetypes.h"
#include "uparser.h"
#line 13 "uparser.c"
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
#define ParseTOKENTYPE u16
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL
#define ParseARG_PDECL
#define ParseARG_FETCH
#define ParseARG_STORE
#define YYNSTATE 155
#define YYNRULE 82
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
#define YY_ACTTAB_COUNT (617)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    44,   13,   28,   25,   24,   24,   24,   24,   24,   24,
 /*    10 */    23,   22,   22,   21,   21,   21,  155,  150,   20,   28,
 /*    20 */    25,   24,   24,   24,   24,   24,   24,   23,   22,   22,
 /*    30 */    21,   21,   21,  138,  149,   20,   21,   21,   21,   41,
 /*    40 */    11,   20,   28,   25,   24,   24,   24,   24,   24,   24,
 /*    50 */    23,   22,   22,   21,   21,   21,    8,   15,   20,   23,
 /*    60 */    22,   22,   21,   21,   21,   85,  137,   20,  142,   28,
 /*    70 */    25,   24,   24,   24,   24,   24,   24,   23,   22,   22,
 /*    80 */    21,   21,   21,   20,  125,   20,  122,   95,   96,    4,
 /*    90 */     5,  140,    2,   97,   28,   25,   24,   24,   24,   24,
 /*   100 */    24,   24,   23,   22,   22,   21,   21,   21,   18,    4,
 /*   110 */    20,  122,   40,   27,  106,    5,   51,    2,  129,   28,
 /*   120 */    25,   24,   24,   24,   24,   24,   24,   23,   22,   22,
 /*   130 */    21,   21,   21,   81,   92,   20,   51,  145,  133,  120,
 /*   140 */    12,   28,   25,   24,   24,   24,   24,   24,   24,   23,
 /*   150 */    22,   22,   21,   21,   21,  136,  139,   20,   28,   25,
 /*   160 */    24,   24,   24,   24,   24,   24,   23,   22,   22,   21,
 /*   170 */    21,   21,   39,   19,   20,   98,   25,   24,   24,   24,
 /*   180 */    24,   24,   24,   23,   22,   22,   21,   21,   21,  135,
 /*   190 */   143,   20,   39,   37,   16,  145,   26,   32,   10,   76,
 /*   200 */    26,   26,   78,  148,  148,  148,  148,  148,  148,   17,
 /*   210 */    30,   19,   39,    2,  132,   98,   26,   90,   89,   82,
 /*   220 */    26,   26,  139,  148,  148,  148,  148,  148,  148,  100,
 /*   230 */   209,   19,  124,    2,  238,  107,   26,   51,    6,   99,
 /*   240 */    26,   26,  209,  148,  148,  148,  148,  148,  148,   17,
 /*   250 */    85,   19,   39,    2,  134,  145,   24,   24,   24,   24,
 /*   260 */    24,   24,   23,   22,   22,   21,   21,   21,  114,  105,
 /*   270 */    20,   51,  101,   93,   51,   51,   26,  208,  128,   42,
 /*   280 */    26,   26,   43,  148,  148,  148,  148,  148,  148,  208,
 /*   290 */    34,   19,   45,    2,   29,   31,    1,   38,  141,   47,
 /*   300 */   145,  118,  121,   51,   54,  109,    7,   46,   91,  116,
 /*   310 */    14,   51,   43,  143,  119,  108,   79,  115,  113,  147,
 /*   320 */    33,  146,  144,   50,  112,   15,   49,   48,   94,   43,
 /*   330 */    65,  123,   77,  126,   70,  123,   19,  127,    9,  143,
 /*   340 */    53,   32,  138,   73,   36,  147,   33,  146,  144,  143,
 /*   350 */   110,   54,   72,  130,  153,  147,   33,  146,  144,    3,
 /*   360 */   143,  154,   35,  152,  117,   62,  147,   33,  146,  144,
 /*   370 */   151,   75,   74,   88,  143,   87,   62,  111,  239,  239,
 /*   380 */   147,   33,  146,  144,   86,  143,  239,   62,  239,  239,
 /*   390 */   239,  147,   33,  146,  144,   84,  143,  239,  239,   62,
 /*   400 */   239,  239,  147,   33,  146,  144,  239,   83,  143,  239,
 /*   410 */    62,  239,  239,  239,  147,   33,  146,  144,   80,  143,
 /*   420 */    65,  239,  214,  214,   58,  147,   33,  146,  144,  143,
 /*   430 */   239,  239,  239,  143,  239,  147,   33,  146,  144,  147,
 /*   440 */    33,  146,  144,  131,   66,  239,  214,  239,  214,  239,
 /*   450 */   239,  239,  214,  143,   55,  239,  239,  239,  239,  147,
 /*   460 */    33,  146,  144,  143,  104,  239,  239,  239,   67,  147,
 /*   470 */    33,  146,  144,  143,  239,  239,  239,  143,   69,  147,
 /*   480 */    33,  146,  144,  147,   33,  146,  144,  143,   68,  239,
 /*   490 */   239,  239,  239,  147,   33,  146,  144,  143,   71,  239,
 /*   500 */   239,  239,  103,  147,   33,  146,  144,  143,  239,  239,
 /*   510 */   239,  143,  239,  147,   33,  146,  144,  147,   33,  146,
 /*   520 */   144,  102,  239,  239,  239,  239,  239,  239,  239,  239,
 /*   530 */   143,   57,  239,  239,  239,  239,  147,   33,  146,  144,
 /*   540 */   143,   64,  239,  239,  239,   56,  147,   33,  146,  144,
 /*   550 */   143,  239,  239,  239,  143,   63,  147,   33,  146,  144,
 /*   560 */   147,   33,  146,  144,  143,   61,  239,  239,  239,  239,
 /*   570 */   147,   33,  146,  144,  143,   60,  239,  239,  239,   52,
 /*   580 */   147,   33,  146,  144,  143,  239,  239,  239,  143,  239,
 /*   590 */   147,   33,  146,  144,  147,   33,  146,  144,   59,  239,
 /*   600 */   239,  239,  239,  239,  239,  239,  239,  143,  239,  239,
 /*   610 */   239,  239,  239,  147,   33,  146,  144,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     3,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*    10 */    30,   31,   32,   33,   34,   35,    0,    4,   38,   22,
 /*    20 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*    30 */    33,   34,   35,   11,    4,   38,   33,   34,   35,   13,
 /*    40 */    14,   38,   22,   23,   24,   25,   26,   27,   28,   29,
 /*    50 */    30,   31,   32,   33,   34,   35,   12,   21,   38,   30,
 /*    60 */    31,   32,   33,   34,   35,   21,   44,   38,   48,   22,
 /*    70 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*    80 */    33,   34,   35,   38,   48,   38,   43,   19,   20,    1,
 /*    90 */    47,    4,   49,   46,   22,   23,   24,   25,   26,   27,
 /*   100 */    28,   29,   30,   31,   32,   33,   34,   35,    9,   21,
 /*   110 */    38,   43,   15,   45,   53,   47,   55,   49,   46,   22,
 /*   120 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*   130 */    33,   34,   35,   21,   53,   38,   55,   11,   50,   58,
 /*   140 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*   150 */    31,   32,   33,   34,   35,   47,   11,   38,   22,   23,
 /*   160 */    24,   25,   26,   27,   28,   29,   30,   31,   32,   33,
 /*   170 */    34,   35,    8,   47,   38,   11,   23,   24,   25,   26,
 /*   180 */    27,   28,   29,   30,   31,   32,   33,   34,   35,   44,
 /*   190 */    68,   38,    8,   64,    9,   11,   32,   75,    9,   77,
 /*   200 */    36,   37,   73,   39,   40,   41,   42,   43,   44,   45,
 /*   210 */    21,   47,    8,   49,   50,   11,   32,   19,   20,    8,
 /*   220 */    36,   37,   11,   39,   40,   41,   42,   43,   44,   70,
 /*   230 */     9,   47,   48,   49,   52,   53,   32,   55,    9,   80,
 /*   240 */    36,   37,   21,   39,   40,   41,   42,   43,   44,   45,
 /*   250 */    21,   47,    8,   49,   50,   11,   24,   25,   26,   27,
 /*   260 */    28,   29,   30,   31,   32,   33,   34,   35,   65,   53,
 /*   270 */    38,   55,   53,   53,   55,   55,   32,    9,   11,    3,
 /*   280 */    36,   37,   79,   39,   40,   41,   42,   43,   44,   21,
 /*   290 */    11,   47,    3,   49,    5,    6,    7,    8,   65,   10,
 /*   300 */    11,   53,    4,   55,   59,   16,   17,   18,   63,   53,
 /*   310 */     2,   55,   79,   68,    4,   65,   71,   72,   11,   74,
 /*   320 */    75,   76,   77,   57,   11,   21,   60,   61,   62,   79,
 /*   330 */    59,   76,   66,   78,   68,   76,   47,   78,    9,   68,
 /*   340 */    59,   75,   11,   77,   11,   74,   75,   76,   77,   68,
 /*   350 */    69,   59,   81,   82,   54,   74,   75,   76,   77,   56,
 /*   360 */    68,    1,   47,   54,   72,   59,   74,   75,   76,   77,
 /*   370 */    54,   70,   70,   67,   68,   11,   59,   11,   83,   83,
 /*   380 */    74,   75,   76,   77,   67,   68,   83,   59,   83,   83,
 /*   390 */    83,   74,   75,   76,   77,   67,   68,   83,   83,   59,
 /*   400 */    83,   83,   74,   75,   76,   77,   83,   67,   68,   83,
 /*   410 */    59,   83,   83,   83,   74,   75,   76,   77,   67,   68,
 /*   420 */    59,   83,   19,   20,   59,   74,   75,   76,   77,   68,
 /*   430 */    83,   83,   83,   68,   83,   74,   75,   76,   77,   74,
 /*   440 */    75,   76,   77,   82,   59,   83,   43,   83,   45,   83,
 /*   450 */    83,   83,   49,   68,   59,   83,   83,   83,   83,   74,
 /*   460 */    75,   76,   77,   68,   59,   83,   83,   83,   59,   74,
 /*   470 */    75,   76,   77,   68,   83,   83,   83,   68,   59,   74,
 /*   480 */    75,   76,   77,   74,   75,   76,   77,   68,   59,   83,
 /*   490 */    83,   83,   83,   74,   75,   76,   77,   68,   59,   83,
 /*   500 */    83,   83,   59,   74,   75,   76,   77,   68,   83,   83,
 /*   510 */    83,   68,   83,   74,   75,   76,   77,   74,   75,   76,
 /*   520 */    77,   59,   83,   83,   83,   83,   83,   83,   83,   83,
 /*   530 */    68,   59,   83,   83,   83,   83,   74,   75,   76,   77,
 /*   540 */    68,   59,   83,   83,   83,   59,   74,   75,   76,   77,
 /*   550 */    68,   83,   83,   83,   68,   59,   74,   75,   76,   77,
 /*   560 */    74,   75,   76,   77,   68,   59,   83,   83,   83,   83,
 /*   570 */    74,   75,   76,   77,   68,   59,   83,   83,   83,   59,
 /*   580 */    74,   75,   76,   77,   68,   83,   83,   83,   68,   83,
 /*   590 */    74,   75,   76,   77,   74,   75,   76,   77,   59,   83,
 /*   600 */    83,   83,   83,   83,   83,   83,   83,   68,   83,   83,
 /*   610 */    83,   83,   83,   74,   75,   76,   77,
};
#define YY_SHIFT_USE_DFLT (-21)
#define YY_SHIFT_COUNT (107)
#define YY_SHIFT_MIN   (-20)
#define YY_SHIFT_MAX   (403)
static const short yy_shift_ofst[] = {
 /*     0 */   -21,  244,  204,  289,  164,  184,  244,  244,  244,  244,
 /*    10 */   244,  244,  244,  244,  244,  244,  244,  244,  244,  244,
 /*    20 */   244,  244,  244,  244,  244,  244,  244,  244,  244,  244,
 /*    30 */   126,  -21,   68,   68,   43,  145,  315,  315,  366,  315,
 /*    40 */   -21,  -21,  -21,  -21,  -21,  -21,  211,  364,  360,  360,
 /*    50 */   360,  -21,  119,  -20,   97,   72,   47,   20,   -3,  136,
 /*    60 */   136,  136,  136,  136,  136,  136,  153,  232,   29,   29,
 /*    70 */   403,    3,   88,  268,  229,   44,  221,  189,  198,   26,
 /*    80 */    36,   22,  333,  304,  304,  331,  304,  329,  304,  313,
 /*    90 */   307,  310,  308,  298,  276,  279,  267,  185,   99,  108,
 /*   100 */   112,   87,   45,   45,   45,   30,   13,   16,
};
#define YY_REDUCE_USE_DFLT (-1)
#define YY_REDUCE_COUNT (51)
#define YY_REDUCE_MIN   (0)
#define YY_REDUCE_MAX   (539)
static const short yy_reduce_ofst[] = {
 /*     0 */   182,  245,  271,  266,  361,  351,  340,  328,  317,  281,
 /*    10 */   306,  292,  539,  520,  516,  506,  496,  486,  482,  472,
 /*    20 */   462,  443,  439,  429,  419,  409,  405,  395,  385,  365,
 /*    30 */   122,   81,  259,  259,  255,  159,  250,  203,  129,  233,
 /*    40 */   256,  248,  220,  219,  216,   61,  302,  301,  316,  309,
 /*    50 */   300,  303,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   161,  237,  237,  158,  237,  237,  237,  181,  237,  237,
 /*    10 */   237,  237,  237,  237,  237,  237,  237,  237,  237,  237,
 /*    20 */   237,  237,  237,  237,  237,  237,  237,  237,  237,  237,
 /*    30 */   237,  161,  237,  198,  237,  225,  237,  237,  237,  237,
 /*    40 */   161,  161,  161,  161,  161,  161,  237,  237,  157,  157,
 /*    50 */   157,  163,  194,  237,  237,  237,  237,  237,  237,  195,
 /*    60 */   160,  193,  192,  236,  235,  234,  201,  202,  204,  203,
 /*    70 */   172,  205,  237,  213,  183,  237,  213,  237,  186,  175,
 /*    80 */   237,  237,  237,  184,  182,  237,  174,  190,  171,  237,
 /*    90 */   237,  237,  237,  237,  237,  237,  237,  237,  210,  237,
 /*   100 */   226,  237,  207,  206,  200,  237,  237,  237,  185,  180,
 /*   110 */   173,  188,  189,  187,  170,  177,  179,  178,  176,  169,
 /*   120 */   168,  167,  221,  220,  218,  219,  217,  216,  212,  211,
 /*   130 */   232,  233,  231,  230,  229,  227,  224,  228,  191,  190,
 /*   140 */   223,  222,  215,  214,  213,  210,  199,  197,  196,  166,
 /*   150 */   165,  164,  162,  159,  156,
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
  "TRUE",          "FALSE",         "NUMBER",        "STRING",      
  "DOTS",          "SLPAREN",       "SRPAREN",       "LPAREN",      
  "RPAREN",        "LBRACE",        "RBRACE",        "error",       
  "chunk",         "block",         "semi",          "scope",       
  "statlist",      "laststat",      "ublock",        "exp",         
  "binding",       "stat",          "repetition",    "conds",       
  "funcname",      "funcbody",      "setlist",       "explist1",    
  "functioncall",  "explist23",     "namelist",      "condlist",    
  "cond",          "dottedname",    "function",      "prefixexp",   
  "tableconstructor",  "var",           "args",          "params",      
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
 /*  41 */ "exp ::= NIL|TRUE|FALSE|NUMBER|STRING|DOTS",
 /*  42 */ "exp ::= function",
 /*  43 */ "exp ::= prefixexp",
 /*  44 */ "exp ::= tableconstructor",
 /*  45 */ "exp ::= NOT|HASH|MINUS exp",
 /*  46 */ "exp ::= exp OR exp",
 /*  47 */ "exp ::= exp AND exp",
 /*  48 */ "exp ::= exp L|LE|G|GE|EQ|NE exp",
 /*  49 */ "exp ::= exp CONCAT exp",
 /*  50 */ "exp ::= exp PLUS|MINUS exp",
 /*  51 */ "exp ::= exp TIMES|DIVIDE|MOD exp",
 /*  52 */ "exp ::= exp POW exp",
 /*  53 */ "setlist ::= var",
 /*  54 */ "setlist ::= setlist COMMA var",
 /*  55 */ "var ::= NAME",
 /*  56 */ "var ::= prefixexp SLPAREN exp SRPAREN",
 /*  57 */ "var ::= prefixexp DOT NAME",
 /*  58 */ "prefixexp ::= var",
 /*  59 */ "prefixexp ::= functioncall",
 /*  60 */ "prefixexp ::= LPAREN exp RPAREN",
 /*  61 */ "functioncall ::= prefixexp args",
 /*  62 */ "functioncall ::= prefixexp COLON NAME args",
 /*  63 */ "args ::= LPAREN RPAREN",
 /*  64 */ "args ::= LPAREN explist1 RPAREN",
 /*  65 */ "args ::= tableconstructor",
 /*  66 */ "args ::= STRING",
 /*  67 */ "function ::= FUNCTION funcbody",
 /*  68 */ "funcbody ::= params block END",
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
  { 52, 1 },
  { 54, 1 },
  { 54, 0 },
  { 53, 2 },
  { 53, 4 },
  { 58, 3 },
  { 55, 0 },
  { 55, 4 },
  { 56, 0 },
  { 56, 3 },
  { 61, 3 },
  { 61, 5 },
  { 61, 4 },
  { 61, 2 },
  { 61, 3 },
  { 61, 3 },
  { 61, 3 },
  { 61, 1 },
  { 62, 4 },
  { 62, 4 },
  { 63, 1 },
  { 63, 3 },
  { 71, 1 },
  { 71, 3 },
  { 72, 3 },
  { 57, 1 },
  { 57, 1 },
  { 57, 2 },
  { 60, 2 },
  { 60, 4 },
  { 60, 4 },
  { 64, 1 },
  { 64, 3 },
  { 73, 1 },
  { 73, 3 },
  { 70, 1 },
  { 70, 3 },
  { 67, 1 },
  { 67, 3 },
  { 69, 3 },
  { 69, 5 },
  { 59, 1 },
  { 59, 1 },
  { 59, 1 },
  { 59, 1 },
  { 59, 2 },
  { 59, 3 },
  { 59, 3 },
  { 59, 3 },
  { 59, 3 },
  { 59, 3 },
  { 59, 3 },
  { 59, 3 },
  { 66, 1 },
  { 66, 3 },
  { 77, 1 },
  { 77, 4 },
  { 77, 3 },
  { 75, 1 },
  { 75, 1 },
  { 75, 3 },
  { 68, 2 },
  { 68, 4 },
  { 78, 2 },
  { 78, 3 },
  { 78, 1 },
  { 78, 1 },
  { 74, 2 },
  { 65, 3 },
  { 79, 3 },
  { 80, 0 },
  { 80, 1 },
  { 80, 1 },
  { 80, 3 },
  { 76, 2 },
  { 76, 3 },
  { 76, 4 },
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
      /* (16) stat ::= setlist SET explist1 */ yytestcase(yyruleno==16);
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
      /* (37) explist1 ::= exp */ yytestcase(yyruleno==37);
      /* (38) explist1 ::= explist1 COMMA exp */ yytestcase(yyruleno==38);
      /* (39) explist23 ::= exp COMMA exp */ yytestcase(yyruleno==39);
      /* (40) explist23 ::= exp COMMA exp COMMA exp */ yytestcase(yyruleno==40);
      /* (41) exp ::= NIL|TRUE|FALSE|NUMBER|STRING|DOTS */ yytestcase(yyruleno==41);
      /* (42) exp ::= function */ yytestcase(yyruleno==42);
      /* (43) exp ::= prefixexp */ yytestcase(yyruleno==43);
      /* (44) exp ::= tableconstructor */ yytestcase(yyruleno==44);
      /* (45) exp ::= NOT|HASH|MINUS exp */ yytestcase(yyruleno==45);
      /* (46) exp ::= exp OR exp */ yytestcase(yyruleno==46);
      /* (47) exp ::= exp AND exp */ yytestcase(yyruleno==47);
      /* (48) exp ::= exp L|LE|G|GE|EQ|NE exp */ yytestcase(yyruleno==48);
      /* (49) exp ::= exp CONCAT exp */ yytestcase(yyruleno==49);
      /* (50) exp ::= exp PLUS|MINUS exp */ yytestcase(yyruleno==50);
      /* (51) exp ::= exp TIMES|DIVIDE|MOD exp */ yytestcase(yyruleno==51);
      /* (52) exp ::= exp POW exp */ yytestcase(yyruleno==52);
      /* (53) setlist ::= var */ yytestcase(yyruleno==53);
      /* (54) setlist ::= setlist COMMA var */ yytestcase(yyruleno==54);
      /* (55) var ::= NAME */ yytestcase(yyruleno==55);
      /* (56) var ::= prefixexp SLPAREN exp SRPAREN */ yytestcase(yyruleno==56);
      /* (57) var ::= prefixexp DOT NAME */ yytestcase(yyruleno==57);
      /* (58) prefixexp ::= var */ yytestcase(yyruleno==58);
      /* (59) prefixexp ::= functioncall */ yytestcase(yyruleno==59);
      /* (60) prefixexp ::= LPAREN exp RPAREN */ yytestcase(yyruleno==60);
      /* (61) functioncall ::= prefixexp args */ yytestcase(yyruleno==61);
      /* (62) functioncall ::= prefixexp COLON NAME args */ yytestcase(yyruleno==62);
      /* (63) args ::= LPAREN RPAREN */ yytestcase(yyruleno==63);
      /* (64) args ::= LPAREN explist1 RPAREN */ yytestcase(yyruleno==64);
      /* (65) args ::= tableconstructor */ yytestcase(yyruleno==65);
      /* (66) args ::= STRING */ yytestcase(yyruleno==66);
      /* (67) function ::= FUNCTION funcbody */ yytestcase(yyruleno==67);
      /* (68) funcbody ::= params block END */ yytestcase(yyruleno==68);
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
#line 10 "uparser.y"

  printf ("Syntax error!\n");
#line 1119 "uparser.c"
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
