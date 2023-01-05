#include <move_generation.hpp>

using namespace Types;

void MoveGeneration::generateMoves() {
    Side side = static_cast<Side>(state.sideToMove);
    generatePawnMoves(side);
    generateCastles(side);
    generateSliderAndLeaperMoves(Piece::N, side);
    generateSliderAndLeaperMoves(Piece::B, side);
    generateSliderAndLeaperMoves(Piece::R, side);
    generateSliderAndLeaperMoves(Piece::Q, side);
    generateSliderAndLeaperMoves(Piece::K, side);
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
        if (clearPath && !attackT.isSquareAttacked(e1, black, state) &&
            !attackT.isSquareAttacked(f1, black, state)) {
            moves.add(EncodedMove(e1, g1, Piece::K, Piece::P, 0, 0, 0, 1));
        }
    }
    // Check if white queenside castle is legal
    if (side == white && (state.castleRights & Castle::wq) != 0) {
        bool clearPath = (Bitboard::getBit(board, d1) == 0ULL) &&
                         (Bitboard::getBit(board, c1) == 0ULL) &&
                         (Bitboard::getBit(board, b1) == 0ULL);

        if (clearPath && !attackT.isSquareAttacked(e1, black, state) &&
            !attackT.isSquareAttacked(d1, black, state)) {
            moves.add(EncodedMove(e1, c1, Piece::K, Piece::P, 0, 0, 0, 1));
        }
    }
    // Check if black kingside castle is legal
    if (side == black && (state.castleRights & Castle::bk) != 0) {
        bool clearPath = (Bitboard::getBit(board, f8) == 0ULL) &&
                         (Bitboard::getBit(board, g8) == 0ULL);

        if (clearPath && !attackT.isSquareAttacked(e8, white, state) &&
            !attackT.isSquareAttacked(f8, white, state)) {
            moves.add(EncodedMove(e8, g8, Piece::k, Piece::p, 0, 0, 0, 1));
        }
    }
    // Check if black queenside castle is legal
    if (side == black && (state.castleRights & Castle::bq) != 0) {
        bool clearPath = (Bitboard::getBit(board, d8) == 0ULL) &&
                         (Bitboard::getBit(board, c8) == 0ULL) &&
                         (Bitboard::getBit(board, b8) == 0ULL);

        if (clearPath && !attackT.isSquareAttacked(e8, white, state) &&
            !attackT.isSquareAttacked(d8, white, state)) {
            moves.add(EncodedMove(e8, c8, Piece::k, Piece::p, 0, 0, 0, 1));
        }
    }
}

void MoveGeneration::generateSliderAndLeaperMoves(Piece pieceType, Side side) {
    int pieceNum = pieceType % 6;
    if (side == black) {
        pieceNum += 6;
    }
    U64 bitboard = state.piecePositions[pieceNum];
    U64 occupancies = state.occupancies[side];
    U64 occupanciesOther =
        (side == white) ? state.occupancies[black] : state.occupancies[white];
    U64 attacks = 0ULL;
    int srcSq = no_sq;
    int dstSq = no_sq;

    while (bitboard != 0ULL) {
        srcSq = static_cast<int>(Bitboard::getLSB(bitboard));
        switch (pieceType) {
        case N:
        case n:
            attacks = attackT.knightAttacks[srcSq] & ~occupancies;
            break;
        case B:
        case b:
            attacks = attackT.getBishopAttacks(static_cast<Square>(srcSq),
                                               state.occupancies[both]) &
                      ~occupancies;
            break;
        case R:
        case r:
            attacks = attackT.getRookAttacks(static_cast<Square>(srcSq),
                                             state.occupancies[both]) &
                      ~occupancies;
            break;
        case Q:
        case q:
            attacks = attackT.getQueenAttacks(static_cast<Square>(srcSq),
                                              state.occupancies[both]) &
                      ~occupancies;
            break;
        case K:
        case k:
            attacks = attackT.kingAttacks[srcSq] & ~occupancies;
            break;
        default:
            break;
        }
        while (attacks != 0ULL) {
            dstSq = static_cast<int>(Bitboard::getLSB(attacks));
            if (Bitboard::getBit(occupanciesOther, dstSq) == 0ULL) {
                moves.add(EncodedMove(
                    static_cast<Square>(srcSq), static_cast<Square>(dstSq),
                    static_cast<Piece>(pieceNum), Piece::P, 0, 0, 0, 0));

            } else {
                moves.add(EncodedMove(
                    static_cast<Square>(srcSq), static_cast<Square>(dstSq),
                    static_cast<Piece>(pieceNum), Piece::P, 1, 0, 0, 0));
            }
            Bitboard::popBit(attacks, dstSq);
        }
        Bitboard::popBit(bitboard, srcSq);
    }
}

void MoveGeneration::generatePawnMoves(Side side) {
    int srcSq;
    int dstSq;
    int srcAttackSq;
    U64 srcBB;
    U64 srcAttackBB;
    U64 dstBB;
    U64 srcDBB;
    U64 dstDBB;
    std::string promotion;
    Piece pawn;
    Piece promoPiece[4] = {Q, R, B, N};

    if (side == white) {
        srcBB = wPawnsAble2Push();
        dstBB = wSinglePushPawns();
        srcDBB = wPawnsAble2DblPush();
        dstDBB = wDblPushPawns();
        srcAttackBB = state.piecePositions[P];
        promotion = "QRBN";
        pawn = Piece::P;
    } else {
        srcBB = bPawnsAble2Push();
        dstBB = bSinglePushPawns();
        srcDBB = bPawnsAble2DblPush();
        dstDBB = bDblPushPawns();
        srcAttackBB = state.piecePositions[p];
        promotion = "qrbn";
        pawn = Piece::p;
        promoPiece[0] = q;
        promoPiece[1] = r;
        promoPiece[2] = b;
        promoPiece[3] = n;
    }

    while ((srcBB != 0ULL)) {
        srcSq = static_cast<int>(Bitboard::getLSB(srcBB));
        dstSq = static_cast<int>(Bitboard::getLSB(dstBB));
        if (isPromotionSquare(side, static_cast<Square>(srcSq))) {
            // Promote pawns
            moves.add(EncodedMove(static_cast<Square>(srcSq),
                                  static_cast<Square>(dstSq), pawn,
                                  promoPiece[0], 0, 0, 0, 0));
            moves.add(EncodedMove(static_cast<Square>(srcSq),
                                  static_cast<Square>(dstSq), pawn,
                                  promoPiece[1], 0, 0, 0, 0));
            moves.add(EncodedMove(static_cast<Square>(srcSq),
                                  static_cast<Square>(dstSq), pawn,
                                  promoPiece[2], 0, 0, 0, 0));
            moves.add(EncodedMove(static_cast<Square>(srcSq),
                                  static_cast<Square>(dstSq), pawn,
                                  promoPiece[3], 0, 0, 0, 0));
        } else {
            // Double push pawns
            while ((srcDBB != 0ULL)) {
                int srcSqD = static_cast<int>(Bitboard::getLSB(srcDBB));
                int dstSqD = static_cast<int>(Bitboard::getLSB(dstDBB));
                moves.add(EncodedMove(static_cast<Square>(srcSqD),
                                      static_cast<Square>(dstSqD), pawn,
                                      Piece::P, 0, 1, 0, 0));

                Bitboard::popBit(srcDBB, srcSqD);
                Bitboard::popBit(dstDBB, dstSqD);
            }
            // Single push pawns
            moves.add(EncodedMove(static_cast<Square>(srcSq),
                                  static_cast<Square>(dstSq), pawn, Piece::P, 0,
                                  0, 0, 0));
        }
        if (state.enPassantSquare != no_sq) {
            U64 enPassantAttacks = attackT.pawnAttacks[side][srcSq] &
                                   (1ULL << state.enPassantSquare);
            if (enPassantAttacks != 0U) {
                int dstEnPassant =
                    static_cast<int>(Bitboard::getLSB(enPassantAttacks));
                moves.add(EncodedMove(static_cast<Square>(srcSq),
                                      static_cast<Square>(dstEnPassant), pawn,
                                      Piece::P, 1, 0, 1, 0));
            }
        }

        Bitboard::popBit(srcBB, srcSq);
        Bitboard::popBit(dstBB, dstSq);
    }

    Side other = (side == white) ? black : white;
    while (srcAttackBB != 0ULL) {
        srcAttackSq = static_cast<int>(Bitboard::getLSB(srcAttackBB));
        U64 pawnAttacks =
            attackT.pawnAttacks[side][srcAttackSq] & state.occupancies[other];
        while (pawnAttacks != 0ULL) {
            int dstSqA = static_cast<int>(Bitboard::getLSB(pawnAttacks));
            if (isPromotionSquare(side, static_cast<Square>(srcAttackSq))) {
                // Promotion capture
                moves.add(EncodedMove(static_cast<Square>(srcAttackSq),
                                      static_cast<Square>(dstSqA), pawn,
                                      promoPiece[0], 1, 0, 0, 0));
                moves.add(EncodedMove(static_cast<Square>(srcAttackSq),
                                      static_cast<Square>(dstSqA), pawn,
                                      promoPiece[1], 1, 0, 0, 0));
                moves.add(EncodedMove(static_cast<Square>(srcAttackSq),
                                      static_cast<Square>(dstSqA), pawn,
                                      promoPiece[2], 1, 0, 0, 0));
                moves.add(EncodedMove(static_cast<Square>(srcAttackSq),
                                      static_cast<Square>(dstSqA), pawn,
                                      promoPiece[3], 1, 0, 0, 0));

            } else {
                // Pawn captures
                moves.add(EncodedMove(static_cast<Square>(srcAttackSq),
                                      static_cast<Square>(dstSqA), pawn,
                                      Piece::P, 1, 0, 0, 0));
            }
            Bitboard::popBit(pawnAttacks, dstSqA);
        }
        Bitboard::popBit(srcAttackBB, srcAttackSq);
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
