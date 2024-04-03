/*Compile with: gcc -o ParticleSim main.c additionalFunctions.c; ./ParticleSim */
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
    double U_0=1500; //[m/s] //Max flow speed
    double R=0.3; //Mass ration
    double L1=1*PI, L2=2*PI; //[m] //Domain size

  /*Simulation Parameters*/
    double end_time = 15; //Choose the duration //Don't choose it to long, because the file will be huge
    double dt =0.01/U_0; //Choose the timestep //The factor 0.01 should be quite precise. Factor 0.1 will work somehow
    int time_steps = end_time/dt; // The number of time steps

 /*Defining particle's coordinates and movement*/
    double* y1 = allocateDoubleArray(time_steps); //Particle's coordinates allocation
    double* y2 = allocateDoubleArray(time_steps);
    *y1 = 2; //Defining of the particle's initial coordinates //Need some cool algorithm to place it randomly or uniformly
    *y2 = 2;
    double v1=0., v2=0., a1=0., a2=0.; //Defining of the particle's initial speeds and accelerations //usually 0

  /*Euler scheme solving the differential equation*/ //There is a chance to place it inside a function
    for (int i = 0; i < time_steps; ++i)
    {
      y1[i+1] = y1[i] + v1*dt;
      y2[i+1] = y2[i] + v2*dt;

      v1 = v1 + a1*dt;
      v2 = v2 + a2*dt;

      a1 = A*(U_0*sin(y1[i+1])*cos(y2[i+1])-v1);
      a2 = A*(-U_0*cos(y1[i+1])*sin(y2[i+1])-v2 + W);
    }
    printf("\n.....Calculation complete..... \nParameters used: A=%.2lf, W=%.2lf, U_0=%.1lf \nTime=%.2lf, dT=%.5lf, n.o.TSteps=%d\n", A, W, U_0,  end_time, dt, time_steps);



    /*Write to file the positions*/
    writeDataToFile("positions_ParticleSim.csv",time_steps,dt,y1,y2);
    /*Clear the space*/
    free(y1);
    free(y2);
    return 0;
}