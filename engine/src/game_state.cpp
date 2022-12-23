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

void GameState::printUnicodeBoard() {
    // clang-format on
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            if (file == 0) {
                printf("  %d ", 8 - rank);
            }
            int piece = -1;
            for (int bb_piece = P; bb_piece <= k; bb_piece++) {
                if (Bitboard::getBit(piecePositions[bb_piece], square) != 0ULL)
                    piece = bb_piece;
            }
            printf(" %s ", (piece == -1) ? "." : unicodePieces[piece].c_str());
        }
        printf("\n");
    }
    printf("     a  b  c  d  e  f  g  h\n");
    printf("     Side:     %s\n", sideToMove == 0 ? "white" : "black");
    printf("     En_passant:   %s\n",
           (enPassantSquare != Types::no_sq)
               ? Types::squareToString(enPassantSquare)
               : "no");
    printf("     Castling:  %c%c%c%c\n\n", (castleRights & wk) != 0 ? 'K' : '-',
           (castleRights & wq) != 0 ? 'Q' : '-',
           (castleRights & bk) != 0 ? 'k' : '-',
           (castleRights & bq) != 0 ? 'q' : '-');
}

void GameState::parseFEN(std::string const &fenStr) {
    memset(piecePositions, 0ULL, sizeof(piecePositions));
    memset(occupancies, 0ULL, sizeof(occupancies));
    sideToMove = 0;
    enPassantSquare = Types::no_sq;
    castleRights = 0;

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
            setBit(piecePositions[pp], sq);
            ++sq;
        }
    }
    fen >> token;
    sideToMove = (token == 'w' ? Types::white : Types::black);
    fen >> token;
    while ((fen >> token) && (isspace(token) == 0)) {
        switch (token) {
        case 'K':
            castleRights |= wk;
            break;
        case 'Q':
            castleRights |= wq;
            break;
        case 'k':
            castleRights |= bk;
            break;
        case 'q':
            castleRights |= bq;
            break;
        case '-':
            break;
        }
    }
    if (((fen >> col) && (col >= 'a' && col <= 'h')) &&
        ((fen >> row) && (row == '3' || row == '6'))) {
        int file = col - 'a';
        int rank = 8 - (row - '0');
        enPassantSquare = rank * 8 + file;
    } else {
        enPassantSquare = Types::no_sq;
    }
    // loop over white pieces bitboards
    for (int piece = P; piece <= K; piece++)
        // populate white occupancy bitboard
        occupancies[Types::white] |= piecePositions[piece];

    // loop over black pieces bitboards
    for (int piece = p; piece <= k; piece++)
        // populate white occupancy bitboard
        occupancies[Types::black] |= piecePositions[piece];

    // init all occupancies
    occupancies[Types::both] |= occupancies[Types::white];
    occupancies[Types::both] |= occupancies[Types::black];
}
