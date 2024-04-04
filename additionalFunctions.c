#include "additionalFunctions.h"

double* allocateDoubleArray(int N) {
    double* x = (double*)calloc(N, sizeof(double));
    if (x == NULL) {
        perror("allocateDoubleArray");
        exit(1);
    }
    return x;
}

double** allocateDoubleArray_rows(int N1, int N2, double* x_row) {
    // N1- number of rows   N2-length of each row
    double** x = (double**)calloc(N1, sizeof(double*));
    if (x == NULL) {
        perror("allocateDoubleArray_rows");
        exit(1);
    }
    for (int i = 0; i < N1; i++) x[i] = &x_row[N2 * i];
    return x;
}

void writeDataToFile(const char *filename, int time_steps, double dt, double **y1, double **y2) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return;
    }

    // Write the header row
    fprintf(file, "time[s],y1[m],y2[m]\n");

    // Write the data rows
    for (int i = time_steps/5*1; i < time_steps; i++) {
        fprintf(file, "%.3f,%.6f,%.6f\n", i*dt, y1[9][i], y2[9][i]);
    }

    // Close the file
    fclose(file);

    printf("Data written to %s\n\n", filename);
}