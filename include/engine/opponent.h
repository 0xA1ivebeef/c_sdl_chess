
#ifndef OPPONENT_H
#define OPPONENT_H

#include <limits.h>
#include <stdint.h>

#include "engine/types.h"
#include "engine/move_handler.h"
#include "engine/constants.h"
#include "engine/move_generator.h"
#include "engine/check.h"
#include "engine/hash.h"
#include "engine/search.h"

int evaluate(Position* pos);
Move opponent_move(Position* pos, LegalMoves* lm);

#endif
