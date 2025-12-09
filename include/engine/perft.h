
#ifndef PERFT_H
#define PERFT_H

#include <stdint.h>

#include "engine/game.h"
#include "engine/position.h"
#include "engine/pieces.h"
#include "engine/move.h"
#include "engine/move_generator.h"
#include "engine/constants.h"
#include "engine/move_handler.h"
#include "engine/attack_generator.h"

#include "utils/bitboard_utils.h"
#include "utils/legal_move_utils.h"

int get_nodes(Position* position, int depth);
// void perft_divide(Position* position, int depth);

#endif
