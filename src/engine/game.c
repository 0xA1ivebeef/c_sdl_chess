
#include "engine/game.h"

#define AI_COLOR BLACK

void position_init(Position* pos)
{
    printf("SETUP IS CALLED\n");

    load_fen_string(pos);
    printf("enpassant after fen string: %d\n", pos->enpassant);

    update_occ(pos->bb, pos->occ);

    load_bitmasks(); // external function for bitmask_loader.c

    // set pos->king_sq[2]
    pos->king_sq[BLACK] = get_king_square(pos->bb[BLACK_KING]);
    pos->king_sq[WHITE] = get_king_square(pos->bb[WHITE_KING]);
	
	printf("king sqs after fen load: %d, %d\n", pos->king_sq[0], pos->king_sq[1]);

    generate_legal_moves(pos);
    filter_moves(pos); 

    log_legal_moves(pos->legal_moves);

    log_gamestate(pos);
}

void update_gamestate(Position* pos)
{
    /* update pos after a move was made (current_player already switched 
     by engine/move_handler:apply_move()) */
 
    // clear and generate legal moves filter illegal moves
    generate_legal_moves(pos); 
    filter_moves(pos);
}

void update(Position* pos, UIContext* ui)
{
    update_gamestate(pos);
    log_legal_moves(pos->legal_moves);

    int king_sq = pos->king_sq[pos->player];
    uint64_t enemy_attack_bb = get_attack_bb(pos);
        
    // check
    if (ui)
    {
        if (is_check(king_sq, enemy_attack_bb))
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
    }

    // TODO implement
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
        SDL_WaitEvent(&e); 
        
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
            printf("UPDATE AND RENDER\n");
            update(pos, ui);
            render(app, pos->bb);
            ui->needs_update = 0;
        }
    }
}
                                                                 
