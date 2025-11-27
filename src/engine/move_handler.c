
#include "engine/move_handler.h"

int is_pawn_promotion(uint64_t* bitboards, int startsquare, int destsquare)
{
    int index = get_bitboard_index(bitboards, startsquare);
    if(!(index == 0 || index == 6))
        return 0;
    return ((destsquare >= 0 && destsquare <= 7) || (destsquare >= 56 && destsquare <= 63));
}

void handle_pawn_promotion(uint64_t* bitboards, int startsquare, int destsquare)
{
    int ds_i = get_bitboard_index(bitboards, destsquare);
    bitboards[ds_i] &= ~(1ULL << destsquare);
    if(destsquare >= 0 && destsquare <= 7)
    {
        bitboards[6] &= ~(1ULL << startsquare); // delete pawn
        bitboards[10] |= (1ULL << destsquare); // place queen
    }   
    else
    { 
        bitboards[0] &= ~(1ULL << startsquare); // delete pawn
        bitboards[4] |= (1ULL << destsquare); // place queen
    }
}

// return bitboard index of destination square, -1 if empty
void apply_move(uint64_t* bitboards, int startsquare, int destsquare)
{
    int bitboard_index = get_bitboard_index(bitboards, startsquare);
    int dest_bitboard_index = get_bitboard_index(bitboards, destsquare);

    if(is_pawn_promotion(bitboards, startsquare, destsquare))
    {
        handle_pawn_promotion(bitboards, startsquare, destsquare);
        return;
    }
    
    bitboards[bitboard_index] &= ~(1ULL << startsquare); // delete piece on startsquare
    bitboards[bitboard_index] |= (1ULL << destsquare); // set new piece on destsquare    

    if(dest_bitboard_index == -1) // dest square empty
        return;

    bitboards[dest_bitboard_index] &= ~(1ULL << destsquare); // delete piece on destsquare
}

int is_legal_move(GameContext* game, int startsquare, int destsquare)
{
    for(int i = 0; i < LEGAL_MOVES_SIZE; ++i)
    {
        if((game->legal_moves[i].startsquare != startsquare) || (game->legal_moves[i].destsquare != destsquare))
            continue;
     
        switch(game->legal_moves[i].flags)
        {
            case 1:
                handle_castling(game->bitboards, &game->legal_moves[i], game->game_flags);
                break;
            case 2:
                handle_enpassant(game->bitboards, game->game_flags);
                break;
            default:
                break;
        }
        
        if(is_double_pawn_push(game-> bitboards, &game->legal_moves[i]))
            handle_double_pawn_push(&game->legal_moves[i], game->game_flags);
        else
            game->game_flags[2] = -1;
        
        update_castle_rights(game->bitboards, &game->legal_moves[i], game->game_flags);
        return 1;
    }
    return 0;
}


int handle_move(GameContext* game, int startsquare, int destsquare)
{
    if(startsquare == -1 || destsquare == -1)
        return 0;

    if(!is_legal_move(game, startsquare, destsquare))
        return 0;

    apply_move(game->bitboards, startsquare, destsquare);
    return 1;
}

