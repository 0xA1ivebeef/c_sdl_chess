
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
    // printf("LOGGING LEGAL_MOVES:\n");
    // log_legal_moves(pos->legal_moves, pos->legal_move_count);
        
    filter_moves(pos);
    // printf("LOGGING VALID MOVES\n");
    log_legal_moves(pos->legal_moves, pos->legal_move_count);

    if (is_check(get_king_sq(pos, pos->player), get_attack_bb(pos, !pos->player)))
    {   
        // printf("CHECK!\n");
        if (pos->legal_move_count == 0)
        {
            // printf("CHECKMATE\n");
            // ui->game_over = 1;  
        }
    }
    else if (pos->legal_move_count == 0)
    {
        // printf("STALEMATE\n");
        // ui->game_over = 1;  
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

    perft(pos, 3);
    return;

    while (ui->running)
    {
        while(SDL_PollEvent(&e))
        {
            if (ui->game_over)
                continue; // dont take inputs
            
            // players move
            if (handle_event(app, pos, ui, &e))
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

