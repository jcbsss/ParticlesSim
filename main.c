/*Compile with: gcc -o ParticleSim main.c additionalFunctions.c -lm; ./ParticleSim */
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include "additionalFunctions.h"
#define PI 3.14159265


int main() {
    /* Variables to store the start and end times */
    double start, end;
    double cpu_time_used;

    /* Start time */
    start = omp_get_wtime();

   /*Flow parameters*/
    double A = 5; //Inertia parameter
    double W = 0.5; //Settling velocity parameter
    double U_0 = 1.5; //[m/s] //Max flow speed
    double R = 0.95; //Mass ration
    double L1 = 2 * PI, L2 = 4 * PI; //[m] //Domain size

  /*Simulation Parameters*/
    double end_time =  50; //Choose the duration //Don't choose it to long, because the file will be huge
    double dt = 0.05; //Choose the timestep //The factor 0.01 should be quite precise. Factor 0.1 will work somehow
    //I don't know why "0.01/U_0" do not work. Something with allocation
    int time_steps = end_time/dt; // The number of time steps
    int N = 220*220; //The number of particles

  /*Defining particles' initial coordinates*/
    double* y1_row = allocateDoubleArray(N*time_steps); //Particles' coordinates allocation in an effecient way
    double** y1 = allocateDoubleArray_rows(N, time_steps, y1_row);
    double* y2_row = allocateDoubleArray(N*time_steps);
    double** y2 = allocateDoubleArray_rows(N, time_steps, y2_row);
    initialize_positions(y1, y2, N, 50, L1, L2); //NEEEEEEDS AN IMPROVEMENT!!!!

  /*Defining particles' initial velocities and accelerations*/
    double* v1 = allocateDoubleArray(N); //Defining of the particle's initial speeds and accelerations 
    double* v2 = allocateDoubleArray(N); //calloc function already initialized 0 values everywhere
    double* a1 = allocateDoubleArray(N);
    double* a2 = allocateDoubleArray(N);

  /*Euler scheme solving the differential equation*/ //There is a chance to place it inside a function (?)
    for (int i = 0; i < time_steps; ++i) //iterate over each timestep
    {
      #pragma omp parallel for schedule(guided)
      for (int j = 0; j < N; ++j) //iterate over each particle
      {  
        y1[j][i+1] = y1[j][i] + v1[j]*dt;
        y2[j][i+1] = y2[j][i] + v2[j]*dt;

        v1[j] = v1[j] + a1[j]*dt;
        v2[j] = v2[j] + a2[j]*dt;

        calculate_accelerations(2, &a1[j], &a2[j], v1[j], v2[j], y1[j][i+1], y2[j][i+1], U_0, A, R, W);
        /*The first variable in a function is a choice of velocity field, based on which, the acceleration is calculated. Choose wisely:
        1- Taylor-Green circulations
        2- More advanced Taylor-Green circulations (from maxey1987 article)
        3- TBC... Some turbulent field*/

        if (y1[j][i+1] > L1) y1[j][i+1] -= L1; //The functionality of a periodic boundary conditions
        if (y1[j][i+1] < 0) y1[j][i+1] += L1;
        if (y2[j][i+1] > L2) y2[j][i+1] -= L2;
        if (y2[j][i+1] < 0) y2[j][i+1] += L2;
      }
    }

    printf("\n.....Calculation complete..... \nParameters used: A=%.2lf, W=%.2lf, U_0=%.1lf \nTime=%.2lf, dT=%.5lf, n.o.TimeSteps=%d\n", A, W, U_0,  end_time, dt, time_steps);

    /*Write the positions to file*/
    //writeDataToFile("positions_ParticleSim.csv",N,time_steps,dt,y1,y2);

    end = omp_get_wtime(); // End time

    cpu_time_used = end - start; //Calcuate and print time used
    printf("Execution time: %f s \n", cpu_time_used);

    /*Clear the space*/
    free(y1); free(y2);
    free(y1_row); free(y2_row);
    free(v1); free(v2);
    free(a1); free(a2);

    return 0;
}