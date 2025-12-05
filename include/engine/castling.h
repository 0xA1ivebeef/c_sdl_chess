
#ifndef CASTLING_H
#define CASTLING_H

#include <stdio.h>

#include "engine/position.h"
#include "engine/pieces.h"
#include "engine/constants.h"
#include "engine/move.h"

#include "utils/bitboard_utils.h"

#include "ui/console_logger.h"

void add_castling(Position* position);
void update_castle_rights(Position* position, Move* this_move);

#endif


