#include "time.hpp"
#include <perft.hpp>

long PERFT::perftDriver(int depth) { // NOLINT
    if (depth == 0) {
        return 1;
    }
    MoveList move_list;
    MoveGeneration moveGenerator(state, move_list, attackT);
    long nodes = 0;

    MakeMove makeMove(state, attackT);
    moveGenerator.generateMoves();

    for (auto const &m : move_list.moves) {
        makeMove.copyBoard();
        if (!makeMove.makeIt(m, Types::all_moves)) {
            continue;
        }
        nodes += perftDriver(depth - 1);
        makeMove.takeBack();
    }
    return nodes;
}

void PERFT::perftTest(int depth) {
    std::cout << "Performance Test\n";
    MoveList move_list;
    MoveGeneration moveGenerator(state, move_list, attackT);
    long nodes = 0;

    MakeMove makeMove(state, attackT);
    moveGenerator.generateMoves();

    long start = getTimeInMS();
    for (auto const &m : move_list.moves) {
        makeMove.copyBoard();
        if (!makeMove.makeIt(m, Types::all_moves)) {
            return;
        }
        long cumulative_nodes = nodes;
        nodes += perftDriver(depth - 1);
        long old_nodes = nodes - cumulative_nodes;
        makeMove.takeBack();
        // std::cout << "move: " << m.toString() << "     nodes: " << old_nodes
        //           << std::endl;
        printf("     move: %s%s%c  nodes: %ld\n",
               squareToString(m.getSrc()).c_str(),
               squareToString(m.getDst()).c_str(),
               (m.getPromoted() != P && m.getPromoted() != p)
                   ? pieceToString(m.getPromoted())
                   : ' ',
               old_nodes);
    }
    long end = getTimeInMS();
    std::cout << "Depth: " << depth << std::endl;
    std::cout << "Nodes: " << nodes << std::endl;
    std::cout << "Time: " << end - start << std::endl;
}
