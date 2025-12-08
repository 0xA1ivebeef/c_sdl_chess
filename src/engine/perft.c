
#include "engine/perft.h"

int get_nodes(Position* position, int depth) 
{
    if (depth == 1) 
        return position->legal_move_count;

    if (position->legal_move_count == 0)
        return 0;

    int nodes = 0;
    for (int i = 0; i < position->legal_move_count; ++i) 
    {
        Undo undo = {0};
        Move m = position->legal_moves[i];

        apply_move(position, &m, &undo);
        update_gamestate(position);

        nodes += get_nodes(position, depth - 1);

        undo_move(position, &m, &undo);
    }
    return nodes;
}

/*
// given max depth return the total number of nodes to that depth
int get_nodes(Position* position, int depth) 
{
    if (depth == 0)
        return 1;

    // no moves = dead end
    if (position->legal_move_count == 0)
        return 0;

    int nodes = 0; // this is reset after each function call !
    for (int i = 0; i < position->legal_move_count; ++i) 
    {
        // for all legal moves in the current position
        Position child = *position;   // copy parent position
        Move m = position->legal_moves[i]; // pick out the move

        // apply the move to the copy and switch current_player
        apply_move(&child, &m);  
        update_gamestate(&child);

        nodes += get_nodes(&child, depth - 1); 
        // generate legal moves, create another copy do all 
        // again till depth is 0 then the node is a leaf and is counted as 1
    }

    return nodes;
}
*/

/*

void perft_divide(Position* position, int depth)
{
    int res = 0;
    printf("starting perft divide at depth %d: \n", depth);
    for (int i = 0; i < position->legal_move_count; ++i)
    {
        Position copy = *position;
        Move m = position->legal_moves[i];
        apply_move(&copy, &m);
        update_gamestate(&copy);
        int nodes = get_nodes(&copy, depth-1);
        res += nodes;
        printf("%s%s: %d\n", square_to_notation(m.startsquare), 
                square_to_notation(m.destsquare), nodes);
    }
    printf("Nodes searched: %d\n", res);
}

*/

/* second approach

    for (int i = 0; i < position->legal_move_count; ++i)
    {
        Move m = position->legal_moves[i];
        Undo undo;

        apply_move(position, &m, &undo);

        update_child(position);

        nodes += get_nodes(position, depth - 1);

        undo_move(position, &m, &undo);
    }

    return nodes;
}

*/
