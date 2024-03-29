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
    std::string tricky_position =
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
    std::string cmk_position =
        "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 ";
    std::string killer_position =
        "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";
    AttackTable attackTable = AttackTable();
    GameState state;

    if (DEBUG) {
        state.parseFEN(cmk_position);
        state.printBoard(true);
        Evaluation eval(state, attackTable);
        eval.searchPosition(4);
    } else {
        UCI uci(attackTable, state);
        uci.uciLoop();
        // PERFT perft(attackTable, state);
        // perft.perftTest(3);
    }
    return 0;
}
