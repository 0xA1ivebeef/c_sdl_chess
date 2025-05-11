
#include "ui/sdl_event_handler.h"

void handle_event(GameContext* game, SDL_Event* event)
{
    int mouse_x = 0; 
    int mouse_y = 0;
    int destsquare = -1;

    SDL_GetMouseState(&mouse_x, &mouse_y);
    switch (e.type)
    {
        case SDL_QUIT:
            game->running = 0;
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_ESCAPE)
                return;
                break;
        case SDL_MOUSEBUTTONDOWN:
            game->selected_square = handle_mouse_event(mouse_x, mouse_y, ~(game->occupancy[game->game_flags[0]]));
            render_legal_moves(game->selected_square, game->legal_moves);
            break;
        case SDL_MOUSEBUTTONUP:
            destsquare = handle_mouse_event(mouse_x, mouse_y, game->occupancy[game->game_flags[0]]);
            if (handle_move(game))
                game->needs_update = 1;
            game->selected_square = -1;
            break;
    }
}

