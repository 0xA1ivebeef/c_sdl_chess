
#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include <stdint.h>
#include <stdio.h>

#include "engine/move.h"
#include "engine/constants.h"

void get_square_string(int square, char* buffer);
void translate_move(Move* m);
void log_bitboard(uint64_t* bitboard);
void log_bitboards(uint64_t* bitboards, uint64_t* occupancy_bitboards);
void log_legal_moves(Move* legal_moves);

#endif
