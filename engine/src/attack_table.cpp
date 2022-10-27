#include "attack_table.h"

U64 arrPawnAttacks[2][64];

U64 pawn_attack_masking(side sd, square sq) {
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, sq);
    if (sd == white) {
        attacks |= noWeOne(bitboard);
        attacks |= noEaOne(bitboard);
    } else {
        attacks |= soWeOne(bitboard);
        attacks |= soEaOne(bitboard);
    }
    return attacks;
}

void init_leapers_attacks() {
    for (int sq = 0; sq < 64; ++sq) {
        arrPawnAttacks[white][sq] = pawn_attack_masking(white, (square)sq);
        arrPawnAttacks[black][sq] = pawn_attack_masking(black, (square)sq);
    }
}
