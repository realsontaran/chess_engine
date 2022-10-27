#include "bitboard.h"
#include <stdio.h>

U64 eastOne(U64 b) {
    return (b & notHFile) << 1;
}
U64 noEaOne(U64 b) {
    return (b & notHFile) >> 7;
}
U64 soEaOne(U64 b) {
    return (b & notHFile) << 9;
}
U64 westOne(U64 b) {
    return (b & notAFile) >> 1;
}
U64 soWeOne(U64 b) {
    return (b & notAFile) << 7;
}
U64 noWeOne(U64 b) {
    return (b & notAFile) >> 9;
}
U64 soutOne(U64 b) {
    return b << 8;
}
U64 nortOne(U64 b) {
    return b >> 8;
}

void printBoard(U64 board) {
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            if (file == 0) {
                printf("  %d ", 8 - rank);
            }
            printf(" %d", get_bit(board, square) ? 1 : 0);
        }
        printf("\n");
    }
    printf("     a b c d e f g h\n\n");
}
