#ifndef PRNG_H_
#define PRNG_H_
#include <bitboard.h>

// https://en.wikipedia.org/wiki/Xorshift#Example_implementation
unsigned int get_random_U32();
U64 get_random_U64();
U64 generate_magic_number();
#endif // PRNG_H_
