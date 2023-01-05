#include "make_move.hpp"
#include "move_list.hpp"
#include "perft.hpp"
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
    std::string tricky_position =
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";

    AttackTable attackTable = AttackTable();
    GameState state;
    state.parseFEN(tricky_position);

    PERFT perft(attackTable, state);

    // Start timer
    long start = getTimeInMS();

    U64 nodes = perft.perftDriver(2);
    // perft.print();

    long end = getTimeInMS();
    std::cout << "\n\nTime: " << end - start << std::endl;
    std::cout << "Nodes: " << nodes << std::endl;
    return 0;
}
