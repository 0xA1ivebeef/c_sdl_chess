
#ifndef GAME_H
#define GAME_H

#undef main

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#include "ui/console_logger.h"
#include "ui/renderer.h"
#include "ui/sdl_event_handler.h"
#include "ui/mouse_event_handler.h"

#include "engine/fen_string_loader.h"
#include "engine/constants.h"
#include "engine/move_generator.h"
#include "engine/move.h"
#include "engine/check.h"
#include "engine/attack_generator.h"
#include "double_pawn_push.h"
#include "castling.h"
#include "engine/tester.h"

#include "utils/bitboard_utils.h"
#include "utils/legal_move_utils.h"

int setup(GameContext* game, SDL_Renderer* renderer);
void game_loop(GameContext* app);

#endif
