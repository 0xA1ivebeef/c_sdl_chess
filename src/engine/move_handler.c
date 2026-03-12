
#include "engine/move_handler.h"

// TODO refactor

void handle_pawn_promotion(Position* pos, Move m)
{
    assert((m >> 12) != 0);

    int dest = move_to(m);

    int pawn = pos->player ? WHITE_PAWN : BLACK_PAWN;
    int promote_piece = m >> 12; 
    promote_piece += pawn; // setting color 

    // replace pawn with promoted piece
    pos->bb[pawn] &= ~(1ULL << dest);
    pos->bb[promote_piece] |= (1ULL << dest); 

    pos->zobrist_hash ^= Random64[my_to_poly(pawn) * 64 + polyglot_sq(dest)];
    pos->zobrist_hash ^= Random64[my_to_poly(promote_piece) * 64 + polyglot_sq(dest)];
}

void handle_enpassant(Position* pos, Move m)
{
    int capture_sq = pos->player ? move_to(m) + 8 : move_to(m) - 8;
    int captured_pawn = pos->player ? BLACK_PAWN : WHITE_PAWN;

    assert(capture_sq >= 0 && capture_sq < 64);
    
    pos->bb[captured_pawn] &= ~(1ULL << capture_sq); // capture enpassanted pawn

    pos->zobrist_hash ^= Random64[my_to_poly(captured_pawn) * 64 + polyglot_sq(capture_sq)];
}

void handle_castling(Position* pos, Move m)
{
    int rook = (move_from(m) > 32) ? WHITE_ROOK : BLACK_ROOK; 
    if(move_from(m) > move_to(m))
    {
        pos->bb[rook] &= ~(1ULL << (move_to(m) - 2));
        pos->bb[rook] |= (1ULL << (move_to(m) + 1));

        pos->zobrist_hash ^= Random64[my_to_poly(rook) * 64 + polyglot_sq(move_to(m) - 2)];
        pos->zobrist_hash ^= Random64[my_to_poly(rook) * 64 + polyglot_sq(move_to(m) + 1)];
    }
    else
    {
        pos->bb[rook] &= ~(1ULL << (move_to(m) + 1));
        pos->bb[rook] |= (1ULL << (move_to(m) - 1));
        
        pos->zobrist_hash ^= Random64[my_to_poly(rook) * 64 + polyglot_sq(move_to(m) + 1)];
        pos->zobrist_hash ^= Random64[my_to_poly(rook) * 64 + polyglot_sq(move_to(m) - 1)];
    }
}

void handle_special_move(Position* pos, Move m, int moved_piece)
{
    if (is_enpassant_move(pos->enpassant, m, moved_piece))
        handle_enpassant(pos, m);

    pos->enpassant = INVALID_SQUARE;

    if (is_castling_move(m, moved_piece))
        handle_castling(pos, m);

    else if (is_promotion_move(m))
        handle_pawn_promotion(pos, m); 

    else if (is_double_pawn_move(m, moved_piece))
        pos->enpassant = move_to(m) + 8 * (pos->player ? 1 : -1);
}

void undo_castling(Position* pos, Move m)
{
    int rook = (move_from(m) > 32 ) ? WHITE_ROOK : BLACK_ROOK; 

    int rook_from, rook_to;
    if(move_from(m) > move_to(m))
    {
        rook_from = move_to(m) - 2; // queenside
        rook_to = move_to(m) + 1;
    }
    else
    {
        rook_from = move_to(m) + 1; // kingside
        rook_to = move_to(m) - 1;
    }

    pos->bb[rook] |= (1ULL << rook_from); // add rook back on a file
    pos->bb[rook] &= ~(1ULL << rook_to); // remove rook
}

void undo_enpassant(Position* pos, Move m, Undo* undo)
{
    int capture_sq = move_to(m) - 8;
    int restore_piece = WHITE_PAWN;

    if (undo->moved_piece == WHITE_PAWN)
    {
        capture_sq = move_to(m) + 8;
        restore_piece = BLACK_PAWN;
    }

    pos->bb[restore_piece] |= (1ULL << capture_sq); // restore captured pawn
}

void undo_promotion(Position* pos, Move m, Undo* undo)
{
    assert((m >> 12) != 0);

    // only remove promoted piece
    int is_white = undo->moved_piece == WHITE_PAWN;
    int promoted_piece = (m >> 12) + is_white*6; 

    pos->bb[promoted_piece] &= ~(1ULL << move_to(m));
}

void undo_move(Position* pos, Move m, Undo* undo)
{
    // undo special moves before undo normal moves to keep the function inversion order correct
    if (is_castling_move(m, undo->moved_piece))
        undo_castling(pos, m);

    if (is_enpassant_move(undo->enpassant, m, undo->moved_piece))
        undo_enpassant(pos, m, undo);
    
    if (is_promotion_move(m))
        undo_promotion(pos, m, undo);

    // reverse capturing
    if (undo->captured_piece > -1) 
        pos->bb[undo->captured_piece] |= (1ULL << move_to(m));

    // restore moved piece 
    pos->bb[undo->moved_piece] |= (1ULL << move_from(m));
    pos->bb[undo->moved_piece] &= ~(1ULL << move_to(m));
 
    // restore state
    pos->castle_rights = undo->castle_rights;
    pos->enpassant = undo->enpassant;
        
    pos->halfmove = undo->halfmove; 
    pos->fullmove = undo->fullmove;

    pos->zobrist_hash = undo->zobrist_hash;

    pos->player ^= 1;
    update_occ(pos);
}

void save_state(Position* pos, Move m, Undo* undo)
{
    assert(pos && undo);

    undo->moved_piece    = get_bb_index(pos->bb, move_from(m));
    undo->captured_piece = get_bb_index(pos->bb, move_to(m)); 
    undo->castle_rights  = pos->castle_rights;
    undo->enpassant      = pos->enpassant;
    undo->halfmove       = pos->halfmove;
    undo->fullmove       = pos->fullmove;
    undo->zobrist_hash   = pos->zobrist_hash;
}

void apply_move(Position* pos, Move m, Undo* undo)
{   
    save_state(pos, m, undo);

    int moved_piece = get_bb_index(pos->bb, move_from(m));
    assert(moved_piece > -1);

    pos->zobrist_hash ^= Random64[my_to_poly(moved_piece) * 64 + polyglot_sq(move_from(m))];

    int captured_piece = get_bb_index(pos->bb, move_to(m)); // -1 if empty

	// capture
    if (captured_piece > -1) 
    {
        pos->bb[captured_piece] &= ~(1ULL << move_to(m));
        pos->zobrist_hash ^= Random64[my_to_poly(captured_piece) * 64 + polyglot_sq(move_to(m))];
    }

    // move
    pos->bb[moved_piece] &= ~(1ULL << move_from(m));
    pos->bb[moved_piece] |=  (1ULL << move_to(m));

    pos->zobrist_hash ^= Random64[my_to_poly(moved_piece) * 64 + polyglot_sq(move_to(m))];

    // remove old enpassant square do this before enpassant square is updated through this move
    if (pos->enpassant != INVALID_SQUARE)
        pos->zobrist_hash ^= Random64[ENPASSANT_BASE + pos->enpassant % 8];

    // special move handling after normal move
    // updates enpassant square
    handle_special_move(pos, m, moved_piece); 
    update_castle_rights(pos, m, moved_piece); 

    if (undo->castle_rights != pos->castle_rights)
    {
        uint8_t lost_rights = undo->castle_rights & ~pos->castle_rights;

        if (lost_rights & 1) pos->zobrist_hash ^= Random64[BQ_CASTLE];
        if (lost_rights & 2) pos->zobrist_hash ^= Random64[BK_CASTLE];
        if (lost_rights & 4) pos->zobrist_hash ^= Random64[WQ_CASTLE];
        if (lost_rights & 8) pos->zobrist_hash ^= Random64[WK_CASTLE];
    }

    // add new enpassant square
    if (pos->enpassant != INVALID_SQUARE)
        pos->zobrist_hash ^= Random64[ENPASSANT_BASE + pos->enpassant % 8];

    // move clocks
    int pawn_move = (moved_piece % WHITE_PAWN == 0);
    if (pawn_move || captured_piece > -1) 
        pos->halfmove = 0;
    else 
        pos->halfmove++;

    if (pos->player == BLACK) 
        pos->fullmove++;

    pos->zobrist_hash ^= Random64[BLACK_TO_MOVE];

    pos->player ^= 1;
    update_occ(pos);
}

int is_legal_move(LegalMoves* lm, Move m)
{
    for(int i = 0; i < lm->count; ++i)
    {
        if ((move_from(lm->moves[i]) == move_from(m)) && 
            (move_to(lm->moves[i])  == move_to(m)))
            return 1;
    }
    return 0;
}

int handle_move(Position* pos, LegalMoves* lm, Move m)
{
    if (!is_legal_move(lm, m))
        return 0;

    Undo undo;
    apply_move(pos, m, &undo);

    return 1;
}

