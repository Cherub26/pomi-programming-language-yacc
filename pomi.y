%{
#include <stdbool.h>  /* This MUST be first */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "pomi.h"

// Forward declarations
nodeType *opr(int oper, int nops, ...);
nodeType *id(int i);
nodeType *con(int value);
nodeType *str(char *value);
void freeNode(nodeType *p);
int ex(nodeType *p);
int yylex(void);
void yyerror(char *s);
extern int yylineno; // Use line number tracking from Flex
int sym[26]; // Symbol table for variables
char* strSym[26]; // String symbol table

// Function table
typedef struct {
    int id;  // Use the integer ID instead of char
    nodeType *params;
    nodeType *body;
} Function;

#define MAX_FUNCTIONS 100
Function functions[MAX_FUNCTIONS];
int function_count = 0;

// Function to register a function definition
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

// Function to find a function by id
Function* find_function(int func_id) {
    for (int i = 0; i < function_count; i++) {
        if (functions[i].id == func_id) {
            return &functions[i];
        }
    }
    return NULL;
}

// For debugging - must be defined after tokens are declared
void print_param_tree(nodeType *node, int level);

%}

%union {
    int integer;   // Integer values
    int id;        // Identifier indices
    char* strval;  // String values
    nodeType *nPtr; // Node pointers
}

// Define tokens
%token <integer> INTEGER
%token <id> IDENTIFIER
%token <strval> STRING
%token WHILE IF PRINT ELSE
%token FOR RETURN FUNCTION
%token DEFINE DO ENDWHILE ENDIF THEN

// Operators and punctuation
%token PLUS MINUS MULTIPLY DIVIDE 
%token LESS_THAN GREATER_THAN EQUALS_EQUALS NOT_EQUALS
%token LESS_THAN_EQUALS GREATER_THAN_EQUALS
%token EQUALS SEMICOLON COMMA
%token LEFT_PAREN RIGHT_PAREN LEFT_BRACE RIGHT_BRACE
%token PLUS_EQUALS MINUS_EQUALS MULTIPLY_EQUALS DIVIDE_EQUALS

// Precedence and associativity
%nonassoc IFX
%nonassoc ELSE

%left EQUALS_EQUALS NOT_EQUALS
%left LESS_THAN GREATER_THAN LESS_THAN_EQUALS GREATER_THAN_EQUALS
%left PLUS MINUS
%left MULTIPLY DIVIDE
%nonassoc UMINUS

// Define non-terminal types
%type <nPtr> stmt expr stmt_list function_call return_stmt parameter_list argument_list

%%

program:
        function                { exit(0); }
        | error                 { 
            /* Serious error detected (should be handled by lexer now) */
            YYABORT;
        }
        ;

function:
          function stmt         { ex($2); freeNode($2); }
        | /* empty */
        ;

stmt:
          SEMICOLON                       { $$ = opr(';', 2, NULL, NULL); }
        | expr SEMICOLON                 { $$ = $1; }
        | PRINT LEFT_PAREN expr RIGHT_PAREN SEMICOLON     
                                        { $$ = opr(PRINT, 1, $3); }
        | IDENTIFIER EQUALS expr SEMICOLON  
                                        { $$ = opr(EQUALS, 2, id($1), $3); }
        | IDENTIFIER PLUS_EQUALS expr SEMICOLON  
                                        { 
                                          // x += y equivalent to x = x + y
                                          $$ = opr(EQUALS, 2, id($1), 
                                                opr(PLUS, 2, id($1), $3));
                                        }
        | IDENTIFIER MINUS_EQUALS expr SEMICOLON  
                                        { 
                                          // x -= y equivalent to x = x - y
                                          $$ = opr(EQUALS, 2, id($1), 
                                                opr(MINUS, 2, id($1), $3));
                                        }
        | WHILE expr DO stmt_list ENDWHILE SEMICOLON       
                                        { $$ = opr(WHILE, 2, $2, $4); }
        | IF expr THEN stmt_list ENDIF SEMICOLON
                                        { $$ = opr(IF, 2, $2, $4); }
        | IF expr THEN stmt_list ELSE stmt_list ENDIF SEMICOLON
                                        { $$ = opr(IF, 3, $2, $4, $6); }
        | DEFINE IDENTIFIER EQUALS expr SEMICOLON
                                        { $$ = opr(DEFINE, 2, id($2), $4); }
        | LEFT_BRACE stmt_list RIGHT_BRACE            
                                        { $$ = $2; }
        | FUNCTION IDENTIFIER LEFT_PAREN parameter_list RIGHT_PAREN LEFT_BRACE stmt_list RIGHT_BRACE
                                        { 
                                          // Register the function without debug output
                                          register_function($2, $4, $7); 
                                          $$ = opr(';', 2, NULL, NULL); // No-op for function definition
                                        }
        | function_call SEMICOLON       { $$ = $1; }
        | return_stmt                   { $$ = $1; } /* Make return a standalone statement */
        | error SEMICOLON               { 
            // Instead of continuing, abort parsing
            YYABORT;
        }
        ;

parameter_list:
          /* empty */                   { $$ = NULL; }
        | IDENTIFIER                    { $$ = id($1); }
        | IDENTIFIER COMMA parameter_list
                                        { $$ = opr(COMMA, 2, id($1), $3); }
        ;

function_call:
          IDENTIFIER LEFT_PAREN argument_list RIGHT_PAREN
                                        { $$ = opr(FUNCTION, 2, id($1), $3); }
        ;

argument_list:
          /* empty */                   { $$ = NULL; }
        | expr                          { $$ = $1; }
        | expr COMMA argument_list      { $$ = opr(COMMA, 2, $1, $3); }
        ;

return_stmt:
          RETURN expr SEMICOLON         { $$ = opr(RETURN, 1, $2); }
        ;

stmt_list:
          stmt                  { $$ = $1; }
        | stmt_list stmt        { $$ = opr(';', 2, $1, $2); }
        ;

expr:
          INTEGER               { $$ = con($1); }
        | STRING                { $$ = str($1); }
        | IDENTIFIER            { $$ = id($1); }
        | MINUS expr %prec UMINUS       
                                { $$ = opr(UMINUS, 1, $2); }
        | expr PLUS expr        { $$ = opr(PLUS, 2, $1, $3); }
        | expr MINUS expr       { $$ = opr(MINUS, 2, $1, $3); }
        | expr MULTIPLY expr    { $$ = opr(MULTIPLY, 2, $1, $3); }
        | expr DIVIDE expr      { $$ = opr(DIVIDE, 2, $1, $3); }
        | expr LESS_THAN expr   { $$ = opr(LESS_THAN, 2, $1, $3); }
        | expr GREATER_THAN expr { $$ = opr(GREATER_THAN, 2, $1, $3); }
        | expr LESS_THAN_EQUALS expr  { $$ = opr(LESS_THAN_EQUALS, 2, $1, $3); }
        | expr GREATER_THAN_EQUALS expr  { $$ = opr(GREATER_THAN_EQUALS, 2, $1, $3); }
        | expr NOT_EQUALS expr  { $$ = opr(NOT_EQUALS, 2, $1, $3); }
        | expr EQUALS_EQUALS expr  { $$ = opr(EQUALS_EQUALS, 2, $1, $3); }
        | LEFT_PAREN expr RIGHT_PAREN  { $$ = $2; }
        | function_call         { $$ = $1; }
        ;

%%

nodeType *con(int value) {
    nodeType *p;

    if ((p = malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    p->type = typeCon;
    p->con.value = value;

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

int ex(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
    case typeCon:       return p->con.value;
    case typeStr:       // For string literals, print them directly
                        if (p->str.value != NULL) {
                            return 0;  // Strings don't have a numerical value
                        }
                        return 0;
    case typeId:        return sym[p->id.i];
    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:     while(ex(p->opr.op[0])) ex(p->opr.op[1]); return 0;
        case IF: {
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
                        } else {
                            printf("%d\n", ex(p->opr.op[0])); 
                        }
                        return 0;
        case ';':       ex(p->opr.op[0]); return ex(p->opr.op[1]);
        case EQUALS:    return sym[p->opr.op[0]->id.i] = ex(p->opr.op[1]);
        case DEFINE:    return sym[p->opr.op[0]->id.i] = ex(p->opr.op[1]); // Same as EQUALS for now
        case UMINUS:    return -ex(p->opr.op[0]);
        case PLUS:      return ex(p->opr.op[0]) + ex(p->opr.op[1]);
        case MINUS:     return ex(p->opr.op[0]) - ex(p->opr.op[1]);
        case MULTIPLY:  return ex(p->opr.op[0]) * ex(p->opr.op[1]);
        case DIVIDE:    return ex(p->opr.op[0]) / ex(p->opr.op[1]);
        case LESS_THAN: return ex(p->opr.op[0]) < ex(p->opr.op[1]);
        case GREATER_THAN: return ex(p->opr.op[0]) > ex(p->opr.op[1]);
        case GREATER_THAN_EQUALS: return ex(p->opr.op[0]) >= ex(p->opr.op[1]);
        case LESS_THAN_EQUALS: return ex(p->opr.op[0]) <= ex(p->opr.op[1]);
        case NOT_EQUALS: return ex(p->opr.op[0]) != ex(p->opr.op[1]);
        case EQUALS_EQUALS: return ex(p->opr.op[0]) == ex(p->opr.op[1]);
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
            memcpy(saved_sym, sym, sizeof(sym));
            
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