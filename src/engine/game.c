
#include "engine/game.h"

#define AI_COLOR BLACK

void position_init(Position* pos)
{
    load_fen_string(pos);
    update_occ(pos);
	
    generate_legal_moves(pos);
    filter_moves(pos); 

    log_gamestate(pos);
}

void update(Position* pos, UIContext* ui)
{
    pos->player ^= 1;
    update_occ(pos);
    
    generate_legal_moves(pos); 
    // filter_moves(pos);

    if (is_check(get_king_sq(pos, pos->player), get_attack_bb(pos, !pos->player)))
    {   
        printf("CHECK!\n");
        if (pos->legal_move_count == 0) 
        {
            printf("CHECKMATE\n");
            ui->game_over = 1;  
        }
    }
    else if (pos->legal_move_count == 0)
    {
        printf("STALEMATE\n");
        ui->game_over = 1;  
    }

    // TODO implement draws
	if (pos->halfmove == 100)
	{
		printf("50 move draw\n");
        ui->game_over = 1;  
	}

    log_gamestate(pos);
}

void perft(Position* pos)
{
	for (int i = 2; i < 5; ++i)
	{
		perft_divide(pos, i);
		printf("\n\n\n");
	}
}

void game_loop(AppContext* app, Position* pos, UIContext* ui)
{
    SDL_Event e;

    // perft(pos); return;

    while (ui->running)
    {
        SDL_PollEvent(&e); 
        
        if (ui->game_over)
        {
            continue; // dont take inputs
        }

        // players move
        if (handle_event(app, pos, ui, &e) == 1)
        {
            ui->needs_update = 1;
            printf("GAMELOOP: move was made\n");
        }

        // ai move 
        if (0) // pos->current_player == AI_COLOR)
        {
            opponent_move(pos);
            ui->needs_update = 1;
            printf("GAMELOOP: ai move was made\n");
        }
    
        // update
        if (ui->needs_update) 
        {
            update(pos, ui);
            render(app, pos->bb);
            ui->needs_update = 0;
        }
    }
}
                                                                 
