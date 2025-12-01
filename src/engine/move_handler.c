
#include "engine/move_handler.h"

void handle_special_move(Position* position, Move* this_move)
{
    switch(this_move->flags)
    {
        case 1:
            handle_castling(position->bitboards, this_move, position->game_flags);
            break;
        case 2:
            handle_enpassant(position->bitboards, position->game_flags);
            break;
        default:
            break;
    }
    if(is_double_pawn_push(position->bitboards, &position->legal_moves[i]))
        handle_double_pawn_push(&position->legal_moves[i], position->game_flags);
    else
        position->game_flags[2] = -1;
        
    update_castle_rights(position->bitboards, &position->legal_moves[i], position->game_flags);
}

void apply_move(Position* position, Move* this_move)
{
    int startsquare = this_move.startsquare;
    int destsquare = this_move.destsquare;

    printf("APPLY MOVE: %d, %d, flag: %d\n", startsquare, destsquare, this_move->flags);

    int bitboard_index = get_bitboard_index(position->bitboards, startsquare);
    int dest_bitboard_index = get_bitboard_index(position->bitboards, destsquare);

    position->bitboards[bitboard_index] &= ~(1ULL << startsquare); // delete piece on startsquare
    position->bitboards[bitboard_index] |= (1ULL << destsquare); // set new piece on destsquare    
    // TODO: special move handling 
    handle_special_moves(position, this_move);

    if(dest_bitboard_index == -1) // dest square empty
        return;

    position->bitboards[dest_bitboard_index] &= ~(1ULL << destsquare); // delete piece on destsquare
}

Move* is_legal_move(Position* position, int startsquare, int destsquare)
{
    printf("validating move: %d, %d\n", startsquare, destsquare);
    for(int i = 0; i < position->legal_move_count; ++i)
    {
        if((position->legal_moves[i].startsquare == startsquare) && (position->legal_moves[i].destsquare == destsquare))
        {
            return &position->legal_moves[i];
        }
    }
    return NULL;
}

/* maybe this is not needed 
Move* get_legal_move_from_squares(Position* position, int startsquare, int destsquare)
{
    for (int i = 0; i < position->legal_move_count; ++i)
    {
        if ((startsquare == position->legal_moves[i].startsquare) && (destsquare == position->legal_moves[i].destsquare))
        {
            return &position->legal_moves[i];
        }
    }
    return NULL;
}
*/

int handle_move(Position* position, int startsquare, int destsquare)
{
    if(startsquare == -1 || destsquare == -1)
        return 0;

    printf("HANDLE MOVE: %d, %d\n", startsquare, destsquare);
    Move* this_move = is_legal_move(position, startsquare, destsquare);
    if (!this_move)
    {
        printf("MOVE HANDLER: illegal move\n");
        return 0;
    }

    printf("MOVE HANDLER: legal move, applying\n"); 
    apply_move(position, this_move);
    return 1;
}

