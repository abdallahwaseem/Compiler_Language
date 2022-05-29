%option yylineno
%{   
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include "yacc.tab.h"
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
"return"                						return RET;
"continue"										return CONTINUE;

"int"											          return INT;
"bool"											          return BOOL;
"float"											        return FLOAT;
"char"											        return CHAR;
"string"										        return STRING;
"const"                 								return CONST;
"void"                 									return VOID;

"false"										        	{yylval.boolValue = 0 ; return FALSE;}
"true"										        	{yylval.boolValue = 1 ; return TRUE;}

"default"												return DEFAULT;	

[0-9]+											        {yylval.intValue = atoi(yytext); return INT;}
[0-9]+\.[0-9]+									    {yylval.floatValue = atof(yytext); return FLOAT;}


\".*\"											        {return STRING;}
\'.\'										          	{yylval.charValue = yytext[1]; return CHAR;}
[a-zA-Z][_a-zA-Z0-9]*						  	{yylval.stringValue = strdup(yytext); return IDENTIFIER; }	
[\t  ]+											        /* ignore whitespace */;
[\n]											          {}


%% 

int yywrap (void) {
	return 1;
}

