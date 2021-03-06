#! /usr/bin/env groovy

/*
 *  Calculation of π using quadrature realized with a fork/join approach with GPars CSP to partition the
 *  problem and hence harness all processors available to the JVM.
 *
 *  Copyright © 2010–2013  Russel Winder
 */

@Grab('org.codehaus.jcsp:jcsp:1.1-rc5')
import org.jcsp.lang.Channel
import org.jcsp.lang.CSProcess

import groovyx.gpars.csp.PAR

void execute(final numberOfTasks) {
  final n = 1000000000
  final delta = 1.0 / n
  final startTimeNanos = System.nanoTime()
  final sliceSize = (int)(n / numberOfTasks)
  final channels = Channel.one2oneArray(numberOfTasks)
  final processes = (0 ..< numberOfTasks).collect{id -> new ProcessSlice_JCSP(id, sliceSize, delta, channels[id].out())}
  processes << {
    final pi = 4.0 * delta * channels.sum{c -> c.in().read()}
    final elapseTime = (System.nanoTime() - startTimeNanos) / 1e9
    Output.out(getClass().name, pi, n, elapseTime, numberOfTasks)
  } as CSProcess
  new PAR(processes as CSProcess[]).run()
}

execute 1
execute 2
execute 8
execute 32
