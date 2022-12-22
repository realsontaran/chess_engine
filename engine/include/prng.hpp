#ifndef PRNG_H_
#define PRNG_H_
#include <bitboard.hpp>

// https://en.wikipedia.org/wiki/Xorshift#Example_implementation
unsigned int getRandomU32();
U64 getRandomU64();
U64 generatMagicNumber();
#endif // PRNG_H_
