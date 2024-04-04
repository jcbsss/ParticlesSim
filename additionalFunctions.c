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

void writeDataToFile(const char *filename, int N, int time_steps, double dt, double **y1, double **y2) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return;
    }

    // Write the header row
    fprintf(file, "time");
    for (int j = 0; j < N; j++) {
        fprintf(file, ",y1_part%d,y2_part%d", j+1, j+1);
    }
    fprintf(file, "\n");

    // Write the data rows
    for (int i = 0; i < time_steps; i++) {
        fprintf(file, "%.3f", i * dt);
        for (int j = 0; j < N; j++) {
            fprintf(file, ",%.6f,%.6f", y1[j][i], y2[j][i]);
        }
        fprintf(file, "\n");
    }

    // Close the file
    fclose(file);

    printf("Data written to %s\n\n", filename);
}

void initialize_positions(double** y1, double** y2, int N, double L1, double L2) {
//NEEEEEEDS AN IMPROVEMENT!!!!
    double dx = L1 / (N - 1); // Spacing along the x-axis
    double dy = L2 / (N - 1); // Spacing along the y-axis

    for (int i = 0; i < N; i++) {
        y1[i][0] = i * dx;
        y2[i][0] = i * dy;
    }
} 
