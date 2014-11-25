#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

using namespace std;

const int dim1 = 1000;
const int dim2 = 1000;
const int dim3 = 1000;

double a[dim1][dim2];
double b[dim2][dim3];
double c[dim1][dim3];

int main(int argc, char* argv[])
{
    // Initialize mpi environment
    int taskid,numtasks;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Status status;

    int numworkers = numtasks-1;
    int source, dest, nbytes, mtype,
      intsize = sizeof(int), dbsize = sizeof(double),
      rows, averow, extra, offset; 

    srand(86456);
    double maxr = (double)RAND_MAX;
    if (taskid == MASTER) {
        // Initialize buffers
        for (int i = 0; i < dim1; ++i)
            for (int j = 0; j < dim2; ++j)
                a[i][j] = rand()/maxr;
        for (int i = 0; i < dim2; ++i)
            for (int j = 0; j < dim3; ++j)
                b[i][j] = rand()/maxr;
        for (int i = 0; i < dim1; ++i)
            for (int j = 0; j < dim3; ++j)
                c[i][j] = 0.0;
        double start = MPI_Wtime();
        // distribute matrix A and send part(A),B to worker tasks
        averow = dim1/numworkers;
        extra = dim1%numworkers;
        offset = 0;
        for (dest=1; dest<=numworkers; dest++) 
        {
            rows = (dest <= extra) ? averow+1 : averow;   	
            printf("Sending %d rows to process %d\n",rows,dest);
            MPI_Send(&offset      ,         1,    MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&rows        ,         1,    MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&a[offset][0], rows*dim2, MPI_DOUBLE, dest, FROM_MASTER, MPI_COMM_WORLD);
            MPI_Send(&b           , dim2*dim3, MPI_DOUBLE, dest, FROM_MASTER, MPI_COMM_WORLD);
            offset = offset + rows;
        }
        // collect results from all workers
        for (source=1; source<=numworkers; source++) 
        {			
            MPI_Recv(&offset      ,         1,    MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows        ,         1,    MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, &status);
            printf("Process %d just sent back %d rows of matrix C from %d onward\n", source, rows, offset);
            MPI_Recv(&c[offset][0], rows*dim3, MPI_DOUBLE, source, FROM_WORKER, MPI_COMM_WORLD, &status);
        }
        // timer stops and print result
        double finish = MPI_Wtime();
        printf("time for C(%d,%d) = A(%d,%d) B(%d,%d) is %f s\n", dim1, dim3, dim1, dim2, dim2, dim3, finish - start);	
    }  // end of master task
    else
    { // section of worker tasks
        // receive part(A) and B
        source = MASTER;
        MPI_Recv(&offset,         1,    MPI_INT, source, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows  ,         1,    MPI_INT, source, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&a     , rows*dim2, MPI_DOUBLE, source, FROM_MASTER, MPI_COMM_WORLD, &status);
        MPI_Recv(&b     , dim2*dim3, MPI_DOUBLE, source, FROM_MASTER, MPI_COMM_WORLD, &status);
        // compute
        for (int i=0; i<rows; ++i) { // <=== Notice I am using rows and not dim1
            for (int k=0; k<dim2; ++k) 
                for (int j=0; j<dim3; ++j)
                    c[i][j] += a[i][k] * b[k][j];
        }
        // return results
        MPI_Send(&offset,         1,    MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(&rows  ,         1,    MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(&c     , rows*dim3, MPI_DOUBLE, MASTER, FROM_WORKER, MPI_COMM_WORLD);
    }  // end of worker task
    MPI_Finalize();
    return 0;
}

