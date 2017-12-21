/* Computes SHA-256 hash of file. */

#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <string.h>
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

int main()
{
  // const char* fileName = "data.txt";
  // char* buffer = getFileContents(fileName);
  cout << hex << fractOfRoot(311, 3);
}