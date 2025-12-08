
#include "engine/tester.h"

void update_child(Position* child)
{
    // no player context:
    update_occupancy_bitboards(child->bitboards, child->occupancy);
    generate_attack_bitboards(child);

    // flip current player
    child->current_player ^= 1;
}

int get_nodes(Position* position, int depth)
{
    if (depth == 0)
        return 1;

    // generate legal moves for this position
    generate_legal_moves(position);      // fills position->legal_moves + count (pseudo-legal)
    filter_moves(position);        // removes illegal moves (your function)

    // No moves = dead end
    if (position->legal_move_count == 0)
        return 0;

    int nodes = 0;

    for (int i = 0; i < position->legal_move_count; ++i)
    {
        Position child = *position;   // copy parent position
        Move m = position->legal_moves[i];

        apply_move(&child, &m); // implicit: handle_special_move()
        
        update_child(&child);
        
        // recurse
        nodes += get_nodes(&child, depth - 1);
    }

    return nodes;
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
