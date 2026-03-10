
#include "engine/game.h"

#define AI_COLOR BLACK

void position_init(Position* pos)
{
    load_fen_string(pos);
    update_occ(pos);
	
    generate_legal_moves(pos); 
    log_gamestate(pos);
}

void update(Position* pos, UIContext* ui)
{
    generate_legal_moves(pos);
    filter_moves(pos);

    if (pos->legal_move_count == 0)
    {
        if (is_check(pos))
        {
            ui->game_over = 1;
            printf("CHECKMATE\n");
        }
        else
        {
            ui->game_over = 1;
            printf("STALEMATE"); 
        }
    }

    // TODO implement draws
	if (pos->halfmove == 100)
	{
		// printf("50 move draw\n");
        // ui->game_over = 1;  
	}

    log_gamestate(pos);
}

void game_loop(AppContext* app, Position* pos, UIContext* ui)
{
    SDL_Event e;

    // perft(pos, 6);

    while (ui->running)
    {
        while (SDL_PollEvent(&e))
        {
            if (pos->player == WHITE)
            {
                if (handle_event(app, pos, ui, &e))
                    ui->needs_update = 1;
            }
        }

        if (pos->player == BLACK && !ui->game_over && !ui->needs_update)
        {
            if (opponent_move(pos) != -1)
                ui->needs_update = 1;
        }

        if (ui->needs_update)
        {
            update(pos, ui);
            render(app, pos->bb);
            ui->needs_update = 0;
        }
    }
}

