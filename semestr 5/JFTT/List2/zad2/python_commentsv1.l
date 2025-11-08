%{

#include <stdio.h>

%}

EVEN_SLASH           (\\\\)*
UNEVEN_SLASH         (\\\\)*\\

%{
// Match either a normal character (not quote not backslash) or even number of backslash or escaped quote
%}

DOUBLE_QUOTED_STRING            \"([^\"\\\n]|(\\\\)|(\\\"))*\" 
SINGLE_QUOTED_STRING            \'([^\'\\\n]|(\\\\)|(\\\'))*\'
MULTILINE_DOUBLE_QUOTED_STRING  \"\"\"([^\"\\]|(\\\\)|(\\\"))*\"\"\" 
MULTILINE_SINGLE_QUOTED_STRING  \'\'\'([^\'\\]|(\\\\)|(\\\'))*\'\'\'
COMMENT                         #.*


%%
{DOUBLE_QUOTED_STRING}           {printf("found double string!: %s\n", yytext);}
{SINGLE_QUOTED_STRING}           {printf("found single string!: %s\n", yytext);}
{MULTILINE_DOUBLE_QUOTED_STRING} {printf("triple double string!: %s\n", yytext);}
{MULTILINE_SINGLE_QUOTED_STRING} {printf("triple single string!: %s\n", yytext);}
{COMMENT}                        {printf("found comment! %s\n", yytext);}
.|\n           { }
%%

int yywrap() {}

int main() {
    yylex();
    return 0;
}