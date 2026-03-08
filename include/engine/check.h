
#ifndef CHECK_H
#define CHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> 
#include <assert.h>

#include "ui/console_logger.h"

#include "engine/types.h"
#include "engine/move_handler.h"
#include "engine/attack_generator.h"

#include "utils/legal_move_utils.h"
#include "utils/bitboard_utils.h"

void log_position_diff(Position* a, Position* b);
int is_check(int king_sq, uint64_t attack_bb);
void filter_moves(Position* pos);

#endif

