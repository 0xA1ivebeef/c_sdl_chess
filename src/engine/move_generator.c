
#include "engine/move_generator.h"

const int PROMOTION_FLAGS[4] = 
{
    KNIGHT_PROMOTION, // 3..6
    BISHOP_PROMOTION, 
    ROOK_PROMOTION,
    QUEEN_PROMOTION
};

const uint64_t PROMOTION_RANK_MASK[2] = 
{
    0xFF00000000000000ULL, // black  
    0x00000000000000FFULL  // white 
};

uint64_t pawn_attacks(int king_sq, int by_side)
{
    int side = by_side ? 1 : -1; // -1 if by_side==black
    int sq1 = king_sq + 7 * side;
    int sq2 = king_sq + 9 * side;
    return (1ULL << sq1) | (1ULL << sq2);
}

void add_move(Position* pos, LegalMoves* lm, int start, int dest, int flag)
{
    lm->moves[lm->count++] = (Move) { start, dest, flag };
}

void add_enpassant(Position* pos, LegalMoves* lm)
{
    if (pos->enpassant < 16 || pos->enpassant > 55)
    {
        // printf("enpassant not possible, square: %d\n", pos->enpassant);
        return;
    }

    uint64_t own_pawns = pos->bb[pos->player*6];

    int left_start;
    int right_start;
                                    
    if (pos->player == WHITE)
    {
        left_start = pos->enpassant + 7;
        right_start = pos->enpassant + 9;
    }
    else
    {
        left_start = pos->enpassant - 9;
        right_start = pos->enpassant - 7;
    }

    int ep_file = pos->enpassant % 8;
    if (ep_file == 0)
        left_start = INVALID_SQUARE;
    else if (ep_file == 7)
        right_start = INVALID_SQUARE;

    if (left_start != INVALID_SQUARE && own_pawns & (1ULL << left_start))
        add_move(pos, lm, left_start, pos->enpassant, ENPASSANT_FLAG);

    if (right_start != INVALID_SQUARE && own_pawns & (1ULL << right_start))
        add_move(pos, lm, right_start, pos->enpassant, ENPASSANT_FLAG);
}

uint64_t get_pawn_attacking_moves(int p, int sq, uint64_t* occ)
{
    if (!p)
	    return black_pawn_attack_bitmasks[sq] & occ[!p];
    return white_pawn_attack_bitmasks[sq] & occ[!p];
}

uint64_t get_pawn_legal_moves(int p, int sq, uint64_t* occ)
{
    uint64_t n_bm = p ? white_pawn_normal_bitmasks[sq] : black_pawn_normal_bitmasks[sq];
    uint64_t d_bm = p ? white_pawn_double_bitmasks[sq] & ~occ[2] : black_pawn_double_bitmasks[sq] & ~occ[2];
    uint64_t c_bm = p ? white_pawn_attack_bitmasks[sq] & occ[!p] : black_pawn_attack_bitmasks[sq] & occ[!p];

    uint64_t normal_legal = (n_bm & ~occ[2]);

    d_bm &= (p ? (normal_legal >> 8) : (normal_legal << 8));

    return (normal_legal | c_bm | d_bm);
}

uint64_t get_knight_legal_moves(int p, int sq, uint64_t* occ)
{
    return (knight_bitmasks[sq] & ~occ[p]);
}

void resolve_dir(int p, int sq, int dir, uint64_t* res, uint64_t* occ)
{
    int file = sq % 8;
    int rank = sq / 8;
    
    int x, y, attacking_sq;

    for (int i = 1; i < 8; ++i)
    {
        x = file + i * file_offsets[dir];
        y = rank + i * rank_offsets[dir];

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

uint64_t get_sliding_piece_legal_moves(int p, int sq, int piece, uint64_t* occ)
{
    uint64_t result = 0;

    int start = (piece == 2 ? 4 : 0); // bishop
    int end = (piece == 3 ? 4 : 8); // rook

    for (int dir = start; dir < end; ++dir)
        resolve_dir(p, sq, dir, &result, occ);

    return result;
}

uint64_t get_king_legal_moves(int p, int sq, uint64_t* occ)
{
    return (king_bitmasks[sq] & ~occ[p]);
}

uint64_t get_legal_moves_bitmask(int p, int piece, int sq, uint64_t* occ)
{
    // returns a bitmask that is resolved into move structs with startsquare and destsquare

    piece %= 6; // abstract color, keep piecetype
               
    switch(piece)
    {
        case 0:
            return get_pawn_legal_moves(p, sq, occ); 
        case 1:
            return get_knight_legal_moves(p, sq, occ);
        case 2:
        case 3:
        case 4:
            return get_sliding_piece_legal_moves(p, sq, piece, occ);
        case 5:
            return get_king_legal_moves(p, sq, occ);
        default:
            return 0;
    }
}

int is_promotion(int p, int piece, int ds)
{
    if (p * 6 != piece)
        return 0;

    if ((PROMOTION_RANK_MASK[p] & (1ULL << ds)) == 0)
        return 0;

    return 1;
}

void add_promotion_moves(Position* pos, LegalMoves* lm, Move* m)
{
    for (int i = 0; i < 4; ++i)
    {
        // printf("adding promotion move with %d\n", m->flags);
        m->flags = PROMOTION_FLAGS[i];
        lm->moves[lm->count++] = *m;
    }
}

// given bitboards and move, return if the move is a double pawn push
int is_double_pawn_push(Move* m, int piece)
{
    int start = m->start;
    int dest = m->dest;
	
    // moved piece is not a pawn
    if(!(piece == BLACK_PAWN || piece == WHITE_PAWN))
        return 0;

    int res = (abs_int(start - dest) == 16); // moved two squares 
    return res;
}

// called for each piece of the current player by resolve bitboard
void get_pieces_moves(Position* pos, LegalMoves* lm, int piece, int sq)
{
    uint64_t moves = get_legal_moves_bitmask(
            pos->player, piece, sq, pos->occ);
    
    while(moves)
    {
        int dest = __builtin_ctzll(moves);
        Move m = {sq, dest, 0}; 

        if (is_promotion(pos->player, piece, dest))
            add_promotion_moves(pos, lm, &m); // add manually cause 4x move
        else if (is_double_pawn_push(&m, piece))
        {
            m.flags = DOUBLE_PAWN_PUSH;
            lm->moves[lm->count++] = m; // set flag and add
        }
        else 
            lm->moves[lm->count++] = m; // just add
        
        moves &= moves - 1;
    }
}

void resolve_bb(Position* pos, LegalMoves* lm, int piece) 
{
    int current_bit = 0;
    uint64_t bb = pos->bb[piece];
    while(bb)
    {
        if(bb & 1)
            get_pieces_moves(pos, lm, piece, current_bit);
        
        bb >>= 1;
        ++current_bit;
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

    // legal move count is updated automatically
    add_castling(pos, lm);
    add_enpassant(pos, lm);
}

