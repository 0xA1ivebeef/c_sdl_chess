
#include "engine/game.h"

#define AI_COLOR BLACK

void position_init(Position* pos, LegalMoves* lm)
{
    load_fen_string(pos, 0);

    generate_occ(pos);
    generate_piece_on_sq(pos);
	
    generate_legal_moves(pos, lm); 
    filter_moves(pos, lm);

    pos->hash = get_zobrist_hash(pos);

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
            ui->game_over = 1;
            printf("CHECKMATE\n");
        }
        else
        {
            ui->game_over = 1;
            printf("STALEMATE"); 
        }
    }

	if (pos->halfmove == 100)
	{
		printf("50 move draw\n");
        ui->game_over = 1;  
	}

    if (insufficient_material(pos))
    {
        printf("insufficient material draw\n");
        ui->game_over = 1;
    }
    
    log_gamestate(pos);
}

void game_loop(AppContext* app, Position* pos, UIContext* ui, LegalMoves* lm)
{
    SDL_Event e;
    Move last_move;

    // search_test(pos); 
    
    // perft(pos, 5); 
    perft_suite(pos);
    // perft_captures_suite(pos);

    return;

    int ai_move_pending = 0;
    while (ui->running)
    {
        if (SDL_WaitEventTimeout(&e, 16))
        {
            if (ui->game_over)
            {
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                    ui->running = 0;
                continue;
            }

            if (handle_event(app, pos, ui, &e, lm, &last_move))
            {  
                update(pos, ui, lm);
                render(app, pos->bb, last_move);
                ai_move_pending = 1;
            }
        }
 
        if (pos->player == BLACK && ai_move_pending && !ui->game_over)
        {
            last_move = opponent_move(pos, lm);
            if (last_move == 0)
            {
                fprintf(stderr, "OPPONENT MOVE FAILED\n");
                exit(EXIT_FAILURE);
            }

            update(pos, ui, lm);
            render(app, pos->bb, last_move);
            ai_move_pending = 0;
        }
    }
}

