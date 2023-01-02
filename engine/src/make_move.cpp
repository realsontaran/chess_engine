#include <cstring>
#include <make_move.hpp>

MakeMove::~MakeMove() {
}

int MakeMove::makeIt(EncodedMove const &move, MoveType type) {
    // castling rights update constants
    int const castling_rights[64] = {
        7,  15, 15, 15, 3,  15, 15, 11, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15, 13, 15, 15, 15, 12, 15, 15, 14};

    if (type == MoveType::all_moves) {
        int src = move.getSrc();
        int dst = move.getDst();
        Piece piece = move.getPiece();
        Piece promoted = move.getPromoted();
        bool capture = move.getCapture();
        bool dblPush = move.getDblPush();
        bool enPassant = move.getEnPassant();
        bool castling = move.getCastling();

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
        if (promoted != P && promoted != p) {
            Bitboard::popBit(
                state.piecePositions[(state.sideToMove == white) ? P : p], dst);
            Bitboard::setBit(state.piecePositions[promoted], dst);
        }
        if (enPassant) {
            (state.sideToMove == white)
                ? Bitboard::popBit(state.piecePositions[p], dst + 8)
                : Bitboard::popBit(state.piecePositions[P], dst - 8);
        }
        state.enPassantSquare = no_sq;
        if (dblPush) {
            (state.sideToMove == white) ? (state.enPassantSquare = dst + 8)
                                        : (state.enPassantSquare = dst - 8);
        }
        if (castling) {
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

        // update occupancies
        memset(state.occupancies, 0ULL, sizeof(state.occupancies));
        for (int i = P; i <= K; ++i) {
            state.occupancies[white] |= state.piecePositions[i];
        }

        for (int i = p; i <= k; ++i) {
            state.occupancies[black] |= state.piecePositions[i];
        }

        state.occupancies[both] |= state.occupancies[white];
        state.occupancies[both] |= state.occupancies[black];

        state.sideToMove ^= 1;

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
