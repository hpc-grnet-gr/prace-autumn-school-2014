#include <stdlib.h>
#include <stdio.h>
#include <mpi.h> /* Include MPI header file */

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
  int rank, size, n, i;
  double *x, *y, *buff;
  n = atoi(argv[1]);  /* Get input size */
  
  MPI_Init(&argc, &argv);               /* Initialize parallel environment */
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); /* Get my rank (MPI ID) */
  MPI_Comm_size(MPI_COMM_WORLD, &size); /* Find out how many procs */
  chunk = n / size; /* Assume size divides n exactly */
  //...
  //...
  //...
  MPI_Scatter(buff,chunk,MPI_DOUBLE,x,chunk,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Scatter(&buff[n],chunk,MPI_DOUBLE,y,chunk,MPI_DOUBLE,0,MPI_COMM_WORLD);
  
  for (i=0; i<chunk; i++) x[i] = x[i] + y[i];
  MPI_Gather(x,chunk,MPI_DOUBLE,buff,chunk,MPI_DOUBLE,0,MPI_COMM_WORLD);
  
  MPI_Finalize();
  return 0;
}
