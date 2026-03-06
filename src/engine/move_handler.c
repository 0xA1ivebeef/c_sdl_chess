
#include "engine/move_handler.h"

// called after every move (through apply_move() !!!)
void handle_special_move(Position* pos, Move* m)
{
    switch (m->flags)
    {
        case CASTLE_FLAG:
            handle_castling(pos->bb, m);
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
    assert(m->flags == CASTLE_FLAG);

    int rook = (m->start == 60) ? WHITE_ROOK : BLACK_ROOK; 

    int rook_from, rook_to;
    if(m->start > m->dest)
    {
        rook_from = m->dest - 2; // queenside
        rook_to = m->dest + 1;
    }
    else
    {
        rook_from = m->dest + 1; // kingside
        rook_to = m->dest - 1;
    }

    pos->bb[rook] |= (1ULL << rook_from); // add rook back on a file
    pos->bb[rook] &= ~(1ULL << rook_to); // remove rook
}

void undo_enpassant(Position* pos, Move* m, Undo* undo)
{
    assert(m->flags == ENPASSANT_FLAG);

    // direction 1 white captured black pawn: -1 black captured white pawn
    int dir = (m->start < m->dest) ? 1 : -1;

    // white captured -> blacks pawn bb 0 : black captured -> whites pawn bb 6
    int restore_piece = dir ? BLACK_PAWN : WHITE_PAWN;    
    int d = undo->enpassant + dir*8; 

    pos->bb[restore_piece] |= (1ULL << d); // restore captured pawn
}

void undo_promotion(Position* pos, Move* m, Undo* undo)
{
    assert(m->flags >= KNIGHT_PROMOTION && m->flags <= QUEEN_PROMOTION);

    // only remove promoted piece
    int is_white = undo->moved_piece == WHITE_PAWN;
    int promoted_piece = m->flags - 2 + is_white*6; // 3..6 -> 1-4 black / 7-10 white
    pos->bb[promoted_piece] &= ~(1ULL << m->dest);
}

void undo_move(Position* pos, Move* m, Undo* undo)
{
    // undo special moves before undo normal moves to keep the function inversion order correct
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
            // double pawn push already undone through normal move undo
            break;
    }

    // reverse capturing
    if (undo->captured_piece > -1) 
        pos->bb[undo->captured_piece] |= (1ULL << m->dest);

    // restore moved piece 
    pos->bb[undo->moved_piece] |= (1ULL << m->start);
    pos->bb[undo->moved_piece] &= ~(1ULL << m->dest);
 
    // restore state
    pos->castle_rights = undo->castle_rights;
    pos->enpassant = undo->enpassant;
        
    pos->halfmove = undo->halfmove; 
    pos->fullmove = undo->fullmove;

    pos->player ^= 1;
    update_occ(pos);
}

void save_state(Position* pos, Move* m, Undo* undo)
{
    assert(pos && m && undo);

    undo->moved_piece    = get_bb_index(pos->bb, m->start);
    undo->captured_piece = get_bb_index(pos->bb, m->dest); 
    undo->castle_rights  = pos->castle_rights;
    undo->enpassant      = pos->enpassant;
    undo->halfmove       = pos->halfmove;
    undo->fullmove       = pos->fullmove;
}

void apply_move(Position* pos, Move* m)
{   
    int moved_piece = get_bb_index(pos->bb, m->start);
    assert(moved_piece > -1);

    int captured_piece = get_bb_index(pos->bb, m->dest); // -1 if empty

	// capture
    if (captured_piece > -1) 
        pos->bb[captured_piece] &= ~(1ULL << m->dest);

    // move
    pos->bb[moved_piece] &= ~(1ULL << m->start);
    pos->bb[moved_piece] |=  (1ULL << m->dest);

    // special move handling after normal move
    handle_special_move(pos, m); 
    update_castle_rights(pos, m); 

    // move clocks
    int pawn_move = (moved_piece % 6 == 0);
    if (pawn_move || captured_piece > -1 || (m->flags == ENPASSANT_FLAG)) 
        pos->halfmove = 0;
    else 
        pos->halfmove++;

    if (pos->player == BLACK) 
        pos->fullmove++;

    pos->player ^= 1;
}

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

int handle_move(Position* pos, Move* m)
{
    // replace with move from legal moves to get flags
    Move* legal = is_legal_move(pos, m);
    if (!legal)
        return 0;

    apply_move(pos, legal);

    return 1;
}

