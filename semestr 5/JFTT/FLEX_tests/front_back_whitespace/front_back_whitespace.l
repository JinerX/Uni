%{
#include <stdio.h>
%}

%array


FRONT_WHITESPACE    ^[ \t]+
END_WHITESPACE      [ \t]+$


/*
MULTIPLE_WHITESPACE [ \t]+
*/

MULTIPLE_WHITESPACE [^ \t\n][ \t]+[^ \t\n]

/*
*/

%%

{FRONT_WHITESPACE}    { }
{END_WHITESPACE}      { }
{MULTIPLE_WHITESPACE} {printf("%c %c", yytext[0], yytext[yyleng-1]);}



%%

int yywrap() {}

int main() {
    yylex();
    return 0;
}