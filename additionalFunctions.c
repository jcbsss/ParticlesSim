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

void initialize_positions_single(double* y1_current, double* y2_current, int N, int N_1, double L1, double L2) {
    int N_2 = N / N_1; // Number of particles in direction 2
    double dy_1 = L1 / N_1; // Spacing along the x-axis
    double dy_2 = L2 / N_2; // Spacing along the y-axis

    for (int i = 0; i < N_1; i++) {
        for (int j = 0; j < N_2; j++) {
            int part_index = i * N_2 + j; // Calculating the particle index
            y1_current[part_index] = dy_1 * (0.5 + i);
            y2_current[part_index] = dy_2 * (0.5 + j);
        }
    }
}

void calculate_accelerations(int field, double* a1, double* a2, double v1, double v2, double y1, double y2, double U_0, double A, double R, double W) {
    switch (field){
        case 1: //The main formula for the purpose of this tutorial. The velocity field is called Taylor-Green circulations
        *a1 = A*(U_0*sin(y1)*cos(y2)-v1);
        *a2 = A*(-U_0*cos(y1)*sin(y2)-v2 - W);
        break;

        case 2: //The formula derived in maxey 1987 paper for the given velocity field
        *a1 = A * (-v1 + sin(y1) * cos(y2)
                + 0.5 * R / A * (v1 * cos(y1) * cos(y2) - v2 * sin(y1) * sin(y2))
                + R / A * sin(y1) * cos(y1));
        *a2 = A * (-v2 - sin(y2) * cos(y1) - W
                + 0.5 * R / A * (v1 * sin(y1) * sin(y2) - v2 * cos(y1) * cos(y2))
                + R / A * sin(y2) * cos(y2));
        break;   
    }
}
