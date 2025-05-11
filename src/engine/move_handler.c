
#include "engine/move_handler.h"

int is_legal_move(uint64_t* bitboards, int startsquare, int destsquare, int* game_flags, Move* legal_moves)
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

int handle_move(uint64_t* bitboards, int startsquare, int destsquare, Move* legal_moves, int* game_flags)
{
    if(startsquare == -1 || destsquare == -1)
        return 1;

    if(!is_legal_move(bitboards, startsquare, destsquare, game_flags, legal_moves))
        return 1;

    apply_move(bitboards, startsquare, destsquare);
    return 0;
}

