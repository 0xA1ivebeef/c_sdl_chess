
#ifndef CHECK_H
#define CHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> 
#include <assert.h>

#include "ui/console_logger.h"

#include "engine/types.h"
#include "engine/attack_generator.h"
#include "engine/move_handler.h"

#include "utils/utils.h"

void sort_mvv_lva(Position* pos, LegalMoves* lm);
int is_check(Position* pos, uint8_t player);
void filter_moves(Position* pos, LegalMoves* lm);

#endif

