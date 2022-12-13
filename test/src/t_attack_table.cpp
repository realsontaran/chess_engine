#include <doctest.h>
#include <attack_table.hpp>
using namespace Bitboard;

/* Numbers generated using https://tearth.dev/bitboard-viewer/ */

TEST_CASE("test knight attack table") {
    AttackTables table;
    CHECK(table.arrKnightAttacks[Types::Square::e4] == 11333767002587136);
}

TEST_CASE("test king attack table") {
    AttackTables table;
    CHECK(table.arrKingAttacks[Types::e4] == 61745389371392);
}

TEST_CASE("test pawn attack table") {
    AttackTables table;
    SUBCASE("white pawn attack")
    CHECK(table.arrPawnAttacks[Types::white][Types::e4] == 671088640);
    SUBCASE("black pawn attack")
    CHECK(table.arrPawnAttacks[Types::black][Types::e4] == 43980465111040);
}
