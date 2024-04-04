/*Compile with: gcc -o ParticleSim main.c additionalFunctions.c -lm; ./ParticleSim */
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "additionalFunctions.h"
#define PI 3.14159265


main() {
   /*Flow parameters*/
    double A=5; //Inertia parameter
    double W=0.5; //Settling velocity parameter
    double U_0=1.5; //[m/s] //Max flow speed
    double R=0.3; //Mass ration
    double L1=2*PI, L2=2*PI; //[m] //Domain size

  /*Simulation Parameters*/
    double end_time = 15; //Choose the duration //Don't choose it to long, because the file will be huge
    double dt = 0.01; //Choose the timestep //The factor 0.01 should be quite precise. Factor 0.1 will work somehow
    //I don't know why "0.01/U_0" do not work. Something with allocation
    int time_steps = end_time/dt; // The number of time steps
    int N = 16; //The number of particles

  /*Defining particles' initial coordinates*/
    double* y1_row = allocateDoubleArray(N*time_steps); //Particles' coordinates allocation in an effecient way
    double** y1 = allocateDoubleArray_rows(N, time_steps, y1_row);
    double* y2_row = allocateDoubleArray(N*time_steps);
    double** y2 = allocateDoubleArray_rows(N, time_steps, y2_row);
    initialize_positions(y1, y2, N, L1, L2); //NEEEEEEDS AN IMPROVEMENT!!!!

  /*Defining particles' initial velocities and accelerations*/
    double* v1 = allocateDoubleArray(N); //Defining of the particle's initial speeds and accelerations 
    double* v2 = allocateDoubleArray(N); //calloc function already initialized 0 values everywhere
    double* a1 = allocateDoubleArray(N);
    double* a2 = allocateDoubleArray(N);

  /*Euler scheme solving the differential equation*/ //There is a chance to place it inside a function (?)
    for (int i = 0; i < time_steps; ++i) //iterate over each timestep
    {
      for (int j = 0; j < N; ++j) //iterate over each particle
      {  
        y1[j][i+1] = y1[j][i] + v1[j]*dt;
        y2[j][i+1] = y2[j][i] + v2[j]*dt;

        v1[j] = v1[j] + a1[j]*dt;
        v2[j] = v2[j] + a2[j]*dt;

        a1[j] = A*(U_0*sin(y1[j][i+1])*cos(y2[j][i+1])-v1[j]);
        a2[j] = A*(-U_0*cos(y1[j][i+1])*sin(y2[j][i+1])-v2[j] + W);

        if (y1[j][i+1] > L1) y1[j][i+1] -= L1; //The functionality of a periodic boundary conditions
        if (y1[j][i+1] < 0) y1[j][i+1] += L1;
        if (y2[j][i+1] > L2) y2[j][i+1] -= L2;
        if (y2[j][i+1] < 0) y2[j][i+1] += L2;
      }
    }

    printf("\n.....Calculation complete..... \nParameters used: A=%.2lf, W=%.2lf, U_0=%.1lf \nTime=%.2lf, dT=%.5lf, n.o.TimeSteps=%d\n", A, W, U_0,  end_time, dt, time_steps);

    /*Write the positions to file*/
    writeDataToFile("positions_ParticleSim.csv",N,time_steps,dt,y1,y2);

    /*Clear the space*/
    free(y1); free(y2);
    free(y1_row); free(y2_row);
    free(v1); free(v2);
    free(a1); free(a2);

    return 0;
}