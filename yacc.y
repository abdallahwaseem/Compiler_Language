%start program
// Tokens for brackets
	%token OCBRACKET
	%token CCBRACKET
	%token ORBRACKET
	%token CRBRACKET

// Tokens for colons
	%token SEMICOLON
	%token COLON
	%token COMMA

// Tokens for x equal
	%token INCREMENT
	%token DECREMENT
	%token PLUSEQUAL
	%token MINUSEQUAL
	%token MULTIPLYEQUAL
	%token DIVIDEEQUAL

// Tokens for x equal
	%token GREATERTHAN
	%token LESSTHAN
	%token GREATERTHANOREQUAL
	%token LESSTHANOREQUAL
	%token EQUALEQUAL
	%token NOTEQUAL

// Tokens for x equal
	%token PLUS
	%token MINUS
	%token MULTIPLY
	%token DIVIDE
	%token POWER
// Tokens for assign
	%token ASSIGN

// Tokens for remainder
	%token REM
	%token REMEQUAL

// Tokens for logic operators
	%token AND
	%token OR
	%token NOT

// Tokens for loops
	%token WHILE
	%token DO
	%token FOR

// Tokens for conditions
	%token IF 
	%token ELSE

// TODO:: print token


// Tokens for primitive data types
	%token BOOL
	%token INT
	%token FLOAT
	%token CHAR
	%token STRING
	%token CONST
	%token VOID

// Tokens for break
	%token BREAK

// Tokens for continue
	%token CONTINUE

// Token for IDENTIFIER
  %token IDENTIFIER

// Tokens for switch case
	%token SWITCH
	%token CASE

// Tokens for booleans
	%token FALSE
	%token TRUE


// Tokens for function return
	%token RET

// Associativity
	%left ASSIGN
	%left GREATERTHAN LESSTHAN
	%left GREATERTHANOREQUAL LESSTHANOREQUAL
	%left EQUALEQUAL NOTEQUAL
	%left AND OR NOT
	%left PLUS MINUS 
	%left DIVIDE MULTIPLY REM
	%left POWER
	%left ORBRACKET CRBRACKET

// token DEFAULT
	%token DEFAULT

%nonassoc IFX
%nonassoc ELSE
%nonassoc UMINUS

%{  
	#include <stdio.h>   
	int yyerror(char *);
	int yylex(void);
	extern int yylineno ;
  	extern char* yytext;
	FILE * f1;
	extern FILE * yyin;
%}


%%
program	: statements
		;
	
statements: statements stmt  
			|		
			;
		
stmt:   
		Type_Identifier IDENTIFIER  SEMICOLON {printf("Undeclared Variable \n");} 
	|	Type_Identifier IDENTIFIER ASSIGN EXPRESSION SEMICOLON {printf("Variable Declaration\n");} 
	|	IDENTIFIER ASSIGN EXPRESSION SEMICOLON {printf("Variable assignment\n");} 
	| 	CONST Type_Identifier IDENTIFIER ASSIGN EXPRESSION SEMICOLON {printf("Constant Variable Declaration\n");} 
	|	Mathematical_Statement SEMICOLON {printf("MATH STATEMET\n");} 
	|	IF_Statement
	|	Scope	
	| 	LOOPS
	|   FUNCTIONS
	| 	Function_Calls SEMICOLON  // f1();
	| 	Switch_Case
	|	BREAK SEMICOLON
	|	CONTINUE SEMICOLON
	;

EXPRESSION: Data_Types {printf("expression datatype\n");}
		| 	Boolean_Expression {printf("Boolean expression\n");}
		|	Function_Calls	{printf("function call\n");}
		;

Number_Declaration: FLOAT 	{printf("float\n");}
				|	INT 	{printf("int \n");}
				|   IDENTIFIER {printf("variable\n");}
				| 	Number_Declaration PLUS Number_Declaration {printf("addition operation\n");}
				| 	Number_Declaration MINUS Number_Declaration {printf("subtraction operation\n");}
				| 	Number_Declaration DIVIDE Number_Declaration {printf("division operation\n");}
				| 	Number_Declaration MULTIPLY Number_Declaration {printf("Multiplication operation\n");}
				| 	Number_Declaration REM Number_Declaration {printf("remainder operation\n");}
				| 	Number_Declaration POWER Number_Declaration {printf("power operation\n");}
				|	ORBRACKET Number_Declaration CRBRACKET {printf("number between brackets\n");}
				| 	'-' Number_Declaration %prec UMINUS {printf("-ve number");}
				;


Data_Types: Number_Declaration {printf("number declaration or identifier \n");}
			|	TRUE			{printf("Boolean Datatype \n");}
			|	FALSE			{printf("Boolean Datatype \n");}
			| 	CHAR			{printf("Character Datatype \n");}
			| 	STRING			{printf("String Datatype \n");}
			;

/* defining boolean expression */
Boolean_Expression: 
					// for logical expressions we can pass any data type
					// as (x) equivalent to (x!=0) whatever x is int, char, string, ...
					EXPRESSION AND EXPRESSION {printf("and operator \n");}
					| EXPRESSION OR EXPRESSION {printf("and operator \n");}
					| NOT EXPRESSION {printf("and operator \n");}
					/* 	boolean expressions for datatypes only 
						ex: 'a' > 'b'   -     1>2
					 */ 
					| Data_Types GREATERTHAN Data_Types {printf("greater than operator \n");}
					| Data_Types GREATERTHANOREQUAL Data_Types {printf("greater than equal operator \n");}
					| Data_Types LESSTHAN Data_Types {printf("less than operator \n");}
					| Data_Types LESSTHANOREQUAL Data_Types {printf("less than equal operator \n");}
					| Data_Types EQUALEQUAL Data_Types {printf("== operator \n");}
					| Data_Types NOTEQUAL Data_Types {printf("!= operator \n");}
					| ORBRACKET Boolean_Expression CRBRACKET {printf("boolean between brackets \n");}
					;

// Mathematical statements  
/* IDENTIFIER ASSIGN EXPRESSION SEMICOLON {printf("Variable assignment\n");}  */
// We made the assign separetely in the stmt cfg since assign can take any thing on the RHS
// while if we made it here , it will take number only
Mathematical_Statement: IDENTIFIER PLUSEQUAL Number_Declaration {printf("Adding a Number to a value\n");}
				|		IDENTIFIER MINUSEQUAL Number_Declaration {printf("Subtracting a Number to a value\n");}
				|		IDENTIFIER MULTIPLYEQUAL Number_Declaration {printf("Multiplying a Number to a value\n");}
				|		IDENTIFIER DIVIDEEQUAL Number_Declaration {printf("Dividing a Number to a value\n");}
				|		IDENTIFIER REMEQUAL Number_Declaration {printf("Remaindering a Number to a value\n");}
				|   	IDENTIFIER INCREMENT {printf("incrementing number\n");}
				|   	IDENTIFIER DECREMENT {printf("decremening number \n");}
				; 

Scope:	OCBRACKET statements CCBRACKET 	{printf("block scope \n");}
	;

LOOPS: FOR ORBRACKET stmt Boolean_Expression SEMICOLON Mathematical_Statement CRBRACKET Scope {printf("for loop \n");}
	|  WHILE Boolean_Expression Scope {printf("while loop \n");}
	|  DO Scope WHILE Boolean_Expression SEMICOLON {printf("Do while loop \n");}
	;

Type_Identifier:  INT {printf("integer type\n");}
				| FLOAT {printf("float type\n");}
				| CHAR  {printf("char type\n");}
				| STRING{printf("string type\n");}
				| BOOL	{printf("bool type\n");}
				;
		

// Function declaration - added void option
// in void we can return or not 
// while in any other return type fn , we must return expression ; 
FUNCTIONS : Type_Identifier IDENTIFIER ORBRACKET ARGUMENTS CRBRACKET Function_Scope {printf("function declaration \n");}
			| VOID IDENTIFIER ORBRACKET ARGUMENTS CRBRACKET Scope {printf("function declaration \n");}
			| VOID IDENTIFIER ORBRACKET ARGUMENTS CRBRACKET Function_Scope {printf("function declaration \n");}
			;

Function_Scope: OCBRACKET statements RET EXPRESSION SEMICOLON CCBRACKET 	{printf("fn scope \n");}
			| OCBRACKET statements RET SEMICOLON CCBRACKET 	{printf("fn scope \n");}
			;

ARGUMENTS: Type_Identifier IDENTIFIER COMMA  ARGUMENTS	{printf("function arguments \n");}
			| Type_Identifier IDENTIFIER 	{printf("function arguments \n");}
			| //it can be empty
			;

Arguments_Call : EXPRESSION COMMA  Arguments_Call	{printf("function arguments \n");}
				| EXPRESSION {printf("function arguments  \n");}
				|//it can be empty
				;

Function_Calls: IDENTIFIER ORBRACKET Arguments_Call CRBRACKET {printf("calling fn \n");}

// we made switch take a no (int , float ,.. ) or a fn call which returns int
// will check that later
Switch_Case : SWITCH Number_Declaration OCBRACKET Case_Expressions CCBRACKET
			| SWITCH ORBRACKET Function_Calls CRBRACKET OCBRACKET Case_Expressions CCBRACKET
			;

Case_Expressions : CASE INT COLON statements BREAK SEMICOLON Case_Expressions
				|	DEFAULT COLON statements 
				|	// since we can have no default or any case (tested on C++)
				;


/* 
IF ELSE Case
https://stackoverflow.com/questions/6911214/how-to-make-else-associate-with-farthest-if-in-yacc 
*/
IF_Statement : IF ORBRACKET EXPRESSION CRBRACKET stmt endCondition {printf("IF_Statement");}
			;

endCondition: %prec IFX | ELSE stmt	{printf("else statement");}
			;


%% 
 int yyerror(char *s) {  int lineno=++yylineno;   fprintf(stderr, "line number : %d %s\n", lineno,s);     return 0; }
 
 int main(void) {
	
	yyin = fopen("input.txt", "r");
 
	f1=fopen("output.txt","w");
	
	if(!yyparse()) {
		printf("\nParsing complete\n");
		fprintf(f1,"hello there");
	}
	else {
		printf("\nParsing failed\n");
		return 0;
	}
	
	fclose(yyin);
	fclose(f1);
    return 0;
}