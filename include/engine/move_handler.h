
#ifndef MOVE_HANDLER_H
#define MOVE_HANDLER_H

#include <stdint.h>

#include "engine/position.h"
#include "engine/move.h"
#include "engine/castling.h"
#include "engine/double_pawn_push.h"

#include "utils/bitboard_utils.h"

void apply_move(uint64_t* bitboards, int startsquare, int destsquare); // needed for check.c 
int handle_move(Position* position, int startsquare, int destsquare);

#endif
