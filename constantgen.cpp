#include <math.h>
#include <string.h>
#include <string>
using namespace std;

/**
 * Computes x^(1/y) as a double, computes its fractional 
 * part + 1, and returns the first 32 bits of the significand.
 */
int fractOfRoot(double x, double y) {
  double s = pow(x, 1.0/y);
  double fp = s - (int)s + 1.0;
  long fplong = *(reinterpret_cast<long*>(&fp));
  return fplong >> 20;
}

/**
 * Computes the list of prime numbers up to and including p
 * using the Sieve of Eratosthenes.
 */
int* primesTo(int p) {
  // create array of numbers 1 to p
  bool* isPrime = new bool[p];
  memset(isPrime, 1, p);

  double sqrtp = sqrt(p);

  // label each composite number
  isPrime[0] = false;
  for (int i=1; i<sqrtp; i++) {
    int factor = i+1;
    if (isPrime[i]) {
      for (int j = i + factor; j < p; j += factor) {
        isPrime[j] = false;
      }
    }
  }

  // get number of primes
  int numPrimes = 0;
  for (int i=0; i<p; i++) {
    if (isPrime[i]) numPrimes++;
  }

  // create array of primes
  int* primes = new int[numPrimes];
  int j = 0;
  for (int i=0; i<p; i++) {
    if (isPrime[i]) {
      primes[j] = i+1;
      j++;
    }
  }
  return primes;
}

/**
 * Returns the first 32 bits of the fractional parts of 
 * the r roots of the first n primes.
 * Precondition: maxPrime is the nth prime.
 */
int* genConstants(int n, int maxPrime, double r) {
  int* primes = primesTo(maxPrime);

  int* vals = new int[n];
  for (int i=0; i<n; i++) {
    vals[i] = fractOfRoot(primes[i], r);
  }
  return vals;
}

/**
 * Returns the list of hash values h0, ..., h7.
 * Same as the first 32 bits of the fractional parts of the
 * square roots of the first 8 primes 2..19.
 */
int* hashValues() {
  return genConstants(8, 19, 2);
}

/**
 * Returns the list of round constants k[0..63].
 * Same as the first 32 bits of the fractional parts of the
 * cube roots of the first 64 primes.
 */
int* roundConstants() {
  return genConstants(64, 311, 3);
}