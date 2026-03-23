
#include "engine/move_generator.h"

uint64_t pawn_attacks(int king_sq, int by_side)
{
    int side = by_side ? 1 : -1; // -1 if by_side==black
    int sq1 = king_sq + 7 * side;
    int sq2 = king_sq + 9 * side;
    return (1ULL << sq1) | (1ULL << sq2);
}

void add_move(LegalMoves* lm, uint8_t start, uint8_t dest, uint8_t flag)
{
    lm->moves[lm->count++] = dest | (start << 6) | ((flag & 0x7) << 12);
}

void add_enpassant(Position* pos, LegalMoves* lm)
{
    if (pos->enpassant < 16 || pos->enpassant > 55)
        return;

    uint64_t own_pawns = pos->bb[pos->player*6];

    int left_start = pos->enpassant - 9;
    int right_start = pos->enpassant - 7;
                                    
    if (pos->player == WHITE)
    {
        left_start = pos->enpassant + 7;
        right_start = pos->enpassant + 9;
    }

    int ep_file = file(pos->enpassant);
    if (ep_file == 0)
        left_start = INVALID_SQUARE;
    else if (ep_file == 7)
        right_start = INVALID_SQUARE;

    if (left_start != INVALID_SQUARE && own_pawns & (1ULL << left_start))
    {
        pos->enpassant_hashed = 1;
        // printf("hashing enpassant\n");
        pos->hash ^= Random64[ENPASSANT_BASE + ep_file];
        add_move(lm, left_start, pos->enpassant, 0);
    }
    if (right_start != INVALID_SQUARE && own_pawns & (1ULL << right_start))
    {
        if (!pos->enpassant_hashed)
        {
            pos->enpassant_hashed = 1;
            pos->hash ^= Random64[ENPASSANT_BASE + ep_file];
        }
        add_move(lm, right_start, pos->enpassant, 0);
    }
}

uint64_t get_pawn_attacking_moves(int p, int sq, uint64_t* occ)
{
    if (!p)
	    return black_pawn_attack_bitmasks[sq] & occ[!p];
    return white_pawn_attack_bitmasks[sq] & occ[!p];
}

uint64_t get_pawn_moves(int p, int sq, uint64_t* occ)
{
    uint64_t n_bm = p ? white_pawn_normal_bitmasks[sq] : black_pawn_normal_bitmasks[sq];
    uint64_t d_bm = p ? white_pawn_double_bitmasks[sq] & ~occ[2] : black_pawn_double_bitmasks[sq] & ~occ[2];
    uint64_t c_bm = p ? white_pawn_attack_bitmasks[sq] & occ[!p] : black_pawn_attack_bitmasks[sq] & occ[!p];

    uint64_t normal_legal = (n_bm & ~occ[2]);

    d_bm &= (p ? (normal_legal >> 8) : (normal_legal << 8));

    return (normal_legal | c_bm | d_bm);
}

uint64_t get_knight_moves(int p, int sq, uint64_t* occ)
{
    return (knight_bitmasks[sq] & ~occ[p]);
}

void resolve_dir(int p, int sq, int dir, uint64_t* res, uint64_t* occ)
{
    int x, y, attacking_sq;

    for (int i = 1; i < 8; ++i)
    {
        x = file(sq) + i * file_offsets[dir];
        y = rank(sq) + i * rank_offsets[dir];

        if (x < 0 || x > 7 || y < 0 || y > 7)
            return;

        attacking_sq = y * 8 + x;

        if (occ[!p] & (1ULL << attacking_sq))
        {
            *res |= (1ULL << attacking_sq);
            return;
        }
        else if (occ[p] & (1ULL << attacking_sq))
            return;
        else
            *res |= (1ULL << attacking_sq);
    }
}

uint64_t get_sliding_moves(int p, int sq, int piece, uint64_t* occ)
{
    uint64_t result = 0;

    int start = (piece == 2 ? 4 : 0); // bishop
    int end = (piece == 3 ? 4 : 8); // rook

    for (int dir = start; dir < end; ++dir)
        resolve_dir(p, sq, dir, &result, occ);

    return result;
}

uint64_t get_king_moves(int p, int sq, uint64_t* occ)
{
    return (king_bitmasks[sq] & ~occ[p]);
}

uint64_t get_moves_bitmask(int p, int piece, int sq, uint64_t* occ)
{
    piece %= 6; // abstract color, keep piecetype
               
    switch(piece)
    {
        case 0:
            return get_pawn_moves(p, sq, occ); 
        case 1:
            return get_knight_moves(p, sq, occ);
        case 2:
        case 3:
        case 4:
            return get_sliding_moves(p, sq, piece, occ);
        case 5:
            return get_king_moves(p, sq, occ);
    }

    return 0;
}

int is_promotion(int piece, int ds)
{
    return (piece % 6 == 0 && (ds < 8 || ds > 55));
}

void add_promotion_moves(LegalMoves* lm, Move m)
{
    assert(move_to(m) < 8 || move_to(m) > 55); 
    for (int i = 0; i < 4; ++i)
        lm->moves[lm->count++] = m | PROMOTION_FLAGS[i];
}

void get_pieces_moves(Position* pos, LegalMoves* lm, int piece, int sq)
{
    uint64_t moves = get_moves_bitmask(pos->player, piece, sq, pos->occ);
    
    while(moves)
    {
        int dest = __builtin_ctzll(moves);
        Move m = (sq << 6) | dest;

        if (is_promotion(piece, dest))
        {
            // printf("move_generator: promotion move piece %d, dest %d\n", piece, dest);
            add_promotion_moves(lm, m); // add manually cause 4x move
        }
        else 
            lm->moves[lm->count++] = m; // just add
        
        moves &= moves - 1;
    }
}

void resolve_bb(Position* pos, LegalMoves* lm, int piece) 
{
    uint64_t bb = pos->bb[piece];
    while(bb)
    {
        int sq = __builtin_ctzll(bb);
        get_pieces_moves(pos, lm, piece, sq);
        bb &= bb - 1;
    }
}

void generate_legal_moves(Position* pos, LegalMoves* lm)
{
    memset(lm->moves, 0, sizeof(Move) * LEGAL_MOVES_SIZE);
    lm->count = 0; 

    // index => only white or black bitboards then resolve these bitboards
    int start = pos->player*6; 
    for(int i = start; i < start + 6; ++i)
        resolve_bb(pos, lm, i);

    add_castling(pos, lm);
    add_enpassant(pos, lm);
}

void resolve_attacks(uint64_t attacks, LegalMoves* cm, uint8_t sq, uint8_t piece)
{
    // resolve piece to attacks squares into moves
    while (attacks)
    {
        int dest = __builtin_ctzll(attacks);
        Move m = (sq << 6) | dest; 

        if (is_promotion(piece, dest))
            add_promotion_moves(cm, m);
        else
            cm->moves[cm->count++] = m;

        attacks &= attacks - 1;
    }
}

void generate_captures(Position* pos, LegalMoves* cm)
{
    cm->count = 0;

    // resolve pieces of every bitboard 
    int start = pos->player * 6;

    uint64_t attacks = 0;
    for (int i = start; i < start + 6; ++i)
    {
        uint64_t bb = pos->bb[i];
        while (bb)
        {
            int sq = __builtin_ctzll(bb);

            attacks = get_moves_bitmask(pos->player, i, sq, pos->occ) & pos->occ[!pos->player]; 
            resolve_attacks(attacks, cm, sq, i);
            
            bb &= bb - 1; 
        }
    }

    if (pos->enpassant != INVALID_SQUARE)
        add_enpassant(pos, cm); // TODO without hashing
}

