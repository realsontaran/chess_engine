#include <moves.hpp>

using namespace Types;

void MoveGeneration::generateMoves() {
    generateBishopMoves(static_cast<Side>(state.sideToMove));
}

bool MoveGeneration::isPromotionSquare(Side side, Square srcSq) {
    if (side == white) {
        return srcSq >= a7 && srcSq <= h7;
    }
    return srcSq >= a2 && srcSq <= h2;
}

void MoveGeneration::generateCastles(Side side) {

    U64 board = state.occupancies[both];
    // Check if white kingside castle is legal
    if (side == white && (state.castleRights & Castle::wk) != 0) {
        bool clearPath = (Bitboard::getBit(board, f1) == 0ULL) &&
                         (Bitboard::getBit(board, g1) == 0ULL);
        if (clearPath && !attackTable.isSquareAttacked(e1, black, state) &&
            !attackTable.isSquareAttacked(f1, black, state)) {
            printf("castling move: e1g1\n");
        }
    }
    // Check if white queenside castle is legal
    if (side == white && (state.castleRights & Castle::wq) != 0) {
        bool clearPath = (Bitboard::getBit(board, d1) == 0ULL) &&
                         (Bitboard::getBit(board, c1) == 0ULL) &&
                         (Bitboard::getBit(board, b1) == 0ULL);

        if (clearPath && !attackTable.isSquareAttacked(e1, black, state) &&
            !attackTable.isSquareAttacked(d1, black, state)) {
            printf("castling move: e1c1\n");
        }
    }
    // Check if black kingside castle is legal
    if (side == black && (state.castleRights & Castle::bk) != 0) {
        bool clearPath = (Bitboard::getBit(board, f8) == 0ULL) &&
                         (Bitboard::getBit(board, g8) == 0ULL);

        if (clearPath && !attackTable.isSquareAttacked(e8, white, state) &&
            !attackTable.isSquareAttacked(f8, white, state)) {
            printf("castling move: e8g8\n");
        }
    }
    // Check if black queenside castle is legal
    if (side == black && (state.castleRights & Castle::bq) != 0) {
        bool clearPath = (Bitboard::getBit(board, d8) == 0ULL) &&
                         (Bitboard::getBit(board, c8) == 0ULL) &&
                         (Bitboard::getBit(board, b8) == 0ULL);

        if (clearPath && !attackTable.isSquareAttacked(e8, white, state) &&
            !attackTable.isSquareAttacked(d8, white, state)) {
            printf("castling move: e8c8\n");
        }
    }
}

void MoveGeneration::generateKnightMoves(Side side) {
    U64 bitboard =
        (side == white) ? state.piecePositions[N] : state.piecePositions[n];
    U64 occupancies =
        (side == white) ? state.occupancies[white] : state.occupancies[black];
    U64 occupanciesOther =
        (side == white) ? state.occupancies[black] : state.occupancies[white];
    U64 attacks = 0ULL;
    int srcSq = no_sq;
    int dstSq = no_sq;
    while (bitboard != 0ULL) {
        srcSq = static_cast<int>(Bitboard::getLSB(bitboard));
        attacks = attackTable.knightAttacks[srcSq] & ~occupancies;
        while (attacks != 0ULL) {
            dstSq = static_cast<int>(Bitboard::getLSB(attacks));
            if (Bitboard::getBit(occupanciesOther, dstSq) == 0ULL) {
                printf("%s%s piece quiet move\n", squareToString(srcSq),
                       squareToString(dstSq));
            } else {
                printf("%s%s piece capture\n", squareToString(srcSq),
                       squareToString(dstSq));
            }
            Bitboard::popBit(attacks, dstSq);
        }
        Bitboard::popBit(bitboard, srcSq);
    }
}

void MoveGeneration::generateBishopMoves(Side side) {
    U64 bitboard =
        (side == white) ? state.piecePositions[B] : state.piecePositions[b];
    U64 occupancies =
        (side == white) ? state.occupancies[white] : state.occupancies[black];
    U64 occupanciesOther =
        (side == white) ? state.occupancies[black] : state.occupancies[white];
    U64 attacks = 0ULL;
    int srcSq = no_sq;
    int dstSq = no_sq;
    while (bitboard != 0ULL) {
        srcSq = static_cast<int>(Bitboard::getLSB(bitboard));
        attacks = attackTable.getBishopAttacks(static_cast<Square>(srcSq),
                                               state.occupancies[both]) &
                  ~occupancies;

        while (attacks != 0ULL) {
            dstSq = static_cast<int>(Bitboard::getLSB(attacks));
            if (Bitboard::getBit(occupanciesOther, dstSq) == 0ULL) {
                printf("%s%s piece quiet move\n", squareToString(srcSq),
                       squareToString(dstSq));
            } else {
                printf("%s%s piece capture\n", squareToString(srcSq),
                       squareToString(dstSq));
            }
            Bitboard::popBit(attacks, dstSq);
        }
        Bitboard::popBit(bitboard, srcSq);
    }
}

void MoveGeneration::generatePawnMoves(Side side) {
    int srcSq;
    int dstSq;
    U64 srcBB = (side == white) ? wPawnsAble2Push() : bPawnsAble2Push();
    U64 dstBB = (side == white) ? wSinglePushPawns() : bSinglePushPawns();
    U64 srcDBB = (side == white) ? wPawnsAble2DblPush() : bPawnsAble2DblPush();
    U64 dstDBB = (side == white) ? wDblPushPawns() : bDblPushPawns();

    std::string promotion = (side == white) ? "QRBN" : "qrbn";

    while ((srcBB != 0ULL)) {
        srcSq = static_cast<int>(Bitboard::getLSB(srcBB));
        dstSq = static_cast<int>(Bitboard::getLSB(dstBB));
        if (isPromotionSquare(side, static_cast<Square>(srcSq))) {
            printf("pawn promote: %s%s%c\n", squareToString(srcSq),
                   squareToString(dstSq), promotion[0]);
            printf("pawn promote: %s%s%c\n", squareToString(srcSq),
                   squareToString(dstSq), promotion[1]);
            printf("pawn promote: %s%s%c\n", squareToString(srcSq),
                   squareToString(dstSq), promotion[2]);
            printf("pawn promote: %s%s%c\n", squareToString(srcSq),
                   squareToString(dstSq), promotion[3]);
        } else {
            while ((srcDBB != 0ULL)) {
                int srcSqD = static_cast<int>(Bitboard::getLSB(srcDBB));
                int dstSqD = static_cast<int>(Bitboard::getLSB(dstDBB));
                printf("double pawn push: %s%s\n", squareToString(srcSqD),
                       squareToString(dstSqD));
                Bitboard::popBit(srcDBB, srcSqD);
                Bitboard::popBit(dstDBB, dstSqD);
            }
            printf("pawn push: %s%s\n", squareToString(srcSq),
                   squareToString(dstSq));
        }
        Side other = (side == white) ? black : white;
        U64 pawnAttacks =
            attackTable.pawnAttacks[side][srcSq] & state.occupancies[other];
        while (pawnAttacks != 0ULL) {
            int dstSqA = static_cast<int>(Bitboard::getLSB(pawnAttacks));
            if (isPromotionSquare(side, static_cast<Square>(srcSq))) {
                printf("pawn capture promote: %s%s%c\n", squareToString(srcSq),
                       squareToString(dstSqA), promotion[0]);
                printf("pawn capture promote: %s%s%c\n", squareToString(srcSq),
                       squareToString(dstSqA), promotion[1]);
                printf("pawn capture promote: %s%s%c\n", squareToString(srcSq),
                       squareToString(dstSqA), promotion[2]);
                printf("pawn capture promote: %s%s%c\n", squareToString(srcSq),
                       squareToString(dstSqA), promotion[3]);
            } else {

                printf("pawn capture: %s%s\n", squareToString(srcSq),
                       squareToString(dstSqA));
            }
            Bitboard::popBit(pawnAttacks, dstSqA);
        }

        if (state.enPassantSquare != no_sq) {
            U64 enPassantAttacks = attackTable.pawnAttacks[side][srcSq] &
                                   (1ULL << state.enPassantSquare);
            if (enPassantAttacks != 0U) {
                int dstEnPassant =
                    static_cast<int>(Bitboard::getLSB(enPassantAttacks));
                printf("pawn enpassant capture: %s%s\n", squareToString(srcSq),
                       squareToString(dstEnPassant));
            }
        }

        Bitboard::popBit(srcBB, srcSq);
        Bitboard::popBit(dstBB, dstSq);
    }
}

MoveGeneration::~MoveGeneration() {
}

U64 MoveGeneration::wSinglePushPawns() {
    return ~state.occupancies[both] & nortOne(state.piecePositions[P]);
}

U64 MoveGeneration::wDblPushPawns() {
    const U64 rank4 = 0x00000000FF00000000ULL;
    U64 empty = ~state.occupancies[both];
    U64 singlePush = wSinglePushPawns();
    return nortOne(singlePush) & empty & rank4;
}

U64 MoveGeneration::wPawnsAble2Push() {
    U64 empty = ~state.occupancies[both];
    return soutOne(empty) & state.piecePositions[P];
}

U64 MoveGeneration::wPawnsAble2DblPush() {
    const U64 rank4 = 0x00000000FF00000000ULL;
    U64 empty = ~state.occupancies[both];
    U64 emptyRank3 = soutOne(empty & rank4) & empty;
    return soutOne(emptyRank3) & state.piecePositions[P];
}

U64 MoveGeneration::bSinglePushPawns() {
    return ~state.occupancies[both] & soutOne(state.piecePositions[p]);
}

U64 MoveGeneration::bDblPushPawns() {
    const U64 rank5 = 0xff000000ULL;
    U64 empty = ~state.occupancies[both];
    U64 singlePush = bSinglePushPawns();
    return soutOne(singlePush) & empty & rank5;
}

U64 MoveGeneration::bPawnsAble2Push() {
    U64 empty = ~state.occupancies[both];
    return nortOne(empty) & state.piecePositions[p];
}

U64 MoveGeneration::bPawnsAble2DblPush() {
    const U64 rank5 = 0xff000000ULL;
    U64 empty = ~state.occupancies[both];
    U64 emptyRank3 = nortOne(empty & rank5) & empty;
    return nortOne(emptyRank3) & state.piecePositions[p];
}
