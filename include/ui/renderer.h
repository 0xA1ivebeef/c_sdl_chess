
#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include <math.h>

#include "engine/constants.h"
#include "engine/types.h"

#include "ui/sdl_wrapper.h"
#include "ui/console_logger.h"

void render(AppContext* app, uint64_t* bb);
void render_legal_moves(AppContext* app, Position* pos, LegalMoves* lm, int start);

#endif
