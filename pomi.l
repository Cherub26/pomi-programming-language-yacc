%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "y.tab.h"  /* Include the YACC-generated header */

int line_num = 1;
%}

%%
[ \t\r]+         ;  /* Ignore whitespace */
\n               { line_num++; }  /* Count line numbers */
"//".*           ;  /* Ignore single-line comments */
"/*"([^*]|\*+[^*/])*\*+"/"  ;  /* Ignore multi-line comments */

 /* Keywords from BNF */
"CREATE"         { return KEYWORD_CREATE; }
"AS"             { return KEYWORD_AS; }
"PLAYER"         { return KEYWORD_PLAYER; }
"ENEMY"          { return KEYWORD_ENEMY; }
"PLATFORM"       { return KEYWORD_PLATFORM; }
"ITEM"           { return KEYWORD_ITEM; }
"GAME"           { return KEYWORD_GAME; }    // Added based on object_type
"LEVEL"          { return KEYWORD_LEVEL; }   // Added based on object_type
"DEFINE"         { return KEYWORD_DEFINE; }
"FUNCTION"       { return KEYWORD_FUNCTION; }
"RETURN"         { return KEYWORD_RETURN; }
"IF"             { return KEYWORD_IF; }
"ELSE"           { return KEYWORD_ELSE; }
"THEN"           { return KEYWORD_THEN; }
"ENDIF"          { return KEYWORD_ENDIF; }
"WHILE"          { return KEYWORD_WHILE; }
"DO"             { return KEYWORD_DO; }
"ENDWHILE"       { return KEYWORD_ENDWHILE; }
"ON"             { return KEYWORD_ON; }
"COLLISION"      { return KEYWORD_COLLISION; }
"PRINT"          { return KEYWORD_PRINT; }
"INPUT"          { return KEYWORD_INPUT; }
"START"          { return KEYWORD_START; }   // Added based on start_block

 /* Boolean literals from BNF */
"true"           { yylval.strval = strdup(yytext); return BOOLEAN_TRUE; } // Return as string for now
"false"          { yylval.strval = strdup(yytext); return BOOLEAN_FALSE; } // Return as string for now

 /* Operators from BNF */
"="              { return EQUALS; }
"=="             { return EQUALS_EQUALS; }
"!="             { return NOT_EQUALS; }
"<"              { return LESS_THAN; }
">"              { return GREATER_THAN; }
"<="             { return LESS_THAN_EQUALS; }
">="             { return GREATER_THAN_EQUALS; }
"+"              { return PLUS; }
"-"              { return MINUS; }
"*"              { return MULTIPLY; }
"/"              { return DIVIDE; }
"++"             { return INCREMENT; }
"--"             { return DECREMENT; }
"&&"             { return LOGICAL_AND; }
"||"             { return LOGICAL_OR; }
"!"              { return LOGICAL_NOT; }
"+="             { return PLUS_EQUALS; }
"-="             { return MINUS_EQUALS; }
"*="             { return MULTIPLY_EQUALS; }
"/="             { return DIVIDE_EQUALS; }

 /* Literals from BNF */
[0-9]+           { yylval.intval = atoi(yytext); return INTEGER; }
[0-9]+\.[0-9]+   { yylval.floatval = atof(yytext); return FLOAT; }
\"[^\"]*\"       { yylval.strval = strdup(yytext + 1); yylval.strval[strlen(yylval.strval)-1] = '\0'; return STRING; } /* Remove quotes */

 /* Identifiers from BNF */
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.strval = strdup(yytext); return IDENTIFIER; }

 /* Punctuation from BNF */
";"              { return SEMICOLON; }
"{"              { return LEFT_BRACE; }
"}"              { return RIGHT_BRACE; }
"("              { return LEFT_PAR; }
")"              { return RIGHT_PAR; }
"["              { return LEFT_BRACKET; }
"]"              { return RIGHT_BRACKET; }
","              { return COMMA; }
"."              { return DOT; }

 /* Error handling */
.                { fprintf(stderr, "Unrecognized character: %s at line %d\n", yytext, line_num); }

%%

int yywrap() {
    return 1;
}