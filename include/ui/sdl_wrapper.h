
#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "engine/constants.h"

#include "ui/app_context.h" 

int init_sdl(AppContext* app);
int load_textures(AppContext* app);
void cleanup(AppContext* app);

#endif
