#include "bitboard.h"
#include "attack_table.h"
#include <iostream>

int main(int argc, char *argv[]) {
    U64 board = 0ULL;
    square sq = a1;
    side sd = white;
    set_bit(board, sq);
    board |= pawn_attack_masking(sd, sq);

    printBoard(board);
    return 0;
}
