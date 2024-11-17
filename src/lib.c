#include <math.h>
#include <omp.h>

#define THREAD_COUNT 2

static long num_steps = 1e9;
double step;

double parallel_pi() {
  int thread_count;
  double pi = 0.0;
  double sum[THREAD_COUNT];
  step = 1.0 / (double)num_steps;
  omp_set_num_threads(THREAD_COUNT);
#pragma omp parallel
  {
    int i;
    double x;
    int thread_id = omp_get_thread_num();
    int count = omp_get_num_threads();
    if (thread_id == 0) {
      thread_count = count;
    }
    for (i = thread_id, sum[thread_id] = 0.0; i < num_steps; i = i + count) {
      x = (i + 0.5) * step;
      sum[thread_id] += 4.0 / (1.0 + x * x);
    }
  }

  for (int i = 0; i < thread_count; i++) {
    pi += step * sum[i];
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

double round_to_decimal(double value, int decimal_places) {
  double multiplier = pow(10.0, decimal_places);
  return round(value * multiplier) / multiplier;
}
