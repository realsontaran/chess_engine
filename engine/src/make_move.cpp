#include <cstring>
#include <make_move.hpp>

MakeMove::~MakeMove() {
}

void MakeMove::handleCastle(EncodedMove const &move) {
    // castling rights update constants
    int const castling_rights[64] = {
        7,  15, 15, 15, 3,  15, 15, 11, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 13, 15, 15, 15, 12, 15, 15, 14};

    int src = move.getSrc();
    int dst = move.getDst();
    if (move.getCastling()) {
        switch (dst) {
        case g1:
            Bitboard::popBit(state.piecePositions[R], h1);
            Bitboard::setBit(state.piecePositions[R], f1);
            break;
        case c1:
            Bitboard::popBit(state.piecePositions[R], a1);
            Bitboard::setBit(state.piecePositions[R], d1);
            break;
        case g8:
            Bitboard::popBit(state.piecePositions[r], h8);
            Bitboard::setBit(state.piecePositions[r], f8);
            break;
        case c8:
            Bitboard::popBit(state.piecePositions[r], a8);
            Bitboard::setBit(state.piecePositions[r], d8);
            break;
        }
    }
    // update castle rights
    state.castleRights &= castling_rights[src];
    state.castleRights &= castling_rights[dst];
}

void MakeMove::handleCapture(EncodedMove const &move) {
    if (move.getCapture()) {
        int start_piece = P;
        int end_piece = K;
        int dst = move.getDst();

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
}

void MakeMove::handlePromotion(EncodedMove const &move) {
    Piece promoted = move.getPromoted();
    int dst = move.getDst();
    if (promoted != P && promoted != p) {
        Bitboard::popBit(
            state.piecePositions[(state.sideToMove == white) ? P : p], dst);
        Bitboard::setBit(state.piecePositions[promoted], dst);
    }
}

void MakeMove::handleEnPassant(EncodedMove const &move) {
    int dst = move.getDst();
    if (move.getEnPassant()) {
        (state.sideToMove == white)
            ? Bitboard::popBit(state.piecePositions[p], dst + 8)
            : Bitboard::popBit(state.piecePositions[P], dst - 8);
    }
    state.enPassantSquare = no_sq;
}

void MakeMove::handleDblPush(EncodedMove const &move) {
    int dst = move.getDst();
    if (move.getDblPush()) {
        (state.sideToMove == white) ? (state.enPassantSquare = dst + 8)
                                    : (state.enPassantSquare = dst - 8);
    }
}

void MakeMove::updateStats() {
    memset(state.occupancies, 0ULL, sizeof(state.occupancies));
    for (int i = P; i <= K; ++i) {
        state.occupancies[white] |= state.piecePositions[i];
    }

    for (int i = p; i <= k; ++i) {
        state.occupancies[black] |= state.piecePositions[i];
    }

    state.occupancies[both] |= state.occupancies[white];
    state.occupancies[both] |= state.occupancies[black];

    state.sideToMove = (state.sideToMove == white) ? black : white;
}

int MakeMove::checkLegalKingMove() {
    unsigned int kingSquare = (state.sideToMove == white)
                                  ? Bitboard::getLSB(state.piecePositions[k])
                                  : Bitboard::getLSB(state.piecePositions[K]);
    if (attackTable.isSquareAttacked(static_cast<Square>(kingSquare),
                                     state.sideToMove, state)) {
        takeBack();
        return 0;
    }
    return 1;
}

int MakeMove::makeIt(EncodedMove const &move, MoveType type) {
    if (type == MoveType::all_moves) {
        copyBoard();
        int src = move.getSrc();
        int dst = move.getDst();
        Piece piece = move.getPiece();

        // move piece
        Bitboard::popBit(state.piecePositions[piece], src);
        Bitboard::setBit(state.piecePositions[piece], dst);

        handleCapture(move);
        handlePromotion(move);
        handleEnPassant(move);
        handleDblPush(move);
        handleCastle(move);

        updateStats();

        return checkLegalKingMove();
    }
    if (move.getCapture()) {
        makeIt(move, all_moves);
    } else {
        return 0;
    }

    return 1;
}

void MakeMove::copyBoard() {
    copyState = state;
}

void MakeMove::takeBack() {
    state = copyState;
}
