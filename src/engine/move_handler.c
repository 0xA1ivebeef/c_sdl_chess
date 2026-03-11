
#include "engine/move_handler.h"

void handle_pawn_promotion(Position* pos, Move* m)
{
    assert(m->flags >= KNIGHT_PROMOTION && m->flags <= QUEEN_PROMOTION);

    int pawn = (m->dest < 8) ? WHITE_PAWN : BLACK_PAWN;
    int promote_piece = m->flags - 2; // PROMOTION_FLAG -> Bitboard Index
    promote_piece += pawn; // fixing color 

    // replace pawn with promoted piece
    pos->bb[pawn] &= ~(1ULL << m->dest);
    pos->bb[promote_piece] |= (1ULL << m->dest); 

    pos->zobrist_hash ^= zobrist_table[pawn * 64 + m->dest];
    pos->zobrist_hash ^= zobrist_table[promote_piece * 64 + m->dest];
}

void handle_enpassant(Position* pos, Move* m)
{
    int capture_sq = pos->player ? m->dest + 8 : m->dest - 8;
    int captured_pawn = pos->player ? BLACK_PAWN : WHITE_PAWN;

    assert(capture_sq >= 0 && capture_sq < 64);
    
    pos->bb[captured_pawn] &= ~(1ULL << capture_sq); // capture enpassanted pawn

    pos->zobrist_hash ^= zobrist_table[captured_pawn * 64 + capture_sq];
}

void handle_castling(Position* pos, Move* m)
{
    int rook = (m->start > 32) ? WHITE_ROOK : BLACK_ROOK; 
    if(m->start > m->dest)
    {
        pos->bb[rook] &= ~(1ULL << (m->dest - 2));
        pos->bb[rook] |= (1ULL << (m->dest + 1));

        pos->zobrist_hash ^= zobrist_table[rook * 64 + m->dest - 2];
        pos->zobrist_hash ^= zobrist_table[rook * 64 + m->dest + 1];
    }
    else
    {
        pos->bb[rook] &= ~(1ULL << (m->dest + 1));
        pos->bb[rook] |= (1ULL << (m->dest - 1));
        
        pos->zobrist_hash ^= zobrist_table[rook * 64 + m->dest + 1];
        pos->zobrist_hash ^= zobrist_table[rook * 64 + m->dest - 1];
    }
}

// called after every move (through apply_move() !!!)
void handle_special_move(Position* pos, Move* m)
{
    pos->enpassant = INVALID_SQUARE;
    switch (m->flags)
    {
        case CASTLE_FLAG:
            handle_castling(pos, m);
            break;
        case ENPASSANT_FLAG:
            handle_enpassant(pos, m);
            break;
        case KNIGHT_PROMOTION:
        case BISHOP_PROMOTION:
        case ROOK_PROMOTION:
        case QUEEN_PROMOTION:
            handle_pawn_promotion(pos, m); 
            break;
        case DOUBLE_PAWN_PUSH:
            pos->enpassant = m->dest + 8*(pos->player ? 1 : -1);
            break;
    }
}

void undo_castling(Position* pos, Move* m)
{
    assert(m->flags == CASTLE_FLAG);

    int rook = (m->start > 32 ) ? WHITE_ROOK : BLACK_ROOK; 

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
    
    int capture_sq = m->dest - 8;
    int restore_piece = WHITE_PAWN;

    if (undo->moved_piece == WHITE_PAWN)
    {
        capture_sq = m->dest + 8;
        restore_piece = BLACK_PAWN;
    }

    pos->bb[restore_piece] |= (1ULL << capture_sq); // restore captured pawn
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

    pos->zobrist_hash = undo->zobrist_hash;

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
    undo->zobrist_hash   = pos->zobrist_hash;
}

void apply_move(Position* pos, Move* m, Undo* undo)
{   
    save_state(pos, m, undo);

    int moved_piece = get_bb_index(pos->bb, m->start);
    assert(moved_piece > -1);

    pos->zobrist_hash ^= zobrist_table[moved_piece * 64 + m->start];

    int captured_piece = get_bb_index(pos->bb, m->dest); // -1 if empty

	// capture
    if (captured_piece > -1) 
    {
        pos->bb[captured_piece] &= ~(1ULL << m->dest);
        pos->zobrist_hash ^= zobrist_table[captured_piece * 64 + m->dest];
    }

    // move
    pos->bb[moved_piece] &= ~(1ULL << m->start);
    pos->bb[moved_piece] |=  (1ULL << m->dest);

    pos->zobrist_hash ^= zobrist_table[moved_piece * 64 + m->dest];

    // remove old enpassant square do this before enpassant square is updated through this move
    if (pos->enpassant != INVALID_SQUARE)
        pos->zobrist_hash ^= zobrist_enpassant[pos->enpassant % 8];

    // remove old castlerights
    pos->zobrist_hash ^= zobrist_castling[pos->castle_rights];

    // special move handling after normal move
    // updates enpassant square
    handle_special_move(pos, m); 
    update_castle_rights(pos, m); 

    // add new castle rights
    pos->zobrist_hash ^= zobrist_castling[pos->castle_rights];

    // add new enpassant square
    if (pos->enpassant != INVALID_SQUARE)
        pos->zobrist_hash ^= zobrist_enpassant[pos->enpassant % 8];

    // move clocks
    int pawn_move = (moved_piece % WHITE_PAWN == 0);
    if (pawn_move || captured_piece > -1 || (m->flags == ENPASSANT_FLAG)) 
        pos->halfmove = 0;
    else 
        pos->halfmove++;

    if (pos->player == BLACK) 
        pos->fullmove++;

    pos->zobrist_hash ^= zobrist_black_to_move;

    pos->player ^= 1;
    update_occ(pos);
}

Move* is_legal_move(LegalMoves* lm, Move* m)
{
    for(int i = 0; i < lm->count; ++i)
    {
        if ((lm->moves[i].start == m->start) && 
            (lm->moves[i].dest  == m->dest))
        {
            if (m->flags >= KNIGHT_PROMOTION && m->flags <= QUEEN_PROMOTION)
                return m;

            return &lm->moves[i];
        }
    }
    return NULL;
}

int handle_move(Position* pos, LegalMoves* lm, Move* m)
{
    // replace with move from legal moves to get flags
    // assume promotion flags are already set in m
    Move* legal = is_legal_move(lm, m);
    if (!legal)
        return 0;

    Undo undo;
    apply_move(pos, legal, &undo);

    return 1;
}

