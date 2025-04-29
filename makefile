pomi: lex.yy.c y.tab.c
	gcc -g lex.yy.c y.tab.c -o pomi

lex.yy.c: y.tab.c pomi.l
	lex pomi.l

y.tab.c: pomi.y
	yacc -d pomi.y

clean: 
	rm -rf lex.yy.c y.tab.c y.tab.h pomi pomi.dSYM