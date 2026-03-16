
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
#include "engine/hash.h"
#include "engine/opponent.h"

#include "utils/utils.h"

void position_init(Position* position, LegalMoves* legal_moves);
void game_loop(AppContext* app, Position* position, UIContext* ui_context, LegalMoves* legal_moves);

#endif
