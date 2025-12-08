
#include "engine/move_handler.h"

// called after every move (through apply_move() !!!)
void handle_special_move(Position* position, Move* this_move)
{
    switch (this_move->flags)
    {
        case CASTLE_FLAG:
            handle_castling(position->bitboards, this_move, &position->castle_rights);
            break;
        case ENPASSANT_FLAG:
            handle_enpassant(position->current_player, position->bitboards, position->enpassant_square);
            break;
        case KNIGHT_PROMOTION:
        case BISHOP_PROMOTION:
        case ROOK_PROMOTION:
        case QUEEN_PROMOTION:
            handle_pawn_promotion(position->bitboards, this_move); 
            break;
        case DOUBLE_PAWN_PUSH:
            handle_double_pawn_push(position->current_player, this_move, &position->enpassant_square);
            break;
        default:
            position->enpassant_square = -1;
            break;
    }
}

void undo_castling(Position* position, Move* m, Undo* undo)
{
    
}

void undo_enpassant(Position* position, Move* m, Undo* undo)
{
    
}

void undo_promotion(Position* position, Move* m, Undo* undo)
{
    int startsquare = m->startsquare;
    int destsquare = m->destsquare;

    // restore capture
    if (undo->ds_bb_i != -1)
        position->bitboards[undo->ds_bb_i] |= 1ULL << destsquare; 
                                                  
    // black n, b, r, q 7, 8, 9, 10
    position->bitboards[ss_bb_i] |= 1ULL << startsquare; // restore pawn
    position->bitboards[undo->promote_bitboard] &= ~(1ULL << destsquare); // delete prom piece
}

// never call undo_move before calling apply_move because it saves the state
void undo_move(Position* position, Move* m, Undo* undo)
{
    int ss = m->startsquare;
	int ds = m->destsquare;

    int ss_bb_i = undo->ss_bb_i;

    // reverse capturing
    int ds_bb_i = undo->ds_bb_i;
    if (ds_bb_i != -1) 
        position->bitboards[ds_bb_i] |= 1ULL << ds;

    // undo move
    position->bitboards[ss_bb_i] |= 1ULL << ss;
    position->bitboards[ss_bb_i] &= ~(1ULL << ds);

    // undo king squares 
    memcpy(position->king_square, undo->king_square, sizeof(undo->king_square));
    
    // undo special moves
    switch (m->flags)
    {
        case 1:
            undo_castling(position, m, undo);
            break;
        case 2:
            undo_enpassant(position, m, undo);
        case 3:
        case 4:
        case 5:
        case 6:
            undo_promotion(position, m, undo);
            break;
        case 7:
            // double_pawn_push doesnt need undo since it only sets enpassant 
            // square and this is already done
            break;
    }

    position->castle_rights = undo->castle_rights;
    position->enpassant_square = undo->enpassant_square;
        
    position->halfmove_clock = undo->halfmove_clock;
    position->fullmove_number = undo->fullmove_number;

    position->current_player ^= 1;
}

void set_promoted_piece(Move* m, Undo* undo)
{
    // set bb index of promoted piece
    int promote_bb = m->flags - 2; // knight..queen 3..6
    
    if(m->destsquare >= 0 && m->destsquare <= 7)
        promote_bb += 6;

    undo->promote_bitboard = promote_bb;
}

void apply_move(Position* position, Move* m, Undo* undo)
    
	int ss = m->startsquare;
	int ds = m->destsquare;
    int ss_bb_i = get_bitboard_index(position->bitboards, ss);
    if (ss_bb_i == -1) 
	{
        fprintf(stderr, "apply_move: no piece on startsquare %d\n", ss_bb_i);
        abort();
    }

    int ds_bb_i = get_bitboard_index(position->bitboards, ds); // -1 if empty

    if (undo)
    {
        undo->ss_bb_i = ss_bb_i;
        undo->ds_bb_i = ds_bb_i;
        undo-> castle_rights = position->castle_rights;
        undo-> enpassant_square = position->enpassant_square;
        undo-> halfmove_clock = position->halfmove_clock;
        undo-> = fullmove_number = position->fullmove_number;
        if (m->flags >= 3 && m->flags <= 6) 
            set_promoted_piece(position, m, undo);
        memcpy(undo->king_square, position->king_square, sizeof(undo->king_square));
    }

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
  
    handle_special_move(position, m);

    update_castle_rights(position, m);

    int is_pawn_move = (ss_bb_i == WHITE_PAWN || ss_bb_i == BLACK_PAWN);
    if (is_pawn_move || ds_bb_i != -1 || (m->flags == ENPASSANT_FLAG)) 
        position->halfmove_clock = 0;
	else 
        position->halfmove_clock++;

    if (position->current_player == BLACK) 
        position->fullmove_number++;

    position->current_player ^= 1;
}


// if given move is legal, return its address
Move* is_legal_move(Position* position, int startsquare, int destsquare)
{
    // printf("IS_LEGAL_MOVE validating move: %d, %d\n", startsquare, destsquare);
    for(int i = 0; i < position->legal_move_count; ++i)
    {
        if((position->legal_moves[i].startsquare == startsquare) && (position->legal_moves[i].destsquare == destsquare))
            return &position->legal_moves[i];
    }
    return NULL;
}

// if undo is NULL, nothing is saved 
int handle_move(Position* position, int startsquare, int destsquare, Undo* undo)
{
    if(startsquare == -1 || destsquare == -1)
        return 0;

    Move* this_move = is_legal_move(position, startsquare, destsquare);
    if (!this_move)
    {
        printf("HANDLE MOVE: illegal move\n");
        return 0;
    }

    // if promotion, ask human which piece to promote to
    if (this_move->flags >= KNIGHT_PROMOTION && this_move->flags <= QUEEN_PROMOTION)
        *this_move = choose_promotion_move(this_move);
  
    apply_move(position, this_move);
    return 1;
}

