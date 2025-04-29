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
	              | <function-call-statement>  // Function call used as a statement
	              | <collision-handler>
	              | <print-statement>
	              | <input-statement>
	              | <start-block>           // Added START block
	
	<definition-statement> ::= "DEFINE" <identifier> "=" <expression> ";"<definition-statement> ::= "DEFINE" <identifier> "=" <expression> ";"
	
	<object-creation> ::= "CREATE" <identifier> "AS" <object-type> "{" <property-list> "}" ";"<object-creation> ::= "CREATE" <identifier> "AS" <object-type> "{" <property-list> "}" ";"
	
	<object-type> ::= "PLAYER" | "ENEMY" | "PLATFORM" | "ITEM" | "GAME" | "LEVEL"<object-type> ::= "PLAYER" | "ENEMY" | "PLATFORM" | "ITEM" | "GAME" | "LEVEL"
	
	<property-list> ::= <property> <property-list> ::= <property> | <property> <property-list>
	                  | <property> <property-list>
	<property> ::= <identifier> "=" <expression> ";"
	<property> ::= <identifier> "=" <expression> ";"
	ression> ";"
	<assignment> ::= <member-access> "=" <expression> ";" // Allow assignment to members
	               | <identifier> "=" <expression> ";"               | <identifier> "--" ";"
	               | <identifier> "++" ";"
	               | <identifier> "--" ";"
	               | <identifier> "+=" <expression> ";" // Added compound assignment                 | "IF" <condition> "THEN" <statement-list> "ELSE" <statement-list> "ENDIF" ";"
	               | <identifier> "-=" <expression> ";" // Added compound assignment
	               | <identifier> "*=" <expression> ";" // Added compound assignment<while-statement> ::= "WHILE" <condition> "DO" <statement-list> "ENDWHILE" ";"
	               | <identifier> "/=" <expression> ";" // Added compound assignment
	-statement> "}" ";"
	<if-statement> ::= "IF" <condition> "THEN" <statement-list> "ENDIF" ";"                        | "FUNCTION" <identifier> "(" <parameter-list> ")" "{" <statement-list> "}" ";"
	                 | "IF" <condition> "THEN" <statement-list> "ELSE" <statement-list> "ENDIF" ";"
	<parameter-list> ::= ε | <identifier> | <identifier> "," <parameter-list>
	<while-statement> ::= "WHILE" <condition> "DO" <statement-list> "ENDWHILE" ";"
	<return-statement> ::= "RETURN" <expression> ";"
	<function-definition> ::= "FUNCTION" <identifier> "(" <parameter-list> ")" "{" <statement-list> <return-statement> "}" ";"
	                        | "FUNCTION" <identifier> "(" <parameter-list> ")" "{" <statement-list> "}" ";"<function-call> ::= <identifier> "(" <argument-list> ")" ";"
	
	<parameter-list> ::= ε <argument-list> ::= ε | <expression> | <expression> "," <argument-list>
	                   | <identifier> 
	                   | <identifier> "," <parameter-list><collision-handler> ::= "ON" "COLLISION" "(" <identifier> "," <identifier> ")" "{" <statement-list> "}" ";"
	
	<return-statement> ::= "RETURN" <expression> ";"<print-statement> ::= "PRINT" "(" <expression> ")" ";"
	
	<function-call-statement> ::= <function-call> ";" // Function call as a standalone statement<input-statement> ::= "INPUT" "(" <identifier> ")" ";"
	
	<function-call> ::= <identifier> "(" <argument-list> ")" // Function call used within expressions or as a statement baserator> <expression>
	
	<argument-list> ::= ε " <condition>
	                  | <expression> 
	                  | <expression> "," <argument-list>              | "(" <condition> ")"
	
	<collision-handler> ::= "ON" "COLLISION" "(" <identifier> "," <identifier> ")" "{" <statement-list> "}" ";"<comparison-operator> ::= "==" | "!=" | "<" | ">" | "<=" | ">="
	
	<print-statement> ::= "PRINT" "(" <expression> ")" ";"
	
	<input-statement> ::= "INPUT" "(" <identifier> ")" ";"               | <expression> "-" <term>
	
	<condition> ::= <expression> // Allow simple expression (truthiness)
	              | <expression> <comparison-operator> <expression>
	              | <condition> "&&" <condition>         | <term> "/" <factor>
	              | <condition> "||" <condition>
	              | "!" <condition>er>
	              | "(" <condition> ")"
	
	<comparison-operator> ::= "==" | "!=" | "<" | ">" | "<=" | ">="           | "(" <expression> ")"
	
	<expression> ::= <term><identifier> ::= [a-zA-Z_][a-zA-Z0-9_]*
	               | <expression> "+" <term>
	               | <expression> "-" <term>r>
	
	<term> ::= <factor>
	         | <term> "*" <factor>            | <boolean>
	         | <term> "/" <factor>
	<integer> ::= [0-9]+
	<factor> ::= <member-access>       // Added member access
	           | <identifier><float> ::= [0-9]+"."[0-9]+
	           | <literal>
	           | <array-literal>       // Added array literal<string> ::= \"[^\"]*\"
	           | <function-call>       // Function call within an expression (no semicolon here)























	<start-block> ::= "START" "{" <statement-list> "}" // Added START block rule		<boolean> ::= "true" | "false"		<string> ::= \"[^\"]*\"		<float> ::= [0-9]+"."[0-9]+		<integer> ::= [0-9]+		            | <boolean> // Added boolean	            | <string>	            | <float>	<literal> ::= <integer>		<identifier> ::= [a-zA-Z_][a-zA-Z0-9_]* // Standard identifier pattern		<array-literal> ::= "[" <argument-list> "]" // Array definition like [1, 2, 3]		                  | <member-access> "." <identifier> // Chained member access (e.g., player.position.x)	<member-access> ::= <identifier> "." <identifier> // Basic member access (e.g., player.health)		           | "(" <expression> ")"	<boolean> ::= "true" | "false"