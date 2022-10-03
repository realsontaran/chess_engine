#include "bitboard.h"
#include <stdio.h>

void printBoard(u64 board) {
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
