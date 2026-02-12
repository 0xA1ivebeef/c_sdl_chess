
#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>

#include "engine/constants.h"
#include "engine/types.h"

#include "ui/app_context.h"
#include "ui/console_logger.h"

void render(AppContext* app, uint64_t* bb);
void render_legal_moves(AppContext* app, Position* pos, int start);

#endif
