#include "prng.h"

static unsigned int state = 1804289383; // Random start state

unsigned int get_random() {
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
