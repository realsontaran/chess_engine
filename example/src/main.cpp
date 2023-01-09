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

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    std::string tricky_position = "r3k2r/p11pqpb1/bn2pnp1/2pPN3/1p2P3/2N2Q1p/"
                                  "PPPBBPPP/R3K2R w KQkq c6 0 1 ";

    AttackTable attackTable = AttackTable();
    GameState state;

    MakeMove makeMove(state, attackTable);
    UCI uci(attackTable, state, makeMove);
    uci.uciLoop();
    // PERFT perft(attackTable, state);
    // perft.perftTest(3);

    return 0;
}
