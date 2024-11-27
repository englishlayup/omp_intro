/*
**  PROGRAM: A simple serial producer/consumer program
**
**  One function generates (i.e. produces) an array of random values.
**  A second functions consumes that array and sums it.
**
**  HISTORY: Written by Tim Mattson, April 2007.
*/
#include <omp.h>
#ifdef APPLE
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <stdio.h>

#define N 1e9

/* Some random number constants from numerical recipies */
#define SEED 2531
#define RAND_MULT 1366
#define RAND_ADD 150889
#define RAND_MOD 714025
int randy = SEED;

/* function to fill an array with random numbers */
void fill_rand(int length, double *a, int *flag) {
  int i;
  for (i = 0; i < length; i++) {
    randy = (RAND_MULT * randy + RAND_ADD) % RAND_MOD;
    *(a + i) = ((double)randy) / ((double)RAND_MOD);
    (*flag) = i;
#pragma omp flush(a, flag)
  }
}

/* function to sum the elements of an array */
double Sum_array(int length, double *a, int *flag) {
  int i;
  double sum = 0.0;
  for (i = 0; i < length; i++) {
    while (*flag < i) {
    }
    sum += *(a + i);
  }
  return sum;
}

int main() {
  double *A, sum, runtime;
  int flag = -1;

  A = (double *)malloc(N * sizeof(double));

  runtime = omp_get_wtime();

#pragma omp parallel
#pragma omp single
  {
#pragma omp task
    fill_rand(N, A, &flag); // Producer: fill an array of data

#pragma omp task
    sum = Sum_array(N, A, &flag); // Consumer: sum the array
  }
  runtime = omp_get_wtime() - runtime;

  printf(" In %f seconds, The sum is %f \n", runtime, sum);
}
