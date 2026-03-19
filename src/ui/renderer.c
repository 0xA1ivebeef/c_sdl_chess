
#include "ui/renderer.h"

void render_move(AppContext* app, Move m)
{
    SDL_SetRenderDrawColor(app->renderer, 100, 230, 230, 30);

    SDL_Rect r = { move_from(m) % 8 * TILESIZE, move_from(m) / 8 * TILESIZE, TILESIZE, TILESIZE };
    SDL_RenderFillRect(app->renderer, &r);
    r.x = move_to(m) % 8 * TILESIZE;
    r.y = move_to(m) / 8 * TILESIZE;
    SDL_RenderFillRect(app->renderer, &r);
    SDL_RenderPresent(app->renderer);
}

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

void render_legal_moves(AppContext* app, Position* pos, LegalMoves* lm, int start)
{
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(app->renderer, 50, 50, 50, 50);
    int radius = 20;
	for(int i = 0; i < lm->count; ++i)
    {
        Move m = lm->moves[i];
        if (move_from(m) == start)
        {
            int cx = move_to(m) % 8 * TILESIZE + TILESIZE / 2;
            int cy = move_to(m) / 8 * TILESIZE + TILESIZE / 2;

            if (pos->piece_on_sq[move_to(m)] != EMPTY)
            {
                SDL_Rect dest = { cx - radius*3, cy - radius*3, radius*6, radius*6 };
                SDL_RenderCopy(app->renderer, app->capture_circle, NULL, &dest);
            }
            else
            {
                SDL_Rect dest = { cx - radius, cy - radius, radius*2, radius*2 };
                SDL_RenderCopy(app->renderer, app->move_circle, NULL, &dest);
            }
        }
    }
    SDL_RenderPresent(app->renderer);
}

void render(AppContext* app, uint64_t* bitboards, Move last_move)
{
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
    SDL_RenderClear(app->renderer);

    render_board(app);
    render_pieces(app, bitboards);
    if (last_move != 0)
        render_move(app, last_move);

    SDL_RenderPresent(app->renderer);
}

