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
{DOUBLE_QUOTED_STRING}           {printf("%s", yytext);}
{SINGLE_QUOTED_STRING}           {printf("%s", yytext);}
{MULTILINE_DOUBLE_QUOTED_STRING} {printf("%s", yytext);}
{MULTILINE_SINGLE_QUOTED_STRING} {printf("%s", yytext);}
{COMMENT}                        { }
%%

int yywrap() {}

int main() {
    yylex();
    return 0;
}