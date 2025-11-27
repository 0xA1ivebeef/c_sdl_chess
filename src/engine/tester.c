
#include "engine/tester.h"

int get_nodes(Position* position, int depth)
{
    if(depth == 0)
        return 1;

    int num_positions = 0;

    for(int i = 0; i < position->legal_move_count; ++i) 
    {
        Position next = *position; // copy parent 
        handle_move(&next, position->legal_moves[i].startsquare, position->legal_moves[i].destsquare);
        num_positions += get_nodes(&next, depth-1);
    }
    return num_positions;
}
