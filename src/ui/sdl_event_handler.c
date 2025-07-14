
#include "ui/sdl_event_handler.h"

void handle_event(GameContext* game, SDL_Event* event)
{
    int mouse_x = 0; 
    int mouse_y = 0;
    int destsquare = -1;

    SDL_GetMouseState(&mouse_x, &mouse_y);
    switch (event->type)
    {
        case SDL_QUIT:
            game->running = 0;
            break;
        case SDL_KEYDOWN:
            if (event->key.keysym.sym == SDLK_ESCAPE)
                game->running = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            game->selected_square = handle_mouse_event(mouse_x, mouse_y, ~(game->occupancy[CURRENT_PLAYER]));
            render_legal_moves(game->selected_square, game->legal_moves);
            break;
        case SDL_MOUSEBUTTONUP:
            destsquare = handle_mouse_event(mouse_x, mouse_y, game->occupancy[game->game_flags[0]]);
            if (handle_move(game->bitboards, game->selected_square, destsquare, game->legal_moves, game->game_flags))
                game->needs_update = 1;
            game->selected_square = -1; // always unselect piece
            break;
    }
}

