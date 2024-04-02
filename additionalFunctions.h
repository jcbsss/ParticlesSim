#ifndef _ADDITIONAL_FUNCTIONS_H_    
#define _ADDITIONAL_FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double* allocateDoubleArray(int N);
/*Allocates the array of doubles*/

void writeDataToFile(const char *filename, int time_steps, double dt, double *y1, double *y2);
/*Prints it to file*/

#endif