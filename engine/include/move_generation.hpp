#ifndef MOVE_GENERATION_H_
#define MOVE_GENERATION_H_

#include <bitboard.hpp>
#include <attack_table.hpp>
#include <game_state.hpp>
#include <move_list.hpp>
#include <encoding.hpp>

class MoveGeneration {
  public:
    MoveGeneration(GameState const &s, MoveList &moveList,
                   AttackTable const &attackTable)
        : attackT(attackTable)
        , state(s)
        , moves(moveList) {
    }

    virtual ~MoveGeneration();

    void generateMoves();

  private:
    AttackTable attackT;
    GameState state;
    MoveList &moves;

    void generatePawnMoves(Types::Side side);
    U64 wSinglePushPawns();
    U64 wDblPushPawns();
    U64 wPawnsAble2Push();
    U64 wPawnsAble2DblPush();

    U64 bSinglePushPawns();
    U64 bDblPushPawns();
    U64 bPawnsAble2Push();
    U64 bPawnsAble2DblPush();
    bool static isPromotionSquare(Types::Side side, Types::Square srcSq);

    void generateCastles(Types::Side side);
    void generateSliderAndLeaperMoves(Types::Piece pieceType, Types::Side side);
};

#endif // MOVE_GENERATION_H_
