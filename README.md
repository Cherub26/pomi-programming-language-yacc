# Programming Language Pomi
Group Members: Batuhan Yılmaz, Emre Kemal Aksel, Muhammed Kaan Uman

## POMI Language: A Platform Game Development Language
### Introduction
POMI is a domain-specific programming language designed for platform game development. It provides an intuitive syntax with built-in support for common game development patterns, making it accessible for programmers who want to quickly prototype or build 2D games.

### Language Design Goals
1. Simplicity: Easy to learn with straightforward syntax    
2. Type flexibility: Support for integers, floats, strings, and booleans   
3. Error handling: Built-in exception management   
4. Statement-by-statement execution: Programs are executed line-by-line, aiding in debugging and making control flow easy to follow
   
### Syntax Highlights

#### Data Types
POMI supports various data types:

	DEFINE score = 100;             // Integer
	DEFINE speed = 5.5;             // Float
	DEFINE message = "Game Over";   // String
	DEFINE isJumping = true;        // Boolean

#### Control Structures
POMI supports standard control structures with game-specific keywords:

	IF player.position.x > 500 THEN
	    PRINT("Player reached checkpoint!");
	ENDIF;
	
	WHILE enemy.health > 0 DO
	    enemy.position.x += enemy.direction * enemy.speed;
	ENDWHILE;

POMI supports nested IF statements with ELSE clauses:

	IF score >= 90 THEN
		PRINT("Grade: A");
	ELSE
		IF score >= 80 THEN
			PRINT("Grade: B");
		ELSE 
			PRINT("Grade: C");
		ENDIF;
	ENDIF;

#### Assignment Operations
POMI supports multiple assignment operators:

	x = 10;         // Standard assignment
	x += 5;         // Add and assign (x = x + 5)
	x -= 3;         // Subtract and assign (x = x - 3)
	x *= 2;         // Multiply and assign (x = x * 2)
	x /= 4;         // Divide and assign (x = x / 4)

#### Functions
Functions are defined with the FUNCTION keyword:

	FUNCTION calculateDamage(attackPower, defense) {
	    RETURN attackPower * 2 - defense;
	}

Functions can be called and their return values used in expressions:

	DEFINE damage = calculateDamage(10, 5);
	PRINT(damage);  // Outputs: 15

#### Boolean Operations
POMI supports logical operations for game state management:

	DEFINE canJump = true;
	DEFINE onGround = true;
	DEFINE shouldJump = canJump && onGround;  // Logical AND
	DEFINE isMoving = playerSpeed > 0 || enemyNearby;  // Logical OR
	DEFINE isStationary = !isMoving;  // Logical NOT

#### Exception Handling
POMI provides try-catch error handling:

	TRY
		DEFINE result = dangerousOperation();
		PRINT("Operation succeeded");
	CATCH errorCode
		PRINT("Operation failed");
	ENDTRY;

### Running POMI Programs
You can run your program by using the provided makefile:

	make
	./pomi < your_program.pomi

### How to test POMI
To use the provided test files with your POMI language parser, follow these steps:
1. Compile the POMI Parser
Use the make command to compile the parser:

		make

2. Run a Test File
Execute the compiled parser with a test file as input:

		./pomi < Test/<test_file>.pomi
	
	Replace <test_file> with the name of the test file you want to run, such as function_test, loop_test, or trycatch_test.
3. Available Test Files
	Here are the test files included in the `Test` directory:
	`function_test.pomi`: Tests function definitions, calls, and nested calls.
	`loop_test.pomi`: Demonstrates WHILE loops and nested loops.
	`trycatch_test.pomi`: Tests exception handling with TRY, CATCH, and THROW.
	`boolean_test.pomi`: Tests boolean operations and conditions.
	`if_test.pomi`: Demonstrates IF, ELSE, and nested conditional statements.
	`floatingpoint_test.pomi`: Tests floating-point operations and comparisons.

## POMI Language BNF Grammar

		/* Program structure */
	<program> ::= <statement_list>

	<statement_list> ::= <statement> | <statement> <statement_list>

	/* Statements */
	<statement> ::= <definition_statement>
				| <assignment_statement>
				| <conditional_statement>
				| <loop_statement>
				| <function_statement>
				| <return_statement>
				| <print_statement>
				| <try_catch_statement>
				| <throw_statement>
				| <expression_statement>

	/* Definition statement */
	<definition_statement> ::= DEFINE IDENTIFIER EQUALS <expression> SEMICOLON

	/* Assignment statements */
	<assignment_statement> ::= <lvalue> EQUALS <expression> SEMICOLON
							| <lvalue> PLUS_EQUALS <expression> SEMICOLON
							| <lvalue> MINUS_EQUALS <expression> SEMICOLON
							| <lvalue> MULTIPLY_EQUALS <expression> SEMICOLON
							| <lvalue> DIVIDE_EQUALS <expression> SEMICOLON

	<lvalue> ::= IDENTIFIER
			| <array_access>
			| <object_access>

	<array_access> ::= <lvalue> LEFT_BRACKET <expression> RIGHT_BRACKET

	<object_access> ::= <lvalue> DOT IDENTIFIER

	/* Conditional statement */
	<conditional_statement> ::= IF <expression> THEN <statement_list> ENDIF
							| IF <expression> THEN <statement_list> ELSE <statement_list> ENDIF

	/* Loop statement */
	<loop_statement> ::= WHILE <expression> DO <statement_list> ENDWHILE

	/* Function definition and call */
	<function_statement> ::= FUNCTION IDENTIFIER LEFT_PAREN <parameter_list> RIGHT_PAREN LEFT_BRACE <statement_list> RIGHT_BRACE

	<parameter_list> ::= ε
					| IDENTIFIER
					| IDENTIFIER COMMA <parameter_list>

	<function_call> ::= IDENTIFIER LEFT_PAREN <argument_list> RIGHT_PAREN

	<argument_list> ::= ε
					| <expression>
					| <expression> COMMA <argument_list>

	<return_statement> ::= RETURN <expression> SEMICOLON

	/* Print statement */
	<print_statement> ::= PRINT <expression> SEMICOLON

	/* Exception handling */
	<try_catch_statement> ::= TRY <statement_list> CATCH IDENTIFIER <statement_list> ENDTRY

	<throw_statement> ::= THROW <expression> SEMICOLON

	/* Expression statement (expression followed by semicolon) */
	<expression_statement> ::= <expression> SEMICOLON

	/* Expressions */
	<expression> ::= <logical_expression>

	<logical_expression> ::= <comparison_expression>
						| <logical_expression> AND <comparison_expression>
						| <logical_expression> OR <comparison_expression>
						| NOT <logical_expression>

	<comparison_expression> ::= <additive_expression>
							| <comparison_expression> LESS_THAN <additive_expression>
							| <comparison_expression> GREATER_THAN <additive_expression>
							| <comparison_expression> LESS_THAN_EQUALS <additive_expression>
							| <comparison_expression> GREATER_THAN_EQUALS <additive_expression>
							| <comparison_expression> EQUALS_EQUALS <additive_expression>
							| <comparison_expression> NOT_EQUALS <additive_expression>

	<additive_expression> ::= <multiplicative_expression>
							| <additive_expression> PLUS <multiplicative_expression>
							| <additive_expression> MINUS <multiplicative_expression>

	<multiplicative_expression> ::= <primary_expression>
								| <multiplicative_expression> MULTIPLY <primary_expression>
								| <multiplicative_expression> DIVIDE <primary_expression>

	<primary_expression> ::= IDENTIFIER
						| INTEGER
						| FLOAT
						| STRING
						| TRUE
						| FALSE
						| LEFT_PAREN <expression> RIGHT_PAREN
						| <function_call>
						



