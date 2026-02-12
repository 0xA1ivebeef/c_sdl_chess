
#include "engine/move_generator.h"

// bitmasks[11][64]
// 0  white pawn normal
// 1  black pawn normal
// 2  white pawn double
// 3  black pawn double
// 4  white pawn capture
// 5  black pawn capture
// 6  knight
// 7  bishop
// 8  rook
// 9  queen
// 10 king  

typedef enum ATTACKS
{
    WHITE_PAWN_ATTACKS = 4,
    BLACK_PAWN_ATTACKS,
    KNIGHT_ATTACKS,
    BISHOP_ATTACKS,
    ROOK_ATTACKS,
    QUEEN_ATTACKS,
    KING_ATTACKS,
} ATTACKS;

int PROMOTION_FLAGS[4] = 
{
    KNIGHT_PROMOTION, // 3..6
    BISHOP_PROMOTION, 
    ROOK_PROMOTION,
    QUEEN_PROMOTION
};

uint64_t PROMOTION_RANK_MASK[2] = 
{
    0xFF00000000000000ULL,  // Black promotion rank (squares 56–63)
    0x00000000000000FFULL // White promotion rank (squares 0–7)
};

uint64_t pawn_attacks(int king_sq, int by_side)
{
    int side = by_side ? 1 : -1; // -1 if by_side==black
    int sq1 = king_sq + 7 * side;
    int sq2 = king_sq + 9 * side;
    return (1ULL << sq1) | (1ULL << sq2);
}

void add_enpassant(Position* pos)
{
    int current_bit = 0;
    uint64_t pawn_bb = pos->bb[pos->player * 6];
    while(pawn_bb)
    {
        if(pawn_bb & 1)
        {
            // if enemy pawn on enpassant square
            if(bitmasks[5 - pos->player][current_bit] & (1ULL << pos->enpassant))
            {
                Move m = {current_bit, pos->enpassant, 2};
                pos->legal_moves[pos->legal_move_count++] = m; // append to legal_moves post increment

                // printf("added enpassant move %d, %d\n", current_bit, pos->enpassant_square);
            }
        }
        pawn_bb >>= 1;
        ++current_bit;
    }
}

uint64_t get_pawn_attacking_moves(int p, int square, uint64_t* occ)
{
	return bitmasks[5 - p][square] & occ[!p];
}

uint64_t get_pawn_legal_moves(int p, int square, uint64_t* occ)
{
    uint64_t normal_bitmask = bitmasks[!p][square];
    uint64_t double_pawn_push_bitmask = (bitmasks[3-p][square] & ~occ[2]);
    uint64_t capture_bitmask = bitmasks[5-p][square] & occ[!p];
    uint64_t normal_legal = (normal_bitmask & ~occ[2]);
    double_pawn_push_bitmask &= (p ? (normal_legal >> 8) : (normal_legal << 8));

    return (normal_legal | capture_bitmask | double_pawn_push_bitmask);
}

uint64_t get_knight_legal_moves(int p, int sq, uint64_t* occ)
{
    return (bitmasks[6][sq] & ~occ[p]);
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

    int start = (piece == 2 ? 4 : 0);
    int end = (piece == 3 ? 4 : 8);

    for (int dir = start; dir < end; ++dir)
        resolve_dir(p, sq, dir, &result, occ);

    return result;
}

uint64_t get_king_legal_moves(int p, int sq, uint64_t* occ)
{
    return (bitmasks[10][sq] & ~occ[p]);
}

uint64_t get_legal_moves_bitmask(int p, int bbi, int sq, uint64_t* occ)
{
    // returns a bitmask that is resolved into move structs with startsquare and destsquare
    bbi %= 6; // abstract color, keep piecetype
    switch(bbi)
    {
        case 0:
            return get_pawn_legal_moves(p, sq, occ); 
        case 1:
            return get_knight_legal_moves(p, sq, occ);
        case 2:
        case 3:
        case 4:
            return get_sliding_piece_legal_moves(p, sq, bbi, occ);
        case 5:
            return get_king_legal_moves(p, sq, occ);
        default:
            return 0;
    }
}

int is_promotion(int p, int bbi, int ds)
{
    if (p * 6 != bbi)
        return 0;

    if ((PROMOTION_RANK_MASK[p] & (1ULL << ds)) == 0)
        return 0;

    return 1;
}

void add_promotion_moves(Position* pos, Move* m)
{
    for (int i = 0; i < 4; ++i)
    {
        m->flags = PROMOTION_FLAGS[i];
        // printf("adding promotion move with %d\n", m->flags);
        pos->legal_moves[pos->legal_move_count++] = *m;
    }
}

// given bitboards and move, return if the move is a double pawn push
int is_double_pawn_push(Move* m, int bbi)
{
    int start = m->start;
    int dest = m->dest;
	
    // moved piece is not a pawn
    if(!(bbi == BLACK_PAWN || bbi == WHITE_PAWN))
        return 0;

    int res = (abs_int(start - dest) == 16); // moved two squares 
    return res;
}

// called for each piece of the current player by resolve bitboard
void get_pieces_moves(Position* pos, int bbi, int sq)
{
    uint64_t legal_moves_bitmask = get_legal_moves_bitmask(
            pos->player, bbi, sq, pos->occ);
    
    int ds = 0;
    while(legal_moves_bitmask)
    {
        if(legal_moves_bitmask & 1)
        {
            Move m = {sq, ds, 0}; 

            if (is_promotion(pos->player, bbi, ds))
                add_promotion_moves(pos, &m); // add manually cause 4x move
            else if (is_double_pawn_push(&m, bbi))
            {
                m.flags = DOUBLE_PAWN_PUSH;
                pos->legal_moves[pos->legal_move_count++] = m; // set flag and add
            }
            else
                pos->legal_moves[pos->legal_move_count++] = m; // just add
        }
        legal_moves_bitmask >>= 1;
        ds++;
    }
}

uint64_t bishop_attacks_naive(int sq, uint64_t occ) 
{
    uint64_t attacks = 0ULL;

    // Directions: NE (+9), NW (+7), SE (-7), SW (-9)
    int dir[4] = {9, 7, -7, -9};

    for (int d = 0; d < 4; d++) 
    {
        int s = sq;
        while (1) 
        {
            s += dir[d];

            if (s < 0 || s >= 64) break;        // off-board
            if (abs_int((s % 8) - ((s - dir[d]) % 8)) != 1) break; // file wrap

            attacks |= (1ULL << s);

            if (occ & (1ULL << s)) break; // stop at first blocker
        }
    }

    return attacks;
}

uint64_t rook_attacks_naive(int sq, uint64_t occ) 
{
    uint64_t attacks = 0ULL;

    int dir[4] = {1, -1, 8, -8}; // E, W, N, S

    for (int d = 0; d < 4; d++) 
    {
        int s = sq;
        while (1) 
        {
            s += dir[d];

            if (s < 0 || s >= 64) break;
            if ((dir[d] == 1 || dir[d] == -1) &&
                s/8 != (s - dir[d])/8) break; // file wrap

            attacks |= (1ULL << s);

            if (occ & (1ULL << s)) 
                break;
        }
    }

    return attacks;
}

// might make this async
void resolve_bb(Position* pos, int bbi) 
{
    int current_bit = 0;
    uint64_t bb = pos->bb[bbi];
    while(bb)
    {
        if(bb & 1)
            get_pieces_moves(pos, bbi, current_bit);
        
        bb >>= 1;
        ++current_bit;
    }
}

void generate_legal_moves(Position* pos)
{
    // clear array
    memset(pos->legal_moves, 0, sizeof(Move) * LEGAL_MOVES_SIZE);

    // reset this and use it for indexing, appending legal_moves 
    pos->legal_move_count = 0; 

    // index => only white or black bitboards then resolve these bitboards
    int bb_start = pos->player*6; 
    for(int i = bb_start; i < bb_start + 6; ++i)
        resolve_bb(pos, i);

    // legal move count is updated automatically
    add_castling(pos);
    add_enpassant(pos);
}

