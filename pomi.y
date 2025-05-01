%{
#include <stdbool.h>  /* This MUST be first */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declarations
void yyerror(const char *s);
int yylex();
extern int line_num; // Use line number from lexer

// Symbol table for variables and functions
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_BOOLEAN,
    TYPE_FUNCTION,
    TYPE_OBJECT
} ValueType;

typedef struct {
    char* name;
    ValueType type;
    union {
        int intval;
        float floatval;
        char* strval;
        bool boolval;
        struct {
            char** params;
            int param_count;
            struct ASTNode* body;
            struct ASTNode* return_stmt;
        } func;
        // We'll add object support later
    } value;
} Symbol;

#define SYMBOL_TABLE_SIZE 1000
Symbol* symbol_table[SYMBOL_TABLE_SIZE];
int symbol_count = 0;

// AST node structure for storing the program
typedef struct ASTNode {
    enum {
        NODE_INT,
        NODE_FLOAT,
        NODE_STRING,
        NODE_BOOL,
        NODE_IDENTIFIER,
        NODE_BINARY_OP,
        NODE_UNARY_OP,
        NODE_ASSIGNMENT,
        NODE_CALL,
        NODE_IF,
        NODE_WHILE,
        NODE_PRINT,
        NODE_BLOCK,
        NODE_FUNCTION,
        NODE_RETURN
    } type;
    union {
        int intval;
        float floatval;
        char* strval;
        bool boolval;
        struct {
            struct ASTNode* left;
            struct ASTNode* right;
            int op; // Token type representing operator
        } binary;
        struct {
            struct ASTNode* expr;
            int op; // Token type representing operator
        } unary;
        struct {
            char* name;
            struct ASTNode* expr;
        } assignment;
        struct {
            char* func_name;
            struct ASTNode** args;
            int arg_count;
        } call;
        struct {
            struct ASTNode* condition;
            struct ASTNode* then_block;
            struct ASTNode* else_block;
        } if_stmt;
        struct {
            struct ASTNode* condition;
            struct ASTNode* body;
        } while_stmt;
        struct {
            struct ASTNode* expr;
        } print_stmt;
        struct {
            struct ASTNode** statements;
            int stmt_count;
        } block;
        struct {
            char* name;
            char** params;
            int param_count;
            struct ASTNode* body;
            struct ASTNode* return_stmt;
        } function;
        struct {
            struct ASTNode* expr;
        } return_stmt;
    } data;
} ASTNode;

// Helper functions
ASTNode* create_int_node(int value);
ASTNode* create_float_node(float value);
ASTNode* create_string_node(char* value);
ASTNode* create_bool_node(bool value);
ASTNode* create_id_node(char* name);
ASTNode* create_binary_node(int op, ASTNode* left, ASTNode* right);
ASTNode* create_unary_node(int op, ASTNode* expr);
ASTNode* create_assignment_node(char* name, ASTNode* expr);
ASTNode* create_call_node(char* name, ASTNode** args, int arg_count);
ASTNode* create_if_node(ASTNode* condition, ASTNode* then_block, ASTNode* else_block);
ASTNode* create_while_node(ASTNode* condition, ASTNode* body);
ASTNode* create_print_node(ASTNode* expr);
ASTNode* create_block_node(ASTNode** statements, int stmt_count);
ASTNode* create_function_node(char* name, char** params, int param_count, ASTNode* body, ASTNode* return_stmt);
ASTNode* create_return_node(ASTNode* expr);

// Symbol table functions
Symbol* define_symbol(char* name, ValueType type);
Symbol* lookup_symbol(char* name);

// Evaluation functions
typedef struct {
    ValueType type;
    union {
        int intval;
        float floatval;
        char* strval;
        bool boolval;
        Symbol* func;
    } value;
} EvalResult;

EvalResult eval_expression(ASTNode* node);
bool eval_condition(ASTNode* node);
EvalResult eval_assignment(ASTNode* node);
EvalResult eval_call(ASTNode* node);
void eval_if(ASTNode* node);
void eval_while(ASTNode* node);
void eval_print(ASTNode* node);
void eval_block(ASTNode* node);
EvalResult eval_node(ASTNode* node);
void register_function(ASTNode* node);

// Current program root node
ASTNode* program_root = NULL;

// Statement accumulation
ASTNode** current_statements = NULL;
int current_stmt_count = 0;
int current_stmt_capacity = 0;

// Function to add a statement to the current block
void add_statement(ASTNode* stmt);
%}

// Define the types of values associated with tokens and non-terminals
%union {
    int intval;
    float floatval;
    char* strval;
    bool boolval;
    struct ASTNode* node;
    struct {
        struct ASTNode** list;
        int count;
    } node_list;
    struct {
        char** list;
        int count;
    } str_list;
}

// Declare tokens returned by the lexer
%token KEYWORD_CREATE KEYWORD_AS KEYWORD_PLAYER KEYWORD_ENEMY KEYWORD_PLATFORM KEYWORD_ITEM KEYWORD_GAME KEYWORD_LEVEL
%token KEYWORD_DEFINE KEYWORD_FUNCTION KEYWORD_RETURN KEYWORD_IF KEYWORD_ELSE
%token KEYWORD_THEN KEYWORD_ENDIF KEYWORD_WHILE KEYWORD_DO KEYWORD_ENDWHILE
%token KEYWORD_ON KEYWORD_COLLISION KEYWORD_PRINT KEYWORD_INPUT KEYWORD_START

// Literals and Identifier tokens with associated types from %union
%token <intval> INTEGER
%token <floatval> FLOAT
%token <strval> STRING IDENTIFIER BOOLEAN_TRUE BOOLEAN_FALSE

// Operator and Punctuation tokens
%token EQUALS EQUALS_EQUALS NOT_EQUALS LESS_THAN GREATER_THAN LESS_THAN_EQUALS GREATER_THAN_EQUALS
%token PLUS MINUS MULTIPLY DIVIDE INCREMENT DECREMENT LOGICAL_AND LOGICAL_OR LOGICAL_NOT
%token PLUS_EQUALS MINUS_EQUALS MULTIPLY_EQUALS DIVIDE_EQUALS
%token SEMICOLON LEFT_BRACE RIGHT_BRACE LEFT_PAR RIGHT_PAR LEFT_BRACKET RIGHT_BRACKET COMMA DOT

// Define operator precedence and associativity
%left LOGICAL_OR
%left LOGICAL_AND
%right LOGICAL_NOT
%nonassoc LESS_THAN GREATER_THAN LESS_THAN_EQUALS GREATER_THAN_EQUALS EQUALS_EQUALS NOT_EQUALS 
%left PLUS MINUS
%left MULTIPLY DIVIDE
%right UMINUS
%left DOT

// Define the start symbol of the grammar
%start program

// Define types for non-terminals
%type <node> program statement definition_statement object_creation
%type <node> assignment if_statement while_statement function_definition
%type <node> return_statement function_call_statement collision_handler 
%type <node> print_statement input_statement start_block
%type <node> expression term factor condition property
%type <node> array_literal
%type <node_list> statement_list property_list argument_list
%type <str_list> parameter_list
%type <strval> object_type member_access literal boolean_literal
%type <intval> comparison_operator

%%

// --- Grammar Rules based on BNF ---

program:
    statement_list
    {
        // Create a block node with all statements
        program_root = create_block_node($1.list, $1.count);
        printf("Parse successful!\n"); 
        
        // Execute the program
        eval_block(program_root);
    }
;

statement_list:
    /* empty */
    {
        $$.list = NULL;
        $$.count = 0;
    }
    | statement_list statement 
    {
        // Add statement to the list
        $$.list = realloc($1.list, sizeof(ASTNode*) * ($1.count + 1));
        $$.list[$1.count] = $2;
        $$.count = $1.count + 1;
    }
;

statement:
    definition_statement { $$ = $1; }
    | object_creation { $$ = $1; }
    | assignment { $$ = $1; }
    | if_statement { $$ = $1; }
    | while_statement { $$ = $1; }
    | function_definition { $$ = $1; }
    | function_call_statement { $$ = $1; }
    | collision_handler { $$ = $1; }
    | print_statement { $$ = $1; }
    | input_statement { $$ = $1; }
    | start_block { $$ = $1; }
    | return_statement { $$ = $1; }  // Make sure return_statement is included
    | error SEMICOLON { yyerrok; $$ = NULL; } // Basic error recovery
;

definition_statement:
    KEYWORD_DEFINE IDENTIFIER EQUALS expression SEMICOLON
    {
        $$ = create_assignment_node($2, $4);
        printf("Parsed definition: %s\n", $2);
    }
;

object_creation:
    KEYWORD_CREATE IDENTIFIER KEYWORD_AS object_type LEFT_BRACE property_list RIGHT_BRACE SEMICOLON
    {
        // Create object creation node (simplified for now)
        $$ = NULL; // Replace with actual object creation
        printf("Parsed object creation: %s AS %s\n", $2, $4);
        free($2); free($4);
    }
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
    /* empty */ 
    {
        $$.list = NULL;
        $$.count = 0;
    }
    | property_list property
    {
        if ($2 != NULL) {
            $$.list = realloc($1.list, sizeof(ASTNode*) * ($1.count + 1));
            $$.list[$1.count] = $2;
            $$.count = $1.count + 1;
        } else {
            $$ = $1;
        }
    }
;

property:
    IDENTIFIER EQUALS expression SEMICOLON
    {
        $$ = create_assignment_node($1, $3);
        printf("  Parsed property: %s\n", $1);
    }
;

assignment:
    IDENTIFIER EQUALS expression SEMICOLON
    {
        $$ = create_assignment_node($1, $3);
        printf("Parsed assignment to: %s\n", $1);
    }
    | IDENTIFIER INCREMENT SEMICOLON
    {
        // x++ is equivalent to x = x + 1
        ASTNode* id_node = create_id_node($1);
        ASTNode* one_node = create_int_node(1);
        ASTNode* add_node = create_binary_node(PLUS, id_node, one_node);
        $$ = create_assignment_node($1, add_node);
        printf("Parsed increment: %s\n", $1);
    }
    | IDENTIFIER DECREMENT SEMICOLON
    {
        // x-- is equivalent to x = x - 1
        ASTNode* id_node = create_id_node($1);
        ASTNode* one_node = create_int_node(1);
        ASTNode* sub_node = create_binary_node(MINUS, id_node, one_node);
        $$ = create_assignment_node($1, sub_node);
        printf("Parsed decrement: %s\n", $1);
    }
    | IDENTIFIER PLUS_EQUALS expression SEMICOLON
    {
        // x += y is equivalent to x = x + y
        ASTNode* id_node = create_id_node($1);
        ASTNode* add_node = create_binary_node(PLUS, id_node, $3);
        $$ = create_assignment_node($1, add_node);
        printf("Parsed plus-equals: %s\n", $1);
    }
    | IDENTIFIER MINUS_EQUALS expression SEMICOLON
    {
        // x -= y is equivalent to x = x - y
        ASTNode* id_node = create_id_node($1);
        ASTNode* sub_node = create_binary_node(MINUS, id_node, $3);
        $$ = create_assignment_node($1, sub_node);
        printf("Parsed minus-equals: %s\n", $1);
    }
;

if_statement:
    KEYWORD_IF condition KEYWORD_THEN statement_list KEYWORD_ENDIF SEMICOLON
    {
        $$ = create_if_node($2, create_block_node($4.list, $4.count), NULL);
    }
    | KEYWORD_IF condition KEYWORD_THEN statement_list KEYWORD_ENDIF
    {
        $$ = create_if_node($2, create_block_node($4.list, $4.count), NULL);
    }
    | KEYWORD_IF condition KEYWORD_THEN statement_list KEYWORD_ELSE statement_list KEYWORD_ENDIF SEMICOLON
    {
        $$ = create_if_node($2, create_block_node($4.list, $4.count), 
                             create_block_node($6.list, $6.count));
    }
;

while_statement:
    KEYWORD_WHILE condition KEYWORD_DO statement_list KEYWORD_ENDWHILE SEMICOLON
    {
        $$ = create_while_node($2, create_block_node($4.list, $4.count));
    }
    | KEYWORD_WHILE condition KEYWORD_DO statement_list KEYWORD_ENDWHILE
    {
        $$ = create_while_node($2, create_block_node($4.list, $4.count));
    }
;

function_definition:
    KEYWORD_FUNCTION IDENTIFIER LEFT_PAR parameter_list RIGHT_PAR LEFT_BRACE statement_list return_statement RIGHT_BRACE
    {
        $$ = create_function_node($2, $4.list, $4.count, 
                                 create_block_node($7.list, $7.count), $8);
        register_function($$);
    }
    | KEYWORD_FUNCTION IDENTIFIER LEFT_PAR parameter_list RIGHT_PAR LEFT_BRACE statement_list RIGHT_BRACE
    {
        $$ = create_function_node($2, $4.list, $4.count, 
                                 create_block_node($7.list, $7.count), NULL);
        register_function($$);
    }
;

parameter_list:
    /* empty */
    {
        $$.list = NULL;
        $$.count = 0;
    }
    | IDENTIFIER
    {
        $$.list = malloc(sizeof(char*));
        $$.list[0] = $1;
        $$.count = 1;
    }
    | IDENTIFIER COMMA parameter_list
    {
        $$.list = realloc($3.list, sizeof(char*) * ($3.count + 1));
        memmove($$.list + 1, $3.list, sizeof(char*) * $3.count);
        $$.list[0] = $1;
        $$.count = $3.count + 1;
    }
;

return_statement:
    KEYWORD_RETURN expression SEMICOLON
    {
        $$ = create_return_node($2);
    }
;

function_call_statement:
    IDENTIFIER LEFT_PAR argument_list RIGHT_PAR SEMICOLON
    {
        $$ = create_call_node($1, $3.list, $3.count);
    }
;

argument_list:
    /* empty */
    {
        $$.list = NULL;
        $$.count = 0;
    }
    | expression
    {
        $$.list = malloc(sizeof(ASTNode*));
        $$.list[0] = $1;
        $$.count = 1;
    }
    | expression COMMA argument_list
    {
        $$.list = realloc($3.list, sizeof(ASTNode*) * ($3.count + 1));
        memmove($$.list + 1, $3.list, sizeof(ASTNode*) * $3.count);
        $$.list[0] = $1;
        $$.count = $3.count + 1;
    }
;

collision_handler:
    KEYWORD_ON KEYWORD_COLLISION LEFT_PAR IDENTIFIER COMMA IDENTIFIER RIGHT_PAR LEFT_BRACE statement_list RIGHT_BRACE SEMICOLON
    {
        $$ = NULL; // Placeholder - will implement collision logic later
        printf("Parsed collision handler for: %s, %s\n", $4, $6);
        free($4); free($6);
    }
;

print_statement:
    KEYWORD_PRINT LEFT_PAR expression RIGHT_PAR SEMICOLON
    {
        $$ = create_print_node($3);
    }
;

input_statement:
    KEYWORD_INPUT LEFT_PAR IDENTIFIER RIGHT_PAR SEMICOLON
    {
        $$ = NULL; // Placeholder for input functionality
        printf("Parsed input statement for: %s\n", $3);
        free($3);
    }
;

condition:
    expression { $$ = $1; }
    | expression comparison_operator expression
    {
        $$ = create_binary_node($2, $1, $3);
    }
    | condition LOGICAL_AND condition
    {
        $$ = create_binary_node(LOGICAL_AND, $1, $3);
    }
    | condition LOGICAL_OR condition
    {
        $$ = create_binary_node(LOGICAL_OR, $1, $3);
    }
    | LOGICAL_NOT condition
    {
        $$ = create_unary_node(LOGICAL_NOT, $2);
    }
;

comparison_operator:
    EQUALS_EQUALS { $$ = EQUALS_EQUALS; }
    | NOT_EQUALS { $$ = NOT_EQUALS; }
    | LESS_THAN { $$ = LESS_THAN; }
    | GREATER_THAN { $$ = GREATER_THAN; }
    | LESS_THAN_EQUALS { $$ = LESS_THAN_EQUALS; }
    | GREATER_THAN_EQUALS { $$ = GREATER_THAN_EQUALS; }
;

expression:
    term { $$ = $1; }
    | expression PLUS term  { $$ = create_binary_node(PLUS, $1, $3); }
    | expression MINUS term { $$ = create_binary_node(MINUS, $1, $3); }
;

term:
    factor { $$ = $1; }
    | term MULTIPLY factor { $$ = create_binary_node(MULTIPLY, $1, $3); }
    | term DIVIDE factor   { $$ = create_binary_node(DIVIDE, $1, $3); }
;

factor:
    IDENTIFIER { $$ = create_id_node($1); }
    | INTEGER { $$ = create_int_node($1); }
    | FLOAT { $$ = create_float_node($1); }
    | STRING { $$ = create_string_node($1); }
    | BOOLEAN_TRUE { $$ = create_bool_node(true); }
    | BOOLEAN_FALSE { $$ = create_bool_node(false); }
    | array_literal { $$ = $1; }
    | IDENTIFIER LEFT_PAR argument_list RIGHT_PAR
    {
        $$ = create_call_node($1, $3.list, $3.count);
    }
    | LEFT_PAR expression RIGHT_PAR { $$ = $2; }
    | MINUS factor %prec UMINUS { $$ = create_unary_node(UMINUS, $2); }
;

member_access:
    IDENTIFIER DOT IDENTIFIER
    {
        char* member = malloc(strlen($1) + strlen($3) + 2);
        sprintf(member, "%s.%s", $1, $3);
        $$ = member;
        free($1); free($3);
    }
;

array_literal:
    LEFT_BRACKET argument_list RIGHT_BRACKET
    {
        $$ = NULL; // For now, just return placeholder
        printf("Parsed array literal\n");
    }
;

literal:
    INTEGER 
    { 
        char* str = malloc(20); 
        sprintf(str, "%d", $1); 
        $$ = str;
    }
    | FLOAT           
    { 
        char* str = malloc(30); 
        sprintf(str, "%f", $1); 
        $$ = str; 
    }
    | STRING { $$ = $1; }
    | boolean_literal { $$ = $1; }
;

boolean_literal:
    BOOLEAN_TRUE    { $$ = $1; }
    | BOOLEAN_FALSE   { $$ = $1; }
;

start_block:
    KEYWORD_START LEFT_BRACE statement_list RIGHT_BRACE
    {
        $$ = create_block_node($3.list, $3.count);
        printf("Parsed START block\n");
    }
;

%%

// Error reporting function
void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", line_num, s);
}

// Create AST node helper functions
ASTNode* create_int_node(int value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_INT;
    node->data.intval = value;
    return node;
}

ASTNode* create_float_node(float value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_FLOAT;
    node->data.floatval = value;
    return node;
}

ASTNode* create_string_node(char* value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_STRING;
    node->data.strval = value;
    return node;
}

ASTNode* create_bool_node(bool value) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_BOOL;
    node->data.boolval = value;
    return node;
}

ASTNode* create_id_node(char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_IDENTIFIER;
    node->data.strval = name;
    return node;
}

ASTNode* create_binary_node(int op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_BINARY_OP;
    node->data.binary.op = op;
    node->data.binary.left = left;
    node->data.binary.right = right;
    return node;
}

ASTNode* create_unary_node(int op, ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_UNARY_OP;
    node->data.unary.op = op;
    node->data.unary.expr = expr;
    return node;
}

ASTNode* create_assignment_node(char* name, ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ASSIGNMENT;
    node->data.assignment.name = name;
    node->data.assignment.expr = expr;
    return node;
}

ASTNode* create_call_node(char* name, ASTNode** args, int arg_count) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_CALL;
    node->data.call.func_name = name;
    node->data.call.args = args;
    node->data.call.arg_count = arg_count;
    return node;
}

ASTNode* create_if_node(ASTNode* condition, ASTNode* then_block, ASTNode* else_block) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_IF;
    node->data.if_stmt.condition = condition;
    node->data.if_stmt.then_block = then_block;
    node->data.if_stmt.else_block = else_block;
    return node;
}

ASTNode* create_while_node(ASTNode* condition, ASTNode* body) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_WHILE;
    node->data.while_stmt.condition = condition;
    node->data.while_stmt.body = body;
    return node;
}

ASTNode* create_print_node(ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_PRINT;
    node->data.print_stmt.expr = expr;
    return node;
}

ASTNode* create_block_node(ASTNode** statements, int stmt_count) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_BLOCK;
    node->data.block.statements = statements;
    node->data.block.stmt_count = stmt_count;
    return node;
}

ASTNode* create_function_node(char* name, char** params, int param_count, ASTNode* body, ASTNode* return_stmt) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_FUNCTION;
    node->data.function.name = name;
    node->data.function.params = params;
    node->data.function.param_count = param_count;
    node->data.function.body = body;
    node->data.function.return_stmt = return_stmt;
    return node;
}

ASTNode* create_return_node(ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_RETURN;
    node->data.return_stmt.expr = expr;
    return node;
}

// Symbol table functions
Symbol* define_symbol(char* name, ValueType type) {
    // Check if symbol already exists
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i]->name, name) == 0) {
            // Update existing symbol
            symbol_table[i]->type = type;
            return symbol_table[i];
        }
    }
    
    // Create new symbol
    Symbol* symbol = malloc(sizeof(Symbol));
    symbol->name = strdup(name);
    symbol->type = type;
    symbol_table[symbol_count++] = symbol;
    return symbol;
}

Symbol* lookup_symbol(char* name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i]->name, name) == 0) {
            return symbol_table[i];
        }
    }
    return NULL;
}

// Evaluation functions
EvalResult eval_expression(ASTNode* node) {
    EvalResult result;
    
    if (node == NULL) {
        result.type = TYPE_INT;
        result.value.intval = 0;
        return result;
    }
    
    switch (node->type) {
        case NODE_INT:
            result.type = TYPE_INT;
            result.value.intval = node->data.intval;
            break;
        case NODE_FLOAT:
            result.type = TYPE_FLOAT;
            result.value.floatval = node->data.floatval;
            break;
        case NODE_STRING:
            result.type = TYPE_STRING;
            result.value.strval = strdup(node->data.strval);
            break;
        case NODE_BOOL:
            result.type = TYPE_BOOLEAN;
            result.value.boolval = node->data.boolval;
            break;
        case NODE_IDENTIFIER: {
            Symbol* sym = lookup_symbol(node->data.strval);
            if (sym == NULL) {
                fprintf(stderr, "Error: Undefined variable '%s'\n", node->data.strval);
                exit(1);
            }
            result.type = sym->type;
            switch (sym->type) {
                case TYPE_INT:
                    result.value.intval = sym->value.intval;
                    break;
                case TYPE_FLOAT:
                    result.value.floatval = sym->value.floatval;
                    break;
                case TYPE_STRING:
                    result.value.strval = strdup(sym->value.strval);
                    break;
                case TYPE_BOOLEAN:
                    result.value.boolval = sym->value.boolval;
                    break;
                default:
                    fprintf(stderr, "Error: Variable type not supported in expressions\n");
                    exit(1);
            }
            break;
        }
        case NODE_BINARY_OP: {
            EvalResult left = eval_expression(node->data.binary.left);
            EvalResult right = eval_expression(node->data.binary.right);
            
            // Handle operations based on types
            if (left.type == TYPE_INT && right.type == TYPE_INT) {
                result.type = TYPE_INT;
                switch (node->data.binary.op) {
                    case PLUS:
                        result.value.intval = left.value.intval + right.value.intval;
                        break;
                    case MINUS:
                        result.value.intval = left.value.intval - right.value.intval;
                        break;
                    case MULTIPLY:
                        result.value.intval = left.value.intval * right.value.intval;
                        break;
                    case DIVIDE:
                        if (right.value.intval == 0) {
                            fprintf(stderr, "Error: Division by zero\n");
                            exit(1);
                        }
                        result.value.intval = left.value.intval / right.value.intval;
                        break;
                    case EQUALS_EQUALS:
                        result.type = TYPE_BOOLEAN;
                        result.value.boolval = left.value.intval == right.value.intval;
                        break;
                    case NOT_EQUALS:
                        result.type = TYPE_BOOLEAN;
                        result.value.boolval = left.value.intval != right.value.intval;
                        break;
                    case LESS_THAN:
                        result.type = TYPE_BOOLEAN;
                        result.value.boolval = left.value.intval < right.value.intval;
                        break;
                    case GREATER_THAN:
                        result.type = TYPE_BOOLEAN;
                        result.value.boolval = left.value.intval > right.value.intval;
                        break;
                    case LESS_THAN_EQUALS:
                        result.type = TYPE_BOOLEAN;
                        result.value.boolval = left.value.intval <= right.value.intval;
                        break;
                    case GREATER_THAN_EQUALS:
                        result.type = TYPE_BOOLEAN;
                        result.value.boolval = left.value.intval >= right.value.intval;
                        break;
                    default:
                        fprintf(stderr, "Error: Unsupported binary operation\n");
                        exit(1);
                }
            }
            // Add support for other type combinations as needed
            break;
        }
        case NODE_CALL:
            result = eval_call(node);
            break;
        default:
            fprintf(stderr, "Error: Unsupported expression type\n");
            exit(1);
    }
    
    return result;
}

bool eval_condition(ASTNode* node) {
    EvalResult result = eval_expression(node);
    
    switch (result.type) {
        case TYPE_BOOLEAN:
            return result.value.boolval;
        case TYPE_INT:
            return result.value.intval != 0;
        case TYPE_FLOAT:
            return result.value.floatval != 0.0;
        case TYPE_STRING:
            return result.value.strval != NULL && strlen(result.value.strval) > 0;
        default:
            return false;
    }
}

EvalResult eval_assignment(ASTNode* node) {
    EvalResult expr_result = eval_expression(node->data.assignment.expr);
    Symbol* sym = lookup_symbol(node->data.assignment.name);
    
    if (sym == NULL) {
        sym = define_symbol(node->data.assignment.name, expr_result.type);
    } else {
        sym->type = expr_result.type;
    }
    
    switch (expr_result.type) {
        case TYPE_INT:
            sym->value.intval = expr_result.value.intval;
            break;
        case TYPE_FLOAT:
            sym->value.floatval = expr_result.value.floatval;
            break;
        case TYPE_STRING:
            if (sym->value.strval != NULL) {
                free(sym->value.strval);
            }
            sym->value.strval = strdup(expr_result.value.strval);
            break;
        case TYPE_BOOLEAN:
            sym->value.boolval = expr_result.value.boolval;
            break;
        default:
            fprintf(stderr, "Error: Unsupported type in assignment\n");
            exit(1);
    }
    
    return expr_result;
}

void register_function(ASTNode* node) {
    Symbol* sym = define_symbol(node->data.function.name, TYPE_FUNCTION);
    sym->value.func.params = node->data.function.params;
    sym->value.func.param_count = node->data.function.param_count;
    sym->value.func.body = node->data.function.body;
    sym->value.func.return_stmt = node->data.function.return_stmt;
}

EvalResult eval_call(ASTNode* node) {
    EvalResult result;
    Symbol* func = lookup_symbol(node->data.call.func_name);
    
    if (func == NULL || func->type != TYPE_FUNCTION) {
        fprintf(stderr, "Error: Undefined function '%s'\n", node->data.call.func_name);
        exit(1);
    }
    
    // Save current symbol table state
    Symbol* saved_symbols[SYMBOL_TABLE_SIZE];
    int saved_count = symbol_count;
    memcpy(saved_symbols, symbol_table, sizeof(Symbol*) * symbol_count);
    
    // Create parameters in new scope
    if (node->data.call.arg_count != func->value.func.param_count) {
        fprintf(stderr, "Error: Function '%s' called with wrong number of arguments\n", 
                node->data.call.func_name);
        exit(1);
    }
    
    // Evaluate arguments and assign to parameters
    for (int i = 0; i < node->data.call.arg_count; i++) {
        EvalResult arg = eval_expression(node->data.call.args[i]);
        Symbol* param = define_symbol(func->value.func.params[i], arg.type);
        
        switch (arg.type) {
            case TYPE_INT:
                param->value.intval = arg.value.intval;
                break;
            case TYPE_FLOAT:
                param->value.floatval = arg.value.floatval;
                break;
            case TYPE_STRING:
                param->value.strval = strdup(arg.value.strval);
                break;
            case TYPE_BOOLEAN:
                param->value.boolval = arg.value.boolval;
                break;
            default:
                fprintf(stderr, "Error: Unsupported argument type\n");
                exit(1);
        }
    }
    
    // Execute function body
    eval_block(func->value.func.body);
    
    // Handle return value if any
    if (func->value.func.return_stmt != NULL) {
        result = eval_expression(func->value.func.return_stmt->data.return_stmt.expr);
    } else {
        result.type = TYPE_INT;
        result.value.intval = 0;
    }
    
    // Restore previous symbol table state
    for (int i = saved_count; i < symbol_count; i++) {
        free(symbol_table[i]->name);
        if (symbol_table[i]->type == TYPE_STRING) {
            free(symbol_table[i]->value.strval);
        }
        free(symbol_table[i]);
    }
    
    symbol_count = saved_count;
    memcpy(symbol_table, saved_symbols, sizeof(Symbol*) * saved_count);
    
    return result;
}

void eval_if(ASTNode* node) {
    if (eval_condition(node->data.if_stmt.condition)) {
        eval_block(node->data.if_stmt.then_block);
    } else if (node->data.if_stmt.else_block != NULL) {
        eval_block(node->data.if_stmt.else_block);
    }
}

void eval_while(ASTNode* node) {
    while (eval_condition(node->data.while_stmt.condition)) {
        eval_block(node->data.while_stmt.body);
    }
}

void eval_print(ASTNode* node) {
    EvalResult result = eval_expression(node->data.print_stmt.expr);
    
    switch (result.type) {
        case TYPE_INT:
            printf("%d\n", result.value.intval);
            break;
        case TYPE_FLOAT:
            printf("%f\n", result.value.floatval);
            break;
        case TYPE_STRING:
            printf("%s\n", result.value.strval);
            break;
        case TYPE_BOOLEAN:
            printf("%s\n", result.value.boolval ? "true" : "false");
            break;
        default:
            printf("(unprintable value)\n");
    }
}

void eval_block(ASTNode* node) {
    for (int i = 0; i < node->data.block.stmt_count; i++) {
        eval_node(node->data.block.statements[i]);
    }
}

EvalResult eval_node(ASTNode* node) {
    EvalResult result;
    result.type = TYPE_INT;
    result.value.intval = 0;
    
    if (node == NULL) return result;
    
    switch (node->type) {
        case NODE_BLOCK:
            eval_block(node);
            break;
        case NODE_ASSIGNMENT:
            result = eval_assignment(node);
            break;
        case NODE_IF:
            eval_if(node);
            break;
        case NODE_WHILE:
            eval_while(node);
            break;
        case NODE_PRINT:
            eval_print(node);
            break;
        case NODE_CALL:
            result = eval_call(node);
            break;
        case NODE_FUNCTION:
            register_function(node);
            break;
        default:
            result = eval_expression(node);
    }
    
    return result;
}

// Main function
int main() {
    printf("POMI Language Interpreter - Enter code (Ctrl+D to end):\n");
    if (yyparse() == 0) {
        // Success already printed in 'program' rule action
    } else {
        fprintf(stderr, "Parsing failed.\n");
    }
    return 0;
}