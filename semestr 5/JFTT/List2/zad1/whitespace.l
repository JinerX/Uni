%{

#include <stdio.h>
int words = 0;
int lines = 0;

%}

ENDLINE_WHITESPACE    [ \t]+$
STARTLINE_WHITESPACE  ^[ \t]+
MULTIPLE_WHITESPACE   [ \t]+
EMPTY_LINE            ^[ \t]*\n
WORD                  [^ \t\n]+

%%
{EMPTY_LINE}           { }
\n                     {printf("\n"); lines++; }
{STARTLINE_WHITESPACE} { }
{ENDLINE_WHITESPACE}   { }
{MULTIPLE_WHITESPACE}  {printf(" ");}
{WORD}                 {printf("%s", yytext); words++; }
%%

int yywrap() {}

int main() {
    yylex();
    printf("===============================\n");
    printf("words: %d, lines: %d", words, lines);
    return 0;
}