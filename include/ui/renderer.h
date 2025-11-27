
#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>

#include "engine/move.h"
#include "engine/constants.h"

#include "ui/console_logger.h"

int load_pieces_images();
int load_renderer(SDL_Renderer* r);
void clear_renderer();
void render(uint64_t* bitboards);
void render_legal_moves(int startsquare, Move* legal_moves);

#endif
