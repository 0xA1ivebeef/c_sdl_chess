
#ifndef CHECK_H
#define CHECK_H

#include <stdio.h>
#include <string.h>
#include <stdint.h> 

#include "ui/console_logger.h"

#include "engine/move_handler.h"
#include "engine/position.h"

#include "utils/legal_move_utils.h"
#include "utils/bitboard_utils.h"

int is_check(int player, uint64_t* bitboards, uint64_t attack_bitboard);
void filter_moves(Position* position);

#endif

