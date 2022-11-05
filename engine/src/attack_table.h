#ifndef ATTACK_TABLE_H_
#define ATTACK_TABLE_H_

#include "bitboard.h"

U64 pawn_attack_masking(side sd, square sq);
U64 knight_attack_masking(square sq);
U64 king_attack_masking(square sq);
U64 bishop_attack_masking(square sq);
U64 rook_attack_masking(square sq);

U64 generate_bishop_attacks(square sq, U64 blocks);
U64 generate_rook_attacks(square sq, U64 blocks);

void init_leapers_attacks();

#endif // ATTACK_TABLE_H_
