#ifndef MAKE_MOVE_H_
#define MAKE_MOVE_H_

#include <encoding.hpp>
#include <game_state.hpp>

class MakeMove {
  public:
    MakeMove(GameState &s)
        : state(s)
        , copyState(s) {
    }

    virtual ~MakeMove();

    int makeIt(EncodedMove const &move, MoveType type);
    void copyBoard();
    void takeBack();

  private:
    GameState &state;
    GameState copyState;
};

#endif // MAKE_MOVE_H_
