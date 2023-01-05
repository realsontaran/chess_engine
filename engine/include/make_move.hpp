#ifndef MAKE_MOVE_H_
#define MAKE_MOVE_H_

#include <attack_table.hpp>
#include <encoding.hpp>
#include <game_state.hpp>

class MakeMove {
  public:
    MakeMove(GameState &s, AttackTable const &attackTable)
        : state(s)
        , copyState(state)
        , attackT(attackTable) {
    }

    virtual ~MakeMove();

    bool makeIt(EncodedMove const &move, MoveType type);
    void copyBoard();
    void takeBack();

  private:
    GameState &state;
    GameState copyState;
    AttackTable attackT;

    void handleCastle(EncodedMove const &move);
    void handleCapture(EncodedMove const &move);
    void handlePromotion(EncodedMove const &move);
    void handleEnPassant(EncodedMove const &move);
    void handleDblPush(EncodedMove const &move);
    void updateStats();
    bool checkLegalKingMove();
};

#endif // MAKE_MOVE_H_
