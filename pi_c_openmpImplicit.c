/*
 *  A C program to calculate Pi using quadrature as an OpenMP annotated algorithm.
 *
 *  Copyright © 2008-10 Russel Winder
 */

#include <stdio.h>
#include <omp.h>
#include "microsecondTime.h"

int main ( ) {
  const long n = 1000000000l ;
  const double delta = 1.0 / n ;
  const long long startTimeMicros = microsecondTime ( ) ;
  double sum = 0.0 ;
  long i ;
#pragma omp parallel for private ( i ) reduction ( + : sum )
  for ( i = 1 ; i <= n ; ++i ) {
    const double x = ( i - 0.5 ) * delta ;
    sum += 1.0 / ( 1.0 + x * x ) ;
  }
  const double pi = 4.0 * sum * delta ;
  const double elapseTime = ( microsecondTime ( ) - startTimeMicros ) / 1e6 ;
  printf ( "==== C OpenMP Implicit pi = %.18lf\n" , pi ) ;
  printf ( "==== C OpenMP Implicit iteration count = %ld\n" ,  n ) ;
  printf ( "==== C OpenMP Implicit elapse = %lf\n" , elapseTime ) ;
  printf ( "==== C OpenMP Implicit processor count = %d\n" , omp_get_num_procs ( ) ) ;
  return 0 ;
}
