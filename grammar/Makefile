NAME=flex-compiler
CC=g++
CC_FLAGS=-std=c++17
all: build-compiler

build-compiler: parser.tab.c lexer.yy.c compiler.o
	$(CC) $(CC_FLAGS) -o $(NAME) parser.tab.c lex.yy.c compiler.o

parser.tab.c:
	bison -d parser.y

lexer.yy.c:
	flex lexer.flex

compiler.o:
	$(CC) $(CC_FLAGS) -c -o compiler.o ../src/compiler.cpp

clean:
	rm -f $(NAME)
	rm -f *.txt
	rm -f *.c
	rm -f *.h
	rm -f *.o
