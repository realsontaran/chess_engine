#include <attack_table.h>
#include <cstdio>
#include <cstring>

// clang-format off
const int AttackTables::bishopRelevants[] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

const int AttackTables::rookRelevants[] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

// clang-format on
AttackTables::AttackTables() {
    initAll();
}

AttackTables::~AttackTables() {
}

// rook magic numbers
const U64 AttackTables::rookMagics[] = {
    0x8a80104000800020ULL, 0x140002000100040ULL,  0x2801880a0017001ULL,
    0x100081001000420ULL,  0x200020010080420ULL,  0x3001c0002010008ULL,
    0x8480008002000100ULL, 0x2080088004402900ULL, 0x800098204000ULL,
    0x2024401000200040ULL, 0x100802000801000ULL,  0x120800800801000ULL,
    0x208808088000400ULL,  0x2802200800400ULL,    0x2200800100020080ULL,
    0x801000060821100ULL,  0x80044006422000ULL,   0x100808020004000ULL,
    0x12108a0010204200ULL, 0x140848010000802ULL,  0x481828014002800ULL,
    0x8094004002004100ULL, 0x4010040010010802ULL, 0x20008806104ULL,
    0x100400080208000ULL,  0x2040002120081000ULL, 0x21200680100081ULL,
    0x20100080080080ULL,   0x2000a00200410ULL,    0x20080800400ULL,
    0x80088400100102ULL,   0x80004600042881ULL,   0x4040008040800020ULL,
    0x440003000200801ULL,  0x4200011004500ULL,    0x188020010100100ULL,
    0x14800401802800ULL,   0x2080040080800200ULL, 0x124080204001001ULL,
    0x200046502000484ULL,  0x480400080088020ULL,  0x1000422010034000ULL,
    0x30200100110040ULL,   0x100021010009ULL,     0x2002080100110004ULL,
    0x202008004008002ULL,  0x20020004010100ULL,   0x2048440040820001ULL,
    0x101002200408200ULL,  0x40802000401080ULL,   0x4008142004410100ULL,
    0x2060820c0120200ULL,  0x1001004080100ULL,    0x20c020080040080ULL,
    0x2935610830022400ULL, 0x44440041009200ULL,   0x280001040802101ULL,
    0x2100190040002085ULL, 0x80c0084100102001ULL, 0x4024081001000421ULL,
    0x20030a0244872ULL,    0x12001008414402ULL,   0x2006104900a0804ULL,
    0x1004081002402ULL};

// bishop magic numbers
const U64 AttackTables::bishopMagics[] = {
    0x40040844404084ULL,   0x2004208a004208ULL,   0x10190041080202ULL,
    0x108060845042010ULL,  0x581104180800210ULL,  0x2112080446200010ULL,
    0x1080820820060210ULL, 0x3c0808410220200ULL,  0x4050404440404ULL,
    0x21001420088ULL,      0x24d0080801082102ULL, 0x1020a0a020400ULL,
    0x40308200402ULL,      0x4011002100800ULL,    0x401484104104005ULL,
    0x801010402020200ULL,  0x400210c3880100ULL,   0x404022024108200ULL,
    0x810018200204102ULL,  0x4002801a02003ULL,    0x85040820080400ULL,
    0x810102c808880400ULL, 0xe900410884800ULL,    0x8002020480840102ULL,
    0x220200865090201ULL,  0x2010100a02021202ULL, 0x152048408022401ULL,
    0x20080002081110ULL,   0x4001001021004000ULL, 0x800040400a011002ULL,
    0xe4004081011002ULL,   0x1c004001012080ULL,   0x8004200962a00220ULL,
    0x8422100208500202ULL, 0x2000402200300c08ULL, 0x8646020080080080ULL,
    0x80020a0200100808ULL, 0x2010004880111000ULL, 0x623000a080011400ULL,
    0x42008c0340209202ULL, 0x209188240001000ULL,  0x400408a884001800ULL,
    0x110400a6080400ULL,   0x1840060a44020800ULL, 0x90080104000041ULL,
    0x201011000808101ULL,  0x1a2208080504f080ULL, 0x8012020600211212ULL,
    0x500861011240000ULL,  0x180806108200800ULL,  0x4000020e01040044ULL,
    0x300000261044000aULL, 0x802241102020002ULL,  0x20906061210001ULL,
    0x5a84841004010310ULL, 0x4010801011c04ULL,    0xa010109502200ULL,
    0x4a02012000ULL,       0x500201010098b028ULL, 0x8040002811040900ULL,
    0x28000010020204ULL,   0x6000020202d0240ULL,  0x8918844842082200ULL,
    0x4010011029020020ULL};

U64 AttackTables::pawnAttackMask(Bitboard::Side sd, Bitboard::Square sq) {
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, sq);
    if (sd == Bitboard::white) {
        attacks |= noWeOne(bitboard);
        attacks |= noEaOne(bitboard);
    } else {
        attacks |= soWeOne(bitboard);
        attacks |= soEaOne(bitboard);
    }
    return attacks;
}

U64 AttackTables::knightAttackMask(Bitboard::Square sq) {
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, sq);

    attacks |= noNoEa(bitboard);
    attacks |= noNoWe(bitboard);
    attacks |= soSoEa(bitboard);
    attacks |= soSoWe(bitboard);
    attacks |= noEaEa(bitboard);
    attacks |= noWeWe(bitboard);
    attacks |= soEaEa(bitboard);
    attacks |= soWeWe(bitboard);

    return attacks;
}

U64 AttackTables::kingAttackMask(Bitboard::Square sq) {
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, sq);

    attacks |= soutOne(bitboard);
    attacks |= nortOne(bitboard);
    attacks |= eastOne(bitboard);
    attacks |= westOne(bitboard);

    attacks |= soEaOne(bitboard);
    attacks |= soWeOne(bitboard);
    attacks |= noEaOne(bitboard);
    attacks |= noWeOne(bitboard);

    return attacks;
}

U64 AttackTables::bishopAttackMask(Bitboard::Square sq) {
    U64 attacks = 0ULL;

    int rank;
    int file;

    // Target rank and file
    int t_rank = sq / 8;
    int t_file = sq % 8;

    for (rank = t_rank + 1, file = t_file + 1; rank < 7 && file < 7;
         ++rank, ++file)
        attacks |= (1ULL << (rank * 8 + file));
    for (rank = t_rank - 1, file = t_file + 1; rank >= 1 && file < 7;
         --rank, ++file)
        attacks |= (1ULL << (rank * 8 + file));
    for (rank = t_rank + 1, file = t_file - 1; rank < 7 && file >= 1;
         ++rank, --file)
        attacks |= (1ULL << (rank * 8 + file));
    for (rank = t_rank - 1, file = t_file - 1; rank >= 1 && file >= 1;
         --rank, --file)
        attacks |= (1ULL << (rank * 8 + file));
    return attacks;
}

U64 AttackTables::rookAttackMask(Bitboard::Square sq) {
    U64 attacks = 0ULL;

    int rank;
    int file;

    // Target rank and file
    int t_rank = sq / 8;
    int t_file = sq % 8;

    for (rank = t_rank + 1; rank < 7; ++rank)
        attacks |= (1ULL << (rank * 8 + t_file));

    for (rank = t_rank - 1; rank >= 1; --rank)
        attacks |= (1ULL << (rank * 8 + t_file));

    for (file = t_file + 1; file < 7; ++file)
        attacks |= (1ULL << (t_rank * 8 + file));

    for (file = t_file - 1; file >= 1; --file)
        attacks |= (1ULL << (t_rank * 8 + file));

    return attacks;
}

U64 AttackTables::generateBishopAttacks(Bitboard::Square sq, U64 blocks) {
    U64 attacks = 0ULL;

    int rank;
    int file;

    // Target rank and file
    int t_rank = sq / 8;
    int t_file = sq % 8;

    for (rank = t_rank + 1, file = t_file + 1; rank < 8 && file < 8;
         ++rank, ++file) {
        attacks |= (1ULL << (rank * 8 + file));
        if (((1ULL << (rank * 8 + file)) & blocks) != 0ULL)
            break;
    }
    for (rank = t_rank - 1, file = t_file + 1; rank >= 0 && file < 8;
         --rank, ++file) {
        attacks |= (1ULL << (rank * 8 + file));
        if (((1ULL << (rank * 8 + file)) & blocks) != 0ULL)
            break;
    }
    for (rank = t_rank + 1, file = t_file - 1; rank < 8 && file >= 0;
         ++rank, --file) {
        attacks |= (1ULL << (rank * 8 + file));
        if (((1ULL << (rank * 8 + file)) & blocks) != 0ULL)
            break;
    }
    for (rank = t_rank - 1, file = t_file - 1; rank >= 0 && file >= 0;
         --rank, --file) {
        attacks |= (1ULL << (rank * 8 + file));
        if (((1ULL << (rank * 8 + file)) & blocks) != 0ULL)
            break;
    }

    return attacks;
}

U64 AttackTables::generateRookAttacks(Bitboard::Square sq, U64 blocks) {
    U64 attacks = 0ULL;

    int rank;
    int file;

    // Target rank and file
    int t_rank = sq / 8;
    int t_file = sq % 8;

    for (rank = t_rank + 1; rank < 8; ++rank) {
        attacks |= (1ULL << (rank * 8 + t_file));
        if (((1ULL << (rank * 8 + t_file)) & blocks) != 0ULL)
            break;
    }

    for (rank = t_rank - 1; rank >= 0; --rank) {
        attacks |= (1ULL << (rank * 8 + t_file));
        if (((1ULL << (rank * 8 + t_file)) & blocks) != 0ULL)
            break;
    }

    for (file = t_file + 1; file < 8; ++file) {
        attacks |= (1ULL << (t_rank * 8 + file));
        if (((1ULL << (t_rank * 8 + file)) & blocks) != 0ULL)
            break;
    }
    for (file = t_file - 1; file >= 0; --file) {
        attacks |= (1ULL << (t_rank * 8 + file));
        if (((1ULL << (t_rank * 8 + file)) & blocks) != 0ULL)
            break;
    }

    return attacks;
}

/*
 * Calculate all occupancy variations from given attack mask
 */
U64 AttackTables::setOccupancies(int index, int bits_in_mask, U64 attack_mask) {
    U64 occupancy_map = 0ULL;
    for (int bits = 0; bits < bits_in_mask; ++bits) {
        unsigned int sq = Bitboard::get_lsb(attack_mask);
        Bitboard::pop_bit(attack_mask, static_cast<int>(sq));
        if ((index & (1 << bits)) != 0) {
            occupancy_map |= (1ULL << sq);
        }
    }
    return occupancy_map;
}

U64 AttackTables::findMagics(Bitboard::Square sq, int relevant_bits,
                             bool bishop) {
    // init occupancies
    U64 occupancies[4096];

    // init attack tables
    U64 attacks[4096];

    // init used attacks
    U64 used_attacks[4096];

    // init attack mask for a current piece
    U64 attack_mask = bishop ? bishopAttackMask(sq) : rookAttackMask(sq);
    // init occupancy indicies
    int occupancy_indicies = 1 << relevant_bits;

    // loop over occupancy indicies
    for (int index = 0; index < occupancy_indicies; index++) {
        // init occupancies
        occupancies[index] = setOccupancies(index, relevant_bits, attack_mask);

        // init attacks
        attacks[index] = bishop ? generateBishopAttacks(sq, occupancies[index])
                                : generateRookAttacks(sq, occupancies[index]);
    }

    // test magic numbers loop
    for (int random_count = 0; random_count < 100000000; random_count++) {
        // generate magic number candidate
        U64 magic_number = generate_magic_number();

        // skip inappropriate magic numbers
        if (Bitboard::bit_count((attack_mask * magic_number) &
                                0xFF00000000000000) < 6)
            continue;

        // init used attacks
        memset(used_attacks, 0ULL, sizeof(used_attacks));

        // init index & fail flag
        int index;
        int fail;

        // test magic index loop
        for (index = 0, fail = 0; (fail == 0) && index < occupancy_indicies;
             index++) {
            // init magic index
            int magic_index = static_cast<int>(
                (occupancies[index] * magic_number) >> (64 - relevant_bits));

            // if magic index works
            if (used_attacks[magic_index] == 0ULL)
                used_attacks[magic_index] = attacks[index];
            else if (used_attacks[magic_index] != attacks[index])
                fail = 1;
        }

        // if magic number works
        if (fail == 0)
            // return it
            return magic_number;
    }

    // if magic number doesn't work
    printf("  Magic number fails!\n");
    return 0ULL;
}

void AttackTables::initSliders(bool bishop) {
    for (int sq = 0; sq < 64; sq++) {
        bishopMasks[sq] = bishopAttackMask(static_cast<Bitboard::Square>(sq));

        rookMasks[sq] = rookAttackMask(static_cast<Bitboard::Square>(sq));

        U64 attack_mask = bishop ? bishopMasks[sq] : rookMasks[sq];
        int relevant_bit_count =
            static_cast<int>(Bitboard::bit_count(attack_mask));
        int occupancy_indices = (1 << relevant_bit_count);

        for (int index = 0; index < occupancy_indices; ++index) {
            if (bishop) {
                U64 occ =
                    setOccupancies(index, relevant_bit_count, attack_mask);
                int magic_index = static_cast<int>((occ * bishopMagics[sq]) >>
                                                   (64 - bishopRelevants[sq]));
                arrBishopAttacks[sq][magic_index] = generateBishopAttacks(
                    static_cast<Bitboard::Square>(sq), occ);
            } else {
                U64 occ =
                    setOccupancies(index, relevant_bit_count, attack_mask);
                int magic_index = static_cast<int>((occ * rookMagics[sq]) >>
                                                   (64 - rookRelevants[sq]));
                arrRookAttacks[sq][magic_index] =
                    generateRookAttacks(static_cast<Bitboard::Square>(sq), occ);
            }
        }
    }
}

U64 AttackTables::getRookAttacks(Bitboard::Square sq, U64 occ) {
    occ &= rookMasks[sq];
    occ *= rookMagics[sq];
    occ >>= (64 - rookRelevants[sq]);
    return arrRookAttacks[sq][occ];
}

U64 AttackTables::getBishopAttacks(Bitboard::Square sq, U64 occ) {
    occ &= bishopMasks[sq];
    occ *= bishopMagics[sq];
    occ >>= (64 - bishopRelevants[sq]);
    return arrBishopAttacks[sq][occ];
}

void AttackTables::initMagics() { // NOLINT
    for (int sq = 0; sq < 64; sq++)
        printf(" 0x%llxULL\n", findMagics(static_cast<Bitboard::Square>(sq),
                                          rookRelevants[sq], false));

    for (int sq = 0; sq < 64; sq++)
        printf("0x%llxULL\n", findMagics(static_cast<Bitboard::Square>(sq),
                                         bishopRelevants[sq], true));
}

void AttackTables::initLeapers() {
    for (int sq = 0; sq < 64; ++sq) {
        /* Init pawn attack tables */
        arrPawnAttacks[Bitboard::white][sq] =
            pawnAttackMask(Bitboard::white, static_cast<Bitboard::Square>(sq));
        arrPawnAttacks[Bitboard::black][sq] =
            pawnAttackMask(Bitboard::black, static_cast<Bitboard::Square>(sq));

        /* Init pawn attack tables */
        arrKnightAttacks[sq] =
            knightAttackMask(static_cast<Bitboard::Square>(sq));

        /* Init king attack tables */
        arrKingAttacks[sq] = kingAttackMask(static_cast<Bitboard::Square>(sq));
    }
}

void AttackTables::initAll() {
    initLeapers();
    // init_magic_numbers();
    initSliders(true);  // Bishop
    initSliders(false); // Rook
}
