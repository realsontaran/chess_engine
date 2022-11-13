#ifndef BITBOARD_H_
#define BITBOARD_H_

typedef unsigned long long U64;

namespace Bitboard {
    constexpr void set_bit(U64 &board, int sq) {
        (board) |= (1ULL << (sq));
    }

    constexpr U64 get_bit(U64 board, int sq) {
        return (board) & (1ULL << (sq));
    }

    constexpr void pop_bit(U64 &board, int sq) {
        (board) &= ~(1ULL << (sq));
    }

    constexpr unsigned int bit_count(U64 board) {
        unsigned int count;
        for (count = 0; board != 0ULL; ++count, board &= board - 1)
            ;
        return count;
    }

    constexpr unsigned int get_lsb(U64 board) {
        return (board != 0ULL) ? bit_count((board & -board) - 1) : 0;
    }

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

    enum Side { white, black, both };

    /* Magic numbers for excluding files for attack mask generation */
    constexpr U64 notAFile = 0xfefefefefefefefe;
    constexpr U64 notAorBFile = 0xfcfcfcfcfcfcfcfc;
    constexpr U64 notHFile = 0x7f7f7f7f7f7f7f7f;
    constexpr U64 notHorGFile = 0x3f3f3f3f3f3f3f3f;

    void printBoard(U64 board);

} // namespace Bitboard

/*
  noWe        nort      noEa
            \  |  /
  west     <-  0  ->    east
            /  |  \
  soWe        sout      soEa

  Compass rose to identify ray-directions
     increments to neighbored squares.
*/

U64 eastOne(U64 b);
U64 westOne(U64 b);
U64 soutOne(U64 b);
U64 nortOne(U64 b);
U64 noEaOne(U64 b);
U64 soEaOne(U64 b);
U64 soWeOne(U64 b);
U64 noWeOne(U64 b);
/*
         noNoWe    noNoEa
          |     |
noWeWe  __|     |__  noEaEa
           \   /
            >0<
        __ /   \ __
soWeWe    |     |    soEaEa
          |     |
        soSoWe    soSoEa

    Knight move compass
 */
U64 noNoEa(U64 b);
U64 noNoWe(U64 b);
U64 soSoEa(U64 b);
U64 soSoWe(U64 b);
U64 noEaEa(U64 b);
U64 noWeWe(U64 b);
U64 soEaEa(U64 b);
U64 soWeWe(U64 b);

#endif // BITBOARD_H_
