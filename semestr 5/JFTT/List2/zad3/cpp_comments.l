%{
#include <stdio.h>   
#include <string.h> 
int save_docs = 1;
int debugging = 0;
%}

DOUBLE_QUOTED_STRING            \"([^\"\\\n]|(\\\\)|(\\\"))*\" 
SINGLE_QUOTED_STRING            \'([^\'\\\n]|(\\\\)|(\\\'))*\'
SINGLE_DOCS_COMMENT             \/\/\/.*
SINGLE_DOCS_COMMENT2            \/\/!.*
MULTILINE_DOCS_COMMENT1         \/\*\*([^*]|\*+[^*/])*\*+\/
MULTILINE_DOCS_COMMENT2         \/\*\!([^*]|\*+[^*/])*\*+\/
STANDARD_COMMENT                \/\/.*
MULTILINE_COMMENT               \/\*([^*]|\*+[^*/])*\*+\/


%%
{DOUBLE_QUOTED_STRING}    {printf("%s", yytext);}
{SINGLE_QUOTED_STRING}    {printf("%s", yytext);}
{MULTILINE_DOCS_COMMENT1} {if (save_docs ==1) printf("%s", yytext);}
{MULTILINE_DOCS_COMMENT2} {if (save_docs ==1) printf("%s", yytext);}
{MULTILINE_COMMENT}       {if (debugging == 1) printf("%s", yytext);}           
{SINGLE_DOCS_COMMENT}     {if (save_docs ==1) printf("%s", yytext);}
{SINGLE_DOCS_COMMENT2}    {if (save_docs ==1) printf("%s", yytext);}
{STANDARD_COMMENT}        {if (debugging == 1) printf("%s", yytext);}
\n|.                      {printf("%s", yytext);}
%%
int yywrap() {}

int main(int argc, char** argv) {
    if (argc != 1) {
        if (strcmp(argv[1], "-s") == 0) {
            save_docs = 1;
        }
    }
    yylex();
    
    return 0;
}