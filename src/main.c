#include <omp.h>
#include <stdio.h>

static long num_steps = 100000;
double step;
static int THREAD_COUNT = 10;

double parallel_integral() {
  double x, pi, sum = 0.0;
  double sums[THREAD_COUNT] = {};

  step = 1.0 / (double)num_steps;
  omp_set_num_threads(THREAD_COUNT);
#pragma omp parallel
  {
    int num_threads = omp_get_num_threads();
    int thread_id = omp_get_thread_num();
    int start = thread_id * (num_steps / num_threads);
    int end = num_steps / num_threads + start - 1;
    for (int i = start; i < end; i++) {
      x = (i + 0.5) * step;
      sums[thread_id] += 4.0 / (1.0 + x * x);
    }
  }

  for (int i = 0; i < THREAD_COUNT; i++) {
    sum += sums[i];
  }
  pi = step * sum;

  return pi;
}

double sequential_integral() {
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
  pi = sequential_integral();
  end = omp_get_wtime();
  printf("Sequential pi = %f\n", pi);
  printf("Took %f\n", (end - start) * 1e6);

  start = omp_get_wtime();
  pi = parallel_integral();
  end = omp_get_wtime();
  printf("Parallel pi = %f\n", pi);
  printf("Took %f\n", (end - start) * 1e6);
}
