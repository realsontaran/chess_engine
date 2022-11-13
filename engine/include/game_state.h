#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <bitboard.h>

// ASCII pieces

class GameState {
  public:
    GameState();
    virtual ~GameState();

  private:
    enum Castle { wk = 1, wq = 2, bk = 4, bq = 8 };

    enum Ascii { P, N, B, R, Q, K, p, n, b, r, q, k };

    char const *ascii_pieces = "PNBRQKpnbrqk";
    int side = -1; // Side to move
    int en_passant = Bitboard::Square::no_sq;
    int castle = 0; // Castling Rights

    U64 pieceBoards[12] = {0ULL}; // All pieces
    U64 occupancies[3] = {0ULL};  // Black, white and all
    static int getPieceIdFromChar(char c);
    void printAsciiBoard();
};

#endif // GAME_STATE_H_
