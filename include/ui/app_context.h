
#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct AppContext
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* textures[12];
} AppContext;

#endif

