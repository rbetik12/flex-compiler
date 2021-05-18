NAME=flex-compiler
CC=g++
CC_FLAGS=-std=c++17
all: build-compiler

build-compiler: parser.tab.cpp lexer.yy.cpp compiler.o
	$(CC) $(CC_FLAGS) -o $(NAME) parser.tab.cpp lex.yy.cpp compiler.o

parser.tab.cpp:
	bison -d -o parser.tab.cpp parser.y

lexer.yy.cpp:
	flex -o lex.yy.cpp lexer.flex

compiler.o:
	$(CC) $(CC_FLAGS) -c -o compiler.o ../src/compiler.cpp

clean:
	rm -f $(NAME)
	rm -f *.txt
	rm -f *.c
	rm -f *.h
	rm -f *.o
	rm -f *.cpp
	rm -f *.hpp
