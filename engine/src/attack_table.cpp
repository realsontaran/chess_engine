#include "attack_table.h"

U64 arrPawnAttacks[2][64];
U64 arrKnightAttacks[64];
U64 arrKingAttacks[64];

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

U64 knight_attack_masking(square sq) {
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, sq);

    attacks |= noNoEa(bitboard);
    attacks |= noNoWe(bitboard);
    attacks |= soSoEa(bitboard);
    attacks |= soSoWe(bitboard);
    attacks |= noEaEa(bitboard);
    attacks |= noWeWe(bitboard);
    attacks |= soEaEa(bitboard);
    attacks |= soWeWe(bitboard);

    return attacks;
}

U64 king_attack_masking(square sq) {
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, sq);

    attacks |= soutOne(bitboard);
    attacks |= nortOne(bitboard);
    attacks |= eastOne(bitboard);
    attacks |= westOne(bitboard);

    attacks |= soEaOne(bitboard);
    attacks |= soWeOne(bitboard);
    attacks |= noEaOne(bitboard);
    attacks |= noWeOne(bitboard);

    return attacks;
}

void init_leapers_attacks() {
    for (int sq = 0; sq < 64; ++sq) {
        /* Init pawn attack tables */
        arrPawnAttacks[white][sq] = pawn_attack_masking(white, (square)sq);
        arrPawnAttacks[black][sq] = pawn_attack_masking(black, (square)sq);

        /* Init pawn attack tables */
        arrKnightAttacks[sq] = knight_attack_masking((square)sq);

        /* Init king attack tables */
        arrKingAttacks[sq] = king_attack_masking((square)sq);
    }
}
