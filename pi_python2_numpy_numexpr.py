#! /usr/bin/env python
# -*- mode:python; coding:utf-8; -*-

#  Calculation of Pi using quadrature. Sequential algorithm. Use NumPy.
#
#  Copyright © 2008–2012 Russel Winder

from output import out
from time import time
from numpy import arange
from numexpr import detect_number_of_cores , evaluate , set_num_threads 

def execute ( threadCount ) :
    n = 100000000 # 10 times fewer than C due to speed issues.
    delta = 1.0 / n
    startTime = time ( )
    set_num_threads ( threadCount )
    value = arange ( n )
    pi = 4.0 * delta * evaluate ( "1.0 / ( 1.0 + ( ( value - 0.5 ) * delta ) ** 2 )" ).sum ( )
    elapseTime = time ( ) - startTime
    out ( __file__ ,  pi , n , elapseTime , threadCount , detect_number_of_cores ( ) )
    
if __name__ == '__main__' :
    execute ( 1 )
    execute ( 2 )
    execute ( 8 )
    execute ( 32 )
