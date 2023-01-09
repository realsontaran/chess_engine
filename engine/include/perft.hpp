#ifndef PERFT_H_
#define PERFT_H_
#include <make_move.hpp>
#include <move_generation.hpp>
#include <move_list.hpp>
#include <game_state.hpp>
#include <string>

class PERFT {
  public:
    PERFT(AttackTable const &attackTable, GameState const &st)
        : state(st)
        , attackT(attackTable) {
    }

    ~PERFT() {
    }

    long perftDriver(int depth);
    void perftTest(int depth);
    void print();

  private:
    GameState state;
    AttackTable attackT;
};

#endif // PERFT_H_
