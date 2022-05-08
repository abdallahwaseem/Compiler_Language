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

// Tokens for print
	%token PRINT

// Tokens for primitive data types
	%token BOOL
	%token INT
	%token FLOAT
	%token CHAR
	%token STRING
	%token CONST

// Tokens for break
	%token BREAK

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

%nonassoc IFX
%nonassoc ELSE
%nonassoc UMINUS

%{  
	#include <stdio.h>   
	int yyerror(char *);
	int yylex(void);
	extern int yylineno;
  extern char* yytext;
	FILE * f1;
	extern FILE * yyin;
%}


%%
program	: 
		function
		;
	
function :      function stmt  
		|		
		;
		
stmt:   Mathematical_Statement SEMICOLON {printf("Doing math");}
		;


Number_Declaration: FLOAT {printf("float\n");}
				|	INT {printf("int \n");}
				|   IDENTIFIER {prinf("variable");}
				| 	Number_Declaration PLUS Number_Declaration {printf("addition operation");}
				| 	Number_Declaration MINUS Number_Declaration {printf("subtraction operation");}
				| 	Number_Declaration DIVIDE Number_Declaration {printf("division operation");}
				| 	Number_Declaration MULTIPLY Number_Declaration {printf("Multiplication operation");}
				| 	Number_Declaration REM Number_Declaration {printf("remainder operation");}
				| 	Number_Declaration POWER Number_Declaration {printf("power operation");}
				|	ORBRACKET Number_Declaration CRBRACKET {printf("number between brackets");}

Mathematical_Statement: IDENTIFIER ASSIGN Number_Declaration {printf("Assigning a Number");}
				|		IDENTIFIER PLUSEQUAL Number_Declaration {printf("Adding a Number to a value");}
				|		IDENTIFIER MINUSEQUAL Number_Declaration {printf("Subtracting a Number to a value");}
				|		IDENTIFIER MULTIPLYEQUAL Number_Declaration {printf("Multiplying a Number to a value");}
				|		IDENTIFIER DIVIDEEQUAL Number_Declaration {printf("Dividing a Number to a value");}
				|		IDENTIFIER REMEQUAL Number_Declaration {printf("Remaindering a Number to a value");}
				|   	IDENTIFIER INCREMENT {printf("incrementing number");}
				|   	IDENTIFIER DECREMENT {printf("decrement");}

Data_Types: Number_Declaration {printf("This is a number");}
			



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