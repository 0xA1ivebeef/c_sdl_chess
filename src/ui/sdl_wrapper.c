
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

SDL_Texture* create_circle_texture(SDL_Renderer* renderer, int radius, SDL_Color color)
{
    int size = radius * 2;
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_TARGET, size, size);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    // Render to texture
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // fully transparent background
    SDL_RenderClear(renderer);

    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            float dx = x - radius + 0.5f;
            float dy = y - radius + 0.5f;
            float dist = sqrtf(dx*dx + dy*dy);

            float t = radius - dist;
            if (t > 0)
            {
                Uint8 alpha = (Uint8)(fminf(t, 1.0f) * color.a); // gradient for soft edge
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }

    SDL_SetRenderTarget(renderer, NULL);
    return texture;
}

SDL_Texture* create_ring_texture(SDL_Renderer* renderer, int outer_radius, int inner_radius, SDL_Color color, float start_angle, float end_angle)
{
    int size = outer_radius * 2;
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_TARGET, size, size);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // transparent background
    SDL_RenderClear(renderer);

    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            float dx = x - outer_radius + 0.5f;
            float dy = y - outer_radius + 0.5f;
            float dist = sqrtf(dx*dx + dy*dy);

            if (dist >= inner_radius && dist <= outer_radius)
            {
                // compute angle in radians (0..2π)
                float angle = atan2f(dy, dx);
                if (angle < 0) angle += 2 * M_PI;

                if (angle >= start_angle && angle <= end_angle)
                {
                    // soft edges: fade out near inner or outer radius
                    float t_outer = outer_radius - dist;
                    float t_inner = dist - inner_radius;
                    float t = fminf(t_outer, t_inner);
                    if (t > 1.0f) t = 1.0f; // clamp
                    Uint8 alpha = (Uint8)(t * color.a);

                    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }
    }

    SDL_SetRenderTarget(renderer, NULL);
    return texture;
}

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

    SDL_DestroyTexture(app->move_circle);
    SDL_DestroyTexture(app->capture_circle);

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

    SDL_Color dark_transparent = { 20, 20, 20, 100 }; // black, alpha 100
    app->move_circle = create_circle_texture(app->renderer, 20, dark_transparent);
    app->capture_circle = create_ring_texture
    (
            app->renderer, 70, 50, 
            dark_transparent, 0, 2*M_PI 
    );

    close_opening_book();

    return 0;
}

