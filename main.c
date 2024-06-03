#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include "additionalFunctions.h"
#define PI 3.14159265

inline void periodic_boundary(double* y, double L) { //inline keyword gives visible increase of efficiency!
    if (*y > L) {
        *y -= L;
    } else if (*y < 0) {
        *y += L;
    }
}

// Function to write results to CSV
void write_to_csv(FILE* file, double W, double average_v2) {
    fprintf(file, "%f,%f\n", W, average_v2);
}

int main() {
    /* Variables to store the start and end times */
    double start, end;
    double cpu_time_used;

    /* Start time */
    start = omp_get_wtime();

    /* Flow parameters */
    const double A = 2; // Inertia parameter
    //const double W = 0.5; // Settling velocity parameter
    const double U_0 = 1.5; // [m/s] Max flow speed
    const double R = 0; // Mass ratio
    const double L1 = 2 * PI, L2 = 2 * PI; // [m] Domain size

    /* Simulation Parameters */
    const double end_time = 200; // Choose the duration
    const double dt = 0.01; // Choose the timestep
    const int time_steps = end_time / dt; // The number of time steps
    const int N = 70 * 70; // The number of particles

    /* Defining particles' initial coordinates */
    double* y1_current = (double*)aligned_alloc(64, N * sizeof(double));
    double* y2_current = (double*)aligned_alloc(64, N * sizeof(double));
    double* y1_next = (double*)aligned_alloc(64, N * sizeof(double));
    double* y2_next = (double*)aligned_alloc(64, N * sizeof(double));
    initialize_positions_single(y1_current, y2_current, N, 70, L1, L2); // Initialize current positions

    /* Defining particles' initial velocities and accelerations */
    double* v1 = (double*)aligned_alloc(64, N * sizeof(double)); // Defining the particles' initial speeds and accelerations 
    double* v2 = (double*)aligned_alloc(64, N * sizeof(double)); // alligned_alloc is used istead of calloc for better performance
    double* a1 = (double*)aligned_alloc(64, N * sizeof(double));
    double* a2 = (double*)aligned_alloc(64, N * sizeof(double));

    double total_v2_sum = 0.0; // To accumulate the sum of v2 for average calculation
    int timestep_threshold = 1000; // choose, from which time step start computing the average
    int effective_time_steps = time_steps - timestep_threshold;
        
     // Open CSV file for writing
    FILE* file = fopen("v2_against_W.csv", "w");
    if (file == NULL) {
        perror("Unable to open file");
        return 1;
    }
    fprintf(file, "W,average_v2\n"); // Write CSV header

    // Loop over different values of W
    for (double W = 0.0; W <= 1.75; W += 0.02) {
        double total_v2_sum = 0.0;

        /* Euler scheme solving the differential equation */
        for (int i = 0; i < time_steps; ++i) // Iterate over each timestep
        {
            #pragma omp parallel for reduction(+:total_v2_sum) schedule(guided)
            for (int j = 0; j < N; ++j) // Iterate over each particle
            {  
                y1_next[j] = y1_current[j] + v1[j] * dt;
                y2_next[j] = y2_current[j] + v2[j] * dt;

                v1[j] += a1[j] * dt;
                v2[j] += a2[j] * dt;

                calculate_accelerations(2, &a1[j], &a2[j], v1[j], v2[j], y1_next[j], y2_next[j], U_0, A, R, W);

                periodic_boundary(&y1_next[j], L1);
                periodic_boundary(&y2_next[j], L2);

                /* Only accumulate v2 if the current timestep is beyond the threshold */
                if (i >= timestep_threshold) {
                    total_v2_sum += v2[j]; // Accumulate the sum of v2
                }
            }

            // Swap current and next positions for the next timestep
            double* temp_y1 = y1_current;
            double* temp_y2 = y2_current;
            y1_current = y1_next;
            y2_current = y2_next;
            y1_next = temp_y1;
            y2_next = temp_y2;
        }

        double average_v2 = total_v2_sum / ((long long)N * (long long)effective_time_steps); // Calculate the overall average settling velocity
        write_to_csv(file, W, -average_v2); //Write results to CSV
    }

    fclose(file); // Close the CSV file

    //printf("\n.....Calculation complete..... \nParameters used: A=%.2lf, W=%.2lf, U_0=%.1lf \nTime=%.2lf, dT=%.5lf, n.o.TimeSteps=%d\n", A, W, U_0, end_time, dt, time_steps);
    //printf("Overall average velocity v2: %f\n", average_v2);

    end = omp_get_wtime(); // End time

    cpu_time_used = end - start; //Calcuate and print time used
    printf("Execution time: %f s \n", cpu_time_used);

    /* Clear the space */
    free(y1_current); free(y2_current);
    free(y1_next); free(y2_next);
    free(v1); free(v2);
    free(a1); free(a2);

    return 0;
}
