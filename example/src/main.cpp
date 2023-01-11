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

#define DEBUG 0

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    std::string tricky_position = "r3k2r/p11pqpb1/bn2pnp1/2pPN3/1p2P3/2N2Q1p/"
                                  "PPPBBPPP/R3K2R w KQkq c6 0 1 ";

    AttackTable attackTable = AttackTable();
    GameState state;

    if (DEBUG) {
        state.parseFEN("4k3/8/5K2/8/1Q6/8/8/8 w - - 1 1");

        Evaluation eval(state, attackTable);
        state.printBoard(true);
        eval.searchPosition(2);
    } else {
        UCI uci(attackTable, state);
        uci.uciLoop();
        // PERFT perft(attackTable, state);
        // perft.perftTest(3);
    }
    return 0;
}
