# Programming Language Pomi
Group Members: Batuhan Yılmaz, Emre Kemal Aksel, Muhammed Kaan Uman

## POMI Language: A Platform Game Development Language
### Introduction
POMI is a programming language designed specifically for creating 2D platformer games. The language offers an intuitive syntax for defining game elements like players, enemies, platforms, and items, along with specifying their behaviors and interactions.

### Language Design Goals

1. Simplicity: Make game development accessible to beginners with minimal programming experience.
2. Domain-Specific: Provide primitives and constructs specifically tailored for platformer game development.
3. Event-Driven: Support event-based programming for game interactions.

### Syntax Highlights
#### Object Creation and Definition
Objects in POMI are created using the CREATE keyword followed by an identifier and an object type:

	CREATE mario AS PLAYER {
	    position = [100, 200];
	    size = [32, 32];
	    speed = 5;
	    jumpPower = 10;
	}

#### Properties
Object properties are defined using name-value pairs within curly braces:
	
	CREATE block AS PLATFORM {
	    position = [300, 400];
	    size = [64, 16];
	    solid = true;
	}

#### Control Structures
POMI supports standard control structures with game-specific keywords:

	IF player.position.x > 500 THEN
	    PRINT("Player reached checkpoint!");
	ENDIF;
	
	WHILE enemy.health > 0 DO
	    enemy.position.x += enemy.direction * enemy.speed;
	ENDWHILE;

#### Event Handlers
Event-based programming is supported through the ON keyword:

	ON COLLISION(player, enemy) {
	    player.health -= 1;
	    IF player.health <= 0 THEN
	        GAME.restart();
	    ENDIF;
	}

#### Variables and Constants
Variables are defined using the DEFINE keyword with the "=" operator:
	
	DEFINE playerSpeed = 5;
	DEFINE gravity = 9.8;

#### Functions
Functions are defined with the FUNCTION keyword:

	FUNCTION calculateDamage(attackPower, defense) {
	    RETURN attackPower * 2 - defense;
	}

You can run your program by running the makefile and giving it to myprog as input:

	make
	./pomi < example.pomi

## POMI Language BNF Grammar

	<program> ::= <statement-list>

	<statement-list> ::= <statement> 
	                   | <statement> <statement-list>

	<statement> ::= <definition-statement>
	              | <object-creation>
	              | <assignment>
	              | <if-statement>
	              | <while-statement>
	              | <function-definition>
	              | <function-call>
	              | <collision-handler>
	              | <print-statement>
	              | <input-statement>
	              | <start-block>           // START block for initialization

	<definition-statement> ::= "DEFINE" <identifier> "=" <expression> ";"

	<object-creation> ::= "CREATE" <identifier> "AS" <object-type> "{" <property-list> "}" ";"

	<object-type> ::= "PLAYER" | "ENEMY" | "PLATFORM" | "ITEM" | "GAME" | "LEVEL"

	<property-list> ::= <property> 
	                  | <property> <property-list>

	<property> ::= <identifier> "=" <expression> ";"
	             | <identifier> "=" <array-literal> ";"

	<assignment> ::= <identifier> "=" <expression> ";"
	               | <member-access> "=" <expression> ";"   // Allow assignment to members
	               | <identifier> "++" ";"
	               | <identifier> "--" ";"
	               | <identifier> "+=" <expression> ";"     // Compound assignment
	               | <identifier> "-=" <expression> ";"     // Compound assignment
	               | <identifier> "*=" <expression> ";"     // Compound assignment
	               | <identifier> "/=" <expression> ";"     // Compound assignment

	<if-statement> ::= "IF" <condition> "THEN" <statement-list> "ENDIF" ";"
	                 | "IF" <condition> "THEN" <statement-list> "ELSE" <statement-list> "ENDIF" ";"

	<while-statement> ::= "WHILE" <condition> "DO" <statement-list> "ENDWHILE" ";"

	<function-definition> ::= "FUNCTION" <identifier> "(" <parameter-list> ")" "{" <statement-list> <return-statement> "}" ";"
	                        | "FUNCTION" <identifier> "(" <parameter-list> ")" "{" <statement-list> "}" ";"

	<parameter-list> ::= ε 
	                   | <identifier> 
	                   | <identifier> "," <parameter-list>

	<return-statement> ::= "RETURN" <expression> ";"

	<function-call> ::= <identifier> "(" <argument-list> ")" ";"

	<argument-list> ::= ε 
	                  | <expression> 
	                  | <expression> "," <argument-list>

	<collision-handler> ::= "ON" "COLLISION" "(" <identifier> "," <identifier> ")" "{" <statement-list> "}" ";"

	<print-statement> ::= "PRINT" "(" <expression> ")" ";"

	<input-statement> ::= "INPUT" "(" <identifier> ")" ";"

	<start-block> ::= "START" "{" <statement-list> "}"    // Initialization block

	<condition> ::= <expression>                          // Allow simple expression (truthiness)
	              | <expression> <comparison-operator> <expression>
	              | <condition> "&&" <condition>
	              | <condition> "||" <condition>
	              | "!" <condition>
	              | "(" <condition> ")"

	<comparison-operator> ::= "==" | "!=" | "<" | ">" | "<=" | ">="

	<expression> ::= <term>
	               | <expression> "+" <term>
	               | <expression> "-" <term>

	<term> ::= <factor>
	         | <term> "*" <factor>
	         | <term> "/" <factor>

	<factor> ::= <identifier>
	           | <member-access>        // Object property access
	           | <literal>
	           | <array-literal>        // Array literal
	           | <function-call>        // Function call within an expression
	           | "(" <expression> ")"
               | "-" <factor>           // Unary minus

	<member-access> ::= <identifier> "." <identifier>             // Basic member access (e.g., player.health)
	                  | <member-access> "." <identifier>          // Chained member access (e.g., player.position.x)

	<array-literal> ::= "[" <argument-list> "]"                   // Array definition like [1, 2, 3]

	<identifier> ::= [a-zA-Z_][a-zA-Z0-9_]*                       // Standard identifier pattern

	<literal> ::= <integer>
	            | <float>
	            | <string>
	            | <boolean>

	<integer> ::= [0-9]+

	<float> ::= [0-9]+"."[0-9]+                                   // Floating point numbers

	<string> ::= \"[^\"]*\"                                       // String literals

	<boolean> ::= "true" | "false"                                // Boolean literals