#ifndef MOVES_H_
#define MOVES_H_

#include <bitboard.hpp>
#include <attack_table.hpp>
#include <game_state.hpp>

class Move {
  public:
    Move(GameState const &s)
        : state(s) {
        attackTable = AttackTables();
    }

    virtual ~Move();

    void generateMoves();

  private:
    AttackTables attackTable;
    GameState state;

    U64 whitePawnAbleToPush();
    U64 whitePawnAbleToDblPush();
};

#endif // MOVES_H_
