/*
**  PROGRAM: Mandelbrot area
**
**  PURPOSE: Program to compute the area of a  Mandelbrot set.
**           Correct answer should be around 1.510659.
**           WARNING: this program may contain errors
**
**  USAGE:   Program runs without input ... just run the executable
**
**  HISTORY: Written:  (Mark Bull, August 2011).
**           Changed "comples" to "d_comples" to avoid collsion with
**           math.h complex type (Tim Mattson, September 2011)
*/

#include <omp.h>
#include <stdio.h>

#define NPOINTS 1000
#define MAXITER 1000

struct d_complex {
  double r;
  double i;
};

void testpoint(int *counter, struct d_complex point);

struct d_complex c;
int numoutside = 0;
int *numoutside_ptr = &numoutside;
omp_lock_t numoutside_lock;

int main() {
  int i, j;
  double area, error, eps = 1.0e-5;
  omp_init_lock(&numoutside_lock);

  //   Loop over grid of points in the complex plane which contains the
  //   Mandelbrot set, testing each point to see whether it is inside or outside
  //   the set.

#pragma omp parallel for private(j, c) firstprivate(eps)
  for (i = 0; i < NPOINTS; i++) {
    for (j = 0; j < NPOINTS; j++) {
      c.r = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS) + eps;
      c.i = 1.125 * (double)(j) / (double)(NPOINTS) + eps;
      testpoint(numoutside_ptr, c);
    }
  }
  omp_destroy_lock(&numoutside_lock);

  // Calculate area of set and error estimate and output the results
  area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS * NPOINTS - numoutside) /
         (double)(NPOINTS * NPOINTS);
  error = area / (double)NPOINTS;

  printf("Area of Mandlebrot set = %12.8f +/- %12.8f\n", area, error);
  printf("Correct answer should be around 1.510659\n");
}

void testpoint(int *numoutside, struct d_complex point) {

  // Does the iteration z=z*z+c, until |z| > 2 when point is known to be outside
  // set. If loop count reaches MAXITER, point is considered to be inside the
  // set

  struct d_complex z;
  int iter;
  double temp;

  z = point;
  for (iter = 0; iter < MAXITER; iter++) {
    temp = (z.r * z.r) - (z.i * z.i) + point.r;
    z.i = z.r * z.i * 2 + point.i;
    z.r = temp;
    if ((z.r * z.r + z.i * z.i) > 4.0) {
      omp_set_lock(&numoutside_lock);
      (*numoutside)++;
      omp_unset_lock(&numoutside_lock);
      break;
    }
  }
}
