#include <math.h>
#include <omp.h>

#define THREAD_COUNT 12

static long num_steps = 1e9;
double step;

double parallel_pi() {
  double pi = 0.0, num_threads;
  double sum[THREAD_COUNT] = {};

  step = 1.0 / (double)num_steps;
  omp_set_num_threads(THREAD_COUNT);
#pragma omp parallel
  {
    int i, id, nthrds;
    double x, thread_sum;
    nthrds = omp_get_num_threads();
    id = omp_get_thread_num();

    if (id == 0)
      num_threads = nthrds;

    for (i = id, thread_sum = 0.0; i < num_steps; i = i + nthrds) {
      x = (i + 0.5) * step;
      thread_sum += 4.0 / (1.0 + x * x);
    }
#pragma omp atomic
    sum[id] += thread_sum;
  }

  for (int i = 0; i < num_threads; i++) {
    pi += sum[i] * step;
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
