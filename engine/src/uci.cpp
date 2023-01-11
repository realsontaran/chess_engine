#include <cstring>
#include <uci.hpp>
#include <iostream>
#include <string>
#include <stdlib.h>

EncodedMove UCI::parseMove(std::string const &move) { // NOLINT
    MoveList moveList;
    MoveGeneration moveGenerator = MoveGeneration(state, moveList, attackT);
    moveGenerator.generateMoves();

    int srcSq = (move[0] - 'a') + (8 - (move[1] - '0')) * 8;
    int dstSq = (move[2] - 'a') + (8 - (move[3] - '0')) * 8;

    for (auto const &m : moveList.moves) {
        if (srcSq == m.getSrc() && dstSq == m.getDst()) {
            Piece promo = m.getPromoted();
            if (promo != P && promo != p) {
                if ((promo == Q || promo == q) && move[4] == 'q') {
                    return m;
                }
                if ((promo == R || promo == r) && move[4] == 'r') {
                    return m;
                }
                if ((promo == B || promo == b) && move[4] == 'b') {
                    return m;
                }
                if ((promo == N || promo == n) && move[4] == 'n') {
                    return m;
                }
                continue;
            }
            return m;
        }
    }
    return EncodedMove(no_sq, no_sq, P, P, 0, 0, 0, 0);
}

// parse UCI "position" command
void UCI::parsePosition(char *command) {
    // shift pointer to the right where next token begins
    command += 9;
    std::string start_position(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ");

    // init pointer to the current character in the command string
    char *current_char = command;

    // parse UCI "startpos" command
    if (strncmp(command, "startpos", 8) == 0) {
        // init chess board with start position
        state.parseFEN(start_position);

        // parse UCI "fen" command
    } else {
        // make sure "fen" command is available within command string
        current_char = strstr(command, "fen");

        // if no "fen" command is available within command string
        if (current_char == NULL) {
            // init chess board with start position
            state.parseFEN(start_position);

            // found "fen" substring
        } else {
            // shift pointer to the right where next token begins
            current_char += 4;

            // init chess board with position from FEN string
            state.parseFEN(current_char);
        }
    }

    // parse moves after position
    current_char = strstr(command, "moves");

    // moves available
    if (current_char != NULL) {
        // shift pointer to the right where next token begins
        current_char += 6;

        // loop over moves within a move string
        while (*current_char != 0) {
            // parse next move
            EncodedMove move = parseMove(current_char);

            // if no more moves
            if (move.getSrc() == no_sq) {
                // break out of the loop
                break;
            }

            // make move on the chess board
            MakeMove makeM(state, attackT);
            makeM.makeIt(move, all_moves);

            // move current character mointer to the end of current move
            while ((*current_char != 0) && *current_char != ' ') {
                current_char++;
            }

            // go to the next move
            current_char++;
        }
    }

    // print board
    state.printBoard(true);
}

void UCI::parseGo(char *command) {
    // init depth
    int depth = -1;

    // init character pointer to the current depth argument
    char *current_depth = NULL;
    // handle fixed depth search
    if ((current_depth = strstr(command, "depth")) != nullptr) {
        // convert string to integer and assign the result value to depth
        depth = atoi(current_depth + 6); // NOLINT
        // different time controls placeholder
    } else {
        depth = 2;
    }

    // search position
    Evaluation eval(state, attackT);
    state.printBoard(true);
    eval.searchPosition(depth);
}

void UCI::uciLoop() {
    // reset STDIN & STDOUT buffers
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    // define user / GUI input buffer
    char input[2000];

    // print engine info
    std::cout << "id name Chess Engine\nid name Emre Ozbay\nuciok\n";

    // main loop
    while (true) {
        // reset user /GUI input
        memset(input, 0, sizeof(input));

        // make sure output reaches the GUI
        std::cout << std::flush;

        // get user / GUI input
        if (fgets(input, 2000, stdin) == nullptr) {
            // continue the loop
            continue;
        }

        // make sure input is available
        if (input[0] == '\n') {
            // continue the loop
            continue;
        }

        // parse UCI "isready" command
        if (strncmp(input, "isready", 7) == 0) {
            std::cout << "readyok\n";
            continue;
        }
        if (strncmp(input, "position", 8) == 0) {
            // call parse position function
            parsePosition(input);

            // parse UCI "ucinewgame" command
        } else if (strncmp(input, "ucinewgame", 10) == 0) {
            // call parse position function
            parsePosition(const_cast<char *>("position startpos"));

            // parse UCI "go" command
        } else if (strncmp(input, "go", 2) == 0) {
            // call parse go function
            parseGo(input);

            // parse UCI "quit" command
        } else if (strncmp(input, "quit", 4) == 0) {
            // quit from the chess engine program execution
            break;

            // parse UCI "uci" command
        } else if (strncmp(input, "uci", 3) == 0) {
            // print engine info
            std::cout << "id name Chess Engine\nid name Emre Ozbay\nuciok\n";
        }
    }
}
