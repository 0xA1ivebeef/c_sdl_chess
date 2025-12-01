
#ifndef MOVE_HANDLER_H
#define MOVE_HANDLER_H

#include <stdint.h>

#include "engine/position.h"
#include "engine/move.h"
#include "engine/castling.h"
#include "engine/special_move_handler.h"

#include "utils/bitboard_utils.h"

void apply_move(Position* position, Move* this_move);
int handle_move(Position* position, int startsquare, int destsquare);

#endif
