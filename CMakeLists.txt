cmake_minimum_required(VERSION 3.17)
project(compilers)

set(CMAKE_CXX_STANDARD 17)

add_executable(compilers src/compiler.h src/compiler.cpp grammar/parser.tab.cpp
        grammar/parser.tab.h grammar/lex.yy.cpp)