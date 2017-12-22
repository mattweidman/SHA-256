/* Computes SHA-256 hash of file. */

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <vector>

#include "constantgen.h"

using namespace std;

/** Returns the contents of the file at path [fileName]. */
vector<unsigned char> getFileContents(string fileName) {
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
  return vector<unsigned char>(buffer, buffer + bufLength / (sizeof 'a'));
}

/**
 * Appends a 1 and a list of zeros to the end of a vector
 * to force the length to be a multiple of t.
 */
vector<unsigned char> preprocess(const vector<unsigned char>& inp, int t) {
  vector<unsigned char> ans(inp);
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

/**
 * Divides a vector v into vectors of size s.
 */
vector<vector<unsigned char> > divideMessage(const vector<unsigned char>& v, 
int s) {
  vector<vector<unsigned char> > chunks;
  for (int i=0; i<v.size(); i+=s) {
    vector<unsigned char> chunk(v.begin() + i, v.begin() + i + s);
    chunks.push_back(chunk);
  }
  return chunks;
}

/** Right rotate operation */
int rightRotate(unsigned int x, int shift) {
  int copy = (x % (1 << (shift+1))) << (sizeof(x)*8 - shift);
  return copy + (x >> shift);
}

/**
 * Hash one 64-byte chunk of data.
 * chunk: 64 bytes of data to be hashed
 * hashes: 8 hash values from previous chunk (or initial hashes)
 * k: 64 round constants
 * returns: hash made of 8 ints
 */
vector<int> hashChunk(const vector<unsigned char>& data, 
const vector<int>& hashes, const vector<int>& k) {
  vector<int> w(64, 0);

  // copy data into first 16 words of w
  for (int di=0; di<64; di++) {
    int wi = di / 4;
    int shift = (3 - (di % 4)) * 8;
    int val = data[di] << shift;
    w[wi] += val;
  }

  // extend first 16 words into remaining 48 words
  for (int i=16; i<64; i++) {
    int s0 = rightRotate(w[i-15], 7) ^ rightRotate(w[i-15], 18)
      ^ rightRotate(w[i-15], 3);
    int s1 = rightRotate(w[i-2], 17) ^ rightRotate(w[i-2], 19)
      ^ rightRotate(w[i-2], 10);
    w[i] = w[i-16] + s0 + w[i-7] + s1;
  }

  // initialize a-h
  int a = hashes[0],
      b = hashes[1],
      c = hashes[2],
      d = hashes[3],
      e = hashes[4],
      f = hashes[5],
      g = hashes[6],
      h = hashes[7];
  
  // compression
  for (int i=0; i<64; i++) {
    int s1 = rightRotate(e, 6) ^ rightRotate(e, 11) ^ rightRotate(e, 25);
    int ch = (e & f) ^ ((~e) & g);
    int temp1 = h + s1 + ch + k[i] + w[i];
    int s0 = rightRotate(a, 2) ^ rightRotate(a, 13) ^ rightRotate(a, 22);
    int maj = (a & b) ^ (a & c) ^ (b & c);
    int temp2 = s0 + maj;

    h = g;
    g = f;
    f = e;
    e = d + temp1;
    d = c;
    c = b;
    b = a;
    a = temp1 + temp2;
  }

  // add compressed chunk to current hash value and return
  int ansarr[8] = {
    hashes[0] + a,
    hashes[1] + b,
    hashes[2] + c,
    hashes[3] + d,
    hashes[4] + e,
    hashes[5] + f,
    hashes[6] + g,
    hashes[7] + h
  };
  return vector<int>(ansarr, ansarr+8);
}

/** Converts int to hexadecimal string. */
string intToHexString(int x) {
  ostringstream s;
  s << hex << x;
  return s.str();
}

/** Converts char to hexadecimal string. */
string charToHexString(unsigned char x) {
  ostringstream s;
  s << hex << (int)x;
  return s.str();
}

/**
 * Takes an array [i_1, i_2, ..., i_n] and length.
 * Returns the string "[i_1, i_2, ..., i_length]".
 */
string intVectorToString(const vector<int>& arr) {
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
string charVectorToString(const vector<unsigned char>& arr) {
  string ans = "[";
  for (int i=0; i<arr.size(); i++) {
    ans = ans + charToHexString(arr[i]);
    if (i != arr.size()-1) ans += ", ";
  }
  return ans + "]";
}

/**
 * Creates a string from a list of byte vectors
 */
string charVector2DToString(const vector<vector<unsigned char> >& vs) {
  string ans = "";
  for (int i=0; i<vs.size(); i++) {
    ans += charVectorToString(vs[i]) + "\n";
  }
  return ans;
}

int main()
{
  string fileName = "data.txt";
  vector<unsigned char> buffer_pre = getFileContents(fileName);
  vector<unsigned char> buffer = preprocess(buffer_pre, 512);
  vector<vector<unsigned char> > chunks = divideMessage(buffer, 64);
  vector<int> hv = hashValues();
  vector<int> k = roundConstants();
  cout << intVectorToString(hashChunk(chunks[0], hv, k)) << endl;
  // vector<int> hs = hashValues();
  // cout << intVectorToString(hs, 8) << endl;
  // vector<int> rc = roundConstants();
  // cout << intVectorToString(rc, 64) << endl;
  // 0xb0000004 = 1011 0000 0000 0000 0000 0000 0000 0100
  // cout << hex << rightRotate(0xb0000004U, 1) << endl; // 0x58.2
  // cout << hex << rightRotate(0xb0000004U, 2) << endl; // 0x2c.1
  // cout << hex << rightRotate(0xb0000004U, 3) << endl; // 0x96.0
  // cout << hex << rightRotate(2, 2) << endl; // 0x80000000
}