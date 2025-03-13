
#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include "constants.h"
#include "move.h"
#include "bitmask_loader.h"
#include "castling.h"

void generate_legal_moves(int p, uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags, move* legal_moves, uint64_t atk_bb);

#endif
