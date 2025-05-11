
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

#include "engine/mouse_event_handler.h"
#include "engine/fen_string_loader.h"
#include "engine/constants.h"
#include "engine/move_generator.h"
#include "engine/move.h"
#include "engine/check.h"
#include "engine/attack_generator.h"
#include "engine/tester.h"
#include "engine/special_move_handler.h"

#include "utils/bitboard_utils.h"
#include "utils/legal_move_utils.h"

// TODO give void game_loop(GameContext* app) for the main;

#endif
