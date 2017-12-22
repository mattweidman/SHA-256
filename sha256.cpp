/* Computes SHA-256 hash of file. */

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <vector>

#include "constantgen.h"

using namespace std;

/** Returns the contents of the file at path [fileName]. */
vector<char> getFileContents(string fileName) {
  ifstream inFile;

  // open file
  inFile.open(fileName.data());
  if (!inFile) {
    cerr << "Unable to open " + fileName + "\n";
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

  // convert to vector
  return vector<char>(buffer, buffer + bufLength / (sizeof 'a'));
}

/**
 * Appends a 1 and a list of zeros to the end of a vector
 * to force the length to be a multiple of t.
 */
vector<char> preprocess(const vector<char>& inp, int t) {
  vector<char> ans(inp);
  long L = ans.size() * 8;
  int K = t - ((L + 64) % t);

  // 1 and 7 zeros
  ans.push_back(0x80);

  // K - 8 zeros
  int zerobytes = K / 8 - 1;
  for (int i=0; i<zerobytes; i++)
    ans.push_back(0);

  // last 64 bits used to store value of L
  for (int off = 56; off >= 0; off -= 8) {
    ans.push_back(L >> off);
  }

  return ans;
}

/** Converts int to hexadecimal string. */
string intToHexString(int x) {
  ostringstream s;
  s << hex << x;
  return s.str();
}

/** Converts char to hexadecimal string. */
string charToHexString(char x) {
  ostringstream s;
  s << hex << (int)x;
  return s.str();
}

/**
 * Takes an array [i_1, i_2, ..., i_n] and length.
 * Returns the string "[i_1, i_2, ..., i_length]".
 */
string intVectorToString(vector<int> arr) {
  string ans = "[";
  for (int i=0; i<arr.size(); i++) {
    ans = ans + intToHexString(arr[i]);
    if (i != arr.size()-1) ans += ", ";
  }
  return ans + "]";
}

/**
 * Takes an array [i_1, i_2, ..., i_n] and length.
 * Returns the string "[i_1, i_2, ..., i_length]".
 */
string charVectorToString(vector<char> arr) {
  string ans = "[";
  for (int i=0; i<arr.size(); i++) {
    ans = ans + charToHexString(arr[i]);
    if (i != arr.size()-1) ans += ", ";
  }
  return ans + "]";
}

int main()
{
  string fileName = "data.txt";
  vector<char> buffer_pre = getFileContents(fileName);
  vector<char> buffer = preprocess(buffer_pre, 512);
  cout << buffer.size() << endl;
  cout << charVectorToString(buffer) << endl;
  // vector<int> hs = hashValues();
  // cout << intVectorToString(hs, 8) << endl;
  // vector<int> rc = roundConstants();
  // cout << intVectorToString(rc, 64) << endl;
}