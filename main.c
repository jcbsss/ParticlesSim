/*Compile with: gcc -o ParticleSim main.c additionalFunctions.c; ./ParticleSim */
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "additionalFunctions.h"
#define PI 3.14159265

main() {
  /*Simulation Parameters*/
    double end_time = 10.5; //Choose the duration
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
    *y2 = 0;
    double v1=0.1, v2=0., a1=2., a2=0.; //Defining of the particle's initial speeds and accelerations //usually 0

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

    writeDataToFile("positions_ParticleSim.csv",time_steps,dt,y1,y2);

    free(y1);
    free(y2);
    return 0;
}