%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pomi.h"
#include "y.tab.h"

int line_num = 1;

void yyerror(char *);
%}

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
","             { return COMMA; }

\"[^\"]*\"      { 
                  yylval.strval = strdup(yytext+1); // Skip opening quote
                  yylval.strval[strlen(yylval.strval)-1] = '\0'; // Remove closing quote
                  return STRING; 
                }

[a-zA-Z][a-zA-Z0-9_]*  {
                         yylval.id = *yytext - 'a'; /* Simple mapping for now */
                         return IDENTIFIER;
                       }

[0-9]+          { yylval.integer = atoi(yytext); return INTEGER; }

[ \t]           ;  /* ignore whitespace */

\n              { line_num++; }

"//".*          ;  /* ignore single-line comments */

"/*"([^*]|"*"[^/])*"*/" ;  /* ignore multi-line comments */

.               { yyerror("Unknown character"); }

%%

int yywrap(void) {
    return 1;
}