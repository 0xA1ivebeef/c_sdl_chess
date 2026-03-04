
#include "engine/move_handler.h"

// called after every move (through apply_move() !!!)
void handle_special_move(Position* pos, Move* m)
{
    printf("handling special move\n");
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
            printf("handling double_pawnpush %d\n", pos->enpassant);
            break;
        default:
            pos->enpassant = INVALID_SQUARE;
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
    pos->bb[undo->promote_bb_i] &= ~(1ULL << m->dest); 
}

// never call undo_move before calling apply_move because it saves the state
// TODO make sure the state is fully restored
void undo_move(Position* pos, Move* m, Undo* undo)
{
    assert(undo->valid);

    int ss_bb_i = undo->ss_bb_i;
    int ds_bb_i = undo->ds_bb_i;

    // reverse capturing
    if (ds_bb_i != -1) 
        pos->bb[ds_bb_i] |= 1ULL << m->dest;

    // restore moved piece on start
    pos->bb[ss_bb_i] |= 1ULL << m->start;

    // remove moved piece on dest
    pos->bb[ss_bb_i] &= ~(1ULL << m->dest);
 
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
            // TODO double_pawn_push doesnt need undo since it only sets enpassant 
            // square and this is already done through bitboard undo
            break;
    }

    // restore state
    pos->castle_rights = undo->castle_rights;
    pos->enpassant = undo->enpassant;
        
    pos->halfmove = undo->halfmove;
    pos->fullmove = undo->fullmove;

    update_occ(pos);
    pos->player ^= 1;

    undo->valid = 0;
}

void set_promoted_piece(Move* m, Undo* undo)
{
    // set bb index of promoted piece
    int promote_bb_i = m->flags - 2; // knight..queen 3..6
    
    if(m->dest <= 7) // WHITE
        promote_bb_i += 6;

    undo->promote_bb_i = promote_bb_i;
}

void apply_move(Position* pos, Move* m, Undo* undo)
{   
    int ss_bb_i = get_bb_index(pos->bb, m->start);
    if (ss_bb_i == -1) 
	{
        fprintf(stderr, "apply_move: no piece on start %d\n", ss_bb_i);
        abort();
    }

    int ds_bb_i = get_bb_index(pos->bb, m->dest); // -1 if empty

    // save old state BEFORE ANY CHANGES
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
        undo->valid = 1;
    }

	// capture
    if (ds_bb_i != -1) 
        pos->bb[ds_bb_i] &= ~(1ULL << m->dest);

    // move
    pos->bb[ss_bb_i] &= ~(1ULL << m->start);
    pos->bb[ss_bb_i] |=  (1ULL << m->dest);

    // special move handling
    handle_special_move(pos, m); // after move was made
    update_castle_rights(pos, m);

    // move clocks
    int is_pawn_move = (ss_bb_i == WHITE_PAWN || ss_bb_i == BLACK_PAWN);
    if (is_pawn_move || ds_bb_i != -1 || (m->flags == ENPASSANT_FLAG)) 
        pos->halfmove = 0;
	else 
        pos->halfmove++;

    // TODO maybe this is wrong
    if (pos->player == BLACK) 
        pos->fullmove++;
}

// TODO promotion moves
Move* is_legal_move(Position* pos, Move* m)
{
    for(int i = 0; i < pos->legal_move_count; ++i)
    {
        if ((pos->legal_moves[i].start == m->start) && 
            (pos->legal_moves[i].dest  == m->dest))
            return &pos->legal_moves[i];
    }
    return NULL;
}

// if undo is NULL, nothing is saved 
int handle_move(Position* pos, Move* m)
{
    // replace with move from legal moves to get flags
    Move* legal = is_legal_move(pos, m);
    if (!legal)
        return 0;

    // normal moves dont need to be undone
    apply_move(pos, legal, NULL);

    return 1;
}

