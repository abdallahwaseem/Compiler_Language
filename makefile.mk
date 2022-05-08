lex:
	flex lex.l

parse: 
	bison yacc.y -d

parse_error:
	bison yacc.y -v


compile:
	gcc lex.yy.c yacc.tab.c -o a.out

try : 
	./a.out
all:
	flex lex.l
	bison yacc.y -d
	gcc lex.yy.c yacc.tab.c -o a.out
	./a.out