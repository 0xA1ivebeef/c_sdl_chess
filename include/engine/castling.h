
#ifndef CASTLING_H
#define CASTLING_H

#include <stdio.h>

#include "engine/constants.h"
#include "engine/types.h"
#include "engine/attack_generator.h"

#include "utils/bitboard_utils.h"

#include "ui/console_logger.h"

void add_castling(Position* position);
void update_castle_rights(Position* position, Move* m);

#endif


