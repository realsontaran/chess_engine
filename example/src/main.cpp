#include <attack_table.h>

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    AttackTables table = AttackTables();

    printBoard(table.arrKnightAttacks[e4]);
    return 0;
}
