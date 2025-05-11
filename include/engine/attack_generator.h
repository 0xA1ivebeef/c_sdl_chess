
#ifndef ATTACK_GENERATOR_H
#define ATTACK_GENERATOR_H

#include "bitmask_loader.h"
#include "constants.h"
#include "console_logger.h"

uint64_t get_attack_bitboard(int p, uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags);

#endif
