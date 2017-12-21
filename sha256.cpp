/* Computes SHA-256 hash of file. */

#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <string>
using namespace std;

/** Returns the contents of the file at path [fileName]. */
char* getFileContents(const char* fileName) {
  ifstream inFile;

  // open file
  inFile.open(fileName);
  if (!inFile) {
    cerr << "Unable to open " + string(fileName) + "\n";
    exit(1);
  }

  // create buffer
  inFile.seekg(0, inFile.end);
  int bufLength = inFile.tellg();
  char* buffer = new char[bufLength];

  // read whole file
  inFile.seekg(0, inFile.beg);
  inFile.read(buffer, bufLength);

  inFile.close();
  return buffer;
}

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

/* Converts int to string. */
string intToString(int x) {
  ostringstream s;
  s << x;
  return s.str();
}

/**
 * Takes an array [i_1, i_2, ..., i_n] and length.
 * Returns the string "[i_1, i_2, ..., i_length]".
 */
string intArrToString(int* arr, int length) {
  string ans = "[";
  for (int i=0; i<length; i++) {
    ans = ans + intToString(arr[i]);
    if (i != length-1) ans += ", ";
  }
  return ans + "]";
}

int main()
{
  // const char* fileName = "data.txt";
  // char* buffer = getFileContents(fileName);
  int* primes = primesTo(311);
  cout << intArrToString(primes, 64) << endl;
}