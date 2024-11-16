#include <stdio.h>

#include "lib.c"

int main() {
  double pi, start, end;

  start = omp_get_wtime();
  pi = parallel_pi();
  end = omp_get_wtime();
  printf("Parallel pi = %f\n", pi);
  printf("Took %f seconds\n", (end - start));
}
