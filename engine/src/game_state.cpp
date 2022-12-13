#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <game_state.hpp>

using namespace Bitboard;

GameState::GameState() {
}

GameState::~GameState() {
}

int GameState::getPieceIdFromChar(char c) {
    switch (c) {
    case 'P':
        return AsciiPieces::P;
    case 'N':
        return AsciiPieces::N;
    case 'B':
        return AsciiPieces::B;
    case 'R':
        return AsciiPieces::R;
    case 'Q':
        return AsciiPieces::Q;
    case 'K':
        return AsciiPieces::K;
    case 'p':
        return AsciiPieces::p;
    case 'n':
        return AsciiPieces::n;
    case 'b':
        return AsciiPieces::b;
    case 'r':
        return AsciiPieces::r;
    case 'q':
        return AsciiPieces::q;
    case 'k':
        return AsciiPieces::k;
    }
    return -1;
}

void GameState::printAsciiBoard() {
    // clang-format on
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
            printf(" %s ", (piece == -1) ? "." : unicode_pieces[piece].c_str());
        }
        printf("\n");
    }
    printf("     a  b  c  d  e  f  g  h\n");
    printf("     Side:     %s\n", side == 0 ? "white" : "black");
    printf("     En_passant:   %s\n", (en_passant != Types::no_sq)
                                          ? Types::squareToString(en_passant)
                                          : "no");
    printf("     Castling:  %c%c%c%c\n\n", (castle & wk) != 0 ? 'K' : '-',
           (castle & wq) != 0 ? 'Q' : '-', (castle & bk) != 0 ? 'k' : '-',
           (castle & bq) != 0 ? 'q' : '-');
}

void GameState::parseFEN(std::string const &fenStr) {
    memset(pieceBoards, 0ULL, sizeof(pieceBoards));
    memset(occupancies, 0ULL, sizeof(occupancies));
    side = 0;
    en_passant = Types::no_sq;
    castle = 0;

    char col;
    char row;
    char token;
    Types::Square sq = Types::a8;
    const std::string PieceToChar("PNBRQKpnbrqk");
    std::istringstream fen(fenStr);
    fen >> std::noskipws;

    while ((fen >> token) && (isspace(token) == 0)) {
        if (isdigit(token) != 0) {
            sq += token - '0';
        } else if (token == '/') {
            continue;
        } else if ((PieceToChar.find(token)) != std::string::npos) {
            int pp = getPieceIdFromChar(token);
            set_bit(pieceBoards[pp], sq);
            ++sq;
        }
    }
    fen >> token;
    side = (token == 'w' ? Types::white : Types::black);
    fen >> token;
    while ((fen >> token) && (isspace(token) == 0)) {
        switch (token) {
        case 'K':
            castle |= wk;
            break;
        case 'Q':
            castle |= wq;
            break;
        case 'k':
            castle |= bk;
            break;
        case 'q':
            castle |= bq;
            break;
        case '-':
            break;
        }
    }
    if (((fen >> col) && (col >= 'a' && col <= 'h')) &&
        ((fen >> row) && (row == '3' || row == '6'))) {
        int file = col - 'a';
        int rank = 8 - (row - '0');
        en_passant = rank * 8 + file;
    } else {
        en_passant = Types::no_sq;
    }
    // loop over white pieces bitboards
    for (int piece = P; piece <= K; piece++)
        // populate white occupancy bitboard
        occupancies[Types::white] |= pieceBoards[piece];

    // loop over black pieces bitboards
    for (int piece = p; piece <= k; piece++)
        // populate white occupancy bitboard
        occupancies[Types::black] |= pieceBoards[piece];

    // init all occupancies
    occupancies[Types::both] |= occupancies[Types::white];
    occupancies[Types::both] |= occupancies[Types::black];
}
