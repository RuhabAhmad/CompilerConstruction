all:
	bison -d parser.y
	flex scanner.l
	gcc parser.tab.c lex.yy.c ast.c xml.c main.c -o json2xml

clean:
	rm -f json2xml lex.yy.c parser.tab.c parser.tab.h