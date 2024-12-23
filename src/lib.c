#include <math.h>
#include <omp.h>

#define THREAD_COUNT 12

static long num_steps = 1e9;
double step;

double parallel_pi() {
  int i;
  double pi, sum = 0.0;
  step = 1.0 / (double)num_steps;
#pragma omp parallel for reduction(+ : sum)
  for (i = 0; i < num_steps; i++) {
    double x = (i + 0.5) * step;
    sum = sum + 4.0 / (1.0 + x * x);
  }
  pi = step * sum;
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
