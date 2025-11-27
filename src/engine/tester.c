
#include "engine/tester.h"

/*
    -> test how many legal moves are found for a given depth
    calculate legal moves, copy the state, make every move recursively till depth
    and count the moves

    -> for the current position - which is described by the GameContext - count all legal moves till a depth
    -> copy the GameContext*

    typedef struct
    {
        uint64_t bitboards[12];
        uint64_t occupancy[3];
        int game_flags[5];
        Move legal_moves[LEGAL_MOVES_SIZE];
        int running;
        int needs_update;
        int selected_square;
        int game_over;
    } GameContext;

*/ 

int get_nodes(GameContext* game, int depth)
{
    if(depth == 0)
        return 1;

    int num_positions = 0;

    for(int i = 0; i < game->legal_move_count; ++i)
    {
        GameContext next = *game; // copy parent 
        handle_move(next.bitboards, next.legal_moves[i].startsquare, next.legal_moves[i].destsquare, next.game_flags);
        num_positions += get_nodes(&next, depth-1);
    }
    return num_positions;
}
