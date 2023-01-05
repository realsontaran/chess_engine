#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <bitboard.hpp>
#include <types.hpp>
#include <sstream>

class GameState {
  public:
    GameState();
    virtual ~GameState();

    void printBoard(bool unicode);
    void parseFEN(std::string const &fenStr);

    U64 piecePositions[12] = {0ULL}; // All pieces
    U64 occupancies[3] = {0ULL};     // Black, white and all
    int enPassantSquare = Types::Square::no_sq;
    Types::Side sideToMove = Types::white; // Side to move
    int castleRights = 0;                  // Castling Rights
  private:
    static int getPieceIdFromChar(char c);
};

#endif // GAME_STATE_H_
