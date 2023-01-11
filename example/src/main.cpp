#include "evaluation.hpp"
#include "make_move.hpp"
#include "move_list.hpp"
#include "perft.hpp"
#include "uci.hpp"
#include <attack_table.hpp>
#include <move_generation.hpp>
#include <game_state.hpp>
#include <cstdio>
#include <string>
#include <encoding.hpp>
#include <time.hpp>

using namespace Types;
using namespace Bitboard;

#define DEBUG 1

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    std::string tricky_position = "r3k2r/p11pqpb1/bn2pnp1/2pPN3/1p2P3/2N2Q1p/"
                                  "PPPBBPPP/R3K2R w KQkq c6 0 1 ";
    std::string cmk_position =
        "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 ";
    AttackTable attackTable = AttackTable();
    GameState state;

    if (DEBUG) {
        state.parseFEN(tricky_position);
        state.printBoard(true);
        MoveList list;
        Evaluation eval(state, attackTable);
        eval.searchPosition(3);
    } else {
        UCI uci(attackTable, state);
        uci.uciLoop();
        // PERFT perft(attackTable, state);
        // perft.perftTest(3);
    }
    return 0;
}
