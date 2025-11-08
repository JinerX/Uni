#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 1) {
        printf("arguments have been passed!\n");
        if (strcmp(argv[1], "-s") == 0) {
            printf("The argument is -s\n");
        }
    }
}
