#include <stdlib.h>
#include <stdio.h>
#include <mpi.h> /* Include MPI header file */
#include <omp.h> /* Include OMP header file */

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
  int rank, size, n, i, info;
  double *x, *y, *buff;
  n = atoi(argv[1]); /* Get input size */
  
  /* Initialize threaded MPI environment */
  MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &info);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); /* Get my rank (MPI ID) */
  MPI_Comm_size(MPI_COMM_WORLD, &size); /* Find out how many MPI procs */
  
  // initialize buffer array to 2*n length on master
  if ( rank == 0 )
  {
    buff=calloc( 2*n , sizeof(double));
    init_vec(buff,2*n);
  }
  int chunk = n / size; /* Assume size divides n exactly */
  // initialize distributed vector placeholders
  x=calloc(chunk , sizeof(double));
  y=calloc(chunk , sizeof(double));
  
  MPI_Scatter(buff,chunk,MPI_DOUBLE,x,chunk,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Scatter(&buff[n],chunk,MPI_DOUBLE,y,chunk,MPI_DOUBLE,0,MPI_COMM_WORLD);
  
  MPI_Barrier(MPI_COMM_WORLD);
  #pragma omp parallel for private(i, chunk) shared(x,y)
  for (i=0; i<chunk; i++){ x[i] = x[i] + y[i]; }
  #pragma omp barrier
  
  MPI_Gather(x,chunk,MPI_DOUBLE,buff,chunk,MPI_DOUBLE,0,MPI_COMM_WORLD);
  if ( rank == 0 )
    print_vec(buff,n);

  MPI_Finalize(); 
  return 0;
}

