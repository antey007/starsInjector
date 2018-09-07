#ifndef DECRYPTOR_H
#define DECRYPTOR_H

#include "FileHeaderBlock.h"

class StarsRandom {
public:
    // We'll use 'long' for our seeds to avoid signed-integer problems
    long seedA;
    long seedB;

    int rounds;
    StarsRandom() {};

    void setRandom(int prime1, int prime2, int initRounds);

    // Get the next random number with this seeded generator
    long nextRandom();
};

static const int primes[] = {
    3, 5, 7, 11, 13, 17, 19, 23,
    29, 31, 37, 41, 43, 47, 53, 59,
    61, 67, 71, 73, 79, 83, 89, 97,
    101, 103, 107, 109, 113, 127, 131, 137,
    139, 149, 151, 157, 163, 167, 173, 179,
    181, 191, 193, 197, 199, 211, 223, 227,
    229, 233, 239, 241, 251, 257, 263, 279,
    271, 277, 281, 283, 293, 307, 311, 313
};


class Decryptor
{
public:
    Decryptor();
    static const int BLOCK_HEADER_SIZE = 2;  // bytes
    static const int BLOCK_MAX_SIZE = 1024;  // bytes

    StarsRandom starsRandom;

    void initDecryption(FileHeaderBlock fileHeaderBlock);


};

#endif // DECRYPTOR_H
