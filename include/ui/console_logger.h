
#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include <stdint.h>
#include <stdio.h>

#include "engine/constants.h"
#include "engine/types.h"

#include "utils/utils.h"

void log_gamestate(Position* pos);
void log_bitboard(uint64_t* bb);
void log_bitboards(uint64_t* bb, uint64_t* occ);
void log_legal_moves(Move* legal_moves, int legal_move_count);

#endif
