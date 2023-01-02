#include "make_move.hpp"
#include "move_list.hpp"
#include <attack_table.hpp>
#include <move_generation.hpp>
#include <game_state.hpp>
#include <cstdio>
#include <string>
#include <encoding.hpp>

using namespace Types;
using namespace Bitboard;

void print_attacked_squares(Side side, AttackTable &table, GameState &state) {
    printf("\n");

    // loop over board ranks
    for (int rank = 0; rank < 8; rank++) {
        // loop over board files
        for (int file = 0; file < 8; file++) {
            // init square
            Square square = Square(rank * 8 + file);

            // print ranks
            if (file == 0)
                printf("  %d ", 8 - rank);

            printf(" %d", table.isSquareAttacked(square, side, state) ? 1 : 0);
        }

        // print new line every rank
        printf("\n");
    }

    // print files
    printf("\n     a b c d e f g h\n\n");
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    GameState state;
    std::string empty_board = "8/8/8/8/8/8/8/8 w - - ";
    std::string start_position =
        "rn1q1rk1/pp2bppp/4pn2/3p1b2/2pP1B2/2P1PN2/PP1NBPPP/R2Q1RK1 w - - 6 9";
    std::string tricky_position =
        "r3k2r/pPppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1 ";
    std::string killer_position =
        "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";
    std::string cmk_position =
        "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 ";

    std::string test("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R "
                     "w KQkq - 0 1 ");
    state.parseFEN(tricky_position);
    printf("FEN: %s\n", tricky_position.c_str());

    AttackTable attack_table;

    MoveList moves;
    MoveGeneration move(state, &moves);
    MakeMove makeMove(state);
    move.generateMoves();

    for (auto const &m : moves.moves) {
        std::cout << m.toString() << std::endl;

        state.printBoard(true);
        makeMove.copyBoard();
        makeMove.makeIt(m, Types::all_moves);

        state.printBoard(true);

        makeMove.takeBack();

        (void)getchar();
    }

    return 0;
}
