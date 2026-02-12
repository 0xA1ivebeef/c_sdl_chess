
#include "ui/sdl_event_handler.h"

int handle_event(AppContext* app, Position* pos, UIContext* ui, 
        SDL_Event* e)
{
    switch (e->type)
    {
        case SDL_QUIT:
            ui->running = 0;
            break;
        case SDL_KEYDOWN:
            if (e->key.keysym.sym == SDLK_ESCAPE)
                ui->running = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            ui->selected_square = handle_mouse_event(
                    e->button.x, e->button.y, ~(pos->occ[pos->player]));
            render_legal_moves(app, pos, ui->selected_square);
            break;
        case SDL_MOUSEBUTTONUP:
            int ds = handle_mouse_event(e->button.x, e->button.y, 
                    pos->occ[pos->player]);
            if (handle_move(pos, ui->selected_square, ds))  
            {
                // move was made
                ui->selected_square = -1;
                return 1;
            }
            // no move was made render and unselect square
            ui->selected_square = -1;
            render(app, pos->bb);
            break;
    }
    return 0;
}

