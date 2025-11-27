
#include "engine/game.h"

#define AI_COLOR BLACK

int setup(Position* position)
{
   if (load_pieces_images() != 0)
    {
        return 1;
    }

    load_fen_string(position->bitboards, position->game_flags);
    update_occupancy_bitboards(position->bitboards, position->occupancy);

    load_bitmasks();

    generate_legal_moves(position);
    generate_attack_bitboards(position);

    render(position->bitboards);

    return 0;
}

void update(Position* position, UIContext* ui_context)
{
    int current_player = position->game_flags[0];
    position->game_flags[0] = !current_player;

    update_occupancy_bitboards(position->bitboards, position->occupancy);

    // attack bitboards
    generate_attack_bitboards(position);
                                                                                                              
    // legal_moves
    memset(position->legal_moves, -1, sizeof(Move) * LEGAL_MOVES_SIZE); 
    generate_legal_moves(position); 
    filter_moves(position);
    position->legal_move_count = get_legal_move_count(position->legal_moves);
    
    // check
    if (is_check(position->game_flags[0], position->bitboards, position->attack_bitboards[!current_player])) 
    {   
        printf("CHECK!\n");
        if (position->legal_move_count == 0) 
        {
            printf("CHECKMATE\n");
            ui_context->running = 0; 
            exit(0);
        }
    }
}

void game_loop(Position* position, UIContext* ui_context)
{
    SDL_Event event;
    while (ui_context->running)
    {
        SDL_WaitEvent(&event); 
        
        // players move
        if (handle_event(position, ui_context, &event))
            ui_context->needs_update = 1;
    
        // ai move 
        if (position->game_flags[0] == AI_COLOR)
        {
            opponent_move(position);
            ui_context->needs_update = 1;
        }
    
        // update
        if (ui_context->needs_update) 
        {
            update(position, ui_context);
            render(position->bitboards);
            ui_context->needs_update = 0;
        }
    }
}

