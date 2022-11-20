#include <game_state.h>
#include <cstdio>
#include <string>

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    GameState state;
    std::string pos =
        "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";
    state.parseFEN(pos);
    state.printAsciiBoard();

    return 0;
}
