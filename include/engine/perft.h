
#ifndef PERFT_H
#define PERFT_H

#include <stdint.h>

#include "engine/constants.h"
#include "engine/types.h"
#include "engine/move_generator.h"
#include "engine/move_handler.h"
#include "engine/attack_generator.h"
#include "engine/check.h"
#include "fen_string_loader.h"

#include "utils/utils.h"

int perft(Position* pos, int depth);
void full_perft_test(Position* pos);

#endif
