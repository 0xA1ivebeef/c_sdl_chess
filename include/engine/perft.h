
#ifndef PERFT_H
#define PERFT_H

#include <stdint.h>

#include "engine/constants.h"
#include "engine/types.h"
#include "engine/move_generator.h"
#include "engine/move_handler.h"
#include "engine/attack_generator.h"
#include "engine/check.h"
#include "engine/fen_string_loader.h"

#include "utils/utils.h"

int perft(Position* pos, int depth);
int perft_suite(Position* pos);
int perft_captures_suite(Position* pos);

#endif
