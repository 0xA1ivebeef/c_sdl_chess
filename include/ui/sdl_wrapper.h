
#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "engine/constants.h"
#include "engine/hash.h"

typedef struct AppContext
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* textures[12];
    SDL_Texture* move_circle;
    SDL_Texture* capture_circle;
} AppContext;

int init_sdl(AppContext* app);
int load_textures(AppContext* app);
void cleanup(AppContext* app);

#endif
