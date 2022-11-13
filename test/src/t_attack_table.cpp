#include <doctest.h>
#include <attack_table.h>
using namespace Bitboard;

/* Numbers generated using https://tearth.dev/bitboard-viewer/ */

TEST_CASE("test knight attack table") {
    AttackTables table;
    CHECK(table.arrKnightAttacks[e4] == 11333767002587136);
}

TEST_CASE("test king attack table") {
    AttackTables table;
    CHECK(table.arrKingAttacks[e4] == 61745389371392);
}

TEST_CASE("test pawn attack table") {
    AttackTables table;
    SUBCASE("white pawn attack")
    CHECK(table.arrPawnAttacks[white][e4] == 671088640);
    SUBCASE("black pawn attack")
    CHECK(table.arrPawnAttacks[black][e4] == 43980465111040);
}
