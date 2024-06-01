// #define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include "additionalFunctions.h"
#define PI 3.14159265

int main() {
    /* Variables to store the start and end times */
    clock_t start, end;
    double cpu_time_used;

    /* Start time */
    start = clock();

    /* Flow parameters */
    double A = 5; // Inertia parameter
    double W = 0.5; // Settling velocity parameter
    double U_0 = 1.5; // [m/s] Max flow speed
    double R = 0.95; // Mass ratio
    double L1 = 2 * PI, L2 = 4 * PI; // [m] Domain size

    /* Simulation Parameters */
    double end_time = 100; // Choose the duration
    double dt = 0.05; // Choose the timestep
    int time_steps = end_time / dt; // The number of time steps
    int N = 10000 * 10000; // The number of particles

    /* Defining particles' initial coordinates */
    double* y1_current = allocateDoubleArray(N); // Particles' current coordinates
    double* y2_current = allocateDoubleArray(N);
    double* y1_next = allocateDoubleArray(N); // Particles' next coordinates
    double* y2_next = allocateDoubleArray(N);
    initialize_positions_single(y1_current, y2_current, N, 10000, L1, L2); // Initialize current positions

    /* Defining particles' initial velocities and accelerations */
    double* v1 = allocateDoubleArray(N); // Defining the particles' initial speeds and accelerations 
    double* v2 = allocateDoubleArray(N); // calloc function already initialized 0 values everywhere
    double* a1 = allocateDoubleArray(N);
    double* a2 = allocateDoubleArray(N);

    double total_v2_sum = 0.0; // To accumulate the sum of v2 for average calculation

    /* Euler scheme solving the differential equation */
    for (int i = 0; i < time_steps; ++i) // Iterate over each timestep
    {
        #pragma omp parallel for reduction(+:total_v2_sum) schedule(static)
        for (int j = 0; j < N; ++j) // Iterate over each particle
        {  
            y1_next[j] = y1_current[j] + v1[j] * dt;
            y2_next[j] = y2_current[j] + v2[j] * dt;

            v1[j] = v1[j] + a1[j] * dt;
            v2[j] = v2[j] + a2[j] * dt;

            calculate_accelerations(2, &a1[j], &a2[j], v1[j], v2[j], y1_next[j], y2_next[j], U_0, A, R, W);

            if (y1_next[j] > L1) y1_next[j] -= L1; // Periodic boundary conditions
            if (y1_next[j] < 0) y1_next[j] += L1;
            if (y2_next[j] > L2) y2_next[j] -= L2;
            if (y2_next[j] < 0) y2_next[j] += L2;

            total_v2_sum += v2[j]; // Accumulate the sum of v2
        }

        // Swap current and next positions for the next timestep
        double* temp_y1 = y1_current;
        double* temp_y2 = y2_current;
        y1_current = y1_next;
        y2_current = y2_next;
        y1_next = temp_y1;
        y2_next = temp_y2;
    }

    double average_v2 = total_v2_sum / (N * time_steps); // Calculate the overall average velocity v2

    printf("\n.....Calculation complete..... \nParameters used: A=%.2lf, W=%.2lf, U_0=%.1lf \nTime=%.2lf, dT=%.5lf, n.o.TimeSteps=%d\n", A, W, U_0, end_time, dt, time_steps);
    printf("Overall average velocity v2: %f\n", average_v2);

    /* End time */
    end = clock();

    /* Calculate the time used */
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %f s \n", cpu_time_used);

    /* Clear the space */
    free(y1_current); free(y2_current);
    free(y1_next); free(y2_next);
    free(v1); free(v2);
    free(a1); free(a2);

    return 0;
}
