/*
 *  Chapel procedures for doing π calculation result output.
 *
 *  Copyright © 2012, 2013, 2015  Russel Winder
 */

module Output {
  proc output(banner:string, pi:real, n:int, elapseTime:real) {
    writeln("======================== " + banner);
    writef("\tπ = %.20r\n", pi);
    writeln("\titeration count = ", n);
    writeln("\telapse time = ", elapseTime);
  }

  proc output_more(banner:string, pi:real, n:int, elapseTime:real, numberOfTasks:int) {
    output(banner + ", task count: " + numberOfTasks, pi, n, elapseTime);
    writeln("\tnumber of processors = ", here.numCores);
  }
}
