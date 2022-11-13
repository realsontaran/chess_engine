#include <cstdio>
#include <game_state.h>

using namespace Bitboard;

GameState::GameState() {
}

GameState::~GameState() {
}

int GameState::getPieceIdFromChar(char c) {
    switch (c) {
    case 'P':
        return Ascii::P;
    case 'N':
        return Ascii::N;
    case 'B':
        return Ascii::B;
    case 'R':
        return Ascii::R;
    case 'Q':
        return Ascii::Q;
    case 'K':
        return Ascii::K;
    case 'p':
        return Ascii::p;
    case 'n':
        return Ascii::n;
    case 'b':
        return Ascii::b;
    case 'r':
        return Ascii::r;
    case 'q':
        return Ascii::q;
    case 'k':
        return Ascii::k;
    }
    return -1;
}

void GameState::printAsciiBoard() {
    char const *square_strings[] = {
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", "a7", "b7", "c7",
        "d7", "e7", "f7", "g7", "h7", "a6", "b6", "c6", "d6", "e6", "f6",
        "g6", "h6", "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a4",
        "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a3", "b3", "c3", "d3",
        "e3", "f3", "g3", "h3", "a2", "b2", "c2", "d2", "e2", "f2", "g2",
        "h2", "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"};

    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            if (file == 0) {
                printf("  %d ", 8 - rank);
            }
            int piece = -1;
            for (int bb_piece = P; bb_piece <= k; bb_piece++) {
                if (Bitboard::get_bit(pieceBoards[bb_piece], square) != 0ULL)
                    piece = bb_piece;
            }
            printf("[%c]", (piece == -1) ? '.' : ascii_pieces[piece]);
        }
        printf("\n");
    }
    printf("     a  b  c  d  e  f  g  h\n");
    printf("     Side:     %s\n", side == 0 ? "white" : "black");
    printf("     Enpassant:   %s\n", (en_passant != Bitboard::Square::no_sq)
                                         ? square_strings[en_passant]
                                         : "no");
    printf("     Castling:  %c%c%c%c\n\n", (castle & wk) != 0 ? 'K' : '-',
           (castle & wq) != 0 ? 'Q' : '-', (castle & bk) != 0 ? 'k' : '-',
           (castle & bq) != 0 ? 'q' : '-');
}
