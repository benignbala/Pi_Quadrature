/*
 *  A C program to calculate Pi using quadrature as a threads-based algorithm.
 *
 *  Copyright © 2009-10 Russel Winder
 */

#include <iostream>
#include <iomanip>
#include <pthread.h>
#include "microsecondTime.h"

double sum ;
pthread_mutex_t sumMutex ;

struct CalculationParameters {
  long id ;
  long sliceSize ;
  double delta ;
  CalculationParameters ( ) : id ( 0l ) , sliceSize ( 0l ) , delta ( 0.0 ) { }
  CalculationParameters ( const long i , const long s , const double d ) : id ( i ) , sliceSize ( s ) , delta ( d ) { }
  CalculationParameters ( const CalculationParameters & x ) {
    id = x.id ;
    sliceSize = x.sliceSize ;
    delta = x.delta ;
  }
} ;

void * partialSum ( void *const arg  ) {
  const long start = 1 + ( (CalculationParameters *const) arg )->id * ( (CalculationParameters *const) arg )->sliceSize ;
  const long end = ( ( (CalculationParameters *const) arg )->id + 1 ) * ( (CalculationParameters *const) arg )->sliceSize ;
  const double delta = ( (CalculationParameters *const) arg )->delta ;
  double localSum = 0.0 ;
  for ( long i = start ; i <= end ; ++i ) {
    const double x = ( i - 0.5 ) * delta ;
    localSum += 1.0 / ( 1.0 + x * x ) ;
  }
  pthread_mutex_lock ( &sumMutex ) ;
  sum += localSum ;
  pthread_mutex_unlock ( &sumMutex ) ;
  pthread_exit ( (void *) 0 ) ;
  return 0 ;
}

void execute ( const int numberOfThreads ) {
  const long n = 1000000000l ;
  const double delta = 1.0 / n ;
  const long long startTimeMicros = microsecondTime ( ) ;
  const long sliceSize = n / numberOfThreads ;
  pthread_mutex_init ( &sumMutex , NULL ) ;
  pthread_attr_t attributes ;
  pthread_attr_init ( &attributes ) ;
  pthread_attr_setdetachstate ( &attributes , PTHREAD_CREATE_JOINABLE ) ;
  sum = 0.0 ; // Only one thread at this point so safe to access without locking.
  pthread_t threads[numberOfThreads] ;
  CalculationParameters parameters[numberOfThreads] ;
  for ( int i = 0 ; i < numberOfThreads ; ++i ) {
    parameters[i] = CalculationParameters ( i , sliceSize , delta ) ;
    pthread_create ( &threads[i] , &attributes , partialSum , (void *) &parameters[i] ) ;
  }
  pthread_attr_destroy ( &attributes ) ;
  int status ;
  for ( int i = 0 ; i < numberOfThreads ; ++i ) { pthread_join ( threads[i] , (void **) &status ) ; }
  const double pi = 4.0 * sum * delta ;
  const double elapseTime = ( microsecondTime ( ) - startTimeMicros ) / 1e6 ;
  std::cout << "==== C++ PThread parameters pi = " << std::setprecision ( 18 ) << pi << std::endl ;
  std::cout << "==== C++ PThread parameters iteration count = " << n << std::endl ;
  std::cout << "==== C++ PThread parameters elapse = " << elapseTime << std::endl ;
  std::cout << "==== C++ PThread parameters thread count = " <<  numberOfThreads << std::endl ;
}

int main ( ) {
  execute ( 1 ) ;
  std::cout << std::endl ;
  execute ( 2 ) ;
  std::cout << std::endl ;
  execute ( 8 ) ;
  std::cout << std::endl ;
  execute ( 32 ) ;
  return 0 ;
}
