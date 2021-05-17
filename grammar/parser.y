%{
	#include "../src/symtab.cpp"
	#include "../src/compiler.cpp"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	extern FILE *yyin;
	extern FILE *yyout;
	extern int lineno;
	extern int yylex();
	void yyerror(char*);
%}

%token VAR
%token COMMA SEMI LPAREN RPAREN LBRACK RBRACK
%token IF ELSE THEN
%token ID CONST
%token MINUS PLUS MUL DIV LESS MORE ASSIGN EQUALS

%start program

%%

program: declarations statements ;

declarations: VAR var_list SEMI ;

var_list: ID | ID COMMA var_list ;

statements: op_list ;

op_list: operator | operator op_list ;

operator: assignment | advanced_op ;

assignment: ID ASSIGN statement SEMI ;

statement: MINUS substatement | substatement ;

substatement: LPAREN statement RPAREN | operand | substatement bin_op substatement ;

bin_op: MINUS | PLUS | MUL | DIV | LESS | MORE | EQUALS ;

operand: ID | CONST ;

advanced_op: IF statement THEN operator | IF statement THEN operator ELSE operator
			| combined_operator ;
			
combined_operator: LBRACK op_list RBRACK

%%
