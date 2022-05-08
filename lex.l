%{   
	#include <stdlib.h>
	#include <stdio.h>
	#include "y.tab.h"
%}

%%
\{                      						return OCBRACKET;
\}                      						return CCBRACKET;
\(												          return ORBRACKET;
\)												          return CRBRACKET;


"++"													  return INCREMENT;
"--"													  return DECREMENT;
"+="											          return PLUSEQUAL;
"-="											          return MINUSEQUAL;
"*="											          return MULTIPLYEQUAL;
"/="											          return DIVIDEEQUAL;
">"         	 								      return GREATERTHAN;
"<"         	 								      return LESSTHAN;
">="         	 								      return GREATERTHANOREQUAL;
"<="             								    return LESSTHANOREQUAL;
"=="             								    return EQUALEQUAL;
"!="             								    return NOTEQUAL;
"+"												          return PLUS;
"-"												          return MINUS;
"*"												          return MULTIPLY;
"^"												return POWER;
"/"												          return DIVIDE;
"="												          return ASSIGN;
"%"												          return REM;
"%="													  return REMEQUAL;


;                       						return SEMICOLON;
:                       						return COLON; 
,                       						return COMMA;

"&&"											          return AND;
"||"											          return OR;
"!"												          return NOT;


"while"          								    return WHILE;
"for"          									    return FOR;
"if"             								    return IF;
"else"           								    return ELSE;
"do"											          return DO;
"break"                 						return BREAK;
"switch"                						return SWITCH;
"case"                  						return CASE;
"print"        								     	return PRINT;
"return"                						return RET;


"bool"				    						      return BOOL;
"int"											          return INT;
"float"											        return FLOAT;
"char"											        return CHAR;
"string"										        return STRING;
"const"                 						return CONST;


"false"										        	return FALSE;
"true"										        	return TRUE;


[0-9]+											        {yylval = atoi(yytext); return INT;}
[0-9]+\.[0-9]+									    {yylval = atof(yytext); return FLOAT;}


\".*\"											        return STRING;
\'.\'										          	return CHAR;
[a-zA-Z][_a-zA-Z0-9]*						  	return IDENTIFIER; 	
[\t  ]+											        /* ignore whitespace */;
[\n]											          {yylineno = yylineno+1;}


%% 

int yywrap (void) {
	return 1;
}

