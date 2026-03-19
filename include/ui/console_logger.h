
#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

#include "engine/constants.h"
#include "engine/types.h"

#include "utils/utils.h"

void log_piece_on_sq(uint8_t n[64]);
void log_move(Move m);
void log_position_diff(Position* a, Position* b);
void log_gamestate(Position* pos);
void log_bitboard(uint64_t* bb);
void log_bitboards(uint64_t* bb, uint64_t* occ);
void log_legal_moves(LegalMoves* lm);
void log_occ(uint64_t* occ);

#endif
