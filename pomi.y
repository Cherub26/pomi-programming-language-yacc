%{
// Include necessary libraries
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "pomi.h"
#include <setjmp.h>

// Forward declarations
nodeType *opr(int oper, int nops, ...);
nodeType *id(int i);
nodeType *con(int value);
nodeType *flt(float value);
nodeType *str(char *value);
nodeType *boolean(bool value);
void freeNode(nodeType *p);
int ex(nodeType *p);
float exf(nodeType *p);
bool isFloatExpression(nodeType *p);
bool isBoolExpression(nodeType *p);
int yylex(void);
void yyerror(char *s);
extern int yylineno; // Tracks line numbers
int sym[26]; // Integer variables
char* strSym[26]; // String variables
float floatSym[26]; // Float variables
bool isFloat[26]; // Tracks if a variable is float
bool isBool[26]; // Tracks if a variable is boolean
bool boolSym[26]; // Boolean variables

// FUNCTION table to store function definitions
typedef struct {
    int id;  // Function identifier
    nodeType *params; // Function parameters
    nodeType *body; // Function body
} Function;

#define MAX_FUNCTIONS 100
Function functions[MAX_FUNCTIONS];
int function_count = 0;

// Add a function to the table
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

// Find a function by its ID
Function* find_function(int func_id) {
    for (int i = 0; i < function_count; i++) {
        if (functions[i].id == func_id) {
            return &functions[i];
        }
    }
    return NULL;
}

// Debugging helper for parameter trees
void print_param_tree(nodeType *node, int level);

typedef struct exception_context {
    jmp_buf env;
    char error_msg[256];
    int active;
    struct exception_context* prev;
} exception_context;

exception_context* current_exception = NULL;

void throw_exception(char* msg) {
    if (current_exception && current_exception->active) {
        strncpy(current_exception->error_msg, msg, sizeof(current_exception->error_msg) - 1);
        current_exception->error_msg[sizeof(current_exception->error_msg) - 1] = '\0';
        
        // Print error but don't exit
        fprintf(stderr, "Exception: %s\n", msg);
        fflush(stderr);
        
        longjmp(current_exception->env, 1);
    } else {
        // No active exception context, treat as fatal error
        fprintf(stderr, "Unhandled exception: %s\n", msg);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }
}

%}

%union {
    int integer;   // Integer values
    float floatval; // Float values
    int id;        // Identifier indices
    char* strval;  // String values
    bool boolval;  // Boolean values
    nodeType *nPtr; // Node pointers
}

// Define tokens
%token <integer> INTEGER
%token <floatval> FLOAT
%token <id> IDENTIFIER
%token <strval> STRING
%token <boolval> TRUE FALSE
%token WHILE IF PRINT ELSE
%token FOR RETURN FUNCTION
%token DEFINE DO ENDWHILE ENDIF THEN
%token CREATE AS PLAYER ENEMY PLATFORM ITEM GAME LEVEL
%token ON COLLISION
%token TRY CATCH ENDTRY
%token THROW

// Operators and punctuation
%token PLUS MINUS MULTIPLY DIVIDE 
%token LESS_THAN GREATER_THAN EQUALS_EQUALS NOT_EQUALS
%token LESS_THAN_EQUALS GREATER_THAN_EQUALS
%token EQUALS SEMICOLON COMMA
%token LEFT_PAREN RIGHT_PAREN LEFT_BRACE RIGHT_BRACE
%token LEFT_BRACKET RIGHT_BRACKET DOT
%token PLUS_EQUALS MINUS_EQUALS MULTIPLY_EQUALS DIVIDE_EQUALS
%token AND OR NOT  // New logical operators

// Precedence and associativity
%nonassoc IFX
%nonassoc ELSE

%left OR
%left AND
%left EQUALS_EQUALS NOT_EQUALS
%left LESS_THAN GREATER_THAN LESS_THAN_EQUALS GREATER_THAN_EQUALS
%left PLUS MINUS
%left MULTIPLY DIVIDE
%right NOT
%nonassoc UMINUS

// Define non-terminal types
%type <nPtr> stmt expr stmt_list function_call return_stmt parameter_list argument_list
%type <nPtr> property_list property property_value array_literal member_access object_creation
%type <nPtr> collision_handler

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
        | object_creation              { $$ = $1; }
        | collision_handler            { $$ = $1; }
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
        | TRY stmt_list CATCH stmt_list ENDTRY SEMICOLON
                                        { $$ = opr(TRY, 2, $2, $4); }
        | THROW expr SEMICOLON          { $$ = opr(THROW, 1, $2); }
        | error SEMICOLON               { 
            // Instead of continuing, abort parsing
            YYABORT;
        }
        ;

object_creation:
        CREATE IDENTIFIER AS PLAYER LEFT_BRACE property_list RIGHT_BRACE SEMICOLON
                                        { $$ = opr(CREATE, 3, id($2), opr(PLAYER, 0), $6); }
        | CREATE IDENTIFIER AS ENEMY LEFT_BRACE property_list RIGHT_BRACE SEMICOLON
                                        { $$ = opr(CREATE, 3, id($2), opr(ENEMY, 0), $6); }
        | CREATE IDENTIFIER AS PLATFORM LEFT_BRACE property_list RIGHT_BRACE SEMICOLON
                                        { $$ = opr(CREATE, 3, id($2), opr(PLATFORM, 0), $6); }
        | CREATE IDENTIFIER AS ITEM LEFT_BRACE property_list RIGHT_BRACE SEMICOLON
                                        { $$ = opr(CREATE, 3, id($2), opr(ITEM, 0), $6); }
        | CREATE IDENTIFIER AS GAME LEFT_BRACE property_list RIGHT_BRACE SEMICOLON
                                        { $$ = opr(CREATE, 3, id($2), opr(GAME, 0), $6); }
        | CREATE IDENTIFIER AS LEVEL LEFT_BRACE property_list RIGHT_BRACE SEMICOLON
                                        { $$ = opr(CREATE, 3, id($2), opr(LEVEL, 0), $6); }
        ;

property_list:
          property                      { $$ = $1; }
        | property property_list        { $$ = opr(';', 2, $1, $2); }
        ;

property:
          IDENTIFIER EQUALS property_value SEMICOLON
                                        { $$ = opr(EQUALS, 2, id($1), $3); }
        ;

property_value:
          expr                          { $$ = $1; }
        | array_literal                 { $$ = $1; }
        ;

array_literal:
          LEFT_BRACKET argument_list RIGHT_BRACKET
                                        { $$ = opr('[', 1, $2); }
        ;

member_access:
          IDENTIFIER DOT IDENTIFIER     { $$ = opr(DOT, 2, id($1), id($3)); }
        | member_access DOT IDENTIFIER  { $$ = opr(DOT, 2, $1, id($3)); }
        ;

collision_handler:
          ON COLLISION LEFT_PAREN IDENTIFIER COMMA IDENTIFIER RIGHT_PAREN LEFT_BRACE stmt_list RIGHT_BRACE
                                        { $$ = opr(COLLISION, 3, id($4), id($6), $9); }
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
          | FLOAT               { $$ = flt($1); }
        | STRING                { $$ = str($1); }
        | IDENTIFIER            { $$ = id($1); }
        | TRUE                  { $$ = boolean(true); }
        | FALSE                 { $$ = boolean(false); }
        | member_access         { $$ = $1; }
        | MINUS expr %prec UMINUS       
                                { $$ = opr(UMINUS, 1, $2); }
        | NOT expr              { $$ = opr(NOT, 1, $2); }
        | expr PLUS expr        { $$ = opr(PLUS, 2, $1, $3); }
        | expr MINUS expr       { $$ = opr(MINUS, 2, $1, $3); }
        | expr MULTIPLY expr    { $$ = opr(MULTIPLY, 2, $1, $3); }
        | expr DIVIDE expr      { $$ = opr(DIVIDE, 2, $1, $3); }
        | expr AND expr         { $$ = opr(AND, 2, $1, $3); }
        | expr OR expr          { $$ = opr(OR, 2, $1, $3); }
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
    
    throw_exception(s);
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
        case DIVIDE: {
            float right = exf(p->opr.op[1]);
            if (right == 0.0f) {
                throw_exception("Division by zero");
                return 0.0f; // Never reached but avoids compiler warning
            }
            return exf(p->opr.op[0]) / right;
        }
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
        case DIVIDE: {
            int right = ex(p->opr.op[1]);
            if (right == 0) {
                throw_exception("Division by zero");
                return 0; // Never reached but avoids compiler warning
            }
            
            if (isFloatExpression(p->opr.op[0]) || isFloatExpression(p->opr.op[1])) {
                float rightf = exf(p->opr.op[1]);
                if (rightf == 0.0f) {
                    throw_exception("Division by zero");
                    return 0; // Never reached but avoids compiler warning
                }
                return (int)(exf(p->opr.op[0]) / rightf);
            }
            
            return ex(p->opr.op[0]) / right;
        }
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
        case TRY: {
            exception_context new_context;
            new_context.active = 1;
            new_context.prev = current_exception;
            current_exception = &new_context;
            
            int result = 0;
            if (setjmp(new_context.env) == 0) {
                // Normal execution path - execute the try block
                result = ex(p->opr.op[0]);
            } else {
                // Exception thrown - execute the catch block
                printf("Caught exception: %s\n", new_context.error_msg);
                result = ex(p->opr.op[1]);
            }
            
            // Restore previous exception context
            current_exception = new_context.prev;
            return result;
        }
        case THROW: {
            int value = ex(p->opr.op[0]);
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Explicit exception thrown with value: %d", value);
            throw_exception(error_msg);
            return 0; // Never reached due to longjmp
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