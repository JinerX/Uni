#include <float.h>
#include <stdio.h>

int main() {
    printf("Float32 max: %.20e\n", FLT_MAX);
    printf("Float64 max: %.20e\n", DBL_MAX);
    printf("Float32 eps: %.20e\n", FLT_EPSILON);
    printf("Float64 eps: %.20e\n", DBL_EPSILON);
    return 0;
}