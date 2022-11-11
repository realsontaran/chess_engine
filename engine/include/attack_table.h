#ifndef ATTACK_TABLE_H_
#define ATTACK_TABLE_H_

#include <bitboard.h>
#include <prng.h>

class AttackTables {
  public:
    AttackTables();
    virtual ~AttackTables();

    U64 arrPawnAttacks[2][64];
    U64 arrKnightAttacks[64];
    U64 arrKingAttacks[64];

  private:
    const static int bishop_relevant_bits[];
    const static int rook_relevant_bits[];
    const static U64 rook_magic_numbers[];
    const static U64 bishop_magic_numbers[];

    void init_leapers_attacks();
    static U64 pawn_attack_masking(side sd, square sq);
    static U64 knight_attack_masking(square sq);
    static U64 king_attack_masking(square sq);
    static U64 bishop_attack_masking(square sq);
    static U64 rook_attack_masking(square sq);
    static U64 generate_bishop_attacks(square sq, U64 blocks);
    static U64 generate_rook_attacks(square sq, U64 blocks);
    static U64 set_occupancy_map(int index, int bit_count, U64 attack_mask);
    static U64 find_magic_number(square sq, int relevant_bits, bool bishop);
    static void init_magic_numbers();
};
#endif // ATTACK_TABLE_H_
