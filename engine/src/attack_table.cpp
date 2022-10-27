#include "attack_table.h"

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
