
#include "ui/sdl_wrapper.h"

static char* piece_image_paths[12] = 
{
    "assets/pieces-basic-png/black-pawn.png",
    "assets/pieces-basic-png/black-knight.png",
    "assets/pieces-basic-png/black-bishop.png",
    "assets/pieces-basic-png/black-rook.png",
    "assets/pieces-basic-png/black-queen.png",
    "assets/pieces-basic-png/black-king.png",
    "assets/pieces-basic-png/white-pawn.png",
    "assets/pieces-basic-png/white-knight.png",
    "assets/pieces-basic-png/white-bishop.png",
    "assets/pieces-basic-png/white-rook.png",
    "assets/pieces-basic-png/white-queen.png",
    "assets/pieces-basic-png/white-king.png"
};

int load_piece_textures(AppContext* app)
{
    SDL_Surface* loaded_surface = NULL;
    for(int i = 0; i < 12; ++i)
    {
        loaded_surface = IMG_Load(piece_image_paths[i]);
        if(!loaded_surface)
        {
            fprintf(stderr, "Image Loading: %s\n", IMG_GetError());
            SDL_FreeSurface(loaded_surface);
            return 1;
        }
        app->textures[i] = SDL_CreateTextureFromSurface(app->renderer, loaded_surface);
        if(!app->textures[i])
        {
            fprintf(stderr, "Texture Loading: %s\n", SDL_GetError());
            SDL_FreeSurface(loaded_surface);
            return 1;
        }
    }
    SDL_FreeSurface(loaded_surface);
    return 0;
}

void cleanup(AppContext* app)
{
    for (int i = 0; i < 12; ++i)
        SDL_DestroyTexture(app->textures[i]);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
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
    
    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);
    if(app->renderer == NULL)
    {
        fprintf(stderr, "renderer creation: %s\n", SDL_GetError());
        SDL_DestroyWindow(app->window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    load_piece_textures(app);

    return 0;
}

