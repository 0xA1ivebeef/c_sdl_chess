
#ifndef CASTLING_H
#define CASTLING_H

#include <stdio.h>

#include "engine/constants.h"
#include "engine/move.h"
#include "engine/attack_generator.h"
#include "engine/check.h"

#include "utils/legal_move_utils.h"

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

int square_under_attack(int square, uint64_t atk_bb); 
void add_castling(int square, uint64_t* occupancy_bitboards, int* game_flags, Move* legal_moves, uint64_t atk_bb);
void update_castle_rights(uint64_t* bitboards, Move* this_move, int* game_flags);

#endif


