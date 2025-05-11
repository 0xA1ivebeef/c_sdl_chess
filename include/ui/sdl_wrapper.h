
#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// needed for clearing the loaded textures in the renderer.c which cleanup() also handles
#include "renderer.h"

typedef struct 
{
    SDL_Window* window;
    SDL_Renderer* renderer;
} AppContext;

int init_sdl(AppContext* app);
void cleanup(AppContext* app);

#endif
