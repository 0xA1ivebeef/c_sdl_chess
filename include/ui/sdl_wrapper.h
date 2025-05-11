
#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H

#include "renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct 
{
    SDL_Window* window;
    SDL_Renderer* renderer;
} AppContext;

void cleanup(AppContext* app);
int init_sdl(AppContext* app);

#endif
