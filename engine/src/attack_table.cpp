#include "attack_table.h"

AttackTables::AttackTables() {
    init_leapers_attacks();
}

AttackTables::~AttackTables() {
}

U64 AttackTables::pawn_attack_masking(side sd, square sq) {
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

U64 AttackTables::knight_attack_masking(square sq) {
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

U64 AttackTables::king_attack_masking(square sq) {
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

U64 AttackTables::bishop_attack_masking(square sq) {
    U64 attacks = 0ULL;

    int rank;
    int file;

    // Target rank and file
    int t_rank = sq / 8;
    int t_file = sq % 8;

    for (rank = t_rank + 1, file = t_file + 1; rank < 7 && file < 7;
         ++rank, ++file)
        attacks |= (1ULL << (rank * 8 + file));
    for (rank = t_rank - 1, file = t_file + 1; rank >= 1 && file < 7;
         --rank, ++file)
        attacks |= (1ULL << (rank * 8 + file));
    for (rank = t_rank + 1, file = t_file - 1; rank < 7 && file >= 1;
         ++rank, --file)
        attacks |= (1ULL << (rank * 8 + file));
    for (rank = t_rank - 1, file = t_file - 1; rank >= 1 && file >= 1;
         --rank, --file)
        attacks |= (1ULL << (rank * 8 + file));

    return attacks;
}

U64 AttackTables::rook_attack_masking(square sq) {
    U64 attacks = 0ULL;

    int rank;
    int file;

    // Target rank and file
    int t_rank = sq / 8;
    int t_file = sq % 8;

    for (rank = t_rank + 1; rank < 7; ++rank)
        attacks |= (1ULL << (rank * 8 + t_file));

    for (rank = t_rank - 1; rank >= 1; --rank)
        attacks |= (1ULL << (rank * 8 + t_file));

    for (file = t_file + 1; file < 7; ++file)
        attacks |= (1ULL << (t_rank * 8 + file));

    for (file = t_file - 1; file >= 1; --file)
        attacks |= (1ULL << (t_rank * 8 + file));

    return attacks;
}

U64 AttackTables::generate_bishop_attacks(square sq, U64 blocks) {
    U64 attacks = 0ULL;

    int rank;
    int file;

    // Target rank and file
    int t_rank = sq / 8;
    int t_file = sq % 8;

    for (rank = t_rank + 1, file = t_file + 1; rank < 8 && file < 8;
         ++rank, ++file) {
        attacks |= (1ULL << (rank * 8 + file));
        if (((1ULL << (rank * 8 + file)) & blocks) != 0ULL)
            break;
    }
    for (rank = t_rank - 1, file = t_file + 1; rank >= 0 && file < 8;
         --rank, ++file) {
        attacks |= (1ULL << (rank * 8 + file));
        if (((1ULL << (rank * 8 + file)) & blocks) != 0ULL)
            break;
    }
    for (rank = t_rank + 1, file = t_file - 1; rank < 8 && file >= 0;
         ++rank, --file) {
        attacks |= (1ULL << (rank * 8 + file));
        if (((1ULL << (rank * 8 + file)) & blocks) != 0ULL)
            break;
    }
    for (rank = t_rank - 1, file = t_file - 1; rank >= 0 && file >= 0;
         --rank, --file) {
        attacks |= (1ULL << (rank * 8 + file));
        if (((1ULL << (rank * 8 + file)) & blocks) != 0ULL)
            break;
    }

    return attacks;
}

U64 AttackTables::generate_rook_attacks(square sq, U64 blocks) {
    U64 attacks = 0ULL;

    int rank;
    int file;

    // Target rank and file
    int t_rank = sq / 8;
    int t_file = sq % 8;

    for (rank = t_rank + 1; rank < 8; ++rank) {
        attacks |= (1ULL << (rank * 8 + t_file));
        if (((1ULL << (rank * 8 + t_file)) & blocks) != 0ULL)
            break;
    }

    for (rank = t_rank - 1; rank >= 0; --rank) {
        attacks |= (1ULL << (rank * 8 + t_file));
        if (((1ULL << (rank * 8 + t_file)) & blocks) != 0ULL)
            break;
    }

    for (file = t_file + 1; file < 8; ++file) {
        attacks |= (1ULL << (t_rank * 8 + file));
        if (((1ULL << (t_rank * 8 + file)) & blocks) != 0ULL)
            break;
    }
    for (file = t_file - 1; file >= 0; --file) {
        attacks |= (1ULL << (t_rank * 8 + file));
        if (((1ULL << (t_rank * 8 + file)) & blocks) != 0ULL)
            break;
    }

    return attacks;
}

/*
 * Calculate all occupancy variations from given attack mask
 */
U64 AttackTables::set_occupancy_map(int index, int bit_count, U64 attack_mask) {
    U64 occupancy_map = 0ULL;
    for (int bits = 0; bits < bit_count; ++bits) {
        square sq = static_cast<square>(get_lsb(attack_mask));
        pop_bit(attack_mask, sq);
        if ((index & (1 << bit_count)) != 0) {
            occupancy_map |= (1ULL << sq);
        }
    }

    return occupancy_map;
}

void AttackTables::init_leapers_attacks() {
    for (int sq = 0; sq < 64; ++sq) {
        /* Init pawn attack tables */
        arrPawnAttacks[white][sq] =
            pawn_attack_masking(white, static_cast<square>(sq));
        arrPawnAttacks[black][sq] =
            pawn_attack_masking(black, static_cast<square>(sq));

        /* Init pawn attack tables */
        arrKnightAttacks[sq] = knight_attack_masking(static_cast<square>(sq));

        /* Init king attack tables */
        arrKingAttacks[sq] = king_attack_masking(static_cast<square>(sq));
    }
}
