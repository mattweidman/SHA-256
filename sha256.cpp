/* Computes SHA-256 hash of file. */

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <vector>

#include "constantgen.h"

using namespace std;

/** Returns the contents of the file at path [fileName]. */
string getFileContents(string fileName) {
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
  return string(buffer);
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
 * data: 64 bytes of data to be hashed
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

/**
 * Hash a list of 64-byte chunks of data.
 * data: vectors of 64 bytes of data to be hashed
 * hashes: initial 8 hashes
 * k: 64 round constants
 * returns: hash made of 8 ints
 */
vector<int> hashChunks(
const vector<vector<unsigned char> >& data, 
const vector<int>& hashes, const vector<int>& k) {
  vector<int> hs(hashes);
  for (int i=0; i<data.size(); i++) {
    hs = hashChunk(data[i], hs, k);
  }
  return hs;
}

/**
 * Takes the SHA-256 hash of a string.
 */
string hashSHA256(string s) {
  // constants
  vector<int> hv = hashValues();
  vector<int> k = roundConstants(); 

  // hash
  vector<unsigned char> predata(s.begin(), s.end());
  vector<unsigned char> data = preprocess(predata, 512);
  vector<vector<unsigned char> > chunks = divideMessage(data, 64);
  vector<int> hashes = hashChunks(chunks, hv, k);

  // convert to string
  ostringstream oss;
  for (int i=0; i<hashes.size(); i++) {
    oss << hex << hashes[i];
  }
  return oss.str();
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

int main(int argc, char* argv[])
{
  if (argc != 2) {
    cout << "Wrong number of arguments: " << argc-1 << endl;
    cout << "Requires name of file to be hashed" << endl;
    exit(1);
  }
  string contents = getFileContents(argv[1]);
  cout << hashSHA256(contents) << endl;
}