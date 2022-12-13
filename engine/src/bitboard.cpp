#include <bitboard.hpp>
#include <stdio.h>

U64 eastOne(U64 b) {
    return (b & Bitboard::notHFile) << 1;
}

U64 noEaOne(U64 b) {
    return (b & Bitboard::notHFile) >> 7;
}

U64 soEaOne(U64 b) {
    return (b & Bitboard::notHFile) << 9;
}

U64 westOne(U64 b) {
    return (b & Bitboard::notAFile) >> 1;
}

U64 soWeOne(U64 b) {
    return (b & Bitboard::notAFile) << 7;
}

U64 noWeOne(U64 b) {
    return (b & Bitboard::notAFile) >> 9;
}

U64 soutOne(U64 b) {
    return b << 8;
}

U64 nortOne(U64 b) {
    return b >> 8;
}

U64 noNoEa(U64 b) {
    return (b & Bitboard::notHFile) >> 15;
}

U64 noNoWe(U64 b) {
    return (b & Bitboard::notAFile) >> 17;
}

U64 soSoEa(U64 b) {
    return (b & Bitboard::notHFile) << 17;
}

U64 soSoWe(U64 b) {
    return (b & Bitboard::notAFile) << 15;
}

/* Knight Movements */
U64 noEaEa(U64 b) {
    return (b & Bitboard::notHorGFile) >> 6;
}

U64 soEaEa(U64 b) {
    return (b & Bitboard::notHorGFile) << 10;
}

U64 noWeWe(U64 b) {
    return (b & Bitboard::notAorBFile) >> 10;
}

U64 soWeWe(U64 b) {
    return (b & Bitboard::notAorBFile) << 6;
}

void Bitboard::printBoard(U64 board) {
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            if (file == 0) {
                printf("  %d ", 8 - rank);
            }
            printf("%s", get_bit(board, square) != 0U ? "[#]" : "[ ]");
        }
        printf("\n");
    }
    printf("     a  b  c  d  e  f  g  h\n");
    printf("\n   Hex = 0x%llx\n   LLU = %llu\n", board, board);
}
