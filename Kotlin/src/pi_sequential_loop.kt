package uk.org.winder.pi_quadrature

fun main(args:Array<String>) {
  val n = 1000000000;
  val delta = 1.0 / n;
  val startTimeNanos = System.nanoTime();
  var sum = 0.0;
  for (i in 1..n) {
     val x = (i - 0.5) * delta;
     sum += 1.0 / (1.0 + x * x);
   }
   val pi = 4.0 * delta * sum;
   val elapseTime = (System.nanoTime() - startTimeNanos) / 1e9;
   out("pi_sequential_loop", pi, n, elapseTime);
}