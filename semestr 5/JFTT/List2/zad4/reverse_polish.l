%{
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#define MAX_STACK 1024
#define ERROR_SIZE 256
static int stack[MAX_STACK];
static int sp = 0;
static char error_message[ERROR_SIZE];
static int error_flag = 0;

static void stack_clear() {sp = 0;}

static int stack_push(int element) {
    if (sp >= MAX_STACK) {
        error_flag = 1;
        snprintf(error_message, sizeof(error_message), "Error: too many elements in stack");
        return -1;
    }
    stack[sp] = element;
    sp ++;
    return 0;
}

static int stack_pop() {
    if (sp <= 0) {
        error_flag = 1;
        snprintf(error_message, sizeof(error_message), "Error: attempted to pop an empty stack");
        return -1;
    }
    sp --;
    return stack[sp];
}

static int int_pow(int base, int exp) {
    int result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

static int apply_operation(int el2, int el1, char op) {
    int r;
    switch (op)
    {
    case '+':
        r = el1 + el2;
        break;
    case '-':
        r = el1 - el2;
        break;
    case '*':
        r = el1 * el2;
        break;
    case '%':
        if (el2 == 0) {
            snprintf(error_message, sizeof(error_message), "Error: attempted module over 0");
            error_flag = 1;
            return -1;
        }
        r = el1 % el2;
        break;
    case '/':
        if (el2 == 0) {
            error_flag = 1;
            snprintf(error_message, sizeof(error_message), "Error: attempted division over 0");
            
            return -1;
        }
        r = el1 / el2;
        break;
    case '^':
        if (el2 < 0) {
            error_flag = 1;
            snprintf(error_message, sizeof(error_message), "Error: attempted to exponentiate with negative exponent");
            return -1;
        }
        r = int_pow(el1,el2);
        break;
    default:
        error_flag = 1;
        snprintf(error_message, sizeof(error_message), "Error: unknown operator: %c", op);
        return -1;
    }
    return r;
}
%}

NUMBER    -?[0-9]+
OPERATION [\^%\*/\+-]
WS        [ \t]+

%%
{WS} { }
{NUMBER} {
    if (error_flag != 0) {
        
    }
    else {
        errno = 0;
        char *endptr;
        long val_l = strtoll(yytext, &endptr, 10);
        if (errno == ERANGE || val_l < INT_MIN || val_l > INT_MAX) {
            snprintf(error_message, sizeof(error_message), "Error: passed value out of range");
            error_flag = 1;
            return 0;
        }
        else {
            int val_i = (int)val_l;
            stack_push(val_i);
        }
    }
}
{OPERATION} {
    if (error_flag != 0) { }
    else {
        if (sp < 2) {
            snprintf(error_message, sizeof(error_message), "Error: too few elements on the stack for operation");
            error_flag = 1;
        }
        else {
            int a = stack_pop();
            int b = stack_pop();
            int r = apply_operation(a,b, yytext[0]);
            if (error_flag != 0) { }
            else {stack_push(r); }
        }
    }
}

\n {
    if (error_flag != 0) {
        printf("%s\n", error_message);
        stack_clear();
        error_flag = 0;
    }
    else if (sp != 1) {
        printf("Error: passed too many operands\n");
        stack_clear();
    }
    else {
        int final = stack_pop();
        printf("%d\n", final);
        stack_clear();
    }
}
\\exit {return 0;}

%%

int yywrap() {}

int main() {
    yylex();
    return 0;
}