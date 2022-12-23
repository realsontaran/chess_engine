#ifndef TYPES_H_
#define TYPES_H_

namespace Types {

    enum File {
        FILE_A,
        FILE_B,
        FILE_C,
        FILE_D,
        FILE_E,
        FILE_F,
        FILE_G,
        FILE_H
    };

    enum Rank {
        RANK_8,
        RANK_7,
        RANK_6,
        RANK_5,
        RANK_4,
        RANK_3,
        RANK_2,
        RANK_1
    };

    // clang-format off
    enum Square {
        a8, b8, c8, d8, e8, f8, g8, h8,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a1, b1, c1, d1, e1, f1, g1, h1, no_sq
    };

    // clang-format on
    inline char const *squareToString(int sq) {
        if (sq < 64 && sq >= 0) {
            static char const *square_strings[] = {
                "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", "a7", "b7",
                "c7", "d7", "e7", "f7", "g7", "h7", "a6", "b6", "c6", "d6",
                "e6", "f6", "g6", "h6", "a5", "b5", "c5", "d5", "e5", "f5",
                "g5", "h5", "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
                "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", "a2", "b2",
                "c2", "d2", "e2", "f2", "g2", "h2", "a1", "b1", "c1", "d1",
                "e1", "f1", "g1", "h1"};
            return square_strings[sq];
        }
        return nullptr;
    }

    enum Side { white, black, both };

    enum Castle { wk = 1, wq = 2, bk = 4, bq = 8 };

    enum Piece { P, N, B, R, Q, K, p, n, b, r, q, k };

    template<typename E>
    E &operator++(E &e) {
        e = static_cast<E>(static_cast<int>(e) + 1);
        return e;
    }

    template<typename E>
    E &operator--(E &e) {
        e = static_cast<E>(static_cast<int>(e) - 1);
        return e;
    }

    template<typename E>
    E operator+(E e, int n) {
        return static_cast<E>(static_cast<int>(e) + n);
    }

    template<typename E>
    void operator+=(E &e, int n) {
        e = static_cast<E>(static_cast<int>(e) + n);
    }

    template<typename E>
    E operator+(E e, E b) {
        return static_cast<E>(static_cast<int>(e) + static_cast<int>(b));
    }

    template<typename E>
    E operator-(E e, int n) {
        return static_cast<E>(static_cast<int>(e) - n);
    }

    constexpr Square makeSquare(File f, Rank r) {
        return Square((r << 3) | f);
    }

    constexpr File fileOfSquare(Square s) {
        return File(s & 7);
    }

    constexpr Rank rankOfSquare(Square s) {
        return Rank(s >> 3);
    }

} // namespace Types
#endif // TYPES_H_
