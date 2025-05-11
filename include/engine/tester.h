
#ifndef TESTER_H
#define TESTER_H

#include <stdint.h>
#include "legal_move_utils.h"
#include "move.h"
#include "move_generator.h"
#include "constants.h"
#include "bitboard_utils.h"

int get_nodes(uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags, int depth);

#endif
