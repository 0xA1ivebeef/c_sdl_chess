
#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>

#include "move.h"
#include "constants.h"
#include "console_logger.h"

int load_pieces_images();
void load_renderer(SDL_Renderer* r);
void clear_renderer();
void render(uint64_t* bitboards);
void render_legal_moves(int startsquare, move* legal_moves);

#endif
