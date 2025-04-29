%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations
void yyerror(const char *s);
int yylex();
extern int line_num; // Use line number from lexer

// You might need data structures here later for symbol tables, AST nodes, etc.
%}

// Define the types of values associated with tokens and non-terminals
%union {
    int intval;
    float floatval;
    char* strval;
    // Add pointers to AST nodes or other structures as needed
}

// Declare tokens returned by the lexer
%token KEYWORD_CREATE KEYWORD_AS KEYWORD_PLAYER KEYWORD_ENEMY KEYWORD_PLATFORM KEYWORD_ITEM KEYWORD_GAME KEYWORD_LEVEL
%token KEYWORD_DEFINE KEYWORD_FUNCTION KEYWORD_RETURN KEYWORD_IF KEYWORD_ELSE
%token KEYWORD_THEN KEYWORD_ENDIF KEYWORD_WHILE KEYWORD_DO KEYWORD_ENDWHILE
%token KEYWORD_ON KEYWORD_COLLISION KEYWORD_PRINT KEYWORD_INPUT KEYWORD_START

// Literals and Identifier tokens with associated types from %union
%token <intval> INTEGER
%token <floatval> FLOAT
%token <strval> STRING IDENTIFIER BOOLEAN_TRUE BOOLEAN_FALSE // Treat booleans as strings for now

// Operator and Punctuation tokens
%token EQUALS EQUALS_EQUALS NOT_EQUALS LESS_THAN GREATER_THAN LESS_THAN_EQUALS GREATER_THAN_EQUALS
%token PLUS MINUS MULTIPLY DIVIDE INCREMENT DECREMENT LOGICAL_AND LOGICAL_OR LOGICAL_NOT
%token PLUS_EQUALS MINUS_EQUALS MULTIPLY_EQUALS DIVIDE_EQUALS
%token SEMICOLON LEFT_BRACE RIGHT_BRACE LEFT_PAR RIGHT_PAR LEFT_BRACKET RIGHT_BRACKET COMMA DOT

// Define operator precedence and associativity (adjust as needed based on language semantics)
%left LOGICAL_OR
%left LOGICAL_AND
%right LOGICAL_NOT
%nonassoc LESS_THAN GREATER_THAN LESS_THAN_EQUALS GREATER_THAN_EQUALS EQUALS_EQUALS NOT_EQUALS // Non-associative comparison
%left PLUS MINUS
%left MULTIPLY DIVIDE
%right UMINUS // For unary minus, if you add it
%left DOT // Member access

// Define the start symbol of the grammar
%start program

// Define types for non-terminals that might have values (using placeholders for now)
%type <strval> object_type literal boolean_literal member_access
%type <intval> expression term factor // Placeholder types, adjust when building AST

%%

// --- Grammar Rules based on BNF ---

program:
    statement_list
    { printf("Parse successful!\n"); } // Simple success message
;

statement_list:
    /* empty */
    | statement_list statement // Changed to left recursion
;

statement:
    definition_statement
    | object_creation
    | assignment
    | if_statement
    | while_statement
    | function_definition
    | function_call_statement
    | collision_handler
    | print_statement
    | input_statement
    | start_block
    | error SEMICOLON { yyerrok; } // Basic error recovery on semicolon
;

definition_statement:
    KEYWORD_DEFINE IDENTIFIER EQUALS expression SEMICOLON
    { printf("Parsed definition: %s\n", $2); free($2); }
;

object_creation:
    KEYWORD_CREATE IDENTIFIER KEYWORD_AS object_type LEFT_BRACE property_list RIGHT_BRACE SEMICOLON
    { printf("Parsed object creation: %s AS %s\n", $2, $4); free($2); free($4); }
    | KEYWORD_CREATE IDENTIFIER KEYWORD_AS object_type LEFT_BRACE property_list RIGHT_BRACE
    { printf("Parsed object creation: %s AS %s\n", $2, $4); free($2); free($4); }
;

object_type:
    KEYWORD_PLAYER   { $$ = strdup("PLAYER"); }
    | KEYWORD_ENEMY    { $$ = strdup("ENEMY"); }
    | KEYWORD_PLATFORM { $$ = strdup("PLATFORM"); }
    | KEYWORD_ITEM     { $$ = strdup("ITEM"); }
    | KEYWORD_GAME     { $$ = strdup("GAME"); }
    | KEYWORD_LEVEL    { $$ = strdup("LEVEL"); }
;

property_list:
    /* empty */ // Allow empty property list
    | property_list property // Correct left recursion
;

property:
    IDENTIFIER EQUALS expression SEMICOLON
    { printf("  Parsed property: %s\n", $1); free($1); }
;

assignment:
    member_access EQUALS expression SEMICOLON
    { printf("Parsed member assignment to: %s\n", $1); free($1); }
    | IDENTIFIER EQUALS expression SEMICOLON
    { printf("Parsed assignment to: %s\n", $1); free($1); }
    | IDENTIFIER INCREMENT SEMICOLON
    { printf("Parsed increment: %s\n", $1); free($1); }
    | IDENTIFIER DECREMENT SEMICOLON
    { printf("Parsed decrement: %s\n", $1); free($1); }
    // Compound assignments for IDENTIFIER
    | IDENTIFIER PLUS_EQUALS expression SEMICOLON
    { printf("Parsed plus-equals: %s\n", $1); free($1); }
    | IDENTIFIER MINUS_EQUALS expression SEMICOLON
    { printf("Parsed minus-equals: %s\n", $1); free($1); }
    | IDENTIFIER MULTIPLY_EQUALS expression SEMICOLON
    { printf("Parsed multiply-equals: %s\n", $1); free($1); }
    | IDENTIFIER DIVIDE_EQUALS expression SEMICOLON
    { printf("Parsed divide-equals: %s\n", $1); free($1); }
    // Compound assignments for member_access (Added)
    | member_access PLUS_EQUALS expression SEMICOLON
    { printf("Parsed member plus-equals: %s\n", $1); free($1); }
    | member_access MINUS_EQUALS expression SEMICOLON
    { printf("Parsed member minus-equals: %s\n", $1); free($1); }
    | member_access MULTIPLY_EQUALS expression SEMICOLON
    { printf("Parsed member multiply-equals: %s\n", $1); free($1); }
    | member_access DIVIDE_EQUALS expression SEMICOLON
    { printf("Parsed member divide-equals: %s\n", $1); free($1); }
;

if_statement:
    KEYWORD_IF condition KEYWORD_THEN statement_list KEYWORD_ENDIF SEMICOLON
    | KEYWORD_IF condition KEYWORD_THEN statement_list KEYWORD_ENDIF
    | KEYWORD_IF condition KEYWORD_THEN statement_list KEYWORD_ELSE statement_list KEYWORD_ENDIF SEMICOLON
    | KEYWORD_IF condition KEYWORD_THEN statement_list KEYWORD_ELSE statement_list KEYWORD_ENDIF
;

while_statement:
    KEYWORD_WHILE condition KEYWORD_DO statement_list KEYWORD_ENDWHILE SEMICOLON
    | KEYWORD_WHILE condition KEYWORD_DO statement_list KEYWORD_ENDWHILE
;

function_definition:
    KEYWORD_FUNCTION IDENTIFIER LEFT_PAR parameter_list RIGHT_PAR LEFT_BRACE statement_list return_statement RIGHT_BRACE SEMICOLON
    { printf("Parsed function definition with return: %s\n", $2); free($2); }
    | KEYWORD_FUNCTION IDENTIFIER LEFT_PAR parameter_list RIGHT_PAR LEFT_BRACE statement_list RIGHT_BRACE SEMICOLON
    { printf("Parsed function definition without return: %s\n", $2); free($2); }
    | KEYWORD_FUNCTION IDENTIFIER LEFT_PAR parameter_list RIGHT_PAR LEFT_BRACE statement_list return_statement RIGHT_BRACE
    { printf("Parsed function definition with return: %s\n", $2); free($2); }
    | KEYWORD_FUNCTION IDENTIFIER LEFT_PAR parameter_list RIGHT_PAR LEFT_BRACE statement_list RIGHT_BRACE
    { printf("Parsed function definition without return: %s\n", $2); free($2); }
;

parameter_list:
    /* empty */
    | IDENTIFIER
    { free($1); } // Free identifier string
    | IDENTIFIER COMMA parameter_list
    { free($1); } // Free identifier string
;

return_statement:
    KEYWORD_RETURN expression SEMICOLON
;

function_call_statement:
    // Allow calls on identifiers or members as standalone statements
    IDENTIFIER LEFT_PAR argument_list RIGHT_PAR SEMICOLON
    { printf("Parsed function call statement: %s()\n", $1); free($1); }
    | member_access LEFT_PAR argument_list RIGHT_PAR SEMICOLON
    { printf("Parsed member function call statement: %s()\n", $1); free($1); }
;

// Removed old function_call rule, handled by function_call_statement and factor

argument_list:
    /* empty */
    | expression
    | expression COMMA argument_list
;

collision_handler:
    KEYWORD_ON KEYWORD_COLLISION LEFT_PAR IDENTIFIER COMMA IDENTIFIER RIGHT_PAR LEFT_BRACE statement_list RIGHT_BRACE SEMICOLON
    { printf("Parsed collision handler for: %s, %s\n", $4, $6); free($4); free($6); }
    | KEYWORD_ON KEYWORD_COLLISION LEFT_PAR IDENTIFIER COMMA IDENTIFIER RIGHT_PAR LEFT_BRACE statement_list RIGHT_BRACE
    { printf("Parsed collision handler for: %s, %s\n", $4, $6); free($4); free($6); }
;

print_statement:
    KEYWORD_PRINT LEFT_PAR STRING RIGHT_PAR SEMICOLON
    { printf("Output: %s\n", $3); free($3); }  // Handle string literals
    | KEYWORD_PRINT LEFT_PAR expression RIGHT_PAR SEMICOLON
    { printf("Output: %d\n", $3); }  // Handle numeric expressions
;

input_statement:
    KEYWORD_INPUT LEFT_PAR IDENTIFIER RIGHT_PAR SEMICOLON
    { printf("Parsed input statement for: %s\n", $3); free($3); }
;

condition:
    expression // Simple truthiness
    | expression comparison_operator expression
    | condition LOGICAL_AND condition
    | condition LOGICAL_OR condition
    | LOGICAL_NOT condition
;

comparison_operator:
    EQUALS_EQUALS
    | NOT_EQUALS
    | LESS_THAN
    | GREATER_THAN
    | LESS_THAN_EQUALS
    | GREATER_THAN_EQUALS
;

expression:
    term                { $$ = $1; }
    | expression PLUS term  { $$ = $1 + $3; } // Handles string concatenation and addition
    | expression MINUS term { $$ = $1 - $3; } 
;

term:
    factor              { $$ = $1; }
    | term MULTIPLY factor { $$ = $1 * $3; } // Example action
    | term DIVIDE factor   { if ($3 != 0) $$ = $1 / $3; else { yyerror("Division by zero"); YYERROR; } } // Example action
;

factor:
    member_access       { $$ = 0; free($1); } 
    | IDENTIFIER        { $$ = 0; free($1); } 
    | literal           { $$ = 0; free($1); } 
    | array_literal     { $$ = 0; } 
    // Function calls within expressions
    | IDENTIFIER LEFT_PAR argument_list RIGHT_PAR
      { $$ = 0; printf("Parsed function call factor: %s()\n", $1); free($1); }
    | member_access LEFT_PAR argument_list RIGHT_PAR
      { $$ = 0; printf("Parsed member function call factor: %s()\n", $1); free($1); }
    | LEFT_PAR expression RIGHT_PAR { $$ = $2; }
    | MINUS factor %prec UMINUS { $$ = -$2; } // Uncomment unary minus
;

member_access:
    IDENTIFIER DOT IDENTIFIER
    {
        // Combine identifiers into a single string like "obj.prop" for now
        char* member = malloc(strlen($1) + strlen($3) + 2);
        sprintf(member, "%s.%s", $1, $3);
        $$ = member;
        free($1); free($3);
    }
    | member_access DOT IDENTIFIER
    {
        // Append identifier like "obj.prop.subprop"
        char* member = malloc(strlen($1) + strlen($3) + 2);
        sprintf(member, "%s.%s", $1, $3);
        $$ = member;
        free($1); free($3);
    }
;

array_literal:
    LEFT_BRACKET argument_list RIGHT_BRACKET
    { printf("Parsed array literal\n"); }
;

literal:
    INTEGER         { $$ = malloc(20); sprintf($$, "%d", $1); } // Convert int back to string for placeholder
    | FLOAT           { $$ = malloc(30); sprintf($$, "%f", $1); } // Convert float back to string for placeholder
    | STRING          { $$ = $1; } // Already a string (strdup'd in lexer)
    | boolean_literal { $$ = $1; } // Already a string (strdup'd in lexer)
;

boolean_literal:
    BOOLEAN_TRUE    { $$ = $1; }
    | BOOLEAN_FALSE   { $$ = $1; }
;

start_block:
    KEYWORD_START LEFT_BRACE statement_list RIGHT_BRACE
    { printf("Parsed START block\n"); }
    | KEYWORD_START LEFT_BRACE statement_list RIGHT_BRACE SEMICOLON
    { printf("Parsed START block\n"); }
;

%%

// Error reporting function
void yyerror(const char *s) {
    // Add more context if possible (e.g., expected token)
    fprintf(stderr, "Syntax error at line %d: %s\n", line_num, s);
}

// Main function (optional, can be linked separately)
int main() {
    printf("POMI Language Parser - Enter code (Ctrl+D to end):\n");
    if (yyparse() == 0) {
        // Success already printed in 'program' rule action
    } else {
        fprintf(stderr, "Parsing failed.\n");
    }
    return 0;
}