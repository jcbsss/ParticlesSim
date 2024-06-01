/* Compile with: gcc -o ParticleSim main.c additionalFunctions.c -lm -fopenmp -O3; ./ParticleSim */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "additionalFunctions.h"
#define PI 3.14159265

inline void apply_periodic_boundary(double* y1, double* y2, double L1, double L2) {
    if (*y1 > L1) {
        *y1 -= L1;
    } else if (*y1 < 0) {
        *y1 += L1;
    }
    if (*y2 > L2) {
        *y2 -= L2;
    } else if (*y2 < 0) {
        *y2 += L2;
    }
}

int main() {
    /* Variables to store the start and end times */
    double start, end;
    double cpu_time_used;

    /* Start time */
    start = omp_get_wtime();

    /* Flow parameters */
    double A = 5; // Inertia parameter
    double W = 2; // Settling velocity parameter
    double U_0 = 1.5; // [m/s] Max flow speed
    double R = 0.95; // Mass ratio
    double L1 = 2 * PI, L2 = 4 * PI; // [m] Domain size

    /* Simulation Parameters */
    double end_time = 20; // Choose the duration
    double dt = 0.05; // Choose the timestep
    int time_steps = end_time / dt; // The number of time steps
    int N = 1000*1000; // The number of particles

    /* Defining particles' initial coordinates */
    double* y1_row = allocateDoubleArray(N * time_steps); // Particles' coordinates allocation in an efficient way
    double** y1 = allocateDoubleArray_rows(N, time_steps, y1_row);
    double* y2_row = allocateDoubleArray(N * time_steps);
    double** y2 = allocateDoubleArray_rows(N, time_steps, y2_row);
    initialize_positions(y1, y2, N, 1000, L1, L2); // Needs improvement

    /* Defining particles' initial velocities and accelerations */
    double* v1 = allocateDoubleArray(N); // Defining the particles' initial speeds and accelerations 
    double* v2 = allocateDoubleArray(N); // calloc function already initialized 0 values everywhere
    double* a1 = allocateDoubleArray(N);
    double* a2 = allocateDoubleArray(N);

    /* Euler scheme solving the differential equation */
    for (int i = 0; i < time_steps; ++i) // Iterate over each timestep
    {
        #pragma omp parallel for schedule(guided) //schedule (static/dynamic/guided) for better performance
        for (int j = 0; j < N; ++j) // Iterate over each particle
        {  
            y1[j][i+1] = y1[j][i] + v1[j] * dt;
            y2[j][i+1] = y2[j][i] + v2[j] * dt;

            v1[j] = v1[j] + a1[j] * dt;
            v2[j] = v2[j] + a2[j] * dt;

            calculate_accelerations(2, &a1[j], &a2[j], v1[j], v2[j], y1[j][i+1], y2[j][i+1], U_0, A, R, W);
           /* The first variable in a function is a choice of velocity field, based on which, the acceleration is calculated. Choose wisely:
           1- Taylor-Green circulations
           2- More advanced Taylor-Green circulations (from maxey1987 article)
           3- TBC... Some turbulent field */

            apply_periodic_boundary(&y1[j][i+1], &y2[j][i+1], L1, L2); // Apply periodic boundary conditions
        }
    }

    /* End time */
    end = omp_get_wtime();

    /* Calculate the time used */
    cpu_time_used = end - start;

    printf("\n.....Calculation complete..... \nParameters used: A=%.2lf, W=%.2lf, U_0=%.1lf \nTime=%.2lf, dT=%.5lf, n.o.TimeSteps=%d\nExecution time: %f s \n", A, W, U_0, end_time, dt, time_steps, cpu_time_used);

    /* Write the positions to file */
    // writeDataToFile("positions_ParticleSim.csv", N, time_steps, dt, y1, y2);

    /* Clear the space */
    free(y1); free(y2);
    free(y1_row); free(y2_row);
    free(v1); free(v2);
    free(a1); free(a2);

    return 0;
}
