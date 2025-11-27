
#include "engine/game.h"

// load the game with all assets and everything for the first move
int setup(GameContext* game, SDL_Renderer* renderer)
{
    load_renderer(renderer);
    if(load_pieces_images() != 0)
        return 1;
    
    load_fen_string(game->bitboards, game->game_flags);
    update_occupancy_bitboards(game->bitboards, game->occupancy);
    load_bitmasks();
    generate_legal_moves(game->game_flags[0], game->bitboards, game->occupancy, game->game_flags, game->legal_moves, 0);
    game->legal_move_count = get_legal_move_count(game->legal_moves);
    log_legal_moves(game->legal_moves);
    render(game->bitboards);
    return 0;
}

Move* pick_move(Move* legal_moves, int legal_move_count)
{
    int r = rand() % legal_move_count;
    return &legal_moves[r];
}

int opponent_move(GameContext* game)
{
    int c = game->legal_moves;
    Move* m = pick_move(game->legal_moves, c);
 
   if (handle_move(game->bitboards, m->startsquare, m->destsquare, game->legal_moves, game->game_flags))
    {
        return 0;
    }
    return 1;
}

// after every move of each player 
void update(GameContext* game)
{
    game->game_flags[0] = !(game->game_flags[0]); // set which players move it is
    update_occupancy_bitboards(game->bitboards, game->occupancy);

    // generate enemy atk_bb
    uint64_t atk_bb = get_attack_bitboard(!(game->game_flags[0]), game->bitboards, game->occupancy); 
    printf("logging atk_bb\n");
    log_bitboard(&atk_bb);
    
    // generate own legal_moves
    memset(game->legal_moves, -1, sizeof(Move) * LEGAL_MOVES_SIZE);
    generate_legal_moves(game->game_flags[0], game->bitboards, game->occupancy, game->game_flags, game->legal_moves, atk_bb); 

    // filter out moves that would put me into check
    filter_moves(game, atk_bb);
    game->legal_move_count = get_legal_move_count(game->legal_moves);
    
    // if im in check, only give me moves that move me out of check
    if (is_check(game->game_flags[0], game->bitboards, atk_bb))
    {   
        printf("CHECK!\n");
        if (game->legal_move_count == 0)
        {
            printf("CHECKMATE\n");
            game->running = 0;
            exit(0);
        }
    }
    render(game->bitboards);

    // opponent: if its black to move its my move
    if (game->game_flags[0] == 0)
    {
        if (opponent_move(game))
        {
            printf("Unknown error in opponents move!\n");
        }
        update(game);
    }
}

void game_loop(GameContext* game)
{
    SDL_Event event;
    while (game->running)
    {
        if (SDL_WaitEvent(&event))
        {
            handle_event(game, &event); // outsourcing ui/sdl_event_handler.c
        }
        if (game->needs_update) // this logic is only to determine if a move was made or not
        {
            update(game);
            render(game->bitboards);
            game->needs_update = 0;
        }
    }
}

