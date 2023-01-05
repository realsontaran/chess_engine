#include <perft.hpp>

U64 PERFT::perftDriver(int depth) {
    if (depth == 0) {
        return 1;
    }
    MoveList move_list;
    MoveGeneration moveGenerator(state, move_list, attackT);
    U64 nodes = 0;

    MakeMove makeMove(state, attackT);
    moveGenerator.generateMoves();

    for (auto const &m : move_list.moves) {
        makeMove.copyBoard();
        if (!makeMove.makeIt(m, Types::all_moves))
            continue;
        // if (m.getCapture()) {
        //     ++capture;
        // }
        // if (m.getEnPassant()) {
        //     ++ep;
        // }
        // if (m.getCastling()) {
        //     ++castle;
        // }
        // if (m.getPromoted() != P && m.getPromoted() != p) {
        //     ++prom;
        // }

        nodes += perftDriver(depth - 1);
        makeMove.takeBack();
    }
    return nodes;
}

void PERFT::print() {
    std::cout << "Capture: " << capture << "\nEP: " << ep
              << "\nCastles: " << castle << "\nPromo: " << prom << std::endl;
}
