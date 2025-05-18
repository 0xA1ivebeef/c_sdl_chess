
#ifndef TESTER_H
#define TESTER_H

#include <stdint.h>

#include "engine/move.h"
#include "engine/move_generator.h"
#include "engine/constants.h"
#include "engine/move_handler.h"
#include "engine/attack_generator.h"

#include "utils/bitboard_utils.h"
#include "utils/legal_move_utils.h"

int get_nodes(uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags, int depth);

#endif
