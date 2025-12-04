
#ifndef ATTACK_GENERATOR_H
#define ATTACK_GENERATOR_H

#include "ui/console_logger.h"

#include "engine/pieces.h"
#include "engine/bitmask_loader.h"
#include "engine/constants.h"
#include "engine/position.h"

uint64_t get_attack_bitboard(int player, uint64_t* bitboards, uint64_t* occupancy_bitboards);
void generate_attack_bitboards(Position* position);

#endif
