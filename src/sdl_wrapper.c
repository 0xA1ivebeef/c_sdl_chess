
#include "include/sdl_wrapper.h"

void cleanup(AppContext* app)
{
    clear_renderer();
    SDL_DestroyWindow(app->window);
    SDL_DestroyRenderer(app->renderer);
    IMG_Quit();
    SDL_Quit();
}

int init_sdl(AppContext* app)
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        fprintf(stderr, "IMG_Init: %s\n", IMG_GetError());
        return 1;
    }
    
    app->window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    if(app->window == NULL)
    {
        fprintf(stderr, "window creation: %s\n", SDL_GetError());
        return 1;
    }
    
    app->renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if(app->renderer == NULL)
    {
        fprintf(stderr, "renderer creation: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    return 0;
}

