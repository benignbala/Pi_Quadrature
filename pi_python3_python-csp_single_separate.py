#! /usr/bin/env python3
# -*- mode:python; coding:utf-8; -*-

#  Calculation of Pi using quadrature.  Using the python-csp package by Sarah Mount.
#
#  Copyright © 2009-10 Russel Winder

import time
import multiprocessing

from csp.os_process import *

@process
def calculator ( channel , id , sliceSize , delta ) :
    sum = 0.0
    for i in range ( 1 + id * sliceSize , ( id + 1 ) * sliceSize + 1 ) :
        x = ( i - 0.5 ) * delta
        sum += 1.0 / ( 1.0 + x * x )
    channel.write ( sum )
        
@process
def accumulator ( channel , n , delta , startTime , processCount ) :
    pi = 4.0 * sum ( [ channel.read ( ) for i in range ( 0 , processCount ) ] ) * delta
    elapseTime = time.time ( ) - startTime
    print ( "==== Python CSP Single Separate pi = " + str ( pi ) )
    print ( "==== Python CSP Single Separate iteration count = "+ str ( n ) )
    print ( "==== Python CSP Single Separate elapse = " + str ( elapseTime ) )
    print ( "==== Python CSP Single Separate process count = "+ str ( processCount ) )
    print ( "==== Python CSP Single Separate processor count = " + str ( multiprocessing.cpu_count ( ) ) )

def execute ( processCount ) :
    n = 10000000 # 100 times fewer due to speed issues.
    delta = 1.0 / n
    startTime = time.time ( )
    sliceSize = n // processCount
    channel = Channel ( )
    processes = [ calculator ( channel , i , sliceSize , delta )  for i in range ( 0 , processCount ) ]
    processes.append ( accumulator ( channel , n , delta , startTime , processCount ) )
    Par ( *processes ).start ( )

if __name__ == '__main__' :
    execute ( 1 )
    print ( )
    execute ( 2 )
    print ( )
    execute ( 8 )
    print ( )
    execute ( 32 )