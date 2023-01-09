#ifndef UCI_H_
#define UCI_H_

#include "attack_table.hpp"
#include "encoding.hpp"
#include "make_move.hpp"
#include <string>

class UCI {
  public:
    UCI(AttackTable const &attackTable, GameState &s, MakeMove const &makeMove)
        : attackT(attackTable)
        , state(s)
        , makeM(makeMove) {
    }

    ~UCI() {
    }

    EncodedMove parseMove(std::string const &move);
    void parsePosition(char *command);
    void parseGo(char *command);
    void uciLoop();

  private:
    AttackTable attackT;
    GameState &state;
    MakeMove makeM;
};

#endif // UCI_H_
