#include <prng.h>

// https://www.chessprogramming.org/Looking_for_Magics#Feeding_in_Randoms

static unsigned int state = 1804289383; // Random start state

unsigned int get_random_U32() {
    // get current state
    unsigned int number = state;

    // XOR shift algorithm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    // update random number state
    state = number;

    // return random number
    return number;
}

U64 get_random_U64() {
    U64 n1;
    U64 n2;
    U64 n3;
    U64 n4;

    // init random numbers slicing 16 bits from MS1B side
    n1 = static_cast<U64>(get_random_U32()) & 0xFFFF;
    n2 = static_cast<U64>(get_random_U32()) & 0xFFFF;
    n3 = static_cast<U64>(get_random_U32()) & 0xFFFF;
    n4 = static_cast<U64>(get_random_U32()) & 0xFFFF;

    // return random number
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

// generate magic number candidate
U64 generate_magic_number() {
    return get_random_U64() & get_random_U64() & get_random_U64(); // NOLINT
}
