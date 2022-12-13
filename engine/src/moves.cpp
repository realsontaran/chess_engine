#include <moves.hpp>

void Move::generateMoves() {
    Bitboard::printBoard(whitePawnAbleToDblPush());
}

Move::~Move() {
}

U64 Move::whitePawnAbleToPush() {
    return soutOne(state.occupancies[Types::both]) &
           state.pieceBoards[Types::P];
}

U64 Move::whitePawnAbleToDblPush() {
    const U64 rank4 = 0x00000000FF00000000ULL;
    const U64 rank3 = soutOne(rank4);

    U64 empty = state.occupancies[Types::both];
    const U64 one = rank3 & empty;
    const U64 two = rank4 & empty;

    Bitboard::printBoard(soutOne(one));
    Bitboard::printBoard(soutOne(one) | soutOne(soutOne(two)));
    Bitboard::printBoard(state.pieceBoards[Types::P]);

    return (soutOne(one) | soutOne(soutOne(two))) & state.pieceBoards[Types::P];
}
