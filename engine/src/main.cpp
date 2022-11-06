#include "attack_table.h"
#include <iostream>

int main(int argc, char *argv[]) {
    U64 board = 0ULL;
    square sq = c2;
    side sd = white;
    set_bit(board, sq);
    U64 blocks = 0ULL;
    set_bit(blocks, f2);
    set_bit(blocks, c6);
    set_bit(blocks, b2);

    printBoard(board);

    return 0;
}
