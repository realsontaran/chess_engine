#include "bitboard.h"
#include "attack_table.h"
#include <iostream>

int main(int argc, char *argv[]) {
    U64 board = 0ULL;
    square sq = e4;
    side sd = white;
    set_bit(board, sq);
    board |= king_attack_masking(sq);

    printBoard(board);

    return 0;
}
