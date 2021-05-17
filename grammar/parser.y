%{
	#include "../src/main.cpp"
	#include "../src/compiler.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	extern FILE *yyin;
	extern FILE *yyout;
	extern int lineno;
	extern int yylex();
	extern char* yyval;
	void yyerror(char*);
%}

%token VAR
%token COMMA SEMI LPAREN RPAREN LBRACK RBRACK
%token IF ELSE THEN
%token ID CONST
%token MINUS PLUS MUL DIV LESS MORE ASSIGN EQUALS

%start program

%right ASSIGN
%left PLUS MINUS
%left MUL DIV

%union {
	struct Ast* tree;
	char* str;
}

%type<tree> program declarations statements var_list op_list operator statement substatement
%type<str> ID CONST
%%

program: declarations statements { $$ = ast_node("root", $1, $2); }
	;

declarations: VAR var_list SEMI { $$ = ast_node("Var Decl", new Ast("var"), $2); }
	;

var_list:
	ID { $$ = add_variable($1, 0); }
	| ID COMMA var_list { $$ = add_variable($1, $3); }
	;

statements: op_list { $$ = ast_node("Program body", $1, nullptr); }
	;

op_list: operator { $$ = $1; }
	| operator op_list { $$ = ast_node("Operators", $1, $2); }
	;

operator: ID ASSIGN statement SEMI { $$ = add_assignment($1, $3); }
 	| IF statement THEN operator { $$ = add_flow("If", $2, $4, nullptr); }
 	| IF statement THEN operator ELSE operator { $$ = add_flow("IfElse", $2, $4, $6); }
 	| LBRACK op_list RBRACK { $$ = $2; }
 	;

statement: MINUS substatement { $$ = ast_node("U", new Ast("-"), $2); }
	 | substatement { $$ = $1; }
	 ;

substatement: LPAREN statement RPAREN { $$ = $2; }
	    | ID { $$ = get_variable($1); }
	    | CONST { $$ = get_constant($1); }
	    | substatement MINUS substatement { $$ = ast_node("-", $1, $3); }
	    | substatement PLUS substatement { $$ = ast_node("+", $1, $3); }
	    | substatement MUL substatement { $$ = ast_node("*", $1, $3); }
	    | substatement DIV substatement { $$ = ast_node("/", $1, $3); }
	    | substatement LESS substatement { $$ = ast_node("<", $1, $3); }
	    | substatement MORE substatement { $$ = ast_node(">", $1, $3); }
	    | substatement EQUALS substatement { $$ = ast_node("====", $1, $3); }
	    ;

%%
