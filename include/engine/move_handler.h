
#ifndef MOVE_HANDLER_H
#define MOVE_HANDLER_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "engine/types.h"
#include "engine/hash.h"
#include "engine/castling.h"

#include "utils/utils.h"

void make_move(Position* pos, Move m, Undo* undo);
void undo_move(Position* pos, Move m, Undo* undo);
int handle_move(Position* pos, LegalMoves* lm, Move m);

#endif
