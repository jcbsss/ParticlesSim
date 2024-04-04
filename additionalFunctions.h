#ifndef _ADDITIONAL_FUNCTIONS_H_    
#define _ADDITIONAL_FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double* allocateDoubleArray(int N);
/*Allocates the array of doubles*/

double** allocateDoubleArray_rows(int N1, int N2, double* x_row);
/*If you want more optimal array's use, you can at first allocate the matrix as a row array.
  Use this function to get pointers of every row and so use it as a standard matrix*/

void writeDataToFile(const char *filename, int time_steps, double dt, double *y1, double *y2);
/*Prints it to file*/

#endif