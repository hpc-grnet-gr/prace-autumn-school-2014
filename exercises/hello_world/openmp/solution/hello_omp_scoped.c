#include <stdio.h>
#include <omp.h>

int main ()
{
  int thread_id,num_threads;
  #pragma omp parallel private(num_threads,thread_id)
  {
    num_threads = omp_get_num_threads();
    thread_id = omp_get_thread_num();
    printf( "Hello World! This is thread %d out of %d\n", thread_id, num_threads);
  }
}

