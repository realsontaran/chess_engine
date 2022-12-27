#ifndef ATTACK_TABLE_H_
#define ATTACK_TABLE_H_

#include <bitboard.hpp>
#include <game_state.hpp>
#include <types.hpp>
#include <prng.hpp>

class AttackTable {
  public:
    AttackTable();
    virtual ~AttackTable();

    U64 pawnAttacks[2][64];
    U64 knightAttacks[64];
    U64 kingAttacks[64];
    bool isSquareAttacked(Types::Square sq, Types::Side side, GameState &state);
    U64 getRookAttacks(Types::Square sq, U64 occ);
    U64 getBishopAttacks(Types::Square sq, U64 occ);
    U64 getQueenAttacks(Types::Square sq, U64 occ);

  private:
    const static int bishopRelevants[];
    const static int rookRelevants[];
    const static U64 bishopMagics[];
    const static U64 rookMagics[];

    U64 bishopMasks[64];
    U64 rookMasks[64];

    U64 rookAttacks[64][4096];
    U64 bishopAttacks[64][512];

    void initAll();
    void initLeapers();
    void initSliders(bool bishop);
    void initMagics();
    static U64 pawnAttackMask(Types::Side sd, Types::Square sq);
    static U64 knightAttackMask(Types::Square sq);
    static U64 kingAttackMask(Types::Square sq);
    static U64 bishopAttackMask(Types::Square sq);
    static U64 rookAttackMask(Types::Square sq);
    static U64 generateBishopAttacks(Types::Square sq, U64 blocks);
    static U64 generateRookAttacks(Types::Square sq, U64 blocks);
    static U64 setOccupancies(int index, int bits_in_mask, U64 attack_mask);
    static U64 findMagics(Types::Square sq, int relevant_bits, bool bishop);
};
#endif // ATTACK_TABLE_H_
