
#include "engine/game.h"

#define AI_COLOR BLACK

int get_king_square(uint64_t king_bitboard)
{
    int i = 0;
    while(king_bitboard)
    {
        king_bitboard >>= 1;
        ++i;
    }
    return i - 1;
}

int setup(Position* position)
{
    // do everything as normal since starting position can be different starting gamestate is loaded (from fen string)
    
    printf("SETUP IS CALLED\n");
   if (load_pieces_images() != 0)
    {
        return 1;
    }

    load_fen_string(position);
    update_occupancy_bitboards(position->bitboards, position->occupancy);

    load_bitmasks(); // external function for bitmask_loader.c

    // set position->king_square[2]
    position->king_square[BLACK] = get_king_square(position->bitboards[BLACK_KING]);
    position->king_square[WHITE] = get_king_square(position->bitboards[WHITE_KING]);

    generate_attack_bitboards(position); // attack bitboards first since they are used in move generation
    
    printf("SETUP: logging attack bitboards: \n");
    printf("BLACK:\n");
    log_bitboard(&position->attack_bitboards[BLACK]);
    printf("\n");
    
    printf("WHITE:\n");
    log_bitboard(&position->attack_bitboards[WHITE]);
    printf("\n");
                                         
    generate_legal_moves(position);
    filter_moves(position); 

    log_legal_moves(position->legal_moves);

    render(position->bitboards);

    log_gamestate(position);

    return 0;
}

void update(Position* position, UIContext* ui_context)
{
    printf("UPDATE IS CALLED\n");

	position->current_player ^= 1;
    update_occupancy_bitboards(position->bitboards, position->occupancy);

    generate_attack_bitboards(position);
 
    // clear and generate legal moves filter illegal moves
    memset(position->legal_moves, -1, sizeof(Move) * LEGAL_MOVES_SIZE); 
    generate_legal_moves(position); 
    filter_moves(position);
    log_legal_moves(position->legal_moves);

    // check
    if (ui_context)
    {
        if (is_check(position, 0))
        {   
            printf("CHECK!\n");
            if (position->legal_move_count == 0) 
            {
                printf("CHECKMATE\n");
                ui_context->game_over = 1;  
            }
        }
        else if (position->legal_move_count == 0)
        {
            printf("STALEMATE\n");
            ui_context->game_over = 1;  
        }
    }

	if (position->halfmove_clock == 100)
	{
		printf("50 move draw\n");
        ui_context->game_over = 1;  
	}

    log_gamestate(position);
}

void perft_test(Position* position)
{
    int expected[6] = {20, 400, 8902, 197281, 4865609, 119060324};
    
    FILE* log_file = fopen("perft.log", "w");
    for (int i = 0; i < 8; ++i)
    {
        Position copy = *position;
        int nodes = get_nodes(&copy, i);
        
        printf("depth %d finished\n", i);
        fprintf(log_file, "Test %d, expected: %d, actual: %d\n", i, expected[i], nodes);
    }
    fclose(log_file);
}

void game_loop(Position* position, UIContext* ui_context)
{
    SDL_Event event;

    perft_test(position);
    return;

    while (ui_context->running)
    {
        SDL_WaitEvent(&event); 
        
        if (ui_context->game_over)
        {
            continue; // dont take inputs
        }

        // players move
        if (0) // handle_event(position, ui_context, &event) == 1)
        {
            ui_context->needs_update = 1;
            printf("GAMELOOP: move was made\n");
        }

        // ai move 
        if (0 == 1) // position->game_flags[0] == AI_COLOR)
        {
            opponent_move(position);
            ui_context->needs_update = 1;
            printf("GAMELOOP: ai move was made\n");
        }
    
        // update
        if (ui_context->needs_update) 
        {
            printf("UPDATE AND RENDER\n");
            update(position, ui_context);
            render(position->bitboards);
            ui_context->needs_update = 0;
        }
    }
}
                                                                 
