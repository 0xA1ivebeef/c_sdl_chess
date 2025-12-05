
#include "engine/move_handler.h"

// called after every move (through apply_move() !!!)
void handle_special_move(Position* position, Move* this_move, int ss_bb_i)
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
    if (is_double_pawn_push(this_move, ss_bb_i))
        handle_double_pawn_push(position->current_player, this_move, &position->enpassant_square);
    else
        position->enpassant_square = -1;       
}

void apply_move(Position* position, Move* m)
{
	int ss = m->startsquare;
	int ds = m->destsquare;
    int ss_bb_i = get_bitboard_index(position->bitboards, ss);
	printf("apply move: ss_bb_i %d\n", ss_bb_i);
    if (ss_bb_i == -1) 
	{
        fprintf(stderr, "apply_move: no piece on startsquare %d\n", ss_bb_i);
        abort();
    }

    int ds_bb_i = get_bitboard_index(position->bitboards, ds); // -1 if empty

	// capture
    if (ds_bb_i != -1) 
        position->bitboards[ds_bb_i] &= ~(1ULL << ds);

    // move
    position->bitboards[ss_bb_i] &= ~(1ULL << ss);
    position->bitboards[ss_bb_i] |=  (1ULL << ds);

    // update king squares 
    if (ss_bb_i == WHITE_KING) 
        position->king_square[WHITE] = ds;
	else if (ss_bb_i == BLACK_KING) 
	    position->king_square[BLACK] = ds;
  
    handle_special_move(position, m, ss_bb_i);

    update_castle_rights(position, m);

    int is_pawn_move = (ss_bb_i == WHITE_PAWN || ss_bb_i == BLACK_PAWN);
    if (is_pawn_move || ds_bb_i != -1 || (m->flags == ENPASSANT_FLAG)) 
        position->halfmove_clock = 0;
	else 
        position->halfmove_clock++;

    if (position->current_player == BLACK) 
        position->fullmove_number++;
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

