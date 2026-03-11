
#ifndef MOVE_HANDLER_H
#define MOVE_HANDLER_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "engine/types.h"
#include "engine/zobrist.h"
#include "engine/castling.h"
#include "engine/move_generator.h"

#include "utils/utils.h"
#include "utils/bitboard_utils.h"

void apply_move(Position* pos, Move* m, Undo* undo);
void undo_move(Position* pos, Move* m, Undo* undo);
int handle_move(Position* pos, LegalMoves* lm, Move* m);

#endif
