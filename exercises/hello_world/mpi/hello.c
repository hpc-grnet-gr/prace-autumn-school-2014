#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){

	/* Initialize threaded MPI */
	MPI_Init_thread(&argc, &argv, THREAD_MODE(?), &info);
	
	/* Find and print out thread mode (from info) */
	MPI_Comm_rank(___
	MPI_Comm_size(___
	printf("My rank is %d\n");

	/* Get number of requested threads from argv[1] */
  	/* Set number of threads omp_set_num_threads(); */
	
	/* Start a parallel region: use #pragma omp parallel */
	{
	    //	Each thread print its id (so get it!)
	}
	/* Finalize mpi */
}

