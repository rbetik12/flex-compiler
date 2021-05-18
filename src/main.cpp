#include <stdio.h>
#include <cstdlib>

extern int lineno;
extern FILE* yyin;
extern FILE* yyout;

int yyparse();

void yyerror (char* str) {
    fprintf(stderr, "Syntax error at line %d\n", lineno);
    exit(1);
}

int main (int argc, char *argv[]) {
    if (argc < 2) {
        fputs("compiler <path to file>", stderr);
        exit(1);
    }
    int flag;
    yyin = fopen(argv[1], "r");
    flag = yyparse();
    fclose(yyin);

    return flag;
}