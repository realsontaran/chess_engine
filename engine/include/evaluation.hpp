#ifndef EVALUATION_H_
#define EVALUATION_H_
#include "attack_table.hpp"
#include "encoding.hpp"
#include "game_state.hpp"
#include "make_move.hpp"
#include "move_generation.hpp"
#include "move_list.hpp"
#include <types.hpp>
#include <bits/stdc++.h>
using namespace Types;

class Evaluation {
  public:
    Evaluation(GameState const &s, AttackTable const &attackT)
        : state(s)
        , attackTable(attackT) {
    }

    ~Evaluation() {
    }

    int evaluate();
    int negamax(int alpha, int beta, int depth);
    void searchPosition(int depth);
    int quiescence(int alpha, int beta);
    int scoreMove(EncodedMove const &move);
    void printScoreMove(MoveList const &moves);
    int sortMoves(MoveList &moves);

  private:
    GameState state;
    AttackTable attackTable;
    long nodes = 0;
    int ply = 0;
    EncodedMove bestMove;
    int const materialScore[12] = {
        100,   // white pawn score
        300,   // white knight scrore
        350,   // white bishop score
        500,   // white rook score
        1000,  // white queen score
        10000, // white king score
        -100,  // black pawn score
        -300,  // black knight scrore
        -350,  // black bishop score
        -500,  // black rook score
        -1000, // black queen score
        -10000 // black king score
    };
    // pawn positional score
    int const pawn_score[64] = {
        90, 90, 90, 90,  90,  90, 90, 90, 30, 30, 30, 40, 40, 30, 30, 30,
        20, 20, 20, 30,  30,  30, 20, 20, 10, 10, 10, 20, 20, 10, 10, 10,
        5,  5,  10, 20,  20,  5,  5,  5,  0,  0,  0,  5,  5,  0,  0,  0,
        0,  0,  0,  -10, -10, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};

    // knight positional score
    int const knight_score[64] = {
        -5, 0,  0,  0,  0,  0,  0,  -5, -5, 0,   0,  10, 10, 0,  0,   -5,
        -5, 5,  20, 20, 20, 20, 5,  -5, -5, 10,  20, 30, 30, 20, 10,  -5,
        -5, 10, 20, 30, 30, 20, 10, -5, -5, 5,   20, 10, 10, 20, 5,   -5,
        -5, 0,  0,  0,  0,  0,  0,  -5, -5, -10, 0,  0,  0,  0,  -10, -5};

    // bishop positional score
    int const bishop_score[64] = {
        0,  0,  0,  0,  0, 0, 0, 0,   0, 0,  0,   0,  0,  0, 0, 0,  0,
        0,  0,  10, 10, 0, 0, 0, 0,   0, 10, 20,  20, 10, 0, 0, 0,  0,
        10, 20, 20, 10, 0, 0, 0, 10,  0, 0,  0,   0,  10, 0, 0, 30, 0,
        0,  0,  0,  30, 0, 0, 0, -10, 0, 0,  -10, 0,  0

    };

    // rook positional score
    int const rook_score[64] = {
        50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 0,
        0,  10, 20, 20, 10, 0,  0,  0,  0,  10, 20, 20, 10, 0,  0,  0,  0,
        10, 20, 20, 10, 0,  0,  0,  0,  10, 20, 20, 10, 0,  0,  0,  0,  10,
        20, 20, 10, 0,  0,  0,  0,  0,  20, 20, 0,  0,  0

    };

    // king positional score
    int const king_score[64] = {
        0, 0, 0,  0,  0,  0,  0, 0, 0, 0, 5,  5,  5,   5,  0,  0,
        0, 5, 5,  10, 10, 5,  5, 0, 0, 5, 10, 20, 20,  10, 5,  0,
        0, 5, 10, 20, 20, 10, 5, 0, 0, 0, 5,  10, 10,  5,  0,  0,
        0, 5, 5,  -5, -5, 0,  5, 0, 0, 0, 5,  0,  -15, 0,  10, 0};

    // mirror positional score tables for opposite side
    int const mirror_score[128] = {
        a1, b1, c1, d1, e1, f1, g1, h1, a2, b2, c2, d2, e2, f2, g2, h2,
        a3, b3, c3, d3, e3, f3, g3, h3, a4, b4, c4, d4, e4, f4, g4, h4,
        a5, b5, c5, d5, e5, f5, g5, h5, a6, b6, c6, d6, e6, f6, g6, h6,
        a7, b7, c7, d7, e7, f7, g7, h7, a8, b8, c8, d8, e8, f8, g8, h8};

    // most valuable victim & less valuable attacker
    /*

        (Victims) Pawn Knight Bishop   Rook  Queen   King
      (Attackers)
            Pawn   105    205    305    405    505    605
          Knight   104    204    304    404    504    604
          Bishop   103    203    303    403    503    603
            Rook   102    202    302    402    502    602
           Queen   101    201    301    401    501    601
            King   100    200    300    400    500    600
    */
    // MVV LVA [attacker][victim]
    int const mvv_lva[12][12] = {
        {105, 205, 305, 405, 505, 605, 105, 205, 305, 405, 505, 605},
        {104, 204, 304, 404, 504, 604, 104, 204, 304, 404, 504, 604},
        {103, 203, 303, 403, 503, 603, 103, 203, 303, 403, 503, 603},
        {102, 202, 302, 402, 502, 602, 102, 202, 302, 402, 502, 602},
        {101, 201, 301, 401, 501, 601, 101, 201, 301, 401, 501, 601},
        {100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600},
        {105, 205, 305, 405, 505, 605, 105, 205, 305, 405, 505, 605},
        {104, 204, 304, 404, 504, 604, 104, 204, 304, 404, 504, 604},
        {103, 203, 303, 403, 503, 603, 103, 203, 303, 403, 503, 603},
        {102, 202, 302, 402, 502, 602, 102, 202, 302, 402, 502, 602},
        {101, 201, 301, 401, 501, 601, 101, 201, 301, 401, 501, 601},
        {100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600}};
};

#endif // EVALUATION_H_
