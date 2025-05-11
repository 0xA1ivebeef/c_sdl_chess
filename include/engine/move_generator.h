
#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <stdint.h>

#include "engine/constants.h"
#include "engine/move.h"
#include "engine/bitmask_loader.h"
#include "engine/castling.h"

void generate_legal_moves(int p, uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags, Move* legal_moves, uint64_t atk_bb);

#endif
