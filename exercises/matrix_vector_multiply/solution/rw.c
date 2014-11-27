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

  /* Initialize local matrices to 1.0 */
  double *x;
  x = (double *)malloc(chunk*sizeof(double)); init_vec(x,chunk);
  if(rank==0) x[1] = 100.0; // Add some custom values just to be able to check the integrity of the results
  double *A;
  A = (double *)malloc(chunk*N*sizeof(double)); init_vec(A,chunk*N);
  if(rank==0) A[1] = 0.1; // Add some custom values just to be able to check the integrity of the results
  double *b; // A*x = b <-- results of m/v multiplication go into here

  // On all processes we need the entire vector x; New buffer we name xb
  double *xb;
  xb = (double *)malloc(N*sizeof(double));
  // Collect x parts into xb
  MPI_Allgather(x, chunk, MPI_DOUBLE, xb, chunk, MPI_DOUBLE, MPI_COMM_WORLD);

  if(rank==0) { start = MPI_Wtime(); }

  /* for loop for the local matrix-vector product */
  #pragma omp parallel for private(j) shared (b,N,A,xb) /* (here or in the inner loop?) */
  for (i=0; i<chunk; i++){
    b[i] = 0.0;
    for (j=0; j<N; j++){
      b[i] += A[i*N+j]*xb[j];
    }
  }
  // Vector xb we don't need anymore so we collect the partial results 
  // back into it..
  MPI_Allgather(b, chunk, MPI_DOUBLE, xb, chunk, MPI_DOUBLE, MPI_COMM_WORLD);
  if(rank==0) { finish = MPI_Wtime(); }
  if(rank==0) { printf("time for b(%d) = A(%d,%d)*x(%d) with %d mpi-processes and %d openmp threads is %f s\n", N , N , N , N , size , omp_get_max_threads(), finish - start); }
  // silly control of result
  if(rank==0) printf("xb[1] = %d = %d\n", N-1+10, xb[1]);
  free(xb);
  free(x);
  free(A);
  free(b);
  MPI_Finalize();
  return 0;
}
