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

int Evaluation::negamax(int alpha, int beta, int depth) {
    if (depth == 0)
        // return evaluation
        return evaluate();
    nodes++;

    unsigned int king_square = (state.sideToMove == white)
                                   ? Bitboard::getLSB(state.piecePositions[K])
                                   : Bitboard::getLSB(state.piecePositions[k]);
    Side side = (state.sideToMove == white) ? black : white;
    bool in_check = attackTable.isSquareAttacked(
        static_cast<Square>(king_square), side, state);
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
            // node (move) fails high
            return beta;
        }
        // found a better move
        if (score > alpha) {
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
        if (in_check)
            // return mating score (assuming closest distance to mating
            // position)
            return -49000 + ply;

        // return stalemate score
        return 0;
    }
    // found better move
    if (old_alpha != alpha)
        // init best move
        bestMove = bestSofar;

    // node (move) fails low
    return alpha;
}

void Evaluation::searchPosition(int depth) {
    int score = negamax(-50000, 50000, depth);
    if (!bestMove.empty)
        std::cout << "bestmove " << bestMove.uciString() << std::endl;
}
