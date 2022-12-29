#ifndef ENCODING_H_
#define ENCODING_H_

#include <types.hpp>
using namespace Types;

/*
          binary move bits                            hexidecimal constants
    0000 0000 0000 0000 0011 1111    source square       0x3f
    0000 0000 0000 1111 1100 0000    target square       0xfc0
    0000 0000 1111 0000 0000 0000    piece               0xf000
    0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
    0001 0000 0000 0000 0000 0000    capture flag        0x100000
    0010 0000 0000 0000 0000 0000    double push flag    0x200000
    0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
    1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/

class Move {
  public:
    Move(Square src, Square dst, Piece piece, Piece promotedPiece,
         unsigned int capture, unsigned int dblPush, unsigned int enPassant,
         unsigned int castling) {
        move = static_cast<unsigned int>(src) |
               (static_cast<unsigned int>(dst) << 6) |
               (static_cast<unsigned int>(piece) << 12) |
               (static_cast<unsigned int>(promotedPiece) << 16) |
               (capture << 20) | (dblPush << 21) | (enPassant << 22) |
               (castling << 23);
    }

    ~Move() {
    }

    int getSrc() const {
        return static_cast<int>(move & 0x3f);
    }

    int getDst() const {
        return static_cast<int>((move & 0xfc0) >> 6);
    }

    Piece getPiece() const {
        return static_cast<Piece>((move & 0xf000) >> 12);
    }

    Piece getPromoted() const {
        return static_cast<Piece>((move & 0xf0000) >> 16);
    }

    bool getCapture() const {
        return (move & 0x100000) != 0;
    }

    bool getDblPush() const {
        return (move & 0x200000) != 0;
    }

    bool getEnPassant() const {
        return (move & 0x400000) != 0;
    }

    bool getCastling() const {
        return (move & 0x800000) != 0;
    }

  protected:
    unsigned int move;
};
#endif // ENCODING_H_
