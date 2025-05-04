#ifndef _yy_defines_h_
#define _yy_defines_h_

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
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE {
    int integer;   /* Integer values*/
    float floatval; /* Float values*/
    int id;        /* Identifier indices*/
    char* strval;  /* String values*/
    bool boolval;  /* Boolean values*/
    nodeType *nPtr; /* Node pointers*/
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;

#endif /* _yy_defines_h_ */
