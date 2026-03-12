
#include "ui/sdl_event_handler.h"

// TODO refactor

uint8_t select_piece(int mx, int my, uint64_t occ)
{
    if (mx < 0 || mx > WINDOW_WIDTH || my < 0 || my > WINDOW_HEIGHT)
        return INVALID_SQUARE;

    int sq = my/TILESIZE * 8 + mx/TILESIZE;
    if (occ & (1ULL << sq)) 
        return sq;

    return INVALID_SQUARE;
}

int handle_event(AppContext* app, Position* pos, UIContext* ui, SDL_Event* e, LegalMoves* lm)
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
        {
            int sq = select_piece(e->button.x, e->button.y, pos->occ[pos->player]);
            if (sq != INVALID_SQUARE) 
            {
                ui->selected_square = sq;
                render_legal_moves(app, pos, lm, ui->selected_square);
            }
            break;
        }

        case SDL_MOUSEBUTTONUP: 
        {
            if (ui->selected_square == INVALID_SQUARE)
                break; // nothing selected

            uint8_t dest = select_piece(
                    e->button.x, e->button.y, ~pos->occ[pos->player]);
            if (dest == INVALID_SQUARE) 
            {
                ui->selected_square = INVALID_SQUARE; // unselect
                render(app, pos->bb);
                break;
            }

            Move tmp = (ui->selected_square << 6) | dest;

            // handle promotion if needed
            if (is_pawn_promotion(pos, tmp)) 
                choose_promotion_move(&tmp);

            if (handle_move(pos, lm, tmp)) 
            {
                ui->selected_square = INVALID_SQUARE;
                return 1; // move applied
            }

            // illegal move, just unselect and redraw
            ui->selected_square = INVALID_SQUARE;
            render(app, pos->bb);
            break;
        }
    }

    return 0;
}
