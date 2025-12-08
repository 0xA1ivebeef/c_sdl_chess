
#include "engine/perft.h"

void update_child(Position* child)
{
    // no player context:
    update_occupancy_bitboards(child->bitboards, child->occupancy);
    generate_attack_bitboards(child);

    // flip current player
    child->current_player ^= 1;
}

// given max depth return the total number of nodes to that depth
int get_nodes(Position* position, int depth) 
{
    if (depth == 0)
        return 1;

    generate_legal_moves(position);      
    filter_moves(position);        

    // no moves = dead end
    if (position->legal_move_count == 0)
        return 0;

    int nodes = 0; // this is reset after each function call !
    for (int i = 0; i < position->legal_move_count; ++i) 
    {
        // for all legal moves in the current position
        Position child = *position;   // copy parent position
        Move m = position->legal_moves[i]; // pick out the move

        apply_move(&child, &m);  // apply the move to the copy
        
        update_child(&child); // update the copy position
         
        // recurse
        nodes += get_nodes(&child, depth - 1); 
        // generate legal moves, create another copy do all 
        // again till depth is 0 then the node is a leaf and is counted as 1
    }

    return nodes;
}

void perft_divide(Position* position, int depth)
{
    /* 
    print nodes after each move of depth 1
    nodes of depth 1 are legal moves

    for each legal move
    calc its line to given depth, output the nodes of the line

    for each legal move at highest depth
    print that move and do get_nodes for the position after that move till the depth 
    print out the nodes after each move

    so basically treat each move as a new get_nodes() with depth -1 and print each move and its nodes
    */
    
    for (int i = 0; i < position->legal_move_count; ++i)
    {
        Position copy = *position;
        Move m = position->legal_moves[i];
        apply_move(&copy, &m);
        update_child(&copy);
        int nodes = get_nodes(&copy, depth-1);
        printf("move %s, %s: NODES: %d\n", square_to_notation(m.startsquare), square_to_notation(m.destsquare), nodes);
    }
}

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
