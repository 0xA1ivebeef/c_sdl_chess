
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#include "engine/types.h"

#include "utils/bitboard_utils.h"
#include "utils/char_utils.h"

int abs_int(int x);
int is_pawn_promotion(Position* pos, Move* m);
void choose_promotion_move(Move* m);
int square_string_to_int(char* square_string);
const char* square_to_notation(int sq);

#endif 

