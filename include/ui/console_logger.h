
#ifndef CONSOLE_LOGGER_H
#define CONSOLE_LOGGER_H

#include <stdint.h>
#include <stdio.h>

#include "engine/constants.h"
#include "engine/types.h"

#include "utils/utils.h"

void print_binary(int x);
void log_gamestate(Position* pos);
void get_square_string(int square, char* buffer);
void translate_move(Move* m);
void log_bitboard(uint64_t* bb);
void log_bitboards(uint64_t* bb, uint64_t* occ);
void log_legal_moves(Move* legal_moves);

#endif
