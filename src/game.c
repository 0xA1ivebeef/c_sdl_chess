
#include "include/game.h"

// load the game with all assets and everything for the first move
int setup(uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags, SDL_Renderer* renderer, move* legal_moves)
{
    load_renderer(renderer);
    if(load_pieces_images(renderer) != 0)
        exit(1);
    
    load_fen_string(bitboards, game_flags);
    update_occupancy_bitboards(bitboards, occupancy_bitboards);
    load_bitmasks();
    generate_legal_moves(game_flags[0], bitboards, occupancy_bitboards, game_flags, legal_moves, 0);
    log_legal_moves(legal_moves);
}

int is_double_pawn_push(uint64_t* bitboards, move* this_move)
{
    int bb_index = get_bitboard_index(bitboards, this_move->startsquare);
    if(!(bb_index == 0 || bb_index == 6))
        return 0;
    return ((max(this_move->startsquare, this_move->destsquare) - min(this_move->startsquare, this_move->destsquare)) == 16);
}

// castle rights = KQkq white - black
void update_castle_rights(uint64_t* bitboards, move* this_move, int* game_flags)
{
    int index = get_bitboard_index(bitboards, this_move->startsquare);
    switch(index)
    {
        case 3:
        case 9:
            // remove castle rights for one side
            switch(this_move->startsquare)
            {
                case 0:
                    game_flags[1] &= 14; // keep all but black queenside
                    break;
                case 7:
                    game_flags[1] &= 13; // keep all but black kingside
                    break;
                case 56:
                    game_flags[1] &= 11; // white qs
                    break;
                case 63:
                    game_flags[1] &= 7; // white kingside
                    break;
                default:
                    break;
            }
            break;
        case 5:
        case 11:
            // remove castle rights for player
            game_flags[1] &= (this_move->startsquare == 60) ? 3 : 12;
            break;
        default:
            return;
    }
}

int is_legal_move(uint64_t* bitboards, int startsquare, int destsquare, int* game_flags, move* legal_moves)
{
    for(int i = 0; i < LEGAL_MOVES_SIZE; ++i)
    {
        if((legal_moves[i].startsquare != startsquare) || (legal_moves[i].destsquare != destsquare))
            continue;
     
        switch(legal_moves[i].flags)
        {
            case 1:
                handle_castling(bitboards, &legal_moves[i], game_flags);
                break;
            case 2:
                handle_enpassant(bitboards, game_flags);
                break;
            default:
                break;
        }
        
        if(is_double_pawn_push(bitboards, &legal_moves[i]))
            handle_double_pawn_push(&legal_moves[i], game_flags);
        else
            game_flags[2] = -1;
        
        update_castle_rights(bitboards, &legal_moves[i], game_flags);
        return 1;
    }
    return 0;
}

int handle_move(uint64_t* bitboards, int startsquare, int destsquare, move* legal_moves, int* game_flags)
{
    if(startsquare == -1 || destsquare == -1)
        return 1;

    if(!is_legal_move(bitboards, startsquare, destsquare, game_flags, legal_moves))
        return 1;

    apply_move(bitboards, startsquare, destsquare);
    return 0;
}

// only keep moves that are not check after making them on the board
void filter_moves(int p, uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags, move* legal_moves, uint64_t attack_bitboard)
{
    move valid_moves[LEGAL_MOVES_SIZE] = {0};
    int endindex = get_endindex(legal_moves);
    int c = 0;
    uint64_t bb_copy[12] = {0};
    uint64_t occ_bb_copy[3] = {0};
    for (int i = 0; i < endindex; i++) 
    {
        // copy, drop method 
        get_bb_copy(bitboards, occupancy_bitboards, bb_copy, occ_bb_copy);
        apply_move(bb_copy, legal_moves[i].startsquare, legal_moves[i].destsquare);
        update_occupancy_bitboards(bb_copy, occ_bb_copy);
        attack_bitboard = get_attack_bitboard(!p, bb_copy, occ_bb_copy, game_flags);
        if (!is_check(p, bb_copy, attack_bitboard)) 
        {
            valid_moves[c] = legal_moves[i];
            ++c;
        }
    }
    memcpy(legal_moves, valid_moves, sizeof(move)*LEGAL_MOVES_SIZE);
    printf("valid moves: \n"); 
    log_legal_moves(legal_moves);
}

// after every move of each player 
void update(uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags, move* legal_moves)
{
    game_flags[0] = !game_flags[0]; 
    update_occupancy_bitboards(bitboards, occupancy_bitboards);

    // generate enemy atk_bb
    uint64_t atk_bb = get_attack_bitboard(!game_flags[0], bitboards, occupancy_bitboards, game_flags);
    printf("logging atk_bb\n");
    log_bitboard(&atk_bb);
    
    // generate own legal_moves
    memset(legal_moves, -1, sizeof(move) * LEGAL_MOVES_SIZE);
    generate_legal_moves(game_flags[0], bitboards, occupancy_bitboards, game_flags, legal_moves, atk_bb); 

    // filter out moves that would put me into check
    filter_moves(game_flags[0], bitboards, occupancy_bitboards, game_flags, legal_moves, atk_bb);
    
    // if im in check, only give me moves that move me out of check
    if(is_check(game_flags[0], bitboards, atk_bb))
    {   
        printf("CHECK!\n");
        if(get_endindex(legal_moves) == 0)
            printf("CHECKMATE\n");
    }
    render(bitboards);
}


void handle_event(GameContext* game, SDL_Event* event)
{
    int mouse_x = 0; 
    int mouse_y = 0;
    int destsquare = -1;

    SDL_GetMouseState(&mouse_x, &mouse_y);
    switch (e.type)
    {
        case SDL_QUIT:
            game->running = 0;
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_ESCAPE)
                return;
                break;
        case SDL_MOUSEBUTTONDOWN:
            game->selected_square = handle_mouse_event(mouse_x, mouse_y, ~(game->occupancy[game->game_flags[0]]));
            render_legal_moves(game->selected_square, game->legal_moves);
            break;
        case SDL_MOUSEBUTTONUP:
            destsquare = handle_mouse_event(mouse_x, mouse_y, game->occupancy[game->game_flags[0]]);
            if (handle_move(game))
                game->needs_update = 1;
            game->selected_square = -1;
            break;
    }
}

void game_loop(GameContext* game)
{
    SDL_Event event;
    while (game->running)
    {
        if (SDL_waitEvent(&event))
            handle_event(game, &event);
        
        if (game->needs_update)
        {
            update(game);
            render(game);
            game->needs_update = 0;
        }

        SDL_Delay(16);
    }
}

