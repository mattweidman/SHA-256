/**
 * Returns the list of hash values h0, ..., h7.
 * Same as the first 32 bits of the fractional parts of the
 * square roots of the first 8 primes 2..19.
 */
int* hashValues();

/**
 * Returns the list of round constants k[0..63].
 * Same as the first 32 bits of the fractional parts of the
 * cube roots of the first 64 primes.
 */
int* roundConstants();