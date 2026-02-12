
#include "ui/renderer.h"

void render_board(AppContext* app)
{
    SDL_SetRenderDrawColor(app->renderer, 130, 115, 185, 255);
    SDL_RenderClear(app->renderer);
    
    SDL_SetRenderDrawColor(app->renderer, 245, 245, 235, 255);
    SDL_Rect r = { 0, 0, TILESIZE, TILESIZE };
    for(int i = 0; i < 8; ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            if((i + j) % 2 == 0)
            {
                r.x = i * TILESIZE;
                r.y = j * TILESIZE;
                SDL_RenderFillRect(app->renderer, &r);
            }
        }
    }
}

void render_pieces(AppContext* app, const uint64_t* bitboards)
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
                SDL_RenderCopy(app->renderer, app->textures[j], NULL, &dstrect);
            }
        }
    }
}

void render_legal_moves(AppContext* app, Position* pos, int startsquare)
{
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(app->renderer, 150, 0, 0, 100);
    SDL_Rect r = {0 , 0, TILESIZE, TILESIZE};
	for(int i = 0; i < pos->legal_move_count; ++i)
    {
        if (pos->legal_moves[i].startsquare == startsquare)
        {
            r.x = pos->legal_moves[i].destsquare % 8 * TILESIZE;
            r.y = pos->legal_moves[i].destsquare / 8 * TILESIZE;
            SDL_RenderFillRect(app->renderer, &r);
        }
    }
    SDL_RenderPresent(app->renderer);
}

void render(AppContext* app, uint64_t* bitboards)
{
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
    SDL_RenderClear(app->renderer);

    render_board(app);
    render_pieces(app, bitboards);

    SDL_RenderPresent(app->renderer);
}

