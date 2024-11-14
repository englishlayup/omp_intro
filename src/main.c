#include <omp.h>
#include <stdio.h>

#define THREAD_COUNT 12
#define PAD 8

static long num_steps = 100000;
double step;

double parallel_pi() {
  double pi, num_threads;
  double sum[THREAD_COUNT][PAD];

  step = 1.0 / (double)num_steps;
  omp_set_num_threads(THREAD_COUNT);
#pragma omp parallel
  {
    int i, id, nthrds;
    double x;
    nthrds = omp_get_num_threads();
    id = omp_get_thread_num();

    if (id == 0)
      num_threads = nthrds;

    for (i = id, sum[id][0] = 0.0; i < num_steps; i = i + nthrds) {
      x = (i + 0.5) * step;
      sum[id][0] += 4.0 / (1.0 + x * x);
    }
  }

  for (int i = 0; i < num_threads; i++) {
    pi += sum[i][0] * step;
  }

  return pi;
}

double sequential_pi() {
  int i;
  double x, pi, sum = 0.0;
  step = 1.0 / (double)num_steps;
  for (i = 0; i < num_steps; i++) {
    x = (i + 0.5) * step;
    sum = sum + 4.0 / (1.0 + x * x);
  }
  pi = step * sum;
  return pi;
}

int main() {
  double pi, start, end;
  start = omp_get_wtime();
  pi = sequential_pi();
  end = omp_get_wtime();
  printf("Sequential pi = %f\n", pi);
  printf("Took %f\n", (end - start) * 1e6);

  start = omp_get_wtime();
  pi = parallel_pi();
  end = omp_get_wtime();
  printf("Parallel pi = %f\n", pi);
  printf("Took %f\n", (end - start) * 1e6);
}
