
#include "engine/opponent.h"

static int get_opening_move = 1;
static const double time_limit = 3.0;

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
            // TODO if its castling handle this, castling is given as e1h1 or e1a1 same for black
            handle_move(pos, lm, opening_move);
            return opening_move;
        }
    }

    double start =  get_time_seconds();
    Move m = iterative_deepening(pos, time_limit);
    double end = get_time_seconds();

    printf("opponent thought for %f sec\n", end-start);
    printf("picked move:\n");
    log_move(m);

    if (!m)
        return 0;
        
    handle_move(pos, lm, m); 
    return m;
}

