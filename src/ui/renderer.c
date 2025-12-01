
#include "ui/renderer.h"

static SDL_Texture* piece_images[12];
static SDL_Renderer* renderer;

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

int load_renderer(SDL_Renderer* r)
{
    renderer = r; // static
    if (!renderer)
    {
        return 1;
    }
    return 0;
}

int load_pieces_images()
{
    SDL_Surface* loaded_surface = NULL;
    for(int i = 0; i < 12; ++i)
    {
        loaded_surface = IMG_Load(piece_image_paths[i]);
        if(loaded_surface == NULL)
        {
            fprintf(stderr, "Image Loading: %s\n", IMG_GetError());
            SDL_FreeSurface(loaded_surface);
            return 1;
        }
        piece_images[i] = SDL_CreateTextureFromSurface(renderer, loaded_surface);
        if(piece_images[i] == NULL)
        {
            fprintf(stderr, "Texture Loading: %s\n", SDL_GetError());
            SDL_FreeSurface(loaded_surface);
            return 1;
        }
    }
    SDL_FreeSurface(loaded_surface);
    return 0;
}

void clear_renderer()
{
    for(int i = 0; i < 12; ++i)
        SDL_DestroyTexture(piece_images[i]);
}

void render_board()
{
    SDL_SetRenderDrawColor(renderer, 130, 115, 185, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 245, 245, 235, 255);
    SDL_Rect r = { 0, 0, TILESIZE, TILESIZE };
    for(int i = 0; i < 8; ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            if((i + j) % 2 == 0)
            {
                r.x = i * TILESIZE;
                r.y = j * TILESIZE;
                SDL_RenderFillRect(renderer, &r);
            }
        }
    }
}

void render_pieces(const uint64_t* bitboards)
{
    SDL_Rect dstrect = { 0, 0, TILESIZE, TILESIZE };
    for(int i = 0; i < 64; ++i)
    {
        for(int j = 0; j < 12; ++j)
        {
            if(bitboards[j] & (1ULL << i))
            {
                dstrect.x = i % 8 * TILESIZE;
                dstrect.y = i / 8 * TILESIZE;
                SDL_RenderCopy(renderer, piece_images[j], NULL, &dstrect);
            }
        }
    }
}

void render_legal_moves(int startsquare, Move* legal_moves)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 150, 0, 0, 100);
    SDL_Rect r = {0 , 0, TILESIZE, TILESIZE};
	for(int i = 0; i < LEGAL_MOVES_SIZE; ++i)
    {
        if(legal_moves[i].startsquare == startsquare)
        {
            r.x = legal_moves[i].destsquare % 8 * TILESIZE;
            r.y = legal_moves[i].destsquare / 8 * TILESIZE;
            SDL_RenderFillRect(renderer, &r);
        }
    }
    SDL_RenderPresent(renderer);
}

void render(uint64_t* bitboards)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    render_board();
    render_pieces(bitboards);

    SDL_RenderPresent(renderer);
}

