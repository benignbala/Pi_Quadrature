#! /usr/bin/env groovy

/*
 *  Calculation of π using quadrature realized with a basic sequential algorithm.
 *
 *  Copyright © 2008–2012, 2014  Russel Winder
 */

final n = 100_000 // 10,000 times fewer than Java due to speed issues.
final delta = 1.0 / n
final startTime = System.nanoTime()
def sum = 0.0
for (i in 1 .. n) {
  final x = (i - 0.5) * delta
  sum += 1 / (1 + x * x)
}
final pi = 4 * delta * sum
final elapseTime = (System.nanoTime() - startTime) / 1e9
Output.out getClass(), pi, n, elapseTime
