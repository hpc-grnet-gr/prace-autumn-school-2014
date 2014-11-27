#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

void init_vec(double *x, int length)
{
  int i;
  for(i=0; i<length; i++)
    x[i] = 1.0;
}

void print_vec(double *x, int length)
{
  int i;
  for(i=0; i<length; i++)
    printf("x[%d] = %f\n",i,x[i]);
}

int main(int argc, char **argv){

  MPI_Init(&argc, &argv);
  int i,j;
  double start,finish; //placeholders for timing
  /* Get size of matrix from the command line. We assume square */
  int N = atoi (argv[1]);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  /* Make sure load imbalance is only +-1! */
  int chunk = N/size + (((N%size)>rank) ? 1 : 0 );

  /* Initialize local matrices */
  // You will need the following:
  // - per process one segment of matrix A (hint: how large is this segment?)
  double *A;
  A = (double *)malloc(N*chunk*sizeof(double));
  init_vec(A,N*chunk);
  // - per process the complete vector x
  double *x;
  x = (double *)malloc(N*sizeof(double));
  init_vec(x,N);
  // - per process a segment of the result vector b (how large?)
  // - per process the complete vector x
  double *b;
  b = (double *)malloc(chunk*sizeof(double));
  // - probably also some global buffer to hold the result? Or can you re-use x after the calculation is finished?
  double *b_all;
  b_all = (double *)malloc(N*sizeof(double));
  
  if(rank==0) { start = MPI_Wtime(); }

  /* for loop for the local matrix-vector product */
  for (i=0; i<chunk; i++){
    b[i] = 0.0;
    for (j=0; j<N; j++){
      // Write here the multiplication ...
      b[i] += A[i*N+j]*x[j]; 
    }
  }
  // After the calculation is finished gather the results and then measure the time it took
  MPI_Allgather(b, chunk, MPI_DOUBLE, b_all, chunk, MPI_DOUBLE, MPI_COMM_WORLD);

  if(rank==0) { finish = MPI_Wtime(); }
  if(rank==0) { printf("time for b(%d) = A(%d,%d)*x(%d) with %d mpi-processes and %d openmp threads is %f s\n", N , N , N , N , size , omp_get_max_threads(), finish - start); }
  // silly control of result
  if(rank==0) printf("b_all[1] = %d = %d\n", N-1+10, b_all[1]);

  // free up memory and exit
  MPI_Finalize();
  return 0;
}
