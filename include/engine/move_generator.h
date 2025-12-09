
#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <string.h>
#include <stdint.h>

#include "engine/constants.h"
#include "engine/move.h"
#include "engine/bitmask_loader.h"
#include "engine/castling.h"

#include "ui/console_logger.h"

#include "utils/utils.h"

int square_under_attack(uint64_t* bitboards, uint64_t* occupancy_bitboards, int king_square, int by_side);
void generate_legal_moves(Position* position);

#endif
