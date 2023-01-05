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

    U64 perftDriver(int depth);
    void print();

  private:
    GameState state;
    AttackTable attackT;
    int capture = 0;
    int ep = 0;
    int castle = 0;
    int prom = 0;
    int check = 0;
};

#endif // PERFT_H_
