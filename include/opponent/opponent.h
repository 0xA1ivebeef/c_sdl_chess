
#ifndef OPPONENT_H
#define OPPONENT_H

#include <limits.h>
#include <stdint.h>

#include "engine/types.h"
#include "engine/move_handler.h"
#include "engine/constants.h"
#include "engine/move_generator.h"
#include "engine/check.h"
#include "engine/zobrist.h"

int evaluate(Position* pos);
int opponent_move(Position* pos, LegalMoves* lm);

#endif
