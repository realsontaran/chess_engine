#include <make_move.hpp>

MakeMove::~MakeMove() {
}

int MakeMove::makeIt(EncodedMove const &move, MoveType type) {
    if (type == MoveType::all_moves) {
        int src = move.getSrc();
        int dst = move.getDst();
        Piece piece = move.getPiece();
        // Piece promoted = move.getPromoted();
        bool capture = move.getCapture();
        // bool dblPush = move.getDblPush();
        // bool enPassant = move.getEnPassant();
        // bool castling = move.getCastling();

        // move piece
        Bitboard::popBit(state.piecePositions[piece], src);
        Bitboard::setBit(state.piecePositions[piece], dst);

        if (capture) {
            int start_piece = P;
            int end_piece = K;
            if (state.sideToMove == white) {
                start_piece = p;
                end_piece = k;
            }
            for (auto i = start_piece; i <= end_piece; ++i) {
                if (Bitboard::getBit(state.piecePositions[i], dst) != 0ULL) {
                    Bitboard::popBit(state.piecePositions[i], dst);
                    break;
                }
            }
        }
    } else {
        if (move.getCapture()) {
            makeIt(move, all_moves);
        } else {
            return 0;
        }
    }
    return 0;
}

void MakeMove::copyBoard() {
    copyState = state;
}

void MakeMove::takeBack() {
    state = copyState;
}
