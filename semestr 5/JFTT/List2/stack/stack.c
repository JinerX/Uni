#include <stdio.h>
#include <string.h>
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

static int int_pow(int a, int b) {
    int result = 1;
    while (b) {
        if (b & 1) result *= a;
        a *= a;
        b >>= 1;
    }
    return result;
}

static int apply_operation(int el1, int el2, char op) {
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
        snprintf(error_message, sizeof(error_message), "Error: unknown operator: %s", op);
        return -1;
    }
    return r;
}


int main() {
    stack_push(1);
    stack_push(5);
    stack_push(10);
    stack_push(20);
    int a;
    while (sp != 0)
    {
        a = stack_pop();
        printf("Popped from stack: %d\n", a);
    }
    
    return 0;
}