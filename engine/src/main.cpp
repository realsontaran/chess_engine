#include "bitboard.h"
#include <iostream>

int main(int argc, char *argv[]) {
    u64 board = 0ULL;
    printBoard(board);
    printBoard(set_bit(board, e4));
    return 0;
}
