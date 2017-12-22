/* Computes SHA-256 hash of file. */

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>

#include "constantgen.h"

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

/* Converts int to string. */
string intToHexString(int x) {
  ostringstream s;
  s << hex << x;
  return s.str();
}

/**
 * Takes an array [i_1, i_2, ..., i_n] and length.
 * Returns the string "[i_1, i_2, ..., i_length]".
 */
string intArrToString(int* arr, int length) {
  string ans = "[";
  for (int i=0; i<length; i++) {
    ans = ans + intToHexString(arr[i]);
    if (i != length-1) ans += ", ";
  }
  return ans + "]";
}

int main()
{
  // const char* fileName = "data.txt";
  // char* buffer = getFileContents(fileName);
  int* hs = hashValues();
  cout << intArrToString(hs, 8) << endl;
  int* rc = roundConstants();
  cout << intArrToString(rc, 64) << endl;
}