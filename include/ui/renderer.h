
#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>

#include "engine/move.h"
#include "engine/constants.h"

#include "ui/app_context.h"
#include "ui/console_logger.h"

void render(AppContext* app, uint64_t* bitboards);
void render_legal_moves(AppContext* app, Position* position, int startsquare);

#endif
