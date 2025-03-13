
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#undef main

#include "console_logger.h"
#include "fen_string_loader.h"
#include "constants.h"
#include "renderer.h"
#include "move_generator.h"
#include "move.h"
#include "check.h"
#include "attack_generator.h"
#include "bitboard_utils.h"
#include "legal_move_utils.h"
#include "tester.h"
#include "mouse_event_handler.h"
#include "special_move_handler.h"

#endif
