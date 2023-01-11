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
    if (depth == 0) {
        // return quiescence
        return quiescence(alpha, beta);
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

    // best move so far
    EncodedMove bestSofar;

    // old value of alpha
    int old_alpha = alpha;

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
            killerMoves[1][ply] = killerMoves[0][ply];
            killerMoves[0][ply] = m;
            // node (move) fails high
            return beta;
        }
        // found a better move
        if (score > alpha) {
            historyMoves[m.getPiece()][m.getDst()] += depth;
            // PV node (move)
            alpha = score;

            // if root move
            if (ply == 0) {
                // associate best move with the best score
                bestSofar = m;
            }
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
    // found better move
    if (old_alpha != alpha) {
        // init best move
        bestMove = bestSofar;
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
    int score = negamax(-50000, 50000, depth);
    if (!bestMove.empty) {
        printf("info score cp %d depth %d nodes %ld\n", score, depth, nodes);
        std::cout << "bestmove " << bestMove.uciString() << std::endl;
    }
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
