/*Compile with: gcc -o ParticleSim main.c additionalFunctions.c; ./ParticleSim */
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "additionalFunctions.h"
#define PI 3.14159265

main() {
  /*Simulation Parameters*/
    double end_time = 1.5; //Choose the duration
    double dt = 0.05; //Choose the timestep
    int time_steps = end_time/dt; // The number of time steps

  /*Flow parameters*/
    double A=5; //Inertia parameter
    double W=0.5; //Settling velocity parameter
    double U=2; //[m/s] //Max flow speed
    double R=0.3; //Mass ration
    double L1=1*PI, L2=2*PI; //[m] //Domain size

  /*Defining particle's coordinates and movement*/
    double* y1 = allocateDoubleArray(time_steps); //Particle's coordinates allocation
    double* y2 = allocateDoubleArray(time_steps);
    *y1 = 1; //Defining of the particle's initial coordinates //Need some cool algorithm to place it randomly or uniformly
    *y2 = 1;
    double v1=0.3, v2=0.6, a1=0.1, a2=0; //Defining of the particle's initial speeds and accelerations //usually 0

  /*Euler scheme solving the differential equation*/ //There is a chance to place it inside a function
    for (int i = 0; i < time_steps; ++i)
    {
      *(y1+i+1) = *(y1+i) + v1*dt;
      *(y2+i+1) = *(y2+i) + v2*dt;

      v1 = v1 + a1*dt;
      v2 = v2 + a2*dt;

      a1 = 1.01*a1+0.01; //You need to upload the real formula here
      a2 = A*(U-v2 + W); //real formula, this is incomplete. You need some maths, I guess
    }


      //printf("y1 = %lf y2 = %lf  v1 = %lf v2 = %lf   a1 = %lf a2 = %lf \n", *(y1 + i), *(y2 + i), v1, v2, a1, a2); //Optional printing
      /*You can put it into some function. CHECK IT*/
      // Open the file for writing
    FILE *file = fopen("positions_sParticleSim.csv", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Write the header row
    fprintf(file, "time[s],y1[m],y2[m]\n");

    // Write the data rows
    for (int i = 0; i < time_steps; i++) {
        fprintf(file, "%.3f,%.6f,%.6f\n", i*dt, *(y1+i), *(y1+i));
    }

    // Close the file
    fclose(file);

    printf("Data written to positions_ParticleSim.csv\n");
    

    free(y1);
    free(y2);
    return 0;
}