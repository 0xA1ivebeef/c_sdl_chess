
#include "engine/move_handler.h"

// called after every move (through apply_move() !!!)
void handle_special_move(Position* position, Move* this_move)
{
    printf("handle_special_move called\n");
    switch (this_move->flags)
    {
        case 1:
            handle_castling(position->bitboards, this_move, &position->castle_rights);
            break;
        case 2:
            handle_enpassant(position->current_player, position->bitboards, position->enpassant_square);
            break;
        default:
            break;
    }

    if (is_pawn_promotion(position->bitboards, this_move))
        handle_pawn_promotion(position->bitboards, this_move);

    // set enpassant flag, otherwise delete it 
    if (is_double_pawn_push(position->bitboards, this_move))
        handle_double_pawn_push(position->current_player, this_move, &position->enpassant_square);
    else
        position->enpassant_square = -1;       
}

void apply_move(Position* position, Move* this_move)
{
    int startsquare = this_move->startsquare;
    int destsquare = this_move->destsquare;

    printf("apply move: %s = %d, %s = %d, flag: %d\n", square_to_notation(startsquare), startsquare, square_to_notation(destsquare), destsquare, this_move->flags);

    int bitboard_index = get_bitboard_index(position->bitboards, startsquare);
    int dest_bitboard_index = get_bitboard_index(position->bitboards, destsquare);

    position->bitboards[bitboard_index] &= ~(1ULL << startsquare); // delete piece on startsquare
    position->bitboards[bitboard_index] |= (1ULL << destsquare); // set new piece on destsquare    

    position->bitboards[dest_bitboard_index] &= ~(1ULL << destsquare); // delete piece on destsquare

    // set position->king_square[2]
    position->king_square[BLACK] = get_king_square(position->bitboards[BLACK_KING]);
    position->king_square[WHITE] = get_king_square(position->bitboards[WHITE_KING]);
    
    handle_special_move(position, this_move);
	update_castle_rights(position, this_move);
}

// if given move is legal, return its address
Move* is_legal_move(Position* position, int startsquare, int destsquare)
{
    printf("IS_LEGAL_MOVE validating move: %d, %d\n", startsquare, destsquare);
    for(int i = 0; i < position->legal_move_count; ++i)
    {
        if((position->legal_moves[i].startsquare == startsquare) && (position->legal_moves[i].destsquare == destsquare))
        {
            return &position->legal_moves[i];
        }
    }
    return NULL;
}

// TODO: adjust to only whats needed 
void save_state(Position* position, Undo* undo)
{
    printf("SAVE_STATE is called in Move handler\n");
    memcpy(undo->bitboards, position->bitboards, sizeof(position->bitboards));
    memcpy(undo->occupancy, position->occupancy, sizeof(position->occupancy));
    
    undo->current_player   = position->current_player;
    undo->castle_rights    = position->castle_rights;
    undo->enpassant_square = position->enpassant_square;
    undo->halfmove_clock   = position->halfmove_clock;
    undo->fullmove_number  = position->fullmove_number;

    memcpy(undo->legal_moves, position->legal_moves, sizeof(position->legal_moves));
    undo->legal_move_count = position->legal_move_count;

    memcpy(undo->attack_bitboards, position->attack_bitboards, sizeof(position->attack_bitboards));

    memcpy(undo->king_square, position->king_square, sizeof(position->king_square));
}

// if undo is NULL, nothing is saved 
int handle_move(Position* position, int startsquare, int destsquare, Undo* undo)
{
    if(startsquare == -1 || destsquare == -1)
        return 0;

    printf("HANDLE MOVE: %d, %d\n", startsquare, destsquare);
    Move* this_move = is_legal_move(position, startsquare, destsquare);
    if (!this_move)
    {
        printf("HANDLE MOVE: illegal move\n");
        return 0;
    }

    // save old state
    if (undo)
        save_state(position, undo);
        
    // apply move
    apply_move(position, this_move);
    return 1;
}

