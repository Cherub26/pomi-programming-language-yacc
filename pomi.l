%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pomi.h"
#include "y.tab.h"

void yyerror(char *);
extern int yylineno; // Use yylineno provided by flex
%}

%option yylineno

%%

"DEFINE"        { return DEFINE; }
"WHILE"         { return WHILE; }
"DO"            { return DO; }
"ENDWHILE"      { return ENDWHILE; }
"IF"            { return IF; }
"THEN"          { return THEN; }
"ELSE"          { return ELSE; }
"ENDIF"         { return ENDIF; }
"FUNCTION"      { return FUNCTION; }
"RETURN"        { return RETURN; }
"PRINT"         { return PRINT; }
"FOR"           { return FOR; }
"CREATE"        { return CREATE; }
"AS"            { return AS; }
"PLAYER"        { return PLAYER; }
"ENEMY"         { return ENEMY; }
"PLATFORM"      { return PLATFORM; }
"ITEM"          { return ITEM; }
"GAME"          { return GAME; }
"LEVEL"         { return LEVEL; }
"ON"            { return ON; }
"COLLISION"     { return COLLISION; }
"true"          { return TRUE; }
"false"         { return FALSE; }
"&&"            { return AND; }
"||"            { return OR; }
"!"             { return NOT; }

"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULTIPLY; }
"/"             { return DIVIDE; }
"<"             { return LESS_THAN; }
">"             { return GREATER_THAN; }
"<="            { return LESS_THAN_EQUALS; }
">="            { return GREATER_THAN_EQUALS; }
"=="            { return EQUALS_EQUALS; }
"!="            { return NOT_EQUALS; }
"="             { return EQUALS; }
"+="            { return PLUS_EQUALS; }
"-="            { return MINUS_EQUALS; }
"*="            { return MULTIPLY_EQUALS; }
"/="            { return DIVIDE_EQUALS; }
";"             { return SEMICOLON; }
"("             { return LEFT_PAREN; }
")"             { return RIGHT_PAREN; }
"{"             { return LEFT_BRACE; }
"}"             { return RIGHT_BRACE; }
"["             { return LEFT_BRACKET; }
"]"             { return RIGHT_BRACKET; }
","             { return COMMA; }
"."             { return DOT; }

\"[^\"]*\"      { 
                  yylval.strval = strdup(yytext+1); // Skip opening quote
                  yylval.strval[strlen(yylval.strval)-1] = '\0'; // Remove closing quote
                  return STRING; 
                }

[a-zA-Z][a-zA-Z0-9_]*  {
                         yylval.id = *yytext - 'a'; /* Simple mapping for now */
                         return IDENTIFIER;
                       }

[0-9]+\.[0-9]+  { 
                  yylval.floatval = atof(yytext); 
                  return FLOAT; 
                }

[0-9]+          { 
                  yylval.integer = atoi(yytext); 
                  return INTEGER; 
                }

[ \t]           ;  /* ignore whitespace */

\n              ;  /* Let flex handle line counting with yylineno */ 

"//".*          ;  /* ignore single-line comments */

"/*"([^*]|"*"[^/])*"*/" ;  /* ignore multi-line comments */

.               { 
                  /* Critical change: Just call yyerror with no return */
                  char message[100];
                  snprintf(message, sizeof(message), "Unknown character: '%s'", yytext);
                  yyerror(message);
                }

%%

int yywrap(void) {
    return 1;
}