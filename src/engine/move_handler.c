
#include "engine/move_handler.h"

// called after every move (through apply_move() !!!)
void handle_special_move(Position* pos, Move* m)
{
    switch (m->flags)
    {
        case CASTLE_FLAG:
            handle_castling(pos->bb, m, &pos->castle_rights);
            break;
        case ENPASSANT_FLAG:
            handle_enpassant(pos->player, pos->bb, pos->enpassant);
            break;
        case KNIGHT_PROMOTION:
        case BISHOP_PROMOTION:
        case ROOK_PROMOTION:
        case QUEEN_PROMOTION:
            handle_pawn_promotion(pos->bb, m); 
            break;
        case DOUBLE_PAWN_PUSH:
            pos->enpassant = m->dest + 8 * (pos->player ? 1 : -1);
            break;
        default:
            pos->enpassant = -1;
            break;
    }
}

void undo_castling(Position* pos, Move* m)
{
    // move the rook back
    int bb_i = (m->start == 60) ? WHITE_ROOK : BLACK_ROOK; // rook bitboards
    if(m->start > m->dest)
    {
        // qs
        pos->bb[bb_i] |= (1ULL << (m->dest - 2)); // add rook back on a file
        pos->bb[bb_i] &= ~(1ULL << (m->dest + 1)); // remove rook
    }
    else
    {
        // ks
        pos->bb[bb_i] |= (1ULL << (m->dest + 1)); 
        pos->bb[bb_i] &= ~(1ULL << (m->dest - 1));
    }
}

// TODO fix this
void undo_enpassant(Position* pos, Move* m, Undo* undo)
{
    // direction 1 white captured black pawn: -1 black captured white pawn
    int dir = ((m->start - m->dest) ? 1 : -1);   

    // white captured -> blacks pawn bb 0 : black captured -> whites pawn bb 6
    int restore_bb_i = (dir == 1) ? 0 : 6;    

    int d = undo->enpassant + 8 * dir; 

    pos->bb[restore_bb_i] |= (1ULL << d);
}

void undo_promotion(Position* pos, Move* m, Undo* undo)
{
    // removed promoted piece, pawn move is already undone
    pos->bb[undo->promote_bb] &= ~(1ULL << m->dest); 
}

// never call undo_move before calling apply_move because it saves the state
void undo_move(Position* pos, Move* m, Undo* undo)
{
    int sq = m->start;
	int ds = m->dest;

    int ss_bb_i = undo->ss_bb_i;
    int ds_bb_i = undo->ds_bb_i;

    // reverse capturing
    if (ds_bb_i != -1) 
        pos->bb[ds_bb_i] |= 1ULL << ds;

    // restore moved piece on start
    pos->bb[ss_bb_i] |= 1ULL << sq;

    // remove moved piece on dest
    pos->bb[ss_bb_i] &= ~(1ULL << ds);
 
    // undo special moves
    switch (m->flags)
    {
        case 1:
            undo_castling(pos, m);
            break;
        case 2:
            undo_enpassant(pos, m, undo);
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            undo_promotion(pos, m, undo);
            break;
        case 7:
            // double_pawn_push doesnt need undo since it only sets enpassant 
            // square and this is already done
            break;
    }

    // restore state
    pos->castle_rights = undo->castle_rights;
    pos->enpassant = undo->enpassant;
        
    pos->halfmove = undo->halfmove;
    pos->fullmove = undo->fullmove;

    update_occ(pos->bb, pos->occ);

	// update king squares 
    if (ss_bb_i == WHITE_KING) 
        pos->king_sq[WHITE] = ds;
	else if (ss_bb_i == BLACK_KING) 
	    pos->king_sq[BLACK] = ds;

    pos->player ^= 1;
}

void set_promoted_piece(Move* m, Undo* undo)
{
    // set bb index of promoted piece
    int promote_bb = m->flags - 2; // knight..queen 3..6
    
    if(m->dest >= 0 && m->dest <= 7)
        promote_bb += 6;

    undo->promote_bb = promote_bb;
}

void apply_move(Position* pos, Move* m, Undo* undo)
{   
	int sq = m->start;
	int ds = m->dest;
    int ss_bb_i = get_bb_index(pos->bb, sq);
    if (ss_bb_i == -1) 
	{
        fprintf(stderr, "apply_move: no piece on start %d\n", ss_bb_i);
        abort();
    }

    int ds_bb_i = get_bb_index(pos->bb, ds); // -1 if empty

    if (undo)
    {
        undo->ss_bb_i = ss_bb_i;
        undo->ds_bb_i = ds_bb_i;
        undo->castle_rights = pos->castle_rights;
        undo->enpassant = pos->enpassant;
        undo->halfmove = pos->halfmove;
        undo->fullmove = pos->fullmove;
        if (m->flags >= 3 && m->flags <= 6) 
            set_promoted_piece(m, undo);
    }

	// capture
    if (ds_bb_i != -1) 
        pos->bb[ds_bb_i] &= ~(1ULL << ds);

    // move
    pos->bb[ss_bb_i] &= ~(1ULL << sq);
    pos->bb[ss_bb_i] |=  (1ULL << ds);

    // update king squares 
    if (ss_bb_i == WHITE_KING) 
        pos->king_sq[WHITE] = ds;
	else if (ss_bb_i == BLACK_KING) 
	    pos->king_sq[BLACK] = ds;
  
    handle_special_move(pos, m);

    update_castle_rights(pos, m);

    int is_pawn_move = (ss_bb_i == WHITE_PAWN || ss_bb_i == BLACK_PAWN);
    if (is_pawn_move || ds_bb_i != -1 || (m->flags == ENPASSANT_FLAG)) 
        pos->halfmove = 0;
	else 
        pos->halfmove++;

    if (pos->player == BLACK) 
        pos->fullmove++;

    update_occ(pos->bb, pos->occ);

    pos->player ^= 1;
}


// if given move is legal, return its address
Move* is_legal_move(Position* pos, int start, int dest)
{
    // printf("IS_LEGAL_MOVE validating move: %d, %d\n", start, dest);
    for(int i = 0; i < pos->legal_move_count; ++i)
    {
        if((pos->legal_moves[i].start == start) && 
                (pos->legal_moves[i].dest == dest))
            return &pos->legal_moves[i];
    }
    return NULL;
}

// if undo is NULL, nothing is saved 
int handle_move(Position* pos, int start, int dest)
{
    if(start == -1 || dest == -1)
        return 0;

    Move* m = is_legal_move(pos, start, dest);
    if (!m)
    {
        printf("HANDLE MOVE: illegal move\n");
        return 0;
    }

    // if promotion, ask human which piece to promote to
    if (m->flags >= KNIGHT_PROMOTION && m->flags <= QUEEN_PROMOTION)
        *m = choose_promotion_move(m);
  
    apply_move(pos, m, NULL);
    return 1;
}

