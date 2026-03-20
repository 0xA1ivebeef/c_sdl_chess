
#ifndef BITMASK_LOADER_H
#define BITMASK_LOADER_H

#include <stdint.h>

#include "engine/constants.h"

#include "utils/utils.h"

extern uint64_t white_pawn_normal_bitmasks[64];
extern uint64_t black_pawn_normal_bitmasks[64];

extern uint64_t white_pawn_double_bitmasks[64];
extern uint64_t black_pawn_double_bitmasks[64];

extern uint64_t white_pawn_attack_bitmasks[64];
extern uint64_t black_pawn_attack_bitmasks[64];

extern uint64_t knight_bitmasks[64];
extern uint64_t bishop_bitmasks[64];
extern uint64_t rook_bitmasks[64];
extern uint64_t queen_bitmasks[64];
extern uint64_t king_bitmasks[64];

void load_bitmasks(void);

#endif
