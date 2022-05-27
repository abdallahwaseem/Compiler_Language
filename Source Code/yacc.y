%start program

%union
{  
    int intValue;
    float floatValue;
	char charValue;                
    char* stringValue;
	int boolValue;
	struct lexemeInfo * information;
	struct argument_info * argument_info;
}

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
	%token <intValue> INT
	%token <charValue> CHAR
	%token <floatValue> FLOAT
	%token <boolValue> BOOL
	%token <stringValue> STRING
	%token CONST
	%token VOID

// Tokens for break
	%token BREAK

// Tokens for continue
	%token CONTINUE

// Token for IDENTIFIER
  %token <stringValue> IDENTIFIER

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

%type <intValue> Type_Identifier   
%type <information> Number_Declaration EXPRESSION Data_Types Boolean_Expression Function_Calls
%type <argument_info> ARGUMENTS

%nonassoc IFX
%nonassoc ELSE
%nonassoc UMINUS

%{  
	#include <stdio.h>
	#include <stdlib.h>   
	#include <string.h>
	#include <math.h>
	#include "scope.h"
	int yyerror(char *);
	int yyerror_with_variable(char*, char*);
	int yylex(void);
	extern int yylineno ;
  	extern char* yytext;
	FILE * f1;
	extern FILE * yyin;
	struct scope* current_scope ;
	struct scope* parent_scope ;
	
	void enter_new_scope();
	void exit_a_scope();
	DataTypes* get_parameters_of_array(struct argument_info*);
	struct variable_entry * current_identifier;
	RETURN_CODES current_return_code;
%}


%%
program	: statements
		;
	
statements: statements stmt  
			|		
			;
		
stmt:   Type_Identifier IDENTIFIER SEMICOLON { current_return_code =add_variable_to_scope(current_scope, $2, 0, $1,VARIABLE,NULL);
												if(current_return_code == FAILURE)
													yyerror_with_variable("Redefinition of variable ", $2);
												else if(current_return_code == CONSTANT_NOT_INITIALIZED)
													yyerror_with_variable("Must initialize constant within declaration ", $2);
											}  

	|	Type_Identifier IDENTIFIER ASSIGN EXPRESSION  SEMICOLON  {if(add_variable_to_scope(current_scope, $2, 1, $1,VARIABLE,NULL) == FAILURE)
																	yyerror_with_variable("Redefinition of variable ", $2);
																}

	|	IDENTIFIER ASSIGN EXPRESSION SEMICOLON { 	current_return_code = assign_previously_declared_variable_in_scope(current_scope, $1);
													if(current_return_code == FAILURE)
														yyerror_with_variable("Undeclared variable ", $1);
													else if(current_return_code == CONSTANT_REASSIGNMENT)
														yyerror_with_variable("cant reassign a constant variable :", $1);													
												} 
 
	|	Mathematical_Statement SEMICOLON {printf("MATH STATEMET\n");} 
	|	IF_Statement
	|	{enter_new_scope();} Scope {exit_a_scope();}
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

Number_Declaration: FLOAT 	{set_lexemeInfo(&$$, FLOAT_DT); $$->floatValue = $1;}
				
				|	INT 	{set_lexemeInfo(&$$, INT_DT); $$->intValue = $1;}

				|   IDENTIFIER { 
						current_identifier = find_variable_in_scope(current_scope,$1);
						if(current_identifier == NULL){
							yyerror("identifier not initialzed in this scope");
						}
						set_lexemeInfo(&$$,current_identifier->my_datatype);
						$$->stringValue = $1;
					}

				| 	Number_Declaration PLUS Number_Declaration {printf("addition operation \n");}			
				
				| 	Number_Declaration MINUS Number_Declaration {printf("subtraction operation \n");}
				| 	Number_Declaration DIVIDE Number_Declaration { printf("division operation \n");}
				| 	Number_Declaration MULTIPLY Number_Declaration {printf("Multiplication operation \n");}
				| 	Number_Declaration REM Number_Declaration { printf("remainder operation \n");}
				| 	Number_Declaration POWER Number_Declaration { printf("power operation \n");}
				|	ORBRACKET Number_Declaration CRBRACKET {printf("number between brackets\n");}
				| 	'-' Number_Declaration %prec UMINUS { printf("-ve number \n");}
				;


Data_Types: Number_Declaration 	{printf("number declaration or identifier \n");}
			|	TRUE			{set_lexemeInfo(&$$, BOOL_DT); $$->boolValue = 1;}
			|	FALSE			{set_lexemeInfo(&$$, BOOL_DT); $$->boolValue = 0;}
			| 	CHAR			{set_lexemeInfo(&$$, CHAR_DT); $$->charValue = $1;}
			| 	STRING			{set_lexemeInfo(&$$, STRING_DT); $$->stringValue = $1;}
			;

/* defining boolean expression */
Boolean_Expression: 
					// for logical expressions we can pass any data type
					// as (x) equivalent to (x!=0) whatever x is int, char, string, ...
					EXPRESSION AND EXPRESSION {printf("and operator \n");}
					| EXPRESSION OR EXPRESSION {printf("or operator \n");}
					| NOT EXPRESSION {printf("not operator \n");}
					/* 	boolean expressions for datatypes only 
						ex: 'a' > 'b'   -     1>2
					 */ 
					| EXPRESSION GREATERTHAN EXPRESSION {printf("greater than operator \n");}
					| EXPRESSION GREATERTHANOREQUAL EXPRESSION {printf("greater than equal operator \n");}
					| EXPRESSION LESSTHAN EXPRESSION {printf("less than operator \n");}
					| EXPRESSION LESSTHANOREQUAL EXPRESSION {printf("less than equal operator \n");}
					| EXPRESSION EQUALEQUAL EXPRESSION {printf("== operator \n");}
					| EXPRESSION NOTEQUAL EXPRESSION {printf("!= operator \n");}
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

Scope: OCBRACKET statements CCBRACKET {printf("entered scope-> \n");} 	 
	;

LOOPS: FOR ORBRACKET stmt Boolean_Expression SEMICOLON Mathematical_Statement CRBRACKET {enter_new_scope();} Scope {exit_a_scope();}
	|  WHILE Boolean_Expression {enter_new_scope();} Scope {exit_a_scope();}
	|  DO {enter_new_scope();} Scope {exit_a_scope();} WHILE Boolean_Expression SEMICOLON {printf("Do while loop \n");}
	;

Type_Identifier:  INT {$$ = INT_DT; }
				| FLOAT {$$ = FLOAT_DT;}
				| CHAR  {$$ = CHAR_DT;}
				| STRING{$$ = STRING_DT;}
				| BOOL {$$ = BOOL_DT;}
				| CONST INT{$$ = CONST_INT_DT;}
				| CONST FLOAT{$$ = CONST_FLOAT_DT;}
				| CONST CHAR{$$ = CONST_CHAR_DT; }
				| CONST STRING{$$ = CONST_STRING_DT;}
				| CONST BOOL{$$ = CONST_BOOL_DT;}
				;
		

// Function declaration - added void option
// in void we can return or not 
// while in any other return type fn , we must return expression ; 



FUNCTIONS : Type_Identifier IDENTIFIER ORBRACKET ARGUMENTS CRBRACKET {
					enter_new_scope(); // entering a new scope
					//getting arguments of these function
					DataTypes* arguments_list = get_parameters_of_array($4);
					// adding function to the symbol table
					current_return_code =add_variable_to_scope(current_scope, $2, 0, $1,VARIABLE,arguments_list);
					if(current_return_code == FAILURE){
						exit_a_scope();
						yyerror_with_variable("Redefinition of function ", $2);
					}
								} Function_Scope {exit_a_scope();} 

			| VOID IDENTIFIER ORBRACKET ARGUMENTS CRBRACKET {
					enter_new_scope(); // entering a new scope
					//getting arguments of these function
					DataTypes* arguments_list = get_parameters_of_array($4);
					// adding function to the symbol table
					current_return_code =add_variable_to_scope(current_scope, $2, 0, VOID_DT, VARIABLE, arguments_list);
					if(current_return_code == FAILURE){
						exit_a_scope();
						yyerror_with_variable("Redefinition of function ", $2);
					}
								} Void_Function_Scope {exit_a_scope();}
			;

Function_Scope:	OCBRACKET statements RET EXPRESSION SEMICOLON CCBRACKET 	
			;
Void_Function_Scope:  Scope
			| OCBRACKET statements RET SEMICOLON CCBRACKET 	
			;

ARGUMENTS: Type_Identifier IDENTIFIER COMMA  ARGUMENTS	{$$ = (struct argument_info *)malloc(sizeof(struct argument_info));
														 $$->next_arg = $4;
														 $$->my_name = $2;
														 $$->my_type = $1;
														}
			| Type_Identifier IDENTIFIER 	{$$ = (struct argument_info *)malloc(sizeof(struct argument_info));
											$$->next_arg = NULL;
											$$->my_name = $2;
											$$->my_type = $1;}
			| {}//it can be empty
			;

Arguments_Call : EXPRESSION COMMA  Arguments_Call	{printf("function arguments \n");}
				| EXPRESSION {printf("function arguments  \n");}
				|//it can be empty
				;

Function_Calls: IDENTIFIER ORBRACKET Arguments_Call CRBRACKET { 
						current_identifier = find_variable_in_scope(current_scope,$1);
						if(current_identifier == NULL){
							yyerror("function not initialzed in this scope");
						}
						set_lexemeInfo(&$$,current_identifier->my_datatype);
						$$->stringValue = $1;
					}

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
 int yyerror(char *s) { fprintf(stderr, "line number : %d %s\n", yylineno,s);     return 0; }
 int yyerror_with_variable(char *s, char* var) { fprintf(stderr, "line number : %d %s %s\n", yylineno,s, var);     return 0; }
 void enter_new_scope(){
	printf("enter scope \n");

	// setting the parent to currnt scope
	parent_scope = current_scope;
	// make the new scope by malloc
 	current_scope = (struct scope *)malloc(sizeof(struct scope)); 
 	// calling initialize fn in scope.h
	(*current_scope) = initialize_scope();
	// updating my parent
 	set_parent_of_scope(current_scope,parent_scope);
 }

 void exit_a_scope(){
	printf("left scope \n");
	// if there's parent, will set it to grandparent
	// not all cases we will have parent since at first and last scope it will be null
	if(parent_scope)
		parent_scope = parent_scope->my_parent;
	// delete_scope fn return the parent of current scope  
	current_scope =  delete_scope(current_scope);
	return;
 }

 DataTypes* get_parameters_of_array(struct argument_info* temp){
	int no_of_arguments = 0 ;
	struct argument_info* start_ptr = temp;
	while(temp){
		no_of_arguments++;
		temp = temp->next_arg;
	}
	temp = start_ptr;
	// Dynamically allocate memory using malloc()
	if(no_of_arguments == 0 )
		return NULL;
	DataTypes* arguments_list = (DataTypes*)malloc(no_of_arguments * sizeof(DataTypes));
	for (int i = 0 ; i <no_of_arguments; i++){
		// adding each parameter to the symbol table
		current_return_code = add_variable_to_scope(current_scope, temp->my_name, 0, temp->my_type,PARAMETER,NULL);
		if(current_return_code == FAILURE)
		{
			yyerror_with_variable("Redefinition of parameter in function ", temp->my_name);
		}
		arguments_list[i] = temp->my_type;
		temp = temp->next_arg;
	}
	return arguments_list;
 }

 int main(void) {

    enter_new_scope(); // whole scope containing all global variables and functions
	
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