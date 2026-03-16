
#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "engine/constants.h"
#include "engine/hash.h"

#define NUM_SOUNDS 5
#define NUM_TEXTURES 12

typedef struct AppContext
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* textures[NUM_TEXTURES];
    SDL_Texture* move_circle;
    SDL_Texture* capture_circle;
    Mix_Chunk* sounds[NUM_SOUNDS];
} AppContext;

int init_sdl(AppContext* app);
int load_textures(AppContext* app);
void cleanup(AppContext* app);

#endif
