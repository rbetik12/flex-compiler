NAME=flex-compiler

all: build-compiler

build-compiler: parser.tab.c lexer.yy.c
	g++ -o $(NAME) parser.tab.c lex.yy.c

parser.tab.c:
	bison -d parser.y

lexer.yy.c:
	flex lexer.flex

clean:
	rm -f $(NAME)
	rm -f *.txt
	rm -f *.c
	rm -f *.h
