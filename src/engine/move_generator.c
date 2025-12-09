
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

#define WHITE_PAWN_ATTACKS 4
#define BLACK_PAWN_ATTACKS 5
#define KNIGHT_ATTACKS 6
#define BISHOP_ATTACKS 7
#define ROOK_ATTACKS 8
#define QUEEN_ATTACKS 9
#define KING_ATTACKS 10

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

uint64_t pawn_attacks(int ksq, int by_side)
{
    int side = by_side ? 1 : -1; // -1 if by_side==black
    int sq1 = ksq + 7 * side;
    int sq2 = ksq + 9 * side;
    return (1ULL << sq1) | (1ULL << sq2);
}

void add_enpassant(Position* position)
{
    int current_player = position->current_player;
    int current_bit = 0;
    uint64_t pawn_bb = position->bitboards[current_player * 6];
    while(pawn_bb)
    {
        if(pawn_bb & 1)
        {
            // if enemy pawn on enpassant square
            if(bitmasks[5 - current_player][current_bit] & (1ULL << position->enpassant_square))
            {
                Move this_move = {current_bit, position->enpassant_square, 2};
                position->legal_moves[position->legal_move_count++] = this_move; // append to legal_moves post increment

                // printf("added enpassant move %d, %d\n", current_bit, position->enpassant_square);
            }
        }
        pawn_bb >>= 1;
        ++current_bit;
    }
}

uint64_t get_pawn_attacking_moves(int p, int square, uint64_t* occupancy_bitboards)
{
	return bitmasks[5 - p][square] & occupancy_bitboards[!p];
}

uint64_t get_pawn_legal_moves(int p, int square, uint64_t* occupancy_bitboards)
{
    uint64_t normal_bitmask = bitmasks[!p][square];
    uint64_t double_pawn_push_bitmask = (bitmasks[3-p][square] & ~occupancy_bitboards[2]);
    uint64_t capture_bitmask = bitmasks[5-p][square] & occupancy_bitboards[!p];
    uint64_t normal_legal = (normal_bitmask & ~occupancy_bitboards[2]);
    double_pawn_push_bitmask &= (p ? (normal_legal >> 8) : (normal_legal << 8));

    return (normal_legal | capture_bitmask | double_pawn_push_bitmask);
}

uint64_t get_knight_legal_moves(int p, int square, uint64_t* occupancy_bitboards)
{
    return (bitmasks[6][square] & ~occupancy_bitboards[p]);
}

void resolve_direction(int p, int square, int dir, uint64_t* result, uint64_t* occupancy_bitboards)
{
    int file = square % 8;
    int rank = square / 8;
    
    int this_file, this_rank, attacking_square;

    for (int i = 1; i < 8; ++i)
    {
        this_file = file + i * file_offsets[dir];
        this_rank = rank + i * rank_offsets[dir];

        if (this_file < 0 || this_file > 7 || this_rank < 0 || this_rank > 7)
            return;

        attacking_square = this_rank * 8 + this_file;

        if (occupancy_bitboards[!p] & (1ULL << attacking_square))
        {
            *result |= (1ULL << attacking_square);
            return;
        }
        else if (occupancy_bitboards[p] & (1ULL << attacking_square))
            return;
        else
            *result |= (1ULL << attacking_square);
    }
}

uint64_t get_sliding_piece_legal_moves(int p, int square, int piece_index, uint64_t* occupancy_bitboards)
{
    uint64_t result = 0;

    int startindex = (piece_index == 2 ? 4 : 0);
    int endindex = (piece_index == 3 ? 4 : 8);

    for (int dir = startindex; dir < endindex; ++dir)
        resolve_direction(p, square, dir, &result, occupancy_bitboards);

    return result;
}

uint64_t get_king_legal_moves(int p, int square, uint64_t* occupancy_bitboards)
{
    return (bitmasks[10][square] & ~occupancy_bitboards[p]);
}

uint64_t get_legal_moves_bitmask(int current_player, int bitboard_index, int piece_square, uint64_t* occupancy_bitboards)
{
    // returns a bitmask that is resolved into move structs with startsquare and destsquare
    bitboard_index %= 6; // abstract color, keep piecetype
    switch(bitboard_index)
    {
        case 0:
            return get_pawn_legal_moves(current_player, piece_square, occupancy_bitboards); 
        case 1:
            return get_knight_legal_moves(current_player, piece_square, occupancy_bitboards);
        case 2:
        case 3:
        case 4:
            return get_sliding_piece_legal_moves(current_player, piece_square, bitboard_index, occupancy_bitboards);
        case 5:
            return get_king_legal_moves(current_player, piece_square, occupancy_bitboards);
        default:
            return 0;
    }
}

int is_promotion(int p, int bitboard_index, int destsquare)
{
    if (p * 6 != bitboard_index)
        return 0;

    if ((PROMOTION_RANK_MASK[p] & (1ULL << destsquare)) == 0)
        return 0;

    return 1;
}

void add_promotion_moves(Position* position, Move* m)
{
    for (int i = 0; i < 4; ++i)
    {
        m->flags = PROMOTION_FLAGS[i];
        // printf("adding promotion move with %d\n", m->flags);
        position->legal_moves[position->legal_move_count++] = *m;
    }
}

// given bitboards and move, return if the move is a double pawn push
int is_double_pawn_push(Move* this_move, int ss_bb_i)
{
    int startsquare = this_move->startsquare;
    int destsquare = this_move->destsquare;
	
    // moved piece is not a pawn
    if(!(ss_bb_i == BLACK_PAWN || ss_bb_i == WHITE_PAWN))
        return 0;

    int res = (abs_int(startsquare - destsquare) == 16); // moved two squares 
    return res;
}

// called for each piece of the current player by resolve bitboard
void get_pieces_moves(Position* position, int bitboard_index, int piece_square)
{
    uint64_t legal_moves_bitmask = get_legal_moves_bitmask(position->current_player, bitboard_index, piece_square, position->occupancy);
    
    int destsquare = 0;
    while(legal_moves_bitmask)
    {
        if(legal_moves_bitmask & 1)
        {
            Move m = {piece_square, destsquare, 0}; 

            if (is_promotion(position->current_player, bitboard_index, destsquare))
                add_promotion_moves(position, &m); // add manually cause 4x move
            else if (is_double_pawn_push(&m, bitboard_index))
            {
                m.flags = DOUBLE_PAWN_PUSH;
                position->legal_moves[position->legal_move_count++] = m; // set flag and add
            }
            else
                position->legal_moves[position->legal_move_count++] = m; // just add
        }
        legal_moves_bitmask >>= 1;
        destsquare++;
    }
}

uint64_t bishop_attacks_naive(int sq, uint64_t occupancy) 
{
    uint64_t attacks = 0ULL;

    // Directions: NE (+9), NW (+7), SE (-7), SW (-9)
    int directions[4] = {9, 7, -7, -9};

    for (int d = 0; d < 4; d++) 
    {
        int s = sq;
        while (1) 
        {
            s += directions[d];

            if (s < 0 || s >= 64) break;        // off-board
            if (abs_int((s % 8) - ((s - directions[d]) % 8)) != 1) break; // file wrap

            attacks |= (1ULL << s);

            if (occupancy & (1ULL << s)) break; // stop at first blocker
        }
    }

    return attacks;
}

uint64_t rook_attacks_naive(int sq, uint64_t occupancy) 
{
    uint64_t attacks = 0ULL;

    int directions[4] = {1, -1, 8, -8}; // E, W, N, S

    for (int d = 0; d < 4; d++) 
    {
        int s = sq;
        while (1) 
        {
            s += directions[d];

            if (s < 0 || s >= 64) break;
            if ((directions[d] == 1 || directions[d] == -1) &&
                s/8 != (s - directions[d])/8) break; // file wrap

            attacks |= (1ULL << s);

            if (occupancy & (1ULL << s)) break;
        }
    }

    return attacks;
}

int square_under_attack(uint64_t* bitboards, uint64_t* occ, int sq, int by_side)
{
    int pawn_bb_i   = by_side * 6 + 0; // 6 if white, 0 if black
    int knight_bb_i = by_side * 6 + 1;
    int bishop_bb_i = by_side * 6 + 2;
    int rook_bb_i   = by_side * 6 + 3;
    int queen_bb_i  = by_side * 6 + 4;
    int king_bb_i   = by_side * 6 + 5;

    uint64_t bishop_attacking_bitboard = bishop_attacks_naive(sq, occ[2]);
    uint64_t rook_attacking_bitboard = rook_attacks_naive(sq, occ[2]);

    // printf("SQUARE UNDER ATTACK bishop and rook attack bitboard form kingsquare\n");
    // log_bitboard(&bishop_attacking_bitboard);
    // log_bitboard(&rook_attacking_bitboard);

    // printf("SQUARE UNDER ATTACK checking if square %d is under attack by %s\n", 
       //     sq, by_side ? "WHITE" : "BLACK");

    if (pawn_attacks(sq, by_side) & bitboards[pawn_bb_i]) 
    {
        // printf("SQUARE UNDER ATTACK: pawn attacks square %d\n", sq);
        return 1;
    }
    if (bitmasks[KNIGHT_ATTACKS][sq] & bitboards[knight_bb_i]) 
    {
        // printf("SQUARE UNDER ATTACK: knight attacks square %d\n", sq);
        return 1;
    } 
    if (bitmasks[KING_ATTACKS][sq] & bitboards[king_bb_i]) 
    {
        // printf("SQUARE UNDER ATTACK: king attacks square %d\n", sq);
        return 1;
    }
    if (bishop_attacking_bitboard & ((bitboards[bishop_bb_i] | bitboards[queen_bb_i]))) 
    {
        // printf("SQUARE UNDER ATTACK: bishop or queen attacks square %d\n", sq);
        return 1;
    }
    
    if (rook_attacking_bitboard & ((bitboards[rook_bb_i] | bitboards[queen_bb_i]))) 
    {
        // printf("SQUARE UNDER ATTACK: rook or queen attacks square %d\n", sq);
        return 1;
    }
    return 0;
}

// might make this async
void resolve_bitboard(Position* position, int bitboard_index) 
{
    int current_bit = 0;
    uint64_t bitboard = position->bitboards[bitboard_index];
    while(bitboard)
    {
        if(bitboard & 1)
            get_pieces_moves(position, bitboard_index, current_bit);
        
        bitboard >>= 1;
        ++current_bit;
    }
}

void generate_legal_moves(Position* position)
{
    // clear array
    memset(position->legal_moves, 0, sizeof(Move) * LEGAL_MOVES_SIZE);

    // reset this and use it for indexing, appending legal_moves 
    position->legal_move_count = 0; 

    // index => only white or black bitboards then resolve these bitboards
    int bitboard_start_index = position->current_player*6; 
    for(int i = bitboard_start_index; i < bitboard_start_index + 6; i++)
        resolve_bitboard(position, i);

    // legal move count is updated automatically
    add_castling(position);
    add_enpassant(position);
}

