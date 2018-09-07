#include "decryptor.h"

void StarsRandom::setRandom(int prime1, int prime2, int initRounds)
{
    seedA = prime1;
    seedB = prime2;
    rounds = initRounds;

    // Now initialize a few rounds
    for(int i = 0; i < rounds; i++)
        nextRandom();
}

long StarsRandom::nextRandom()
{
    // First, calculate new seeds using some constants
    long seedApartA = (seedA % 53668) * 40014;
    long seedApartB = (seedA / 53668) * 12211;  // integer division OK
    long newSeedA = seedApartA - seedApartB;

    long seedBpartA = (seedB % 52774) * 40692;
    long seedBpartB = (seedB / 52774) * 3791;
    long newSeedB = seedBpartA - seedBpartB;

    // If negative add a whole bunch (there's probably some weird bit math
    // going on here that the disassembler didn't make obvious)
    if(newSeedA < 0)
        newSeedA += 0x7fffffab;

    if(newSeedB < 0)
        newSeedB += 0x7fffff07;

    // Set our new seeds
    seedA = newSeedA;
    seedB = newSeedB;

    // Generate "random" number.  This will fit into an unsigned 32bit integer
    // We use 'long' because...  java...
    long randomNumber = seedA - seedB;
    if(seedA < seedB)
        randomNumber += 0x100000000l;  // 2^32

    // Now return our random number
    return randomNumber;
}

Decryptor::Decryptor()
{
}

void Decryptor::initDecryption(FileHeaderBlock fileHeaderBlock)
{
    int salt = fileHeaderBlock.encryptionSalt;

    // Use two prime numbers as random seeds.
    // First one comes from the lower 5 bits of the salt
    int index1 = salt & 0x1F;
    // Second index comes from the next higher 5 bits
    int index2 = (salt >> 5) & 0x1F;

    // Adjust our indexes if the highest bit (bit 11) is set
    // If set, change index1 to use the upper half of our primes table
    if((salt >> 10) == 1)
        index1 += 32;
    // Else index2 uses the upper half of the primes table
    else
        index2 += 32;

    // Determine the number of initialization rounds from 4 other data points
    // 0 or 1 if shareware (I think this is correct, but may not be - so far
    // I have not encountered a shareware flag
    int part1 = fileHeaderBlock.shareware ? 1 : 0;

    // Lower 2 bits of player number, plus 1
    int part2 = (fileHeaderBlock.playerNumber & 0x3) + 1;

    // Lower 2 bits of turn number, plus 1
    int part3 = (fileHeaderBlock.turn & 0x3) + 1;

    // Lower 2 bits of gameId, plus 1
    int part4 = ((int) fileHeaderBlock.gameId & 0x3) + 1;

    // Now put them all together, this could conceivably generate up to 65
    // rounds  (4 * 4 * 4) + 1
    int rounds = (part4 * part3 * part2) + part1;

    // Now initialize our random number generator
    starsRandom.setRandom(primes[index1], primes[index2], rounds);

}

