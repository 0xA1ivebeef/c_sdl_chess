
#ifndef MOVE_HANDLER_H
#define MOVE_HANDLER_H

#include <string.h>
#include <stdint.h>

#include "engine/position.h"
#include "engine/move.h"
#include "engine/castling.h"
#include "engine/special_move_handler.h"
#include "engine/pieces.h"
#include "engine/game.h"

#include "utils/utils.h"
#include "utils/bitboard_utils.h"

void apply_move(Position* position, Move* this_move);
int handle_move(Position* position, int startsquare, int destsquare, Undo* undo);

#endif
