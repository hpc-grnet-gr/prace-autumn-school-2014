#include <stdio.h>
#include <omp.h>

int main ( int argc, char* argv[] )
{
  int i, num_steps = atoi (argv[1]);
  double x, pi, sum = 0.0;
  double start_time, run_time;
  double step = 1.0/(double) num_steps;

  start_time = omp_get_wtime();

  #pragma omp parallel
  {
    #pragma omp for reduction(+:sum) private(x)
    for (i=0; i<num_steps; i++)
    {
      x = ( (double) i + 0.5 ) * step;
      sum = sum + 4.0 / (1.0 + x*x);
    }
  }

  pi = step * sum;
  run_time = omp_get_wtime() - start_time;
  printf("Pi with %d steps is %.15f in %f seconds\n",num_steps,pi,run_time);

  return 0;
}

