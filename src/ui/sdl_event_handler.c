
#include "ui/sdl_event_handler.h"

int handle_event(Position* position, UIContext* ui_context, SDL_Event* event)
{
    int mouse_x = 0; 
    int mouse_y = 0;
    int destsquare = -1;
    int current_player = position->current_player;

    SDL_GetMouseState(&mouse_x, &mouse_y);
    switch (event->type)
    {
        case SDL_QUIT:
            ui_context->running = 0;
            break;
        case SDL_KEYDOWN:
            if (event->key.keysym.sym == SDLK_ESCAPE)
                ui_context->running = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            ui_context->selected_square = handle_mouse_event(mouse_x, mouse_y, ~(position->occupancy[current_player]));
            render_legal_moves(ui_context->selected_square, position->legal_moves);
            break;
        case SDL_MOUSEBUTTONUP:
            destsquare = handle_mouse_event(mouse_x, mouse_y, position->occupancy[current_player]);
            if (handle_move(position, ui_context->selected_square, destsquare, NULL))  
            {
                // move was made
                ui_context->selected_square = -1;
                return 1;
            }
            // no move was made render and unselect square
            ui_context->selected_square = -1;
            render(position->bitboards);
            break;
    }
    return 0;
}

