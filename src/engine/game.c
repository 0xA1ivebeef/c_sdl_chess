
#include "engine/game.h"

// load the game with all assets and everything for the first move
int setup(GameContext* game, SDL_Renderer* renderer)
{
    load_renderer(renderer);
    if(load_pieces_images(renderer) != 0)
        return 1;
    
    load_fen_string(game->bitboards, game->game_flags);
    update_occupancy_bitboards(game->bitboards, game->occupancy);
    load_bitmasks();
    generate_legal_moves(game->game_flags[0], game->bitboards, game->occupancy, game->game_flags, game->legal_moves, 0);
    log_legal_moves(game->legal_moves);
    return 0;
}

// after every move of each player 
void update(GameContext* game)
{
    game->game_flags[0] = !(game->game_flags[0]); 
    update_occupancy_bitboards(game->bitboards, game->occupancy);

    // generate enemy atk_bb
    uint64_t atk_bb = get_attack_bitboard(!(game->game_flags[0]), game->bitboards, game->occupancy); 
    printf("logging atk_bb\n");
    log_bitboard(&atk_bb);
    
    // generate own legal_moves
    memset(game->legal_moves, -1, sizeof(Move) * LEGAL_MOVES_SIZE);
    generate_legal_moves(game->game_flags[0], game->bitboards, game->occupancy, game->game_flags, game->legal_moves, atk_bb); 

    // filter out moves that would put me into check
    filter_moves(game->game_flags[0], game->bitboards, game->occupancy, game->game_flags, game->legal_moves, atk_bb);
    
    // if im in check, only give me moves that move me out of check
    if(is_check(game->game_flags[0], game->bitboards, atk_bb))
    {   
        printf("CHECK!\n");
        if(get_endindex(game->legal_moves) == 0)
            printf("CHECKMATE\n");
    }
    render(game->bitboards);
}

void game_loop(GameContext* game)
{
    SDL_Event event;
    while (game->running)
    {
        if (SDL_WaitEvent(&event))
            handle_event(game, &event);
        
        if (game->needs_update)
        {
            update(game);
            render(game->bitboards);
            game->needs_update = 0;
        }

        SDL_Delay(16);
    }
}

