#! /usr/bin/env python
# -*- mode:python; coding:utf-8; -*-

#  Calculation of Pi using quadrature. Using the multiprocessing package to provide a process pool to enable
#  asynchronous function calls very akin to futures..
#
#  Copyright © 2008-10 Russel Winder

import time
import multiprocessing

def processSlice ( id , sliceSize , delta ) :
    sum = 0.0
    for i in xrange (  1 + id * sliceSize , ( id + 1 ) * sliceSize + 1 ) :
        x = ( i - 0.5 ) * delta
        sum += 1.0 / ( 1.0 + x * x )
    return sum

def execute ( processCount ) :
    n = 10000000 # 100 times fewer due to speed issues.
    delta = 1.0 / n
    startTime = time.time ( )
    sliceSize = n / processCount
    pool = multiprocessing.Pool ( processes = processCount )
    results = [ pool.apply_async ( processSlice , args = ( i , sliceSize , delta ) ) for i in xrange ( 0 , processCount ) ]
    pool.close ( )
    pool.join ( )
    results = [ item.get ( ) for item in results ]
    pi = 4.0 * sum ( results ) * delta
    elapseTime = time.time ( ) - startTime
    print ( "==== Python Multiprocessing Pool pi = " + str ( pi ) )
    print ( "==== Python Multiprocessing Pool iteration count = "+ str ( n ) )
    print ( "==== Python Multiprocessing Pool elapse = " + str ( elapseTime ) )
    print ( "==== Python Multiprocessing Pool process count = " + str ( processCount ) )
    print ( "==== Python Multiprocessing Pool processor count = " + str ( multiprocessing.cpu_count ( ) ) )

if __name__ == '__main__' :
    execute ( 1 )
    print
    execute ( 2 )
    print
    execute ( 8 )
    print
    execute ( 32 )