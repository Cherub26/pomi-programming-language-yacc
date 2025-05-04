/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 2
#define YYMINOR 0
#define YYPATCH 20240109

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#undef YYBTYACC
#define YYBTYACC 0
#define YYDEBUGSTR YYPREFIX "debug"
#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "pomi.y"
#include <stdbool.h>  /* This MUST be first */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "pomi.h"

/* Forward declarations*/
nodeType *opr(int oper, int nops, ...);
nodeType *id(int i);
nodeType *con(int value);
nodeType *flt(float value);
nodeType *str(char *value);
nodeType *boolean(bool value);  /* New function for boolean literals*/
void freeNode(nodeType *p);
int ex(nodeType *p);
float exf(nodeType *p);  /* New float-returning evaluation function*/
bool isFloatExpression(nodeType *p);  /* Add declaration for isFloatExpression*/
bool isBoolExpression(nodeType *p);   /* Add declaration for isBoolExpression*/
int yylex(void);
void yyerror(char *s);
extern int yylineno; /* Use line number tracking from Flex*/
int sym[26]; /* Symbol table for variables*/
char* strSym[26]; /* String symbol table*/
float floatSym[26]; /* Float symbol table*/
bool isFloat[26]; /* To track whether a variable holds an integer or float*/
bool isBool[26]; /* To track whether a variable holds a boolean*/
bool boolSym[26]; /* Boolean symbol table*/

/* Function table*/
typedef struct {
    int id;  /* Use the integer ID instead of char*/
    nodeType *params;
    nodeType *body;
} Function;

#define MAX_FUNCTIONS 100
Function functions[MAX_FUNCTIONS];
int function_count = 0;

/* Function to register a function definition*/
void register_function(int func_id, nodeType *params, nodeType *body) {
    if (function_count < MAX_FUNCTIONS) {
        functions[function_count].id = func_id;
        functions[function_count].params = params;
        functions[function_count].body = body;
        function_count++;
    } else {
        fprintf(stderr, "Too many functions defined\n");
    }
}

/* Function to find a function by id*/
Function* find_function(int func_id) {
    for (int i = 0; i < function_count; i++) {
        if (functions[i].id == func_id) {
            return &functions[i];
        }
    }
    return NULL;
}

/* For debugging - must be defined after tokens are declared*/
void print_param_tree(nodeType *node, int level);

#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#line 69 "pomi.y"
typedef union YYSTYPE {
    int integer;   /* Integer values*/
    float floatval; /* Float values*/
    int id;        /* Identifier indices*/
    char* strval;  /* String values*/
    bool boolval;  /* Boolean values*/
    nodeType *nPtr; /* Node pointers*/
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 106 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

#if !(defined(yylex) || defined(YYSTATE))
int YYLEX_DECL();
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define INTEGER 257
#define FLOAT 258
#define IDENTIFIER 259
#define STRING 260
#define TRUE 261
#define FALSE 262
#define WHILE 263
#define IF 264
#define PRINT 265
#define ELSE 266
#define FOR 267
#define RETURN 268
#define FUNCTION 269
#define DEFINE 270
#define DO 271
#define ENDWHILE 272
#define ENDIF 273
#define THEN 274
#define CREATE 275
#define AS 276
#define PLAYER 277
#define ENEMY 278
#define PLATFORM 279
#define ITEM 280
#define GAME 281
#define LEVEL 282
#define ON 283
#define COLLISION 284
#define PLUS 285
#define MINUS 286
#define MULTIPLY 287
#define DIVIDE 288
#define LESS_THAN 289
#define GREATER_THAN 290
#define EQUALS_EQUALS 291
#define NOT_EQUALS 292
#define LESS_THAN_EQUALS 293
#define GREATER_THAN_EQUALS 294
#define EQUALS 295
#define SEMICOLON 296
#define COMMA 297
#define LEFT_PAREN 298
#define RIGHT_PAREN 299
#define LEFT_BRACE 300
#define RIGHT_BRACE 301
#define LEFT_BRACKET 302
#define RIGHT_BRACKET 303
#define DOT 304
#define PLUS_EQUALS 305
#define MINUS_EQUALS 306
#define MULTIPLY_EQUALS 307
#define DIVIDE_EQUALS 308
#define AND 309
#define OR 310
#define NOT 311
#define IFX 312
#define UMINUS 313
#define YYERRCODE 256
typedef int YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    0,   15,   15,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,   13,   13,   13,   13,   13,   13,    8,    8,    9,
   10,   10,   11,   12,   12,   14,    6,    6,    6,    4,
    7,    7,    7,    5,    3,    3,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    2,
};
static const YYINT yylen[] = {                            2,
    1,    1,    2,    0,    1,    2,    5,    4,    4,    4,
    6,    6,    8,    5,    1,    1,    3,    8,    2,    1,
    2,    8,    8,    8,    8,    8,    8,    1,    2,    4,
    1,    1,    3,    3,    3,   10,    0,    1,    3,    4,
    0,    1,    3,    3,    1,    2,    1,    1,    1,    1,
    1,    1,    1,    2,    2,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    1,
};
static const YYINT yydefred[] = {                         0,
    2,    0,    0,    0,   47,   48,    0,   49,   51,   52,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    5,
    0,    0,    0,    3,    0,    0,   20,    0,   15,   16,
   21,    0,    0,    0,    0,    0,    0,    0,   69,    0,
    0,    0,    0,    0,    0,    0,   54,    0,   45,    0,
   55,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    6,    0,    0,   19,    0,    0,    0,    0,   34,
    0,    0,    0,    0,    0,   44,    0,    0,    0,    0,
   68,   17,   46,    0,    0,   58,   59,    0,    0,    0,
    0,    0,    0,    0,    0,   35,    8,    0,   40,    9,
   10,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   43,    0,    0,    0,    7,    0,
    0,   14,    0,    0,    0,    0,    0,    0,    0,   11,
    0,   12,   39,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   29,    0,    0,
    0,    0,    0,    0,   13,   18,    0,    0,    0,   32,
   22,   23,   24,   25,   26,   27,    0,    0,   30,    0,
   33,   36,
};
#if defined(YYDESTRUCT_CALL) || defined(YYSTYPE_TOSTRING)
static const YYINT yystos[] = {                           0,
  256,  315,  330,  256,  257,  258,  259,  260,  261,  262,
  263,  264,  265,  268,  269,  270,  275,  283,  286,  296,
  298,  300,  311,  316,  317,  319,  320,  327,  328,  329,
  296,  295,  298,  304,  305,  306,  259,  317,  319,  317,
  298,  317,  259,  259,  259,  284,  317,  317,  316,  318,
  317,  285,  286,  287,  288,  289,  290,  291,  292,  293,
  294,  296,  309,  310,  296,  304,  317,  317,  322,  259,
  317,  317,  271,  274,  317,  296,  298,  295,  276,  298,
  299,  301,  316,  317,  317,  317,  317,  317,  317,  317,
  317,  317,  317,  317,  317,  259,  296,  297,  299,  296,
  296,  318,  318,  299,  259,  321,  317,  277,  278,  279,
  280,  281,  282,  259,  322,  272,  266,  273,  296,  297,
  299,  296,  300,  300,  300,  300,  300,  300,  297,  296,
  318,  296,  321,  300,  259,  323,  324,  323,  323,  323,
  323,  323,  259,  273,  318,  295,  301,  323,  301,  301,
  301,  301,  301,  299,  296,  301,  302,  317,  325,  326,
  296,  296,  296,  296,  296,  296,  300,  322,  296,  318,
  303,  301,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT yydgoto[] = {                          2,
   49,   25,   50,   39,   27,  106,   69,  136,  137,  159,
  160,   28,   29,   30,    3,
};
static const YYINT yysindex[] = {                      -226,
    0,    0,  248, -261,    0,    0, -280,    0,    0,    0,
 -166, -166, -262, -166, -219, -217, -215, -239, -166,    0,
 -166,  248, -166,    0,  453, -250,    0, -256,    0,    0,
    0, -166, -166, -208, -166, -166, -275,  -37,    0,  443,
 -166,  479, -234, -229, -209, -230,    0,  491,    0,  -65,
    0, -166, -166, -166, -166, -166, -166, -166, -166, -166,
 -166,    0, -166, -166,    0, -190,  517,  531, -213,    0,
  557,  569,  248,  248,  583,    0, -172, -166, -144, -170,
    0,    0,    0, -260, -260,    0,    0, -228, -228,   -8,
   -8, -228, -228,  544,  661,    0,    0, -166,    0,    0,
    0,   46, -110, -206, -200, -187,  609, -191, -184, -181,
 -179, -178, -175, -171,    0, -169,  248, -167,    0, -172,
 -161,    0, -119, -119, -119, -119, -119, -119, -118,    0,
   91,    0,    0,  248, -152, -140, -119, -139, -133, -131,
 -130, -129, -142, -122,  136, -248, -121,    0, -117, -116,
 -114, -113, -111,  -98,    0,    0, -166,  635, -109,    0,
    0,    0,    0,    0,    0,    0,  248, -125,    0,  192,
    0,    0,
};
static const YYINT yyrindex[] = {                         1,
    0,    0,  189,    0,    0,    0,  621,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  647,    0,  264,    0,    0,
    0,    0,  -91,    0,    0,    0,  291,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -283,    0,    0,
    0,    0,    0,    0,    0,    0,  -88,    0,    0,    0,
    0,    0,    0,  318,  345,    0,    0, -186,  369,  167,
  223,  393,  417,  -90,   66,    0,    0, -266,    0,    0,
    0,    0,    0,    0,  -87,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -88,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -86,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -89, -261,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
#if YYBTYACC
static const YYINT yycindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
#endif
static const YYINT yygindex[] = {                         0,
   -1,   20,  -52,   -3,    0,   96,  -95, -120,    0,    0,
    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 970
static const YYINT yytable[] = {                         26,
    4,   24,  115,  138,  139,  140,  141,  142,    5,    6,
   37,    8,    9,   10,   32,   42,  148,   33,   26,   42,
  102,  103,   33,   34,   35,   36,   54,   55,   34,    1,
   38,   40,   41,   42,   31,   41,   41,   19,   47,   43,
   48,   44,   51,   45,   46,   65,   26,   66,   83,   21,
   70,   67,   68,  157,   71,   72,   52,   53,   54,   55,
   75,  168,   23,   77,  131,   78,   79,   80,   96,   26,
   26,   84,   85,   86,   87,   88,   89,   90,   91,   92,
   93,  145,   94,   95,   62,   99,  105,   62,  114,  119,
    5,    6,   37,    8,    9,   10,  120,  107,   26,   26,
   83,   83,   62,   62,   62,   62,   62,   62,  123,   62,
   62,  121,   62,   26,  170,  124,   62,   68,  125,   19,
  126,  127,   62,   62,  128,  129,  130,   26,  132,   83,
   26,   21,  108,  109,  110,  111,  112,  113,  134,  135,
  143,   26,  146,   83,   23,    4,    5,    6,    7,    8,
    9,   10,   11,   12,   13,  117,  154,   14,   15,   16,
  147,  149,  118,   26,   17,  158,   26,  150,   83,  151,
  152,  153,   18,  155,  161,   19,   68,  171,  162,  163,
   60,  164,  165,   60,  166,   20,  169,   21,    1,   22,
    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,
   23,  167,   14,   15,   16,   60,   60,   41,   60,   17,
   37,   38,   60,   41,   28,  133,    0,   18,   60,   60,
   19,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   20,    0,   21,   73,   22,   82,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   23,    0,   52,   53,   54,
   55,   56,   57,   58,   59,   60,   61,    4,    4,    4,
    4,    4,    4,    4,    4,    4,    0,    0,    4,    4,
    4,   63,   64,    0,    0,    4,   52,   53,   54,   55,
   56,   57,    0,    4,   60,   61,    4,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    4,    0,    4,    0,
    4,    4,    5,    6,    7,    8,    9,   10,   11,   12,
   13,    4,    0,   14,   15,   16,    0,  116,    0,    0,
   17,    0,    0,    0,    0,    0,    0,    0,   18,    0,
    0,   19,    0,    0,    0,    0,   61,    0,    0,   61,
    0,   20,    0,   21,    0,   22,    4,    5,    6,    7,
    8,    9,   10,   11,   12,   13,   23,    0,   14,   15,
   16,   61,   61,  144,   61,   17,    0,    0,   61,    0,
    0,    0,    0,   18,    0,   61,   19,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   20,    0,   21,    0,
   22,    4,    5,    6,    7,    8,    9,   10,   11,   12,
   13,   23,    0,   14,   15,   16,    0,    0,    0,    0,
   17,    0,    0,    0,    0,    0,    0,    0,   18,    0,
    0,   19,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   20,    0,   21,    0,   22,  156,   67,    0,    0,
   67,    0,    0,    0,    0,    0,   23,    4,    5,    6,
    7,    8,    9,   10,   11,   12,   13,   67,   67,   14,
   15,   16,   67,   67,    0,   67,   17,    0,    0,   67,
    0,    0,    0,    0,   18,   67,   67,   19,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   20,    0,   21,
    0,   22,  172,   66,    0,    0,   66,    0,    0,    0,
    0,    0,   23,    4,    5,    6,    7,    8,    9,   10,
   11,   12,   13,   66,   66,   14,   15,   16,   66,   66,
    0,   66,   17,    0,    0,   66,    0,    0,    0,    0,
   18,   66,   66,   19,   53,    0,    0,   53,    0,    0,
    0,    0,    0,   20,    0,   21,    0,   22,   53,   53,
   53,   53,   53,   53,   53,   53,   53,   53,   23,   53,
   53,   50,   53,    0,   50,    0,   53,    0,    0,    0,
    0,    0,   53,   53,    0,   50,   50,   50,   50,   50,
   50,   50,   50,   50,   50,    0,   50,   50,   56,   50,
    0,   56,    0,   50,    0,    0,    0,    0,    0,   50,
   50,    0,   56,   56,    0,    0,   56,   56,   56,   56,
   56,   56,    0,   56,   56,   57,   56,    0,   57,    0,
   56,    0,    0,    0,    0,    0,   56,   56,    0,   57,
   57,    0,    0,   57,   57,   57,   57,   57,   57,   63,
   57,   57,   63,   57,    0,    0,    0,   57,    0,    0,
    0,    0,    0,   57,   57,    0,    0,   63,   63,   63,
   63,   63,   63,   64,   63,   63,   64,   63,    0,    0,
    0,   63,    0,    0,    0,    0,    0,   63,   63,    0,
    0,   64,   64,   64,   64,   64,   64,   65,   64,   64,
   65,   64,    0,    0,    0,   64,    0,    0,    0,    0,
    0,   64,   64,    0,    0,   65,   65,   65,   65,   65,
   65,    0,   65,   65,    0,   65,   74,    0,    0,   65,
    0,    0,    0,    0,    0,   65,   65,   52,   53,   54,
   55,   56,   57,   58,   59,   60,   61,   52,   53,   54,
   55,   56,   57,   58,   59,   60,   61,    0,   62,    0,
    0,   63,   64,    0,    0,    0,    0,    0,    0,    0,
    0,   63,   64,   52,   53,   54,   55,   56,   57,   58,
   59,   60,   61,    0,   76,   52,   53,   54,   55,   56,
   57,   58,   59,   60,   61,    0,    0,   63,   64,   81,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   63,
   64,   52,   53,   54,   55,   56,   57,   58,   59,   60,
   61,    0,   97,    0,    0,   52,   53,   54,   55,   56,
   57,   58,   59,   60,   61,   63,   64,   98,   52,   53,
   54,   55,   56,   57,   58,   59,   60,   61,    0,   63,
   64,   52,   53,   54,   55,   56,   57,   58,   59,   60,
   61,    0,  100,   52,   53,   54,   55,   56,   57,   58,
   59,   60,   61,    0,  101,   63,   64,   52,   53,   54,
   55,   56,   57,   58,   59,   60,   61,   63,   64,    0,
    0,  104,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   63,   64,   52,   53,   54,   55,   56,   57,   58,
   59,   60,   61,    0,  122,   50,   50,   50,   50,   50,
   50,   50,   50,   50,   50,    0,   50,   63,   64,   52,
   53,   54,   55,   56,   57,   58,   59,   60,   61,   50,
   50,   69,   69,   69,   69,   69,   69,   69,   69,   69,
   69,    0,    0,   63,   64,   52,   53,   54,   55,   56,
   57,   58,   59,   60,   61,   69,   69,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   63,
};
static const YYINT yycheck[] = {                          3,
    0,    3,   98,  124,  125,  126,  127,  128,  257,  258,
  259,  260,  261,  262,  295,  299,  137,  298,   22,  303,
   73,   74,  298,  304,  305,  306,  287,  288,  304,  256,
   11,   12,  299,   14,  296,  298,  303,  286,   19,  259,
   21,  259,   23,  259,  284,  296,   50,  304,   50,  298,
  259,   32,   33,  302,   35,   36,  285,  286,  287,  288,
   41,  157,  311,  298,  117,  295,  276,  298,  259,   73,
   74,   52,   53,   54,   55,   56,   57,   58,   59,   60,
   61,  134,   63,   64,  271,  299,  259,  274,  259,  296,
  257,  258,  259,  260,  261,  262,  297,   78,  102,  103,
  102,  103,  289,  290,  291,  292,  293,  294,  300,  296,
  297,  299,  299,  117,  167,  300,  303,   98,  300,  286,
  300,  300,  309,  310,  300,  297,  296,  131,  296,  131,
  134,  298,  277,  278,  279,  280,  281,  282,  300,  259,
  259,  145,  295,  145,  311,  256,  257,  258,  259,  260,
  261,  262,  263,  264,  265,  266,  299,  268,  269,  270,
  301,  301,  273,  167,  275,  146,  170,  301,  170,  301,
  301,  301,  283,  296,  296,  286,  157,  303,  296,  296,
  271,  296,  296,  274,  296,  296,  296,  298,    0,  300,
  256,  257,  258,  259,  260,  261,  262,  263,  264,  265,
  311,  300,  268,  269,  270,  296,  297,  299,  299,  275,
  299,  299,  303,  303,  301,  120,   -1,  283,  309,  310,
  286,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  296,   -1,  298,  271,  300,  301,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  311,   -1,  285,  286,  287,
  288,  289,  290,  291,  292,  293,  294,  257,  258,  259,
  260,  261,  262,  263,  264,  265,   -1,   -1,  268,  269,
  270,  309,  310,   -1,   -1,  275,  285,  286,  287,  288,
  289,  290,   -1,  283,  293,  294,  286,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  296,   -1,  298,   -1,
  300,  256,  257,  258,  259,  260,  261,  262,  263,  264,
  265,  311,   -1,  268,  269,  270,   -1,  272,   -1,   -1,
  275,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  283,   -1,
   -1,  286,   -1,   -1,   -1,   -1,  271,   -1,   -1,  274,
   -1,  296,   -1,  298,   -1,  300,  256,  257,  258,  259,
  260,  261,  262,  263,  264,  265,  311,   -1,  268,  269,
  270,  296,  297,  273,  299,  275,   -1,   -1,  303,   -1,
   -1,   -1,   -1,  283,   -1,  310,  286,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  296,   -1,  298,   -1,
  300,  256,  257,  258,  259,  260,  261,  262,  263,  264,
  265,  311,   -1,  268,  269,  270,   -1,   -1,   -1,   -1,
  275,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  283,   -1,
   -1,  286,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  296,   -1,  298,   -1,  300,  301,  271,   -1,   -1,
  274,   -1,   -1,   -1,   -1,   -1,  311,  256,  257,  258,
  259,  260,  261,  262,  263,  264,  265,  291,  292,  268,
  269,  270,  296,  297,   -1,  299,  275,   -1,   -1,  303,
   -1,   -1,   -1,   -1,  283,  309,  310,  286,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  296,   -1,  298,
   -1,  300,  301,  271,   -1,   -1,  274,   -1,   -1,   -1,
   -1,   -1,  311,  256,  257,  258,  259,  260,  261,  262,
  263,  264,  265,  291,  292,  268,  269,  270,  296,  297,
   -1,  299,  275,   -1,   -1,  303,   -1,   -1,   -1,   -1,
  283,  309,  310,  286,  271,   -1,   -1,  274,   -1,   -1,
   -1,   -1,   -1,  296,   -1,  298,   -1,  300,  285,  286,
  287,  288,  289,  290,  291,  292,  293,  294,  311,  296,
  297,  271,  299,   -1,  274,   -1,  303,   -1,   -1,   -1,
   -1,   -1,  309,  310,   -1,  285,  286,  287,  288,  289,
  290,  291,  292,  293,  294,   -1,  296,  297,  271,  299,
   -1,  274,   -1,  303,   -1,   -1,   -1,   -1,   -1,  309,
  310,   -1,  285,  286,   -1,   -1,  289,  290,  291,  292,
  293,  294,   -1,  296,  297,  271,  299,   -1,  274,   -1,
  303,   -1,   -1,   -1,   -1,   -1,  309,  310,   -1,  285,
  286,   -1,   -1,  289,  290,  291,  292,  293,  294,  271,
  296,  297,  274,  299,   -1,   -1,   -1,  303,   -1,   -1,
   -1,   -1,   -1,  309,  310,   -1,   -1,  289,  290,  291,
  292,  293,  294,  271,  296,  297,  274,  299,   -1,   -1,
   -1,  303,   -1,   -1,   -1,   -1,   -1,  309,  310,   -1,
   -1,  289,  290,  291,  292,  293,  294,  271,  296,  297,
  274,  299,   -1,   -1,   -1,  303,   -1,   -1,   -1,   -1,
   -1,  309,  310,   -1,   -1,  289,  290,  291,  292,  293,
  294,   -1,  296,  297,   -1,  299,  274,   -1,   -1,  303,
   -1,   -1,   -1,   -1,   -1,  309,  310,  285,  286,  287,
  288,  289,  290,  291,  292,  293,  294,  285,  286,  287,
  288,  289,  290,  291,  292,  293,  294,   -1,  296,   -1,
   -1,  309,  310,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  309,  310,  285,  286,  287,  288,  289,  290,  291,
  292,  293,  294,   -1,  296,  285,  286,  287,  288,  289,
  290,  291,  292,  293,  294,   -1,   -1,  309,  310,  299,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  309,
  310,  285,  286,  287,  288,  289,  290,  291,  292,  293,
  294,   -1,  296,   -1,   -1,  285,  286,  287,  288,  289,
  290,  291,  292,  293,  294,  309,  310,  297,  285,  286,
  287,  288,  289,  290,  291,  292,  293,  294,   -1,  309,
  310,  285,  286,  287,  288,  289,  290,  291,  292,  293,
  294,   -1,  296,  285,  286,  287,  288,  289,  290,  291,
  292,  293,  294,   -1,  296,  309,  310,  285,  286,  287,
  288,  289,  290,  291,  292,  293,  294,  309,  310,   -1,
   -1,  299,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  309,  310,  285,  286,  287,  288,  289,  290,  291,
  292,  293,  294,   -1,  296,  285,  286,  287,  288,  289,
  290,  291,  292,  293,  294,   -1,  296,  309,  310,  285,
  286,  287,  288,  289,  290,  291,  292,  293,  294,  309,
  310,  285,  286,  287,  288,  289,  290,  291,  292,  293,
  294,   -1,   -1,  309,  310,  285,  286,  287,  288,  289,
  290,  291,  292,  293,  294,  309,  310,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  309,
};
#if YYBTYACC
static const YYINT yyctable[] = {                        -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
};
#endif
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 313
#define YYUNDFTOKEN 331
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"$end",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"error","INTEGER","FLOAT","IDENTIFIER",
"STRING","TRUE","FALSE","WHILE","IF","PRINT","ELSE","FOR","RETURN","FUNCTION",
"DEFINE","DO","ENDWHILE","ENDIF","THEN","CREATE","AS","PLAYER","ENEMY",
"PLATFORM","ITEM","GAME","LEVEL","ON","COLLISION","PLUS","MINUS","MULTIPLY",
"DIVIDE","LESS_THAN","GREATER_THAN","EQUALS_EQUALS","NOT_EQUALS",
"LESS_THAN_EQUALS","GREATER_THAN_EQUALS","EQUALS","SEMICOLON","COMMA",
"LEFT_PAREN","RIGHT_PAREN","LEFT_BRACE","RIGHT_BRACE","LEFT_BRACKET",
"RIGHT_BRACKET","DOT","PLUS_EQUALS","MINUS_EQUALS","MULTIPLY_EQUALS",
"DIVIDE_EQUALS","AND","OR","NOT","IFX","UMINUS","$accept","program","stmt",
"expr","stmt_list","function_call","return_stmt","parameter_list",
"argument_list","property_list","property","property_value","array_literal",
"member_access","object_creation","collision_handler","function",
"illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : program",
"program : function",
"program : error",
"function : function stmt",
"function :",
"stmt : SEMICOLON",
"stmt : expr SEMICOLON",
"stmt : PRINT LEFT_PAREN expr RIGHT_PAREN SEMICOLON",
"stmt : IDENTIFIER EQUALS expr SEMICOLON",
"stmt : IDENTIFIER PLUS_EQUALS expr SEMICOLON",
"stmt : IDENTIFIER MINUS_EQUALS expr SEMICOLON",
"stmt : WHILE expr DO stmt_list ENDWHILE SEMICOLON",
"stmt : IF expr THEN stmt_list ENDIF SEMICOLON",
"stmt : IF expr THEN stmt_list ELSE stmt_list ENDIF SEMICOLON",
"stmt : DEFINE IDENTIFIER EQUALS expr SEMICOLON",
"stmt : object_creation",
"stmt : collision_handler",
"stmt : LEFT_BRACE stmt_list RIGHT_BRACE",
"stmt : FUNCTION IDENTIFIER LEFT_PAREN parameter_list RIGHT_PAREN LEFT_BRACE stmt_list RIGHT_BRACE",
"stmt : function_call SEMICOLON",
"stmt : return_stmt",
"stmt : error SEMICOLON",
"object_creation : CREATE IDENTIFIER AS PLAYER LEFT_BRACE property_list RIGHT_BRACE SEMICOLON",
"object_creation : CREATE IDENTIFIER AS ENEMY LEFT_BRACE property_list RIGHT_BRACE SEMICOLON",
"object_creation : CREATE IDENTIFIER AS PLATFORM LEFT_BRACE property_list RIGHT_BRACE SEMICOLON",
"object_creation : CREATE IDENTIFIER AS ITEM LEFT_BRACE property_list RIGHT_BRACE SEMICOLON",
"object_creation : CREATE IDENTIFIER AS GAME LEFT_BRACE property_list RIGHT_BRACE SEMICOLON",
"object_creation : CREATE IDENTIFIER AS LEVEL LEFT_BRACE property_list RIGHT_BRACE SEMICOLON",
"property_list : property",
"property_list : property property_list",
"property : IDENTIFIER EQUALS property_value SEMICOLON",
"property_value : expr",
"property_value : array_literal",
"array_literal : LEFT_BRACKET argument_list RIGHT_BRACKET",
"member_access : IDENTIFIER DOT IDENTIFIER",
"member_access : member_access DOT IDENTIFIER",
"collision_handler : ON COLLISION LEFT_PAREN IDENTIFIER COMMA IDENTIFIER RIGHT_PAREN LEFT_BRACE stmt_list RIGHT_BRACE",
"parameter_list :",
"parameter_list : IDENTIFIER",
"parameter_list : IDENTIFIER COMMA parameter_list",
"function_call : IDENTIFIER LEFT_PAREN argument_list RIGHT_PAREN",
"argument_list :",
"argument_list : expr",
"argument_list : expr COMMA argument_list",
"return_stmt : RETURN expr SEMICOLON",
"stmt_list : stmt",
"stmt_list : stmt_list stmt",
"expr : INTEGER",
"expr : FLOAT",
"expr : STRING",
"expr : IDENTIFIER",
"expr : TRUE",
"expr : FALSE",
"expr : member_access",
"expr : MINUS expr",
"expr : NOT expr",
"expr : expr PLUS expr",
"expr : expr MINUS expr",
"expr : expr MULTIPLY expr",
"expr : expr DIVIDE expr",
"expr : expr AND expr",
"expr : expr OR expr",
"expr : expr LESS_THAN expr",
"expr : expr GREATER_THAN expr",
"expr : expr LESS_THAN_EQUALS expr",
"expr : expr GREATER_THAN_EQUALS expr",
"expr : expr NOT_EQUALS expr",
"expr : expr EQUALS_EQUALS expr",
"expr : LEFT_PAREN expr RIGHT_PAREN",
"expr : function_call",

};
#endif

#if YYDEBUG
int      yydebug;
#endif

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;
int      yynerrs;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
YYLTYPE  yyloc; /* position returned by actions */
YYLTYPE  yylloc; /* position from the lexer */
#endif

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(loc, rhs, n) \
do \
{ \
    if (n == 0) \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 0).last_line; \
        (loc).first_column = YYRHSLOC(rhs, 0).last_column; \
        (loc).last_line    = YYRHSLOC(rhs, 0).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, 0).last_column; \
    } \
    else \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 1).first_line; \
        (loc).first_column = YYRHSLOC(rhs, 1).first_column; \
        (loc).last_line    = YYRHSLOC(rhs, n).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, n).last_column; \
    } \
} while (0)
#endif /* YYLLOC_DEFAULT */
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#if YYBTYACC

#ifndef YYLVQUEUEGROWTH
#define YYLVQUEUEGROWTH 32
#endif
#endif /* YYBTYACC */

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#ifndef YYINITSTACKSIZE
#define YYINITSTACKSIZE 200
#endif

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  *p_base;
    YYLTYPE  *p_mark;
#endif
} YYSTACKDATA;
#if YYBTYACC

struct YYParseState_s
{
    struct YYParseState_s *save;    /* Previously saved parser state */
    YYSTACKDATA            yystack; /* saved parser stack */
    int                    state;   /* saved parser state */
    int                    errflag; /* saved error recovery status */
    int                    lexeme;  /* saved index of the conflict lexeme in the lexical queue */
    YYINT                  ctry;    /* saved index in yyctable[] for this conflict */
};
typedef struct YYParseState_s YYParseState;
#endif /* YYBTYACC */
/* variables for the parser stack */
static YYSTACKDATA yystack;
#if YYBTYACC

/* Current parser state */
static YYParseState *yyps = 0;

/* yypath != NULL: do the full parse, starting at *yypath parser state. */
static YYParseState *yypath = 0;

/* Base of the lexical value queue */
static YYSTYPE *yylvals = 0;

/* Current position at lexical value queue */
static YYSTYPE *yylvp = 0;

/* End position of lexical value queue */
static YYSTYPE *yylve = 0;

/* The last allocated position at the lexical value queue */
static YYSTYPE *yylvlim = 0;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
/* Base of the lexical position queue */
static YYLTYPE *yylpsns = 0;

/* Current position at lexical position queue */
static YYLTYPE *yylpp = 0;

/* End position of lexical position queue */
static YYLTYPE *yylpe = 0;

/* The last allocated position at the lexical position queue */
static YYLTYPE *yylplim = 0;
#endif

/* Current position at lexical token queue */
static YYINT  *yylexp = 0;

static YYINT  *yylexemes = 0;
#endif /* YYBTYACC */
#line 278 "pomi.y"

nodeType *con(int value) {
    nodeType *p;

    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    p->type = typeCon;
    p->con.value = value;

    return p;
}

nodeType *flt(float value) {
    nodeType *p;

    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    p->type = typeFloat;
    p->flt.value = value;

    return p;
}

nodeType *str(char *value) {
    nodeType *p;

    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    p->type = typeStr;
    p->str.value = value;

    return p;
}

nodeType *boolean(bool value) {
    nodeType *p;

    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    p->type = typeBool;
    p->boolean.value = value;

    return p;
}

nodeType *id(int i) {
    nodeType *p;

    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    p->type = typeId;
    p->id.i = i;

    return p;
}

nodeType *opr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    int i;

    if ((p = malloc(sizeof(nodeType) + (nops-1) * sizeof(nodeType *))) == NULL)
        yyerror("out of memory");

    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}

void freeNode(nodeType *p) {
    int i;

    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
    }
    free(p);
}

void yyerror(char *s) {
    fprintf(stderr, "Line %d: %s\n", yylineno, s);
    fflush(stderr);
    exit(EXIT_FAILURE); // Keep the immediate exit
}

int main(void) {
    printf("POMI Language Parser\n");
    /* We don't need to check parse_result since yyerror will exit on failure */
    yyparse();
    return EXIT_SUCCESS;
}

// Define the print_param_tree function after tokens are declared
void print_param_tree(nodeType *node, int level) {
    if (!node) {
        return;
    }
    
    if (node->type == typeId) {
        // No debug output
    } else if (node->type == typeOpr && node->opr.oper == COMMA) {
        print_param_tree(node->opr.op[0], level + 1);
        print_param_tree(node->opr.op[1], level + 1);
    } 
}

float exf(nodeType *p) {
    if (!p) return 0.0f;
    switch(p->type) {
    case typeCon:       return (float)p->con.value;
    case typeFloat:     return p->flt.value;
    case typeStr:       return 0.0f;  // Strings have no floating-point value
    case typeId:        
                        // Return the appropriate value depending on if it's a float or int
                        if (isFloat[p->id.i])
                            return floatSym[p->id.i];
                        return (float)sym[p->id.i];
    case typeOpr:
        switch(p->opr.oper) {
        case CREATE:    return 0.0f;  // Object creation doesn't have a float value
        case COLLISION: ex(p->opr.op[2]); return 0.0f;
        case DOT:       return 10.0f; // Placeholder for member access
        case '[':       return 0.0f;  // Placeholder for array literals
        case WHILE:     while(ex(p->opr.op[0])) ex(p->opr.op[1]); return 0.0f;
        case IF:        
            if (ex(p->opr.op[0])) {
                return exf(p->opr.op[1]);
            } else if (p->opr.nops > 2) {
                return exf(p->opr.op[2]); 
            }
            return 0.0f;
        case PRINT:     return 0.0f;  // Print doesn't return a value
        case ';':       exf(p->opr.op[0]); return exf(p->opr.op[1]);
        case EQUALS:    
        case DEFINE:    return 0.0f;  // Assignments don't return float values
        case UMINUS:    return -exf(p->opr.op[0]);
        case PLUS:      return exf(p->opr.op[0]) + exf(p->opr.op[1]);
        case MINUS:     return exf(p->opr.op[0]) - exf(p->opr.op[1]);
        case MULTIPLY:  return exf(p->opr.op[0]) * exf(p->opr.op[1]);
        case DIVIDE:    return exf(p->opr.op[0]) / exf(p->opr.op[1]);
        case LESS_THAN: return exf(p->opr.op[0]) < exf(p->opr.op[1]) ? 1.0f : 0.0f;
        case GREATER_THAN: return exf(p->opr.op[0]) > exf(p->opr.op[1]) ? 1.0f : 0.0f;
        case GREATER_THAN_EQUALS: return exf(p->opr.op[0]) >= exf(p->opr.op[1]) ? 1.0f : 0.0f;
        case LESS_THAN_EQUALS: return exf(p->opr.op[0]) <= exf(p->opr.op[1]) ? 1.0f : 0.0f;
        case NOT_EQUALS: return exf(p->opr.op[0]) != exf(p->opr.op[1]) ? 1.0f : 0.0f;
        case EQUALS_EQUALS: return exf(p->opr.op[0]) == exf(p->opr.op[1]) ? 1.0f : 0.0f;
        case FUNCTION:  {
            // Similar to the regular ex() function, but return float results
            int func_idx = p->opr.op[0]->id.i;
            Function* func = find_function(func_idx);
            
            if (!func) {
                fprintf(stderr, "Error: Undefined function '%c'\n", 'a' + func_idx);
                return 0.0f;
            }
            
            // Save current symbol table states
            int saved_sym[26];
            bool saved_isFloat[26];
            float saved_floatSym[26];
            memcpy(saved_sym, sym, sizeof(sym));
            memcpy(saved_isFloat, isFloat, sizeof(isFloat));
            memcpy(saved_floatSym, floatSym, sizeof(floatSym));
            
            // Process function parameters
            // ...existing function parameter code...
            
            // Execute function with float return value
            float result = 0.0f;
            
            // Check if the function body itself is a RETURN statement
            if (func->body && func->body->type == typeOpr && func->body->opr.oper == RETURN) {
                result = exf(func->body->opr.op[0]);
            }
            // Otherwise execute the function body normally and look for RETURN statements
            else {
                // ...existing function body execution code, but using exf() for results...
            }
            
            // Restore symbol table
            memcpy(sym, saved_sym, sizeof(sym));
            memcpy(isFloat, saved_isFloat, sizeof(isFloat));
            memcpy(floatSym, saved_floatSym, sizeof(floatSym));
            
            return result;
        }
        case COMMA:     return exf(p->opr.op[0]);
        case RETURN:    return exf(p->opr.op[0]);
        }
    }
    return 0.0f;
}

int ex(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
    case typeCon:       return p->con.value;
    case typeFloat:     
                        return (int)p->flt.value;
    case typeBool:      return p->boolean.value ? 1 : 0;
    case typeStr:       // For string literals, print them directly
                        if (p->str.value != NULL) {
                            return 0;  // Strings don't have a numerical value
                        }
                        return 0;
    case typeId:        
                        // Return the appropriate value depending on type
                        if (isFloat[p->id.i])
                            return (int)floatSym[p->id.i];
                        else if (isBool[p->id.i])
                            return boolSym[p->id.i] ? 1 : 0;
                        return sym[p->id.i];
    case typeOpr:
        switch(p->opr.oper) {
        case CREATE:    {
                          // Handle object creation
                          // For now, just output what we're creating
                          int objId = p->opr.op[0]->id.i;
                          printf("Created object %c\n", 'a' + objId);
                          return 0;
                        }
        case COLLISION: {
                          // Handle collision event
                          int obj1 = p->opr.op[0]->id.i;
                          int obj2 = p->opr.op[1]->id.i;
                          printf("Registered collision between %c and %c\n", 'a' + obj1, 'a' + obj2);
                          // Execute the collision handler body
                          ex(p->opr.op[2]);
                          return 0;
                        }
        case DOT:       {
                          // Handle member access (e.g., player.health)
                          // This is just a placeholder implementation
                          int objId = p->opr.op[0]->id.i;
                          int propId = p->opr.op[1]->id.i;
                          // For demonstration purposes only
                          return 10; // Return dummy value
                        }
        case '[':       {
                          // Handle array literals
                          // Just a placeholder implementation
                          return 0;
                        }
        case WHILE:     while(ex(p->opr.op[0])) ex(p->opr.op[1]); return 0;
        case IF:        {
            if (ex(p->opr.op[0])) {
                return ex(p->opr.op[1]);
            } else if (p->opr.nops > 2) {
                return ex(p->opr.op[2]); 
            }
            return 0;
        }
        case PRINT:     
                        if (p->opr.op[0]->type == typeStr) {
                            printf("%s\n", p->opr.op[0]->str.value);
                        } else if (p->opr.op[0]->type == typeFloat) {
                            printf("%.2f\n", p->opr.op[0]->flt.value);
                        } else if (p->opr.op[0]->type == typeBool) {
                            printf("%s\n", p->opr.op[0]->boolean.value ? "true" : "false");
                        } else if (isFloatExpression(p->opr.op[0])) {
                            printf("%.2f\n", exf(p->opr.op[0]));
                        } else if (isBool[p->opr.op[0]->id.i]) {
                            printf("%s\n", boolSym[p->opr.op[0]->id.i] ? "true" : "false");
                        } else if (isBoolExpression(p->opr.op[0])) {
                            // Print boolean expressions as true/false
                            printf("%s\n", ex(p->opr.op[0]) ? "true" : "false");
                        } else {
                            printf("%d\n", ex(p->opr.op[0])); 
                        }
                        return 0;
        case ';':       ex(p->opr.op[0]); return ex(p->opr.op[1]);
        case EQUALS:    {
                          if (p->opr.op[1]->type == typeFloat) {
                              isFloat[p->opr.op[0]->id.i] = true;
                              isBool[p->opr.op[0]->id.i] = false;
                              floatSym[p->opr.op[0]->id.i] = p->opr.op[1]->flt.value;
                              return (int)p->opr.op[1]->flt.value;
                          } else if (p->opr.op[1]->type == typeBool) {
                              isFloat[p->opr.op[0]->id.i] = false;
                              isBool[p->opr.op[0]->id.i] = true;
                              boolSym[p->opr.op[0]->id.i] = p->opr.op[1]->boolean.value;
                              return p->opr.op[1]->boolean.value ? 1 : 0;
                          } else if (isFloatExpression(p->opr.op[1])) {
                              isFloat[p->opr.op[0]->id.i] = true;
                              isBool[p->opr.op[0]->id.i] = false;
                              floatSym[p->opr.op[0]->id.i] = exf(p->opr.op[1]);
                              return (int)floatSym[p->opr.op[0]->id.i];
                          } else {
                              isFloat[p->opr.op[0]->id.i] = false;
                              isBool[p->opr.op[0]->id.i] = false;
                              return sym[p->opr.op[0]->id.i] = ex(p->opr.op[1]);
                          }
                        }
        case DEFINE:    {
                          if (p->opr.op[1]->type == typeFloat) {
                              isFloat[p->opr.op[0]->id.i] = true;
                              isBool[p->opr.op[0]->id.i] = false;
                              floatSym[p->opr.op[0]->id.i] = p->opr.op[1]->flt.value;
                              return (int)p->opr.op[1]->flt.value;
                          } else if (p->opr.op[1]->type == typeBool) {
                              isFloat[p->opr.op[0]->id.i] = false;
                              isBool[p->opr.op[0]->id.i] = true;
                              boolSym[p->opr.op[0]->id.i] = p->opr.op[1]->boolean.value;
                              return p->opr.op[1]->boolean.value ? 1 : 0;
                          } else if (isFloatExpression(p->opr.op[1])) {
                              isFloat[p->opr.op[0]->id.i] = true;
                              isBool[p->opr.op[0]->id.i] = false;
                              floatSym[p->opr.op[0]->id.i] = exf(p->opr.op[1]);
                              return (int)floatSym[p->opr.op[0]->id.i];
                          } else {
                              isFloat[p->opr.op[0]->id.i] = false;
                              isBool[p->opr.op[0]->id.i] = false;
                              return sym[p->opr.op[0]->id.i] = ex(p->opr.op[1]);
                          }
                        }
        case UMINUS:    return -ex(p->opr.op[0]);
        case PLUS:      
                        if (isFloatExpression(p->opr.op[0]) || isFloatExpression(p->opr.op[1])) {
                            return (int)(exf(p->opr.op[0]) + exf(p->opr.op[1]));
                        }
                        return ex(p->opr.op[0]) + ex(p->opr.op[1]);
        case MINUS:     
                        if (isFloatExpression(p->opr.op[0]) || isFloatExpression(p->opr.op[1])) {
                            return (int)(exf(p->opr.op[0]) - exf(p->opr.op[1]));
                        }
                        return ex(p->opr.op[0]) - ex(p->opr.op[1]);
        case MULTIPLY:  
                        if (isFloatExpression(p->opr.op[0]) || isFloatExpression(p->opr.op[1])) {
                            return (int)(exf(p->opr.op[0]) * exf(p->opr.op[1]));
                        }
                        return ex(p->opr.op[0]) * ex(p->opr.op[1]);
        case DIVIDE:    
                        if (isFloatExpression(p->opr.op[0]) || isFloatExpression(p->opr.op[1])) {
                            return (int)(exf(p->opr.op[0]) / exf(p->opr.op[1]));
                        }
                        return ex(p->opr.op[0]) / ex(p->opr.op[1]);
        case LESS_THAN: return ex(p->opr.op[0]) < ex(p->opr.op[1]);
        case GREATER_THAN: return ex(p->opr.op[0]) > ex(p->opr.op[1]);
        case GREATER_THAN_EQUALS: return ex(p->opr.op[0]) >= ex(p->opr.op[1]);
        case LESS_THAN_EQUALS: return ex(p->opr.op[0]) <= ex(p->opr.op[1]);
        case NOT_EQUALS: return ex(p->opr.op[0]) != ex(p->opr.op[1]);
        case EQUALS_EQUALS: return ex(p->opr.op[0]) == ex(p->opr.op[1]);
        case NOT:       return !ex(p->opr.op[0]);
        case AND:       return ex(p->opr.op[0]) && ex(p->opr.op[1]);
        case OR:        return ex(p->opr.op[0]) || ex(p->opr.op[1]);
        case FUNCTION:  {
            // Get the function by id
            int func_idx = p->opr.op[0]->id.i;
            Function* func = find_function(func_idx);
            
            if (!func) {
                fprintf(stderr, "Error: Undefined function '%c'\n", 'a' + func_idx);
                return 0;
            }
            
            // Save current symbol table states
            int saved_sym[26];
            bool saved_isFloat[26];
            float saved_floatSym[26];
            memcpy(saved_sym, sym, sizeof(sym));
            memcpy(saved_isFloat, isFloat, sizeof(isFloat));
            memcpy(saved_floatSym, floatSym, sizeof(floatSym));
            
            // Process arguments and assign to parameters
            if (func->params != NULL) {
                if (func->params->type == typeId) {
                    // Single parameter
                    int param_id = func->params->id.i;
                    int arg_value;
                    
                    if (p->opr.op[1] != NULL) {
                        arg_value = ex(p->opr.op[1]);
                    } else {
                        arg_value = 0; // Default value if no argument provided
                    }
                    
                    sym[param_id] = arg_value;
                } 
                else if (func->params->type == typeOpr && func->params->opr.oper == COMMA) {
                    // Multiple parameters (comma-separated)
                    nodeType* params = func->params;
                    nodeType* args = p->opr.op[1];
                    
                    // Handle first parameter (always exists in a comma node)
                    if (params->opr.op[0]->type == typeId) {
                        int param1_id = params->opr.op[0]->id.i;
                        int arg1_value;
                        
                        if (args != NULL && args->type == typeOpr && args->opr.oper == COMMA) {
                            arg1_value = ex(args->opr.op[0]);
                        } else if (args != NULL) {
                            arg1_value = ex(args);
                        } else {
                            arg1_value = 0; // Default
                        }
                        
                        sym[param1_id] = arg1_value;
                    }
                    
                    // Handle second parameter
                    if (params->opr.op[1] != NULL) {
                        if (params->opr.op[1]->type == typeId) {
                            int param2_id = params->opr.op[1]->id.i;
                            int arg2_value = 0; // Default
                            
                            if (args != NULL && args->type == typeOpr && args->opr.oper == COMMA) {
                                if (args->opr.op[1] != NULL) {
                                    arg2_value = ex(args->opr.op[1]);
                                }
                            }
                            
                            sym[param2_id] = arg2_value;
                        } 
                        else if (params->opr.op[1]->type == typeOpr && params->opr.op[1]->opr.oper == COMMA) {
                            // Handle more than two parameters if needed
                            // (This example only handles up to 2 parameters)
                        }
                    }
                }
            }
            
            // Execute function body with special handling for RETURN statements
            int result = 0;
            
            // Check if the function body itself is a RETURN statement
            if (func->body && func->body->type == typeOpr && func->body->opr.oper == RETURN) {
                result = ex(func->body->opr.op[0]);
            }
            // Otherwise execute the function body normally
            else {
                // Find return statements inside the function body
                nodeType* body = func->body;
                if (body && body->type == typeOpr) {
                    // Process each statement in the function body
                    if (body->opr.oper == ';') {
                        // Handle sequential statements
                        nodeType* current = body;
                        
                        // Look for RETURN statements at any level inside the function
                        while (current && current->type == typeOpr && current->opr.oper == ';') {
                            // Check left node for RETURN
                            if (current->opr.op[0] && current->opr.op[0]->type == typeOpr && 
                                current->opr.op[0]->opr.oper == RETURN) {
                                result = ex(current->opr.op[0]->opr.op[0]);
                                break;
                            }
                            
                            // Check right node for RETURN
                            if (current->opr.op[1] && current->opr.op[1]->type == typeOpr && 
                                current->opr.op[1]->opr.oper == RETURN) {
                                result = ex(current->opr.op[1]->opr.op[0]);
                                break;
                            }
                            
                            // If we found a non-sequential statement on the right
                            // (like an IF statement), execute it to find any RETURN
                            if (current->opr.op[1] && current->opr.op[1]->type == typeOpr &&
                                current->opr.op[1]->opr.oper != ';') {
                                result = ex(current->opr.op[1]);
                                break;
                            }
                            
                            // Move to the next statement
                            if (current->opr.op[1] && current->opr.op[1]->type == typeOpr && 
                                current->opr.op[1]->opr.oper == ';') {
                                current = current->opr.op[1];
                            } else {
                                break;
                            }
                        }
                    } else {
                        // If it's just a single statement
                        result = ex(body);
                    }
                }
            }
            
            // Restore symbol table
            memcpy(sym, saved_sym, sizeof(sym));
            memcpy(isFloat, saved_isFloat, sizeof(isFloat));
            memcpy(floatSym, saved_floatSym, sizeof(floatSym));
            
            return result;
        }
        case COMMA:     
            return ex(p->opr.op[0]); // Just return first value for comma operator
        case RETURN: {
            // When we encounter a RETURN statement, return the evaluated expression
            return ex(p->opr.op[0]);
        }
        }
    }
    return 0;
}

bool isFloatExpression(nodeType *p) {
    if (!p) return false;
    
    switch(p->type) {
        case typeFloat:
            return true;
        case typeId:
            return isFloat[p->id.i];
        case typeCon:
        case typeStr:
        case typeBool:
            return false;
        case typeOpr:
            switch(p->opr.oper) {
                case PLUS:
                case MINUS:
                case MULTIPLY:
                case DIVIDE:
                    return isFloatExpression(p->opr.op[0]) || isFloatExpression(p->opr.op[1]);
                case UMINUS:
                    return isFloatExpression(p->opr.op[0]);
                case FUNCTION:
                    // Check if any arguments are floats, which might make the result a float
                    if (p->opr.op[1]) {
                        if (p->opr.op[1]->type == typeOpr && p->opr.op[1]->opr.oper == COMMA) {
                            return isFloatExpression(p->opr.op[1]->opr.op[0]) || 
                                   isFloatExpression(p->opr.op[1]->opr.op[1]);
                        } else {
                            return isFloatExpression(p->opr.op[1]);
                        }
                    }
                    return false;
                default:
                    return false;
            }
        default:
            return false;
    }
}

bool isBoolExpression(nodeType *p) {
    if (!p) return false;
    
    switch(p->type) {
        case typeBool:
            return true;
        case typeId:
            return isBool[p->id.i];
        case typeCon:
        case typeFloat:
        case typeStr:
            return false;
        case typeOpr:
            switch(p->opr.oper) {
                case AND:
                case OR:
                case NOT:
                case LESS_THAN:
                case GREATER_THAN:
                case LESS_THAN_EQUALS:
                case GREATER_THAN_EQUALS:
                case EQUALS_EQUALS:
                case NOT_EQUALS:
                    return true;  // These operators always produce boolean results
                case FUNCTION:
                    // The function might return a boolean value
                    // We can't easily determine this, but we can be conservative
                    return false;
                default:
                    return false;
            }
        default:
            return false;
    }
}
#line 1446 "y.tab.c"

/* For use in generated program */
#define yydepth (int)(yystack.s_mark - yystack.s_base)
#if YYBTYACC
#define yytrial (yyps->save)
#endif /* YYBTYACC */

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE *newps;
#endif

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    newps = (YYLTYPE *)realloc(data->p_base, newsize * sizeof(*newps));
    if (newps == 0)
        return YYENOMEM;

    data->p_base = newps;
    data->p_mark = newps + i;
#endif

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;

#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%sdebug: stack size increased to %d\n", YYPREFIX, newsize);
#endif
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    free(data->p_base);
#endif
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif /* YYPURE || defined(YY_NO_LEAKS) */
#if YYBTYACC

static YYParseState *
yyNewState(unsigned size)
{
    YYParseState *p = (YYParseState *) malloc(sizeof(YYParseState));
    if (p == NULL) return NULL;

    p->yystack.stacksize = size;
    if (size == 0)
    {
        p->yystack.s_base = NULL;
        p->yystack.l_base = NULL;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        p->yystack.p_base = NULL;
#endif
        return p;
    }
    p->yystack.s_base    = (YYINT *) malloc(size * sizeof(YYINT));
    if (p->yystack.s_base == NULL) return NULL;
    p->yystack.l_base    = (YYSTYPE *) malloc(size * sizeof(YYSTYPE));
    if (p->yystack.l_base == NULL) return NULL;
    memset(p->yystack.l_base, 0, size * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    p->yystack.p_base    = (YYLTYPE *) malloc(size * sizeof(YYLTYPE));
    if (p->yystack.p_base == NULL) return NULL;
    memset(p->yystack.p_base, 0, size * sizeof(YYLTYPE));
#endif

    return p;
}

static void
yyFreeState(YYParseState *p)
{
    yyfreestack(&p->yystack);
    free(p);
}
#endif /* YYBTYACC */

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab
#if YYBTYACC
#define YYVALID        do { if (yyps->save)            goto yyvalid; } while(0)
#define YYVALID_NESTED do { if (yyps->save && \
                                yyps->save->save == 0) goto yyvalid; } while(0)
#endif /* YYBTYACC */

int
YYPARSE_DECL()
{
    int yym, yyn, yystate, yyresult;
#if YYBTYACC
    int yynewerrflag;
    YYParseState *yyerrctx = NULL;
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  yyerror_loc_range[3]; /* position of error start/end (0 unused) */
#endif
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
    if (yydebug)
        fprintf(stderr, "%sdebug[<# of symbols on state stack>]\n", YYPREFIX);
#endif
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    memset(yyerror_loc_range, 0, sizeof(yyerror_loc_range));
#endif

#if YYBTYACC
    yyps = yyNewState(0); if (yyps == 0) goto yyenomem;
    yyps->save = 0;
#endif /* YYBTYACC */
    yym = 0;
    /* yyn is set below */
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base;
#endif
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
#if YYBTYACC
        do {
        if (yylvp < yylve)
        {
            /* we're currently re-reading tokens */
            yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc = *yylpp++;
#endif
            yychar = *yylexp++;
            break;
        }
        if (yyps->save)
        {
            /* in trial mode; save scanner results for future parse attempts */
            if (yylvp == yylvlim)
            {   /* Enlarge lexical value queue */
                size_t p = (size_t) (yylvp - yylvals);
                size_t s = (size_t) (yylvlim - yylvals);

                s += YYLVQUEUEGROWTH;
                if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL) goto yyenomem;
                if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL) goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL) goto yyenomem;
#endif
                yylvp   = yylve = yylvals + p;
                yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp   = yylpe = yylpsns + p;
                yylplim = yylpsns + s;
#endif
                yylexp  = yylexemes + p;
            }
            *yylexp = (YYINT) YYLEX;
            *yylvp++ = yylval;
            yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *yylpp++ = yylloc;
            yylpe++;
#endif
            yychar = *yylexp++;
            break;
        }
        /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
        yychar = YYLEX;
#if YYBTYACC
        } while (0);
#endif /* YYBTYACC */
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, " <%s>", YYSTYPE_TOSTRING(yychar, yylval));
#endif
            fputc('\n', stderr);
        }
#endif
    }
#if YYBTYACC

    /* Do we have a conflict? */
    if (((yyn = yycindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
        yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        YYINT ctry;

        if (yypath)
        {
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: CONFLICT in state %d: following successful trial parse\n",
                                YYDEBUGSTR, yydepth, yystate);
#endif
            /* Switch to the next conflict context */
            save = yypath;
            yypath = save->save;
            save->save = NULL;
            ctry = save->ctry;
            if (save->state != yystate) YYABORT;
            yyFreeState(save);

        }
        else
        {

            /* Unresolved conflict - start/continue trial parse */
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
            {
                fprintf(stderr, "%s[%d]: CONFLICT in state %d. ", YYDEBUGSTR, yydepth, yystate);
                if (yyps->save)
                    fputs("ALREADY in conflict, continuing trial parse.\n", stderr);
                else
                    fputs("Starting trial parse.\n", stderr);
            }
#endif
            save                  = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (save == NULL) goto yyenomem;
            save->save            = yyps->save;
            save->state           = yystate;
            save->errflag         = yyerrflag;
            save->yystack.s_mark  = save->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (save->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            save->yystack.l_mark  = save->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (save->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            save->yystack.p_mark  = save->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (save->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            ctry                  = yytable[yyn];
            if (yyctable[ctry] == -1)
            {
#if YYDEBUG
                if (yydebug && yychar >= YYEOF)
                    fprintf(stderr, "%s[%d]: backtracking 1 token\n", YYDEBUGSTR, yydepth);
#endif
                ctry++;
            }
            save->ctry = ctry;
            if (yyps->save == NULL)
            {
                /* If this is a first conflict in the stack, start saving lexemes */
                if (!yylexemes)
                {
                    yylexemes = (YYINT *) malloc((YYLVQUEUEGROWTH) * sizeof(YYINT));
                    if (yylexemes == NULL) goto yyenomem;
                    yylvals   = (YYSTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYSTYPE));
                    if (yylvals == NULL) goto yyenomem;
                    yylvlim   = yylvals + YYLVQUEUEGROWTH;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpsns   = (YYLTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYLTYPE));
                    if (yylpsns == NULL) goto yyenomem;
                    yylplim   = yylpsns + YYLVQUEUEGROWTH;
#endif
                }
                if (yylvp == yylve)
                {
                    yylvp  = yylve = yylvals;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp  = yylpe = yylpsns;
#endif
                    yylexp = yylexemes;
                    if (yychar >= YYEOF)
                    {
                        *yylve++ = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                        *yylpe++ = yylloc;
#endif
                        *yylexp  = (YYINT) yychar;
                        yychar   = YYEMPTY;
                    }
                }
            }
            if (yychar >= YYEOF)
            {
                yylvp--;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp--;
#endif
                yylexp--;
                yychar = YYEMPTY;
            }
            save->lexeme = (int) (yylvp - yylvals);
            yyps->save   = save;
        }
        if (yytable[yyn] == ctry)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                                YYDEBUGSTR, yydepth, yystate, yyctable[ctry]);
#endif
            if (yychar < 0)
            {
                yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp++;
#endif
                yylexp++;
            }
            if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                goto yyoverflow;
            yystate = yyctable[ctry];
            *++yystack.s_mark = (YYINT) yystate;
            *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *++yystack.p_mark = yylloc;
#endif
            yychar  = YYEMPTY;
            if (yyerrflag > 0) --yyerrflag;
            goto yyloop;
        }
        else
        {
            yyn = yyctable[ctry];
            goto yyreduce;
        }
    } /* End of code dealing with conflicts */
#endif /* YYBTYACC */
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                            YYDEBUGSTR, yydepth, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yylloc;
#endif
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;
#if YYBTYACC

    yynewerrflag = 1;
    goto yyerrhandler;
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */

yyerrlab:
    /* explicit YYERROR from an action -- pop the rhs of the rule reduced
     * before looking for error recovery */
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif

    yynewerrflag = 0;
yyerrhandler:
    while (yyps->save)
    {
        int ctry;
        YYParseState *save = yyps->save;
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: ERROR in state %d, CONFLICT BACKTRACKING to state %d, %d tokens\n",
                            YYDEBUGSTR, yydepth, yystate, yyps->save->state,
                    (int)(yylvp - yylvals - yyps->save->lexeme));
#endif
        /* Memorize most forward-looking error state in case it's really an error. */
        if (yyerrctx == NULL || yyerrctx->lexeme < yylvp - yylvals)
        {
            /* Free old saved error context state */
            if (yyerrctx) yyFreeState(yyerrctx);
            /* Create and fill out new saved error context state */
            yyerrctx                 = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (yyerrctx == NULL) goto yyenomem;
            yyerrctx->save           = yyps->save;
            yyerrctx->state          = yystate;
            yyerrctx->errflag        = yyerrflag;
            yyerrctx->yystack.s_mark = yyerrctx->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (yyerrctx->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yyerrctx->yystack.l_mark = yyerrctx->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (yyerrctx->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yyerrctx->yystack.p_mark = yyerrctx->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (yyerrctx->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yyerrctx->lexeme         = (int) (yylvp - yylvals);
        }
        yylvp          = yylvals   + save->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yylpp          = yylpsns   + save->lexeme;
#endif
        yylexp         = yylexemes + save->lexeme;
        yychar         = YYEMPTY;
        yystack.s_mark = yystack.s_base + (save->yystack.s_mark - save->yystack.s_base);
        memcpy (yystack.s_base, save->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
        yystack.l_mark = yystack.l_base + (save->yystack.l_mark - save->yystack.l_base);
        memcpy (yystack.l_base, save->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yystack.p_mark = yystack.p_base + (save->yystack.p_mark - save->yystack.p_base);
        memcpy (yystack.p_base, save->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
        ctry           = ++save->ctry;
        yystate        = save->state;
        /* We tried shift, try reduce now */
        if ((yyn = yyctable[ctry]) >= 0) goto yyreduce;
        yyps->save     = save->save;
        save->save     = NULL;
        yyFreeState(save);

        /* Nothing left on the stack -- error */
        if (!yyps->save)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%sdebug[%d,trial]: trial parse FAILED, entering ERROR mode\n",
                                YYPREFIX, yydepth);
#endif
            /* Restore state as it was in the most forward-advanced error */
            yylvp          = yylvals   + yyerrctx->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylpp          = yylpsns   + yyerrctx->lexeme;
#endif
            yylexp         = yylexemes + yyerrctx->lexeme;
            yychar         = yylexp[-1];
            yylval         = yylvp[-1];
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc         = yylpp[-1];
#endif
            yystack.s_mark = yystack.s_base + (yyerrctx->yystack.s_mark - yyerrctx->yystack.s_base);
            memcpy (yystack.s_base, yyerrctx->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yystack.l_mark = yystack.l_base + (yyerrctx->yystack.l_mark - yyerrctx->yystack.l_base);
            memcpy (yystack.l_base, yyerrctx->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yystack.p_mark = yystack.p_base + (yyerrctx->yystack.p_mark - yyerrctx->yystack.p_base);
            memcpy (yystack.p_base, yyerrctx->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yystate        = yyerrctx->state;
            yyFreeState(yyerrctx);
            yyerrctx       = NULL;
        }
        yynewerrflag = 1;
    }
    if (yynewerrflag == 0) goto yyinrecovery;
#endif /* YYBTYACC */

    YYERROR_CALL("syntax error");
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yyerror_loc_range[1] = yylloc; /* lookahead position is error start position */
#endif

#if !YYBTYACC
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
#endif
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: state %d, error recovery shifting to state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* lookahead position is error end position */
                yyerror_loc_range[2] = yylloc;
                YYLLOC_DEFAULT(yyloc, yyerror_loc_range, 2); /* position of error span */
                *++yystack.p_mark = yyloc;
#endif
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: error recovery discarding state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* the current TOS position is the error start position */
                yyerror_loc_range[1] = *yystack.p_mark;
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
                if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark, yystack.p_mark);
#else
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
                --yystack.s_mark;
                --yystack.l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                --yystack.p_mark;
#endif
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, error recovery discarding token %d (%s)\n",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
        }
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval, &yylloc);
#else
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
    yym = yylen[yyn];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: state %d, reducing by rule %d (%s)",
                        YYDEBUGSTR, yydepth, yystate, yyn, yyrule[yyn]);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            if (yym > 0)
            {
                int i;
                fputc('<', stderr);
                for (i = yym; i > 0; i--)
                {
                    if (i != yym) fputs(", ", stderr);
                    fputs(YYSTYPE_TOSTRING(yystos[yystack.s_mark[1-i]],
                                           yystack.l_mark[1-i]), stderr);
                }
                fputc('>', stderr);
            }
#endif
        fputc('\n', stderr);
    }
#endif
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)

    /* Perform position reduction */
    memset(&yyloc, 0, sizeof(yyloc));
#if YYBTYACC
    if (!yytrial)
#endif /* YYBTYACC */
    {
        YYLLOC_DEFAULT(yyloc, &yystack.p_mark[-yym], yym);
        /* just in case YYERROR is invoked within the action, save
           the start of the rhs as the error start position */
        yyerror_loc_range[1] = yystack.p_mark[1-yym];
    }
#endif

    switch (yyn)
    {
case 1:
#line 121 "pomi.y"
	{ exit(0); }
#line 2119 "y.tab.c"
break;
case 2:
#line 122 "pomi.y"
	{ 
            /* Serious error detected (should be handled by lexer now) */
            YYABORT;
        }
#line 2127 "y.tab.c"
break;
case 3:
#line 129 "pomi.y"
	{ ex(yystack.l_mark[0].nPtr); freeNode(yystack.l_mark[0].nPtr); }
#line 2132 "y.tab.c"
break;
case 5:
#line 134 "pomi.y"
	{ yyval.nPtr = opr(';', 2, NULL, NULL); }
#line 2137 "y.tab.c"
break;
case 6:
#line 135 "pomi.y"
	{ yyval.nPtr = yystack.l_mark[-1].nPtr; }
#line 2142 "y.tab.c"
break;
case 7:
#line 137 "pomi.y"
	{ yyval.nPtr = opr(PRINT, 1, yystack.l_mark[-2].nPtr); }
#line 2147 "y.tab.c"
break;
case 8:
#line 139 "pomi.y"
	{ yyval.nPtr = opr(EQUALS, 2, id(yystack.l_mark[-3].id), yystack.l_mark[-1].nPtr); }
#line 2152 "y.tab.c"
break;
case 9:
#line 141 "pomi.y"
	{ 
                                          /* x += y equivalent to x = x + y*/
                                          yyval.nPtr = opr(EQUALS, 2, id(yystack.l_mark[-3].id), 
                                                opr(PLUS, 2, id(yystack.l_mark[-3].id), yystack.l_mark[-1].nPtr));
                                        }
#line 2161 "y.tab.c"
break;
case 10:
#line 147 "pomi.y"
	{ 
                                          /* x -= y equivalent to x = x - y*/
                                          yyval.nPtr = opr(EQUALS, 2, id(yystack.l_mark[-3].id), 
                                                opr(MINUS, 2, id(yystack.l_mark[-3].id), yystack.l_mark[-1].nPtr));
                                        }
#line 2170 "y.tab.c"
break;
case 11:
#line 153 "pomi.y"
	{ yyval.nPtr = opr(WHILE, 2, yystack.l_mark[-4].nPtr, yystack.l_mark[-2].nPtr); }
#line 2175 "y.tab.c"
break;
case 12:
#line 155 "pomi.y"
	{ yyval.nPtr = opr(IF, 2, yystack.l_mark[-4].nPtr, yystack.l_mark[-2].nPtr); }
#line 2180 "y.tab.c"
break;
case 13:
#line 157 "pomi.y"
	{ yyval.nPtr = opr(IF, 3, yystack.l_mark[-6].nPtr, yystack.l_mark[-4].nPtr, yystack.l_mark[-2].nPtr); }
#line 2185 "y.tab.c"
break;
case 14:
#line 159 "pomi.y"
	{ yyval.nPtr = opr(DEFINE, 2, id(yystack.l_mark[-3].id), yystack.l_mark[-1].nPtr); }
#line 2190 "y.tab.c"
break;
case 15:
#line 160 "pomi.y"
	{ yyval.nPtr = yystack.l_mark[0].nPtr; }
#line 2195 "y.tab.c"
break;
case 16:
#line 161 "pomi.y"
	{ yyval.nPtr = yystack.l_mark[0].nPtr; }
#line 2200 "y.tab.c"
break;
case 17:
#line 163 "pomi.y"
	{ yyval.nPtr = yystack.l_mark[-1].nPtr; }
#line 2205 "y.tab.c"
break;
case 18:
#line 165 "pomi.y"
	{ 
                                          /* Register the function without debug output*/
                                          register_function(yystack.l_mark[-6].id, yystack.l_mark[-4].nPtr, yystack.l_mark[-1].nPtr); 
                                          yyval.nPtr = opr(';', 2, NULL, NULL); /* No-op for function definition*/
                                        }
#line 2214 "y.tab.c"
break;
case 19:
#line 170 "pomi.y"
	{ yyval.nPtr = yystack.l_mark[-1].nPtr; }
#line 2219 "y.tab.c"
break;
case 20:
#line 171 "pomi.y"
	{ yyval.nPtr = yystack.l_mark[0].nPtr; }
#line 2224 "y.tab.c"
break;
case 21:
#line 172 "pomi.y"
	{ 
            /* Instead of continuing, abort parsing*/
            YYABORT;
        }
#line 2232 "y.tab.c"
break;
case 22:
#line 180 "pomi.y"
	{ yyval.nPtr = opr(CREATE, 3, id(yystack.l_mark[-6].id), opr(PLAYER, 0), yystack.l_mark[-2].nPtr); }
#line 2237 "y.tab.c"
break;
case 23:
#line 182 "pomi.y"
	{ yyval.nPtr = opr(CREATE, 3, id(yystack.l_mark[-6].id), opr(ENEMY, 0), yystack.l_mark[-2].nPtr); }
#line 2242 "y.tab.c"
break;
case 24:
#line 184 "pomi.y"
	{ yyval.nPtr = opr(CREATE, 3, id(yystack.l_mark[-6].id), opr(PLATFORM, 0), yystack.l_mark[-2].nPtr); }
#line 2247 "y.tab.c"
break;
case 25:
#line 186 "pomi.y"
	{ yyval.nPtr = opr(CREATE, 3, id(yystack.l_mark[-6].id), opr(ITEM, 0), yystack.l_mark[-2].nPtr); }
#line 2252 "y.tab.c"
break;
case 26:
#line 188 "pomi.y"
	{ yyval.nPtr = opr(CREATE, 3, id(yystack.l_mark[-6].id), opr(GAME, 0), yystack.l_mark[-2].nPtr); }
#line 2257 "y.tab.c"
break;
case 27:
#line 190 "pomi.y"
	{ yyval.nPtr = opr(CREATE, 3, id(yystack.l_mark[-6].id), opr(LEVEL, 0), yystack.l_mark[-2].nPtr); }
#line 2262 "y.tab.c"
break;
case 28:
#line 194 "pomi.y"
	{ yyval.nPtr = yystack.l_mark[0].nPtr; }
#line 2267 "y.tab.c"
break;
case 29:
#line 195 "pomi.y"
	{ yyval.nPtr = opr(';', 2, yystack.l_mark[-1].nPtr, yystack.l_mark[0].nPtr); }
#line 2272 "y.tab.c"
break;
case 30:
#line 200 "pomi.y"
	{ yyval.nPtr = opr(EQUALS, 2, id(yystack.l_mark[-3].id), yystack.l_mark[-1].nPtr); }
#line 2277 "y.tab.c"
break;
case 31:
#line 204 "pomi.y"
	{ yyval.nPtr = yystack.l_mark[0].nPtr; }
#line 2282 "y.tab.c"
break;
case 32:
#line 205 "pomi.y"
	{ yyval.nPtr = yystack.l_mark[0].nPtr; }
#line 2287 "y.tab.c"
break;
case 33:
#line 210 "pomi.y"
	{ yyval.nPtr = opr('[', 1, yystack.l_mark[-1].nPtr); }
#line 2292 "y.tab.c"
break;
case 34:
#line 214 "pomi.y"
	{ yyval.nPtr = opr(DOT, 2, id(yystack.l_mark[-2].id), id(yystack.l_mark[0].id)); }
#line 2297 "y.tab.c"
break;
case 35:
#line 215 "pomi.y"
	{ yyval.nPtr = opr(DOT, 2, yystack.l_mark[-2].nPtr, id(yystack.l_mark[0].id)); }
#line 2302 "y.tab.c"
break;
case 36:
#line 220 "pomi.y"
	{ yyval.nPtr = opr(COLLISION, 3, id(yystack.l_mark[-6].id), id(yystack.l_mark[-4].id), yystack.l_mark[-1].nPtr); }
#line 2307 "y.tab.c"
break;
case 37:
#line 224 "pomi.y"
	{ yyval.nPtr = NULL; }
#line 2312 "y.tab.c"
break;
case 38:
#line 225 "pomi.y"
	{ yyval.nPtr = id(yystack.l_mark[0].id); }
#line 2317 "y.tab.c"
break;
case 39:
#line 227 "pomi.y"
	{ yyval.nPtr = opr(COMMA, 2, id(yystack.l_mark[-2].id), yystack.l_mark[0].nPtr); }
#line 2322 "y.tab.c"
break;
case 40:
#line 232 "pomi.y"
	{ yyval.nPtr = opr(FUNCTION, 2, id(yystack.l_mark[-3].id), yystack.l_mark[-1].nPtr); }
#line 2327 "y.tab.c"
break;
case 41:
#line 236 "pomi.y"
	{ yyval.nPtr = NULL; }
#line 2332 "y.tab.c"
break;
case 42:
#line 237 "pomi.y"
	{ yyval.nPtr = yystack.l_mark[0].nPtr; }
#line 2337 "y.tab.c"
break;
case 43:
#line 238 "pomi.y"
	{ yyval.nPtr = opr(COMMA, 2, yystack.l_mark[-2].nPtr, yystack.l_mark[0].nPtr); }
#line 2342 "y.tab.c"
break;
case 44:
#line 242 "pomi.y"
	{ yyval.nPtr = opr(RETURN, 1, yystack.l_mark[-1].nPtr); }
#line 2347 "y.tab.c"
break;
case 45:
#line 246 "pomi.y"
	{ yyval.nPtr = yystack.l_mark[0].nPtr; }
#line 2352 "y.tab.c"
break;
case 46:
#line 247 "pomi.y"
	{ yyval.nPtr = opr(';', 2, yystack.l_mark[-1].nPtr, yystack.l_mark[0].nPtr); }
#line 2357 "y.tab.c"
break;
case 47:
#line 251 "pomi.y"
	{ yyval.nPtr = con(yystack.l_mark[0].integer); }
#line 2362 "y.tab.c"
break;
case 48:
#line 252 "pomi.y"
	{ yyval.nPtr = flt(yystack.l_mark[0].floatval); }
#line 2367 "y.tab.c"
break;
case 49:
#line 253 "pomi.y"
	{ yyval.nPtr = str(yystack.l_mark[0].strval); }
#line 2372 "y.tab.c"
break;
case 50:
#line 254 "pomi.y"
	{ yyval.nPtr = id(yystack.l_mark[0].id); }
#line 2377 "y.tab.c"
break;
case 51:
#line 255 "pomi.y"
	{ yyval.nPtr = boolean(true); }
#line 2382 "y.tab.c"
break;
case 52:
#line 256 "pomi.y"
	{ yyval.nPtr = boolean(false); }
#line 2387 "y.tab.c"
break;
case 53:
#line 257 "pomi.y"
	{ yyval.nPtr = yystack.l_mark[0].nPtr; }
#line 2392 "y.tab.c"
break;
case 54:
#line 259 "pomi.y"
	{ yyval.nPtr = opr(UMINUS, 1, yystack.l_mark[0].nPtr); }
#line 2397 "y.tab.c"
break;
case 55:
#line 260 "pomi.y"
	{ yyval.nPtr = opr(NOT, 1, yystack.l_mark[0].nPtr); }
#line 2402 "y.tab.c"
break;
case 56:
#line 261 "pomi.y"
	{ yyval.nPtr = opr(PLUS, 2, yystack.l_mark[-2].nPtr, yystack.l_mark[0].nPtr); }
#line 2407 "y.tab.c"
break;
case 57:
#line 262 "pomi.y"
	{ yyval.nPtr = opr(MINUS, 2, yystack.l_mark[-2].nPtr, yystack.l_mark[0].nPtr); }
#line 2412 "y.tab.c"
break;
case 58:
#line 263 "pomi.y"
	{ yyval.nPtr = opr(MULTIPLY, 2, yystack.l_mark[-2].nPtr, yystack.l_mark[0].nPtr); }
#line 2417 "y.tab.c"
break;
case 59:
#line 264 "pomi.y"
	{ yyval.nPtr = opr(DIVIDE, 2, yystack.l_mark[-2].nPtr, yystack.l_mark[0].nPtr); }
#line 2422 "y.tab.c"
break;
case 60:
#line 265 "pomi.y"
	{ yyval.nPtr = opr(AND, 2, yystack.l_mark[-2].nPtr, yystack.l_mark[0].nPtr); }
#line 2427 "y.tab.c"
break;
case 61:
#line 266 "pomi.y"
	{ yyval.nPtr = opr(OR, 2, yystack.l_mark[-2].nPtr, yystack.l_mark[0].nPtr); }
#line 2432 "y.tab.c"
break;
case 62:
#line 267 "pomi.y"
	{ yyval.nPtr = opr(LESS_THAN, 2, yystack.l_mark[-2].nPtr, yystack.l_mark[0].nPtr); }
#line 2437 "y.tab.c"
break;
case 63:
#line 268 "pomi.y"
	{ yyval.nPtr = opr(GREATER_THAN, 2, yystack.l_mark[-2].nPtr, yystack.l_mark[0].nPtr); }
#line 2442 "y.tab.c"
break;
case 64:
#line 269 "pomi.y"
	{ yyval.nPtr = opr(LESS_THAN_EQUALS, 2, yystack.l_mark[-2].nPtr, yystack.l_mark[0].nPtr); }
#line 2447 "y.tab.c"
break;
case 65:
#line 270 "pomi.y"
	{ yyval.nPtr = opr(GREATER_THAN_EQUALS, 2, yystack.l_mark[-2].nPtr, yystack.l_mark[0].nPtr); }
#line 2452 "y.tab.c"
break;
case 66:
#line 271 "pomi.y"
	{ yyval.nPtr = opr(NOT_EQUALS, 2, yystack.l_mark[-2].nPtr, yystack.l_mark[0].nPtr); }
#line 2457 "y.tab.c"
break;
case 67:
#line 272 "pomi.y"
	{ yyval.nPtr = opr(EQUALS_EQUALS, 2, yystack.l_mark[-2].nPtr, yystack.l_mark[0].nPtr); }
#line 2462 "y.tab.c"
break;
case 68:
#line 273 "pomi.y"
	{ yyval.nPtr = yystack.l_mark[-1].nPtr; }
#line 2467 "y.tab.c"
break;
case 69:
#line 274 "pomi.y"
	{ yyval.nPtr = yystack.l_mark[0].nPtr; }
#line 2472 "y.tab.c"
break;
#line 2474 "y.tab.c"
    default:
        break;
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
        {
            fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[YYFINAL], yyval));
#endif
            fprintf(stderr, "shifting from state 0 to final state %d\n", YYFINAL);
        }
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yyloc;
#endif
        if (yychar < 0)
        {
#if YYBTYACC
            do {
            if (yylvp < yylve)
            {
                /* we're currently re-reading tokens */
                yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylloc = *yylpp++;
#endif
                yychar = *yylexp++;
                break;
            }
            if (yyps->save)
            {
                /* in trial mode; save scanner results for future parse attempts */
                if (yylvp == yylvlim)
                {   /* Enlarge lexical value queue */
                    size_t p = (size_t) (yylvp - yylvals);
                    size_t s = (size_t) (yylvlim - yylvals);

                    s += YYLVQUEUEGROWTH;
                    if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL)
                        goto yyenomem;
                    if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL)
                        goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL)
                        goto yyenomem;
#endif
                    yylvp   = yylve = yylvals + p;
                    yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp   = yylpe = yylpsns + p;
                    yylplim = yylpsns + s;
#endif
                    yylexp  = yylexemes + p;
                }
                *yylexp = (YYINT) YYLEX;
                *yylvp++ = yylval;
                yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                *yylpp++ = yylloc;
                yylpe++;
#endif
                yychar = *yylexp++;
                break;
            }
            /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
            yychar = YYLEX;
#if YYBTYACC
            } while (0);
#endif /* YYBTYACC */
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)\n",
                                YYDEBUGSTR, yydepth, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[yystate], yyval));
#endif
        fprintf(stderr, "shifting from state %d to state %d\n", *yystack.s_mark, yystate);
    }
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    *++yystack.p_mark = yyloc;
#endif
    goto yyloop;
#if YYBTYACC

    /* Reduction declares that this path is valid. Set yypath and do a full parse */
yyvalid:
    if (yypath) YYABORT;
    while (yyps->save)
    {
        YYParseState *save = yyps->save;
        yyps->save = save->save;
        save->save = yypath;
        yypath = save;
    }
#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%s[%d]: state %d, CONFLICT trial successful, backtracking to state %d, %d tokens\n",
                        YYDEBUGSTR, yydepth, yystate, yypath->state, (int)(yylvp - yylvals - yypath->lexeme));
#endif
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    yylvp          = yylvals + yypath->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yylpp          = yylpsns + yypath->lexeme;
#endif
    yylexp         = yylexemes + yypath->lexeme;
    yychar         = YYEMPTY;
    yystack.s_mark = yystack.s_base + (yypath->yystack.s_mark - yypath->yystack.s_base);
    memcpy (yystack.s_base, yypath->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
    yystack.l_mark = yystack.l_base + (yypath->yystack.l_mark - yypath->yystack.l_base);
    memcpy (yystack.l_base, yypath->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base + (yypath->yystack.p_mark - yypath->yystack.p_base);
    memcpy (yystack.p_base, yypath->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
    yystate        = yypath->state;
    goto yyloop;
#endif /* YYBTYACC */

yyoverflow:
    YYERROR_CALL("yacc stack overflow");
#if YYBTYACC
    goto yyabort_nomem;
yyenomem:
    YYERROR_CALL("memory exhausted");
yyabort_nomem:
#endif /* YYBTYACC */
    yyresult = 2;
    goto yyreturn;

yyabort:
    yyresult = 1;
    goto yyreturn;

yyaccept:
#if YYBTYACC
    if (yyps->save) goto yyvalid;
#endif /* YYBTYACC */
    yyresult = 0;

yyreturn:
#if defined(YYDESTRUCT_CALL)
    if (yychar != YYEOF && yychar != YYEMPTY)
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval, &yylloc);
#else
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */

    {
        YYSTYPE *pv;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYLTYPE *pp;

        for (pv = yystack.l_base, pp = yystack.p_base; pv <= yystack.l_mark; ++pv, ++pp)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv, pp);
#else
        for (pv = yystack.l_base; pv <= yystack.l_mark; ++pv)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
    }
#endif /* defined(YYDESTRUCT_CALL) */

#if YYBTYACC
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    while (yyps)
    {
        YYParseState *save = yyps;
        yyps = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
    while (yypath)
    {
        YYParseState *save = yypath;
        yypath = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
#endif /* YYBTYACC */
    yyfreestack(&yystack);
    return (yyresult);
}
