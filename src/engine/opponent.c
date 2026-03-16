
#include "engine/opponent.h"

static int get_opening_move = 1;

Move opponent_move(Position* pos, LegalMoves* lm)
{
    // assuming opening book moves are legal
    if (get_opening_move)
    {
        Move opening_move = get_random_opening_move(pos->hash);
        if (!opening_move)
        {
            printf("opening is finished\n");
            get_opening_move = 0; // dont look for openings anymore
        }
        else
        {
            handle_move(pos, lm, opening_move);
            return opening_move;
        }
    }

    Move m = search_root(pos, 5);
    if (!m)
        return 0;
        
    printf("picked move: %s%s\n", square_to_notation(move_from(m)), square_to_notation(move_to(m)));

    handle_move(pos, lm, m); 
    return m;
}

