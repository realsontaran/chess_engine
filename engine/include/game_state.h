#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <bitboard.h>
#include <types.h>
#include <sstream>

class GameState {
  public:
    GameState();
    virtual ~GameState();
    void printAsciiBoard();
    void parseFEN(std::string const &fenStr);

    U64 pieceBoards[12] = {0ULL}; // All pieces
    U64 occupancies[3] = {0ULL};  // Black, white and all
  private:
    enum Castle { wk = 1, wq = 2, bk = 4, bq = 8 };

    enum AsciiPieces { P, N, B, R, Q, K, p, n, b, r, q, k };

    char const *ascii_pieces = "PNBRQKpnbrqk";
    int side = -1; // Side to move
    int en_passant = Types::Square::no_sq;
    int castle = 0; // Castling Rights

    static int getPieceIdFromChar(char c);
};

#endif // GAME_STATE_H_
