%option noyywrap

%{
    #include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "parser.tab.hpp"
	extern FILE *yyin;
	extern FILE *yyout;
 
	int lineno = 1; // initialize to 1
	void ret_print(char *token_type);
	void yyerror(char*);
%}

%%

"//".* 			{printf("Comment at line %d\n", lineno);}

"var" 			{return VAR;}
"," 			{return COMMA;}
";" 			{return SEMI;}
"("				{return LPAREN;}
")"				{return RPAREN;}
"{"				{return LBRACK;}
"}"				{return RBRACK;}
"if" 			{return IF;}
"then" 			{return THEN;}
"else" 			{return ELSE;}

"-"				{return MINUS;}
"+"				{return PLUS;}
"/"				{return DIV;}
"*"				{return MUL;}
">"				{return MORE;}
"<"				{return LESS;}
"===="			{return EQUALS;}
"="				{return ASSIGN;}

[a-zA-Z]+		{
                    yylval.str = strdup(yytext);
					return ID;
				}
[0-9]+			{
                    yylval.str = strdup(yytext);
                    return CONST;
                }

"\n"			{lineno += 1;}
[ \t\r\f]+

.				{ yyerror("Unrecognized character"); }
