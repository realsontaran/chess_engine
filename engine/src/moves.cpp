#include <moves.hpp>

void MoveGeneration::generateMoves() {
    generateCastles(static_cast<Types::Side>(state.sideToMove));
}

bool MoveGeneration::isPromotionSquare(Types::Side side, Types::Square srcSq) {
    if (side == Types::white) {
        return srcSq >= Types::a7 && srcSq <= Types::h7;
    }
    return srcSq >= Types::a2 && srcSq <= Types::h2;
}

void MoveGeneration::generateCastles(Types::Side side) {

    U64 board = state.occupancies[Types::both];
    // Check if white kingside castle is legal
    if (side == Types::white && (state.castleRights & Types::Castle::wk) != 0) {
        bool clearPath = (Bitboard::getBit(board, Types::f1) == 0ULL) &&
                         (Bitboard::getBit(board, Types::g1) == 0ULL);
        if (clearPath &&
            !attackTable.isSquareAttacked(Types::e1, Types::black, state) &&
            !attackTable.isSquareAttacked(Types::f1, Types::black, state)) {
            printf("castling move: e1g1\n");
        }
    }
    // Check if white queenside castle is legal
    if (side == Types::white && (state.castleRights & Types::Castle::wq) != 0) {
        bool clearPath = (Bitboard::getBit(board, Types::d1) == 0ULL) &&
                         (Bitboard::getBit(board, Types::c1) == 0ULL) &&
                         (Bitboard::getBit(board, Types::b1) == 0ULL);

        if (clearPath &&
            !attackTable.isSquareAttacked(Types::e1, Types::black, state) &&
            !attackTable.isSquareAttacked(Types::d1, Types::black, state)) {
            printf("castling move: e1c1\n");
        }
    }
    // Check if black kingside castle is legal
    if (side == Types::black && (state.castleRights & Types::Castle::bk) != 0) {
        bool clearPath = (Bitboard::getBit(board, Types::f8) == 0ULL) &&
                         (Bitboard::getBit(board, Types::g8) == 0ULL);

        if (clearPath &&
            !attackTable.isSquareAttacked(Types::e8, Types::white, state) &&
            !attackTable.isSquareAttacked(Types::f8, Types::white, state)) {
            printf("castling move: e8g8\n");
        }
    }
    // Check if black queenside castle is legal
    if (side == Types::black && (state.castleRights & Types::Castle::bq) != 0) {
        bool clearPath = (Bitboard::getBit(board, Types::d8) == 0ULL) &&
                         (Bitboard::getBit(board, Types::c8) == 0ULL) &&
                         (Bitboard::getBit(board, Types::b8) == 0ULL);

        if (clearPath &&
            !attackTable.isSquareAttacked(Types::e8, Types::white, state) &&
            !attackTable.isSquareAttacked(Types::d8, Types::white, state)) {
            printf("castling move: e8c8\n");
        }
    }
}

void MoveGeneration::generatePawnMoves(Types::Side side) {
    int srcSq;
    int dstSq;
    U64 srcBB = (side == Types::white) ? wPawnsAble2Push() : bPawnsAble2Push();
    U64 dstBB =
        (side == Types::white) ? wSinglePushPawns() : bSinglePushPawns();
    U64 srcDBB =
        (side == Types::white) ? wPawnsAble2DblPush() : bPawnsAble2DblPush();
    U64 dstDBB = (side == Types::white) ? wDblPushPawns() : bDblPushPawns();

    std::string promotion = (side == Types::white) ? "QRBN" : "qrbn";

    while ((srcBB != 0ULL)) {
        srcSq = static_cast<int>(Bitboard::getLSB(srcBB));
        dstSq = static_cast<int>(Bitboard::getLSB(dstBB));
        if (isPromotionSquare(side, static_cast<Types::Square>(srcSq))) {
            printf("pawn promote: %s%s%c\n", Types::squareToString(srcSq),
                   Types::squareToString(dstSq), promotion[0]);
            printf("pawn promote: %s%s%c\n", Types::squareToString(srcSq),
                   Types::squareToString(dstSq), promotion[1]);
            printf("pawn promote: %s%s%c\n", Types::squareToString(srcSq),
                   Types::squareToString(dstSq), promotion[2]);
            printf("pawn promote: %s%s%c\n", Types::squareToString(srcSq),
                   Types::squareToString(dstSq), promotion[3]);
        } else {
            while ((srcDBB != 0ULL)) {
                int srcSqD = static_cast<int>(Bitboard::getLSB(srcDBB));
                int dstSqD = static_cast<int>(Bitboard::getLSB(dstDBB));
                printf("double pawn push: %s%s\n",
                       Types::squareToString(srcSqD),
                       Types::squareToString(dstSqD));
                Bitboard::popBit(srcDBB, srcSqD);
                Bitboard::popBit(dstDBB, dstSqD);
            }
            printf("pawn push: %s%s\n", Types::squareToString(srcSq),
                   Types::squareToString(dstSq));
        }
        Types::Side other =
            (side == Types::white) ? Types::black : Types::white;
        U64 pawnAttacks =
            attackTable.pawnAttacks[side][srcSq] & state.occupancies[other];
        while (pawnAttacks != 0ULL) {
            int dstSqA = static_cast<int>(Bitboard::getLSB(pawnAttacks));
            if (isPromotionSquare(side, static_cast<Types::Square>(srcSq))) {
                printf("pawn capture promote: %s%s%c\n",
                       Types::squareToString(srcSq),
                       Types::squareToString(dstSqA), promotion[0]);
                printf("pawn capture promote: %s%s%c\n",
                       Types::squareToString(srcSq),
                       Types::squareToString(dstSqA), promotion[1]);
                printf("pawn capture promote: %s%s%c\n",
                       Types::squareToString(srcSq),
                       Types::squareToString(dstSqA), promotion[2]);
                printf("pawn capture promote: %s%s%c\n",
                       Types::squareToString(srcSq),
                       Types::squareToString(dstSqA), promotion[3]);
            } else {

                printf("pawn capture: %s%s\n", Types::squareToString(srcSq),
                       Types::squareToString(dstSqA));
            }
            Bitboard::popBit(pawnAttacks, dstSqA);
        }

        if (state.enPassantSquare != Types::no_sq) {
            U64 enPassantAttacks = attackTable.pawnAttacks[side][srcSq] &
                                   (1ULL << state.enPassantSquare);
            if (enPassantAttacks != 0U) {
                int dstEnPassant =
                    static_cast<int>(Bitboard::getLSB(enPassantAttacks));
                printf("pawn enpassant capture: %s%s\n",
                       Types::squareToString(srcSq),
                       Types::squareToString(dstEnPassant));
            }
        }

        Bitboard::popBit(srcBB, srcSq);
        Bitboard::popBit(dstBB, dstSq);
    }
}

MoveGeneration::~MoveGeneration() {
}

U64 MoveGeneration::wSinglePushPawns() {
    return ~state.occupancies[Types::both] &
           nortOne(state.piecePositions[Types::P]);
}

U64 MoveGeneration::wDblPushPawns() {
    const U64 rank4 = 0x00000000FF00000000ULL;
    U64 empty = ~state.occupancies[Types::both];
    U64 singlePush = wSinglePushPawns();
    return nortOne(singlePush) & empty & rank4;
}

U64 MoveGeneration::wPawnsAble2Push() {
    U64 empty = ~state.occupancies[Types::both];
    return soutOne(empty) & state.piecePositions[Types::P];
}

U64 MoveGeneration::wPawnsAble2DblPush() {
    const U64 rank4 = 0x00000000FF00000000ULL;
    U64 empty = ~state.occupancies[Types::both];
    U64 emptyRank3 = soutOne(empty & rank4) & empty;
    return soutOne(emptyRank3) & state.piecePositions[Types::P];
}

U64 MoveGeneration::bSinglePushPawns() {
    return ~state.occupancies[Types::both] &
           soutOne(state.piecePositions[Types::p]);
}

U64 MoveGeneration::bDblPushPawns() {
    const U64 rank5 = 0xff000000ULL;
    U64 empty = ~state.occupancies[Types::both];
    U64 singlePush = bSinglePushPawns();
    return soutOne(singlePush) & empty & rank5;
}

U64 MoveGeneration::bPawnsAble2Push() {
    U64 empty = ~state.occupancies[Types::both];
    return nortOne(empty) & state.piecePositions[Types::p];
}

U64 MoveGeneration::bPawnsAble2DblPush() {
    const U64 rank5 = 0xff000000ULL;
    U64 empty = ~state.occupancies[Types::both];
    U64 emptyRank3 = nortOne(empty & rank5) & empty;
    return nortOne(emptyRank3) & state.piecePositions[Types::p];
}
