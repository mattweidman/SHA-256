# SHA-256

Implementing SHA-256 hashing for fun.

## How to use

Compile with `make compile`.

Run with `./sha256 filename`.

For example, running `./sha256 data.txt` returns

`f4c3471c5fc2a9fdab77b49ad1e39ce934e095b8711b1fe3d60f14cc1daad0a6`

## Code

The most important function is `hashSHA256()` in sha256.cpp.
If you read this function, you can probably understand most of the code.