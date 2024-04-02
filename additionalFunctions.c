#include "additionalFunctions.h"

double* allocateDoubleArray(int N) {
    double* x = (double*)calloc(N, sizeof(double));
    if (x == NULL) {
        perror("allocateDoubleArray");
        exit(1);
    }
    return x;
}
