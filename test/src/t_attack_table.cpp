#include <doctest.h>
#include <attack_table.hpp>
using namespace Bitboard;

/* Numbers generated using https://tearth.dev/bitboard-viewer/ */

TEST_CASE("test knight attack table") {
    AttackTable table;
    CHECK(table.knightAttacks[Types::Square::e4] == 11333767002587136);
}

TEST_CASE("test king attack table") {
    AttackTable table;
    CHECK(table.kingAttacks[Types::e4] == 61745389371392);
}

TEST_CASE("test pawn attack table") {
    AttackTable table;
    SUBCASE("white pawn attack")
    CHECK(table.pawnAttacks[Types::white][Types::e4] == 671088640);
    SUBCASE("black pawn attack")
    CHECK(table.pawnAttacks[Types::black][Types::e4] == 43980465111040);
}
