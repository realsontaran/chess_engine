#include <evaluation.hpp>

int Evaluation::evaluate() {
    int score = 0;
    int piece;
    unsigned int square;
    for (int i = P; i <= k; ++i) {
        U64 bitboard = state.piecePositions[i];
        while (bitboard != 0U) {
            // init piece
            piece = i;

            // init square
            square = Bitboard::getLSB(bitboard);

            // score material weights
            score += materialScore[piece];
            // score positional piece scores
            switch (piece) {
            // evaluate white pieces
            case P:
                score += pawn_score[square];
                break;
            case N:
                score += knight_score[square];
                break;
            case B:
                score += bishop_score[square];

                break;
            case R:
                score += rook_score[square];
                break;
            case K:
                score += king_score[square];
                break;

            // evaluate black pieces
            case p:
                score -= pawn_score[mirror_score[square]];
                break;
            case n:
                score -= knight_score[mirror_score[square]];
                break;
            case b:
                score -= bishop_score[mirror_score[square]];
                break;
            case r:
                score -= rook_score[mirror_score[square]];
                break;
            case k:
                score -= king_score[mirror_score[square]];
                break;
            }

            // pop ls1b
            Bitboard::popBit(bitboard, static_cast<int>(square));
        }
    }

    return (state.sideToMove == white) ? score : -score;
}

int Evaluation::negamax(int alpha, int beta, int depth) { // NOLINT
    // Init PV length
    pvLength[ply] = ply;

    if (depth == 0) {
        // return quiescence
        return quiescence(alpha, beta);
    }

    if (ply > MAX_PLY - 1) {
        return evaluate();
    }

    nodes++;

    unsigned int king_square = (state.sideToMove == white)
                                   ? Bitboard::getLSB(state.piecePositions[K])
                                   : Bitboard::getLSB(state.piecePositions[k]);
    Side side = (state.sideToMove == white) ? black : white;
    bool in_check = attackTable.isSquareAttacked(
        static_cast<Square>(king_square), side, state);
    if (in_check) {
        depth++;
    }
    // legal moves counter
    int legal_moves = 0;

    MoveList moveList;
    MoveGeneration generator(state, moveList, attackTable);
    generator.generateMoves();

    MakeMove makeMove(state, attackTable);

    sortMoves(moveList);
    for (auto const &m : moveList.moves) {
        makeMove.copyBoard();

        ++ply;

        if (!makeMove.makeIt(m, Types::all_moves)) {
            ply--;
            continue;
        }
        // increment legal moves
        legal_moves++;

        int score = -negamax(-beta, -alpha, depth - 1);
        ply--;
        makeMove.takeBack();
        if (score >= beta) {
            // store killer moves
            if (!m.getCapture()) {
                killerMoves[1][ply] = killerMoves[0][ply];
                killerMoves[0][ply] = m;
            }
            // node (move) fails high
            return beta;
        }
        // found a better move
        if (score > alpha) {
            if (!m.getCapture()) {
                historyMoves[m.getPiece()][m.getDst()] += depth;
            }
            // PV node (move)
            alpha = score;

            // write PV move
            pvTable[ply][ply] = m;

            for (int nextPly = ply + 1; nextPly < pvLength[ply + 1];
                 ++nextPly) {
                // copy move from deeper ply
                pvTable[ply][nextPly] = pvTable[ply + 1][nextPly];
            }
            pvLength[ply] = pvLength[ply + 1];
        }
    }
    // we don't have any legal moves to make in the current postion
    if (legal_moves == 0) {
        // king is in check
        if (in_check) {
            // return mating score (assuming closest distance to mating
            // position)
            return -49000 + ply;
        }

        // return stalemate score
        return 0;
    }
    // node (move) fails low
    return alpha;
}

int Evaluation::quiescence(int alpha, int beta) { // NOLINT
    nodes++;
    int evaluation = evaluate();
    // fail-hard beta cutoff
    if (evaluation >= beta) {
        // node (move) fails high
        return beta;
    }
    // found a better move
    if (evaluation > alpha) {
        // PV node (move)
        alpha = evaluation;
    }
    MoveList moveList;
    MoveGeneration generator(state, moveList, attackTable);
    generator.generateMoves();

    MakeMove makeMove(state, attackTable);
    sortMoves(moveList);
    for (auto const &m : moveList.moves) {
        makeMove.copyBoard();

        ++ply;

        if (!makeMove.makeIt(m, Types::only_captures)) {
            ply--;
            continue;
        }
        int score = -quiescence(-beta, -alpha);
        ply--;
        makeMove.takeBack();
        if (score >= beta) {
            // node (move) fails high
            return beta;
        }
        // found a better move
        if (score > alpha) {
            // PV node (move)
            alpha = score;
        }
    }
    return alpha;
}

void Evaluation::searchPosition(int depth) {
    int score = 0;
    reset();

    for (int current = 1; current <= depth; ++current) {
        nodes = 0;
        score = negamax(-50000, 50000, current);
        std::cout << "info score cp " << score << " depth " << current
                  << " nodes " << nodes << " pv ";
        for (int i = 0; i < pvLength[0]; i++) {
            std::cout << pvTable[0][i].uciString() << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "bestmove " << pvTable[0][0].uciString() << std::endl;

    reset();
    score = negamax(-50000, 50000, depth);
    std::cout << "info score cp " << score << " depth " << depth << " nodes "
              << nodes << " pv ";
    for (int i = 0; i < pvLength[0]; i++) {
        std::cout << pvTable[0][i].uciString() << " ";
    }
    std::cout << std::endl;
    std::cout << "bestmove " << pvTable[0][0].uciString() << std::endl;
}

int Evaluation::scoreMove(EncodedMove const &move) {
    if (move.getCapture()) {
        Piece target = P;
        int start = P;
        int end = K;
        if (state.sideToMove == white) {
            start = p;
            end = k;
        }

        for (int i = start; i <= end; ++i) {
            if (Bitboard::getBit(state.piecePositions[i], move.getDst()) !=
                0U) {
                target = static_cast<Piece>(i);
                break;
            }
        }
        return mvv_lva[move.getPiece()][target];
    }

    if (killerMoves[0][ply] == move) {
        return 9000;
    }
    if (killerMoves[1][ply] == move) {
        return 8000;
    }
    return historyMoves[move.getPiece()][move.getDst()];
}

void Evaluation::printScoreMove(MoveList const &moves) {
    std::cout << "Move Scores:\n";
    for (auto const &m : moves.moves) {
        printf("     move: ");
        std::cout << m.toString();
        printf(" score: %d\n", scoreMove(m));
    }
}

int Evaluation::sortMoves(MoveList &moves) {
    std::vector<int> moveScores;
    for (auto const &m : moves.moves) {
        moveScores.push_back(scoreMove(m));
    }

    for (size_t i = 0; i < moves.getSize(); i++) {

        for (size_t j = i + 1; j < moves.getSize(); j++) {
            if (moveScores[i] < moveScores[j]) {
                int tmpScore = moveScores[i];
                moveScores[i] = moveScores[j];
                moveScores[j] = tmpScore;

                EncodedMove tmpMove = moves.moves[i];
                moves.moves[i] = moves.moves[j];
                moves.moves[j] = tmpMove;
            }
        }
    }
    return 0;
}

void Evaluation::reset() {
    for (int i = 0; i < MAX_PLY; i++) {
        for (int j = 0; j < MAX_PLY; j++) {
            pvTable[i][j] = EncodedMove();
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < MAX_PLY; j++) {
            killerMoves[i][j] = EncodedMove();
        }
    }
    memset(pvLength, 0, sizeof(pvLength));
    memset(historyMoves, 0, sizeof(historyMoves));
    nodes = 0;
}
