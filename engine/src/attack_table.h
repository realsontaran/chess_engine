#ifndef ATTACK_TABLE_H_
#define ATTACK_TABLE_H_

#include "bitboard.h"

U64 pawn_attack_masking(side sd, square sq);
U64 knight_attack_masking(square sq);

#endif // ATTACK_TABLE_H_
