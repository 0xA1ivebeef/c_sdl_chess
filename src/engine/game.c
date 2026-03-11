
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
        if (is_check(pos, pos->player))
        {
            ui->running = 0;
            printf("CHECKMATE\n");
        }
        else
        {
            ui->running = 0;
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
    int ai_move_pending = 0;
    while (ui->running)
    {
        if (SDL_WaitEventTimeout(&e, 16))
        {
            if (handle_event(app, pos, ui, &e))
            {  
                get_zobrist_hash(pos);
                ai_move_pending = 1;

                update(pos, ui);
                render(app, pos->bb);
            }
        }
        
        if (pos->player == BLACK && ai_move_pending)
        {
            ai_move_pending = 0;

            if (opponent_move(pos) <= 0)
            {
                printf("OPPONENT MOVE FAILED\n");
            }
            update(pos, ui);
            render(app, pos->bb);
        }
    }
}

