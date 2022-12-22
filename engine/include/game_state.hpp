#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <bitboard.hpp>
#include <types.hpp>
#include <sstream>

class GameState {
  public:
    GameState();
    virtual ~GameState();

    void printUnicodeBoard();
    void parseFEN(std::string const &fenStr);

    U64 piecePositions[12] = {0ULL}; // All pieces
    U64 occupancies[3] = {0ULL};  // Black, white and all
    int enPassantSquare = Types::Square::no_sq;
    int sideToMove = -1;  // Side to move
    int castleRights = 0; // Castling Rights
  private:
    enum Castle { wk = 1, wq = 2, bk = 4, bq = 8 };

    enum AsciiPieces { P, N, B, R, Q, K, p, n, b, r, q, k };

    std::string unicodePieces[12] = {"\u2659", "\u2658", "\u2657", "\u2656",
                                      "\u2655", "\u2654", "\u265F", "\u265E",
                                      "\u265D", "\u265C", "\u265B", "\u265A"};

    char const *asciiPieces = "PNBRQKpnbrqk";

    static int getPieceIdFromChar(char c);
};

#endif // GAME_STATE_H_
