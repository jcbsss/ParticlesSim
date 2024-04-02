#include "additionalFunctions.h"

double* allocateDoubleArray(int N) {
    double* x = (double*)calloc(N, sizeof(double));
    if (x == NULL) {
        perror("allocateDoubleArray");
        exit(1);
    }
    return x;
}

void writeDataToFile(const char *filename, int time_steps, double dt, double *y1, double *y2) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return;
    }

    // Write the header row
    fprintf(file, "time[s],y1[m],y2[m]\n");

    // Write the data rows
    for (int i = 0; i < time_steps; i++) {
        fprintf(file, "%.3f,%.6f,%.6f\n", i*dt, *(y1+i), *(y2+i));
    }

    // Close the file
    fclose(file);

    printf("Data written to %s\n", filename);
}