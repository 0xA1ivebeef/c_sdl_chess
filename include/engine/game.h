
#ifndef GAME_H
#define GAME_H

#undef main

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#include "ui/sdl_wrapper.h"
#include "ui/console_logger.h"
#include "ui/renderer.h"
#include "ui/sdl_event_handler.h"

#include "engine/constants.h"
#include "engine/types.h"
#include "engine/fen_string_loader.h"
#include "engine/move_generator.h"
#include "engine/attack_generator.h"
#include "engine/check.h"
#include "engine/perft.h"
#include "engine/zobrist.h"

#include "opponent/opponent.h"

#include "utils/utils.h"
#include "utils/bitboard_utils.h"
#include "utils/legal_move_utils.h"

void position_init(Position* position);
void game_loop(AppContext* app, Position* position, UIContext* ui_context);

#endif
