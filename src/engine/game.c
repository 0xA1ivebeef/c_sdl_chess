
#include "engine/game.h"

#define AI_COLOR BLACK

void position_init(Position* pos, LegalMoves* lm)
{
    load_fen_string(pos, 0);
    update_occ(pos);
	
    generate_legal_moves(pos, lm); 

    pos->zobrist_hash = get_zobrist_hash(pos);

    log_gamestate(pos);
}

void update(Position* pos, UIContext* ui, LegalMoves* lm)
{
    generate_legal_moves(pos, lm);
    filter_moves(pos, lm);

    if (lm->count == 0)
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

	if (pos->halfmove == 100)
	{
		printf("50 move draw\n");
        ui->game_over = 1;  
	}

    log_gamestate(pos);
}

void game_loop(AppContext* app, Position* pos, UIContext* ui, LegalMoves* lm)
{
    SDL_Event e;

    // int ai_move_pending = 0;

    // perft(pos, lm, 3);
    // full_perft_test(pos, lm);
    // return;

    while (ui->running)
    {
        if (SDL_WaitEventTimeout(&e, 16))
        {
            if (handle_event(app, pos, ui, &e, lm))
            {  
                // ai_move_pending = 1;

                update(pos, ui, lm);
                render(app, pos->bb);
            }
        }
 
        /*
        if (pos->player == BLACK && ai_move_pending)
        {
            ai_move_pending = 0;

            if (opponent_move(pos, lm) <= 0)
                printf("OPPONENT MOVE FAILED\n");

            update(pos, ui, lm);
            render(app, pos->bb);
        }
        */
    }
}

