%start program

%union
{  
    int intValue;
    float floatValue;
	char charValue;                
    char* stringValue;
	int boolValue;
    char* variableName;
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
  %token <variableName> IDENTIFIER

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
%type <information> Number_Declaration EXPRESSION Boolean_Expression Function_Calls
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
	#include "typeConversion.h"

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
	void add_parameters_to_function_symbol_table(DataTypes*, struct argument_info*);
	void assigning_operation_with_conversion(char* , struct lexemeInfo **);
	// variables to use through the code to check semantics
	struct variable_entry * current_identifier;
	RETURN_CODES current_return_code;
	OperationsToDo operation;
%}


%%
program	: statements
		;
	
statements: statements stmt  
			|		
			;
		
stmt:   Type_Identifier IDENTIFIER SEMICOLON { current_return_code =add_variable_to_scope(current_scope, $2, 0, $1,VARIABLE_KIND,NULL);
												if(current_return_code == FAILURE)
													yyerror_with_variable("Redefinition of variable ", $2);
												else if(current_return_code == CONSTANT_NOT_INITIALIZED)
													yyerror_with_variable("Must initialize constant within declaration ", $2);
											}  

	|	Type_Identifier IDENTIFIER ASSIGN EXPRESSION  SEMICOLON  {if(add_variable_to_scope(current_scope, $2, 1, $1,VARIABLE_KIND,NULL) == FAILURE){
																		yyerror_with_variable("Redefinition of variable ", $2);
																	}else{
																		operation = implicit_conversion($1,$4->my_type);
																		if(operation == EVAL_THEN_DOWNGRADE_RHS){
																			// downgrade conv to result dt needed
																			current_return_code = downgrade_my_value(&$4,$4->my_type, $1,yylineno);
																			if(current_return_code == STRING_INVALID_OPERATION){
																				yyerror("invalid string conversion");
																			}
																		}else if(operation == EVAL_THEN_UPGRADE_RHS){
																			// upgrade to result dt needed
																			current_return_code = upgrade_my_value(&$4,$4->my_type, $1,yylineno);
																			if(current_return_code == STRING_INVALID_OPERATION){
																				yyerror("invalid string conversion");
																			}
																		}else if(operation == RAISE_ERROR){
																			yyerror("invalid string conversion");
																		}
																	}
																}

	|	IDENTIFIER ASSIGN EXPRESSION SEMICOLON { 	current_return_code = assign_previously_declared_variable_in_scope(current_scope, $1);
													if(current_return_code == FAILURE)
														yyerror_with_variable("Undeclared variable ", $1);
													else if(current_return_code == CONSTANT_REASSIGNMENT)
														yyerror_with_variable("cant reassign a constant variable :", $1);
													else{
														current_identifier = find_variable_in_scope(current_scope,$1);
														operation = implicit_conversion(current_identifier->my_datatype,$3->my_type);
														if(operation == EVAL_THEN_DOWNGRADE_RHS){
															// downgrade conv to result dt needed
															current_return_code = downgrade_my_value(&$3,$3->my_type, current_identifier->my_datatype,yylineno);
															if(current_return_code == STRING_INVALID_OPERATION){
																yyerror("invalid string conversion");
															}
														}else if(operation == EVAL_THEN_UPGRADE_RHS){
															// upgrade to result dt needed
															current_return_code = upgrade_my_value(&$3,$3->my_type, current_identifier->my_datatype,yylineno);
															if(current_return_code == STRING_INVALID_OPERATION){
																yyerror("invalid string conversion");
															}
														}else if(operation == RAISE_ERROR){
															yyerror("invalid string conversion");
														}
													}					
												} 
 
	|	Mathematical_Statement SEMICOLON
	|	IF_Statement
	|	{enter_new_scope();} Scope {exit_a_scope();}
	| 	LOOPS
	|   FUNCTIONS
	| 	Function_Calls SEMICOLON  // f1();
	| 	Switch_Case
	;

EXPRESSION: Number_Declaration {$$ =$1;}
		| 	Boolean_Expression {$$ =$1;}
		|	Function_Calls	{$$ =$1;}
		;

Number_Declaration: FLOAT 	{set_lexemeInfo(&$$, FLOAT_DT); $$->floatValue = $1;}
				
				|	INT 	{set_lexemeInfo(&$$, INT_DT); $$->intValue = $1;}

				|   IDENTIFIER { 
						current_identifier = find_variable_in_scope(current_scope,$1);
						if(current_identifier == NULL){
							yyerror_with_variable("identifier not declared in this scope",$1 );
						}else{
							set_lexemeInfo(&$$,current_identifier->my_datatype);
							$$->variableName = $1;
						}
					}
				| 	Number_Declaration PLUS Number_Declaration  { 	current_return_code =  compute_rhs_value(&$$,$1,$3,PLUS_OP,yylineno);
													if(current_return_code == STRING_INVALID_OPERATION){
														yyerror("invalid operation on strings");
													}else if(current_return_code == OPERATION_NOT_SUPPORTED){
														yyerror("Invalid Operations ");
													}
												}			

				| 	Number_Declaration MINUS Number_Declaration { 	current_return_code =  compute_rhs_value(&$$,$1,$3,MINUS_OP,yylineno);
													if(current_return_code == STRING_INVALID_OPERATION){
														yyerror("invalid operation on strings");
													}else if(current_return_code == OPERATION_NOT_SUPPORTED){
														yyerror("Invalid Operations ");
													}
												}	
				
				| 	Number_Declaration DIVIDE Number_Declaration { 	current_return_code =  compute_rhs_value(&$$,$1,$3,DIVIDE_OP,yylineno);
																	if(current_return_code == STRING_INVALID_OPERATION){
																		yyerror("invalid operation on strings");
																	}else if(current_return_code == OPERATION_NOT_SUPPORTED){
																		yyerror("Invalid Operations ");
																	}else if(current_return_code ==DIVISION_BY_ZERO_ERROR){
																		yyerror("Divison by zerro !");
																	}
																}	
				| 	Number_Declaration MULTIPLY Number_Declaration { 	current_return_code =  compute_rhs_value(&$$,$1,$3,MULTIPLY_OP,yylineno);
																		if(current_return_code == STRING_INVALID_OPERATION){
																			yyerror("invalid operation on strings");
																		}else if(current_return_code == OPERATION_NOT_SUPPORTED){
																			yyerror("Invalid Operations ");
																		}
																	}	
				| 	Number_Declaration REM Number_Declaration { 	current_return_code =  compute_rhs_value(&$$,$1,$3,REM_OP,yylineno);
																		if(current_return_code == STRING_INVALID_OPERATION){
																			yyerror("invalid operation on strings");
																		}else if(current_return_code == OPERATION_NOT_SUPPORTED){
																			yyerror("Invalid Operations ");
																		}
																}	
				| 	Number_Declaration POWER Number_Declaration { 	current_return_code =  compute_rhs_value(&$$,$1,$3,POWER_OP,yylineno);
																		if(current_return_code == STRING_INVALID_OPERATION){
																			yyerror("invalid operation on strings");
																		}else if(current_return_code == OPERATION_NOT_SUPPORTED){
																			yyerror("Invalid Operations ");
																		}
																	}	
				|	ORBRACKET Number_Declaration CRBRACKET {$$=$2;}
				| 	'-' Number_Declaration %prec UMINUS { 	current_return_code =  compute_rhs_value(&$$,$2,NULL,UMINUS_OP,yylineno);
																		if(current_return_code == STRING_INVALID_OPERATION){
																			yyerror("invalid operation on strings");
																		}else if(current_return_code == OPERATION_NOT_SUPPORTED){
																			yyerror("Invalid Operations ");
																		}
														}	
				|	TRUE			{set_lexemeInfo(&$$, BOOL_DT); $$->boolValue = 1;}
				|	FALSE			{set_lexemeInfo(&$$, BOOL_DT); $$->boolValue = 0;}
				| 	CHAR			{set_lexemeInfo(&$$, CHAR_DT); $$->charValue = $1;}
				| 	STRING			{set_lexemeInfo(&$$, STRING_DT); $$->stringValue = $1;}
				;


/* defining boolean expression */
Boolean_Expression: 
					// for logical expressions we can pass any data type
					// as (x) equivalent to (x!=0) whatever x is int, char, string, ...
					EXPRESSION AND EXPRESSION { current_return_code = down_convert_boolean_expression(&$$, $1, $3, AND_OP);
													if(current_return_code == STRING_INVALID_OPERATION)
														yyerror("Invalid String Conversion to Boolean ");
												}
					| EXPRESSION OR EXPRESSION { current_return_code = down_convert_boolean_expression(&$$, $1, $3, OR_OP);
													if(current_return_code == STRING_INVALID_OPERATION)
														yyerror("Invalid String Conversion to Boolean ");
												}
					| NOT EXPRESSION { current_return_code = down_convert_boolean_expression(&$$, $2, NULL, NOT_OP);
													if(current_return_code == STRING_INVALID_OPERATION)
														yyerror("Invalid String Conversion to Boolean ");
									}
					/* 	boolean expressions for datatypes only 
						ex: 'a' > 'b'   -     1>2
					 */ 
					| EXPRESSION GREATERTHAN EXPRESSION { current_return_code = down_convert_boolean_expression(&$$, $1, $3, GREATERTHAN_OP);
															if(current_return_code == STRING_INVALID_OPERATION)
																yyerror("Invalid String Conversion to Boolean ");
														}
					| EXPRESSION GREATERTHANOREQUAL EXPRESSION{ current_return_code = down_convert_boolean_expression(&$$, $1, $3, GREATERTHANOREQUAL_OP);
																if(current_return_code == STRING_INVALID_OPERATION)
																	yyerror("Invalid String Conversion to Boolean ");
															}
					| EXPRESSION LESSTHAN EXPRESSION { current_return_code = down_convert_boolean_expression(&$$, $1, $3, LESSTHAN_OP);
																if(current_return_code == STRING_INVALID_OPERATION)
																	yyerror("Invalid String Conversion to Boolean ");
															}
					| EXPRESSION LESSTHANOREQUAL EXPRESSION { current_return_code = down_convert_boolean_expression(&$$, $1, $3, LESSTHANOREQUAL_OP);
																if(current_return_code == STRING_INVALID_OPERATION)
																	yyerror("Invalid String Conversion to Boolean ");
															}
					| EXPRESSION EQUALEQUAL EXPRESSION { current_return_code = down_convert_boolean_expression(&$$, $1, $3, EQUALEQUAL_OP);
																if(current_return_code == STRING_INVALID_OPERATION)
																	yyerror("Invalid String Conversion to Boolean ");
															}
					| EXPRESSION NOTEQUAL EXPRESSION { current_return_code = down_convert_boolean_expression(&$$, $1, $3, NOTEQUAL_OP);
																if(current_return_code == STRING_INVALID_OPERATION)
																	yyerror("Invalid String Conversion to Boolean ");
														}
					| ORBRACKET Boolean_Expression CRBRACKET {$$ = $2; }
					;

// Mathematical statements  
/* IDENTIFIER ASSIGN EXPRESSION SEMICOLON {printf("Variable assignment\n");}  */
// We made the assign separetely in the stmt cfg since assign can take any thing on the RHS
// while if we made it here , it will take number only
Mathematical_Statement: IDENTIFIER PLUSEQUAL Number_Declaration {assigning_operation_with_conversion($1, &$3);}
				|		IDENTIFIER MINUSEQUAL Number_Declaration {assigning_operation_with_conversion($1, &$3);}
				|		IDENTIFIER MULTIPLYEQUAL Number_Declaration {assigning_operation_with_conversion($1, &$3);}
				|		IDENTIFIER DIVIDEEQUAL Number_Declaration {assigning_operation_with_conversion($1, &$3);}
				|		IDENTIFIER REMEQUAL Number_Declaration {assigning_operation_with_conversion($1, &$3);}
				|   	IDENTIFIER INCREMENT {printf("incrementing number\n");}
				|   	IDENTIFIER DECREMENT {printf("decremening number \n");}
				; 

Scope: OCBRACKET statements CCBRACKET {printf("");} 	 
	;

// we separated Loop_statements bec they contain BREAK, Continue and we cant use them in normal context
Loop_statements: Loop_statements stmt
				| Loop_statements BREAK SEMICOLON {printf("break!! \n");}
				| Loop_statements CONTINUE SEMICOLON {printf("continue!! \n");}
				|
				;

Loop_Scope : OCBRACKET Loop_statements CCBRACKET {printf("");} 	 
			;

LOOPS: FOR ORBRACKET stmt Boolean_Expression SEMICOLON Mathematical_Statement CRBRACKET {enter_new_scope();} Loop_Scope {exit_a_scope();}
	|  WHILE Boolean_Expression {enter_new_scope();} Loop_Scope {exit_a_scope();}
	|  DO {enter_new_scope();} Loop_Scope {exit_a_scope();} WHILE Boolean_Expression SEMICOLON {printf("Do while loop \n");}
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
					//getting arguments of these function
					DataTypes* arguments_list = get_parameters_of_array($4);
					// adding function to the symbol table
					current_return_code = add_variable_to_scope(current_scope, $2, 0, $1, FUNCTION_KIND, arguments_list);
					if(current_return_code == FAILURE){
						yyerror_with_variable("Redefinition of function ", $2);
					}
					enter_new_scope(); // entering a new scope
					// we add parameters to symbol table after making new scope
					add_parameters_to_function_symbol_table(arguments_list, $4);
								} Function_Scope {print_symbol_table(&(current_scope)->my_table); exit_a_scope();} 

			| VOID IDENTIFIER ORBRACKET ARGUMENTS CRBRACKET {
					//getting arguments of these function
					DataTypes* arguments_list = get_parameters_of_array($4);
					// adding function to the symbol table
					current_return_code = add_variable_to_scope(current_scope, $2, 0, VOID_DT, FUNCTION_KIND, arguments_list);
					if(current_return_code == FAILURE){
						yyerror_with_variable("Redefinition of function ", $2);
					}
					enter_new_scope(); // entering a new scope
					// we add parameters to symbol table after making new scope
					add_parameters_to_function_symbol_table(arguments_list, $4);
								} Void_Function_Scope {print_symbol_table(&(current_scope)->my_table); exit_a_scope();}
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
			| {$$ = NULL}//it can be empty
			;

Arguments_Call : EXPRESSION COMMA  Arguments_Call	{printf("function arguments \n");}
				| EXPRESSION {printf("function arguments  \n");}
				|//it can be empty
				;

Function_Calls: IDENTIFIER ORBRACKET Arguments_Call CRBRACKET { 
						current_identifier = find_variable_in_scope(current_scope,$1);
						if(current_identifier == NULL){
							yyerror("function not initialzed in this scope");
						}else{
							set_lexemeInfo(&$$,current_identifier->my_datatype);
							$$->variableName = $1;
						}
					}

// we made switch take a no (int , float ,.. ) or a fn call which returns int
// will check that later
Switch_Case : SWITCH  Number_Declaration OCBRACKET Case_Expressions CCBRACKET 
			| SWITCH  ORBRACKET Function_Calls CRBRACKET OCBRACKET Case_Expressions CCBRACKET
			;

Case_Expressions : CASE INT COLON {enter_new_scope();} statements BREAK SEMICOLON {exit_a_scope();} Case_Expressions
				|	DEFAULT COLON {enter_new_scope();} statements {exit_a_scope();} 
				|	// since we can have no default or any case (tested on C++)
				;

/* 
IF ELSE Case
https://stackoverflow.com/questions/6911214/how-to-make-else-associate-with-farthest-if-in-yacc 
*/
// {enter_new_scope();}  {exit_a_scope();}
IF_Statement : IF  ORBRACKET EXPRESSION {if($3->my_type==STRING_DT)yyerror("Invalid String Conversion to Boolean");} CRBRACKET stmt endCondition 
			;

endCondition: %prec IFX | ELSE  stmt
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
	while(start_ptr){
		no_of_arguments++;
		start_ptr = start_ptr->next_arg;
	}
	start_ptr = temp;
	// Dynamically allocate memory using malloc()
	if(no_of_arguments == 0 )
		return NULL;
	DataTypes* arguments_list = (DataTypes*)malloc(no_of_arguments * sizeof(DataTypes));
	int i = 0 ;
	while(start_ptr){
		arguments_list[i] = start_ptr->my_type;
		start_ptr = start_ptr->next_arg;
		i++;
	}
	return arguments_list;
 }

 void add_parameters_to_function_symbol_table(DataTypes* arguments_list,struct argument_info* temp){
	int i = 0 ;
	struct argument_info* start_ptr = temp;
	while(start_ptr){
		// adding each parameter to the symbol table
		current_return_code = add_variable_to_scope(current_scope, start_ptr->my_name, 0, start_ptr->my_type,PARAMETER_KIND,NULL);
		if(current_return_code == FAILURE)
		{
			yyerror_with_variable("Redefinition of parameter in function ", start_ptr->my_name);
		}
		start_ptr = start_ptr->next_arg;
		i++;
	}
 }


void assigning_operation_with_conversion(char* lhs, struct lexemeInfo ** rhs){
	// Number_Declaration will be ready and upgraded if needed
	current_identifier = find_variable_in_scope(current_scope,lhs);
	if(current_identifier == NULL){
		yyerror_with_variable("identifier not declared in this scope",lhs );
	}else{
		operation = implicit_conversion(current_identifier->my_datatype,(*rhs)->my_type);
		if(operation == EVAL_THEN_DOWNGRADE_RHS){
			// downgrade conv to result dt needed
			current_return_code = downgrade_my_value(rhs,(*rhs)->my_type, current_identifier->my_datatype,yylineno);
			if(current_return_code == STRING_INVALID_OPERATION){
				yyerror("invalid string conversion");
			}
		}else if(operation == EVAL_THEN_UPGRADE_RHS){
			// upgrade to result dt needed
			current_return_code = upgrade_my_value(rhs,(*rhs)->my_type, current_identifier->my_datatype,yylineno);
			if(current_return_code == STRING_INVALID_OPERATION){
				yyerror("invalid string conversion");
			}
		}else if(operation == RAISE_ERROR){
			yyerror("invalid string conversion");
		}
	}
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

	// clearing the final scope 
	print_symbol_table(&(current_scope)->my_table);
	exit_a_scope();
    return 0;
}