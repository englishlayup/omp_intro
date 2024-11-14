#include <omp.h>
#include <stdio.h>

static long num_steps = 100000;
double step;

int main() {
  double start = omp_get_wtime();
  double x, pi, sum = 0.0;
  double areas[num_steps];

  step = 1.0 / (double)num_steps;
  omp_set_num_threads(4);
#pragma omp parallel
  {
    int num_threads = omp_get_num_threads();
    int thread_id = omp_get_thread_num();
    int start = thread_id * (num_steps / num_threads);
    int end = num_steps / num_threads + start - 1;
    for (int i = start; i < end; i++) {
      x = (i + 0.5) * step;
      areas[i] = 4.0 / (1.0 + x * x);
    }
  }

  for (int i = 0; i < num_steps; i++) {
    sum += areas[i];
  }
  pi = step * sum;

  printf("%f\n", pi);
  double end = omp_get_wtime();
  printf("Took %f", (end - start) * 1e6);
}
