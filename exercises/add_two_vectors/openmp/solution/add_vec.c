#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void init_vec(double *x, int length)
{
  int i;
  for ( i=0 ; i < length ; i++)
    x[i] = (double) (i*i - 2*i);
}

void print_vec(double *x, int length)
{
  int i;
  for ( i=0 ; i < length ; i++)
    printf("buff[%d] = %f\n", i, x[i]);
}

int main(int argc, char **argv)
{
  int n, i;
  double *x, *y;
  n = atoi(argv[1]); /* Get input size */
  
  x = (double *)malloc(n*sizeof(double)); init_vec(x,n);
  y = (double *)malloc(n*sizeof(double)); init_vec(y,n);
  
  #pragma omp parallel for private(i) shared (x,y)
  for (i=0; i<n; i++){
  	 x[i] = x[i] + y[i];
  }
  print_vec(x,n);
  return 0;
}

