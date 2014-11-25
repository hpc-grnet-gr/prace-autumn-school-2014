#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int dim1 = 1000;
const int dim2 = 1000;
const int dim3 = 1000;

float a[dim1][dim2];
float b[dim2][dim3];
float c[dim1][dim3];

int main()
{
    // Initialize buffers
    srand(86456);
    double maxr = (double)RAND_MAX;
    for (int i = 0; i < dim1; ++i)
        for (int j = 0; j < dim2; ++j)
            a[i][j] = rand()/maxr;
    for (int i = 0; i < dim2; ++i)
        for (int j = 0; j < dim3; ++j)
            b[i][j] = rand()/maxr;
    for (int i = 0; i < dim1; ++i)
        for (int j = 0; j < dim3; ++j)
            c[i][j] = 0.0;

    clock_t start = clock();
    // Compute matrix multiplication.
    for (int i = 0; i < dim1; ++i) {
        for (int k = 0; k < dim2; ++k) {
            for (int j = 0; j < dim3; ++j) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    clock_t finish = clock();

    double time_used = ((double) (finish - start)) / CLOCKS_PER_SEC;
    printf("time for C(%d,%d) = A(%d,%d) B(%d,%d) is %f s\n", dim1, dim3, dim1, dim2, dim2, dim3, time_used);	
    return 0;
}

