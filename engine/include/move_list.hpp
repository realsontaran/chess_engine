#ifndef MOVE_LIST_H_
#define MOVE_LIST_H_
#include <vector>
#include <iostream>
#include <encoding.hpp>

class MoveList {
    // TODO Maybe add UCI
    // https://www.youtube.com/watch?v=AINYYiV-83I&list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs&index=29
  public:
    MoveList() {
    }

    ~MoveList() {
    }

    void print() {
        for (auto const &move : moves) {
            std::cout << move.toString() << std::endl;
        }
    }

    void add(EncodedMove const &move) {
        moves.push_back(move);
    }

    size_t getSize() const {
        return moves.size();
    }

    std::vector<EncodedMove> moves;
};

#endif // MOVE_LIST_H_
