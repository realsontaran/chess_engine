#ifndef MOVES_H_
#define MOVES_H_

#include <bitboard.hpp>
#include <attack_table.hpp>
#include <game_state.hpp>

class MoveGeneration {
  public:
    MoveGeneration(GameState const &s)
        : state(s) {
        attackTable = AttackTable();
    }

    virtual ~MoveGeneration();

    void generateMoves();

  private:
    AttackTable attackTable;
    GameState state;

    void generatePawnMoves(Types::Side side);
    U64 wSinglePushPawns();
    U64 wDblPushPawns();
    U64 wPawnsAble2Push();
    U64 wPawnsAble2DblPush();

    U64 bSinglePushPawns();
    U64 bDblPushPawns();
    U64 bPawnsAble2Push();
    U64 bPawnsAble2DblPush();

    void generateCastles(Types::Side side);
    bool static isPromotionSquare(Types::Side side, Types::Square srcSq);
};

#endif // MOVES_H_
