
#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include <stdint.h>
#include <stdio.h>

#include "move.h"
#include "constants.h"

void get_square_string(int square, char* buffer);
void translate_move(move* m);
void log_bitboard(uint64_t* bitboard);
void log_bitboards(uint64_t* bitboards, uint64_t* occupancy_bitboards);
void log_legal_moves(move* legal_moves);
void log_game_flags(int* game_flags);

#endif
