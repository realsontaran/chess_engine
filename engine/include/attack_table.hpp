#ifndef ATTACK_TABLE_H_
#define ATTACK_TABLE_H_

#include <bitboard.hpp>
#include <game_state.hpp>
#include <types.hpp>
#include <prng.hpp>

class AttackTables {
  public:
    AttackTables();
    virtual ~AttackTables();

    U64 arrPawnAttacks[2][64];
    U64 arrKnightAttacks[64];
    U64 arrKingAttacks[64];
    U64 arrRookAttacks[64][4096];
    U64 arrBishopAttacks[64][512];
    bool isSquareAttacked(Types::Square sq, Types::Side side, GameState &state);

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
    static U64 pawnAttackMask(Types::Side sd, Types::Square sq);
    static U64 knightAttackMask(Types::Square sq);
    static U64 kingAttackMask(Types::Square sq);
    static U64 bishopAttackMask(Types::Square sq);
    static U64 rookAttackMask(Types::Square sq);
    static U64 generateBishopAttacks(Types::Square sq, U64 blocks);
    static U64 generateRookAttacks(Types::Square sq, U64 blocks);
    U64 getRookAttacks(Types::Square sq, U64 occ);
    U64 getBishopAttacks(Types::Square sq, U64 occ);
    U64 getQueenAttacks(Types::Square sq, U64 occ);
    static U64 setOccupancies(int index, int bits_in_mask, U64 attack_mask);
    static U64 findMagics(Types::Square sq, int relevant_bits, bool bishop);
};
#endif // ATTACK_TABLE_H_
