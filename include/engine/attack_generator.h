
#ifndef ATTACK_GENERATOR_H
#define ATTACK_GENERATOR_H

#include "ui/console_logger.h"

#include "engine/constants.h"
#include "engine/types.h"
#include "engine/bitmask_loader.h"

uint64_t get_attack_bb(Position* pos, uint8_t player);
void generate_attack_bb(Position* position);
uint64_t get_castling_attack_bitboard(Position* pos);

#endif
