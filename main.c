/*Compile with: gcc -o ParticleSim main.c additionalFunctions.c*/
//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "additionalFunctions.h"

main() {
    double end_time = 1.5; //Choose the duration
    double dt = 0.05; //Choose the timestep
    int time_steps = end_time/dt; // Calculate the number of time steps
    

double* x = allocateDoubleArray(113);

    for (int i = 1; i < time_steps; ++i)
  {
    printf("%d ", i);
  }
  
    printf("\nYou're welcome to this new project %lf %lf %i \n", end_time, dt, time_steps);
    free(x);
    return 0;
}