#include <stdio.h>
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