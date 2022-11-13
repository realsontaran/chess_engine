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
    U64 arrRookAttacks[64][4096];
    U64 arrBishopAttacks[64][512];

  private:
    const static int bishopRelevants[];
    const static int rookRelevants[];
    const static U64 bishopMagics[];
    const static U64 rookMagics[];

    U64 bishopMasks[64];
    U64 rookMasks[64];

    void initAll();
    void initLeapers();
    void initSliders(bool bishop);
    void initMagics();
    static U64 pawnAttackMask(Bitboard::Side sd, Bitboard::Square sq);
    static U64 knightAttackMask(Bitboard::Square sq);
    static U64 kingAttackMask(Bitboard::Square sq);
    static U64 bishopAttackMask(Bitboard::Square sq);
    static U64 rookAttackMask(Bitboard::Square sq);
    static U64 generateBishopAttacks(Bitboard::Square sq, U64 blocks);
    static U64 generateRookAttacks(Bitboard::Square sq, U64 blocks);
    U64 getRookAttacks(Bitboard::Square sq, U64 occ);
    U64 getBishopAttacks(Bitboard::Square sq, U64 occ);
    static U64 setOccupancies(int index, int bits_in_mask, U64 attack_mask);
    static U64 findMagics(Bitboard::Square sq, int relevant_bits, bool bishop);
};
#endif // ATTACK_TABLE_H_
