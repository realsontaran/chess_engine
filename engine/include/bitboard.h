#ifndef BITBOARD_H_
#define BITBOARD_H_

typedef unsigned long long U64;
#define set_bit(board, sq) ((board) |= (1ULL << (sq)))
#define get_bit(board, sq) ((board) & (1ULL << (sq)))
#define pop_bit(board, sq) (get_bit(board, sq) ? (board) ^= (1ULL << (sq)) : 0)

inline unsigned int bit_count(U64 board) {
    unsigned int count;
    for (count = 0; board != 0ULL; ++count, board &= board - 1)
        ;
    return count;
}

inline unsigned int get_lsb(U64 board) {
    if (board != 0ULL) {
        return bit_count((board & -board) - 1);
    }
    return 0;
}

namespace Bitboard {
// clang-format off

enum square {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
};

// static const char *square_strings[] = {
//     "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
//     "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
//     "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
//     "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
//     "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
//     "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
//     "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
//     "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
// };

// clang-format on

enum side { white, black };

/* Magic numbers for excluding files for attack mask generation */
const U64 notAFile = 0xfefefefefefefefe;
const U64 notAorBFile = 0xfcfcfcfcfcfcfcfc;
const U64 notHFile = 0x7f7f7f7f7f7f7f7f;
const U64 notHorGFile = 0x3f3f3f3f3f3f3f3f;
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

void printBoard(U64 board);

#endif // BITBOARD_H_
