
#include "engine/move_generator.h"

static int legal_moves_index = 0;

// TODO: where is the game_flags en passant square set?
void add_enpassant(int p, uint64_t* bitboards, int* game_flags, Move* legal_moves)
{
    int enpassant_square = game_flags[2];
    int current_bit = 0;
    uint64_t bb = bitboards[p*6];
    while(bb)
    {
        if(bb & 1)
        {
            // if enemy pawn on gameflags enpassant square
            if(bitmasks[5 - p][current_bit] & (1ULL << enpassant_square))
            {
                Move this_move = {current_bit, enpassant_square, 2};
                legal_moves[legal_moves_index] = this_move;
                ++legal_moves_index;
                printf("added enpassant move %d, %d\n", current_bit, enpassant_square);
            }
        }
        bb >>= 1;
        ++current_bit;
    }
}

int get_piece_square(uint64_t bb)
{
    int index = 0;
    while(bb)
    {
        bb >>= 1;
        ++index;
    }
    return index - 1;
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

uint64_t get_legal_moves_bitmask(int p, int bitboard_index, int square, uint64_t* occupancy_bitboards)
{
    bitboard_index %= 6;
    switch(bitboard_index)
    {
        case 0:
            return get_pawn_legal_moves(p, square, occupancy_bitboards);
        case 1:
            return get_knight_legal_moves(p, square, occupancy_bitboards);
        case 2:
        case 3:
        case 4:
            return get_sliding_piece_legal_moves(p, square, bitboard_index, occupancy_bitboards);
        case 5:
            return get_king_legal_moves(p, square, occupancy_bitboards);
        default:
            return 0;
    }
}

void get_pieces_moves(int p, int bitboard_index, int square, Move* legal_moves, uint64_t* occupancy_bitboards)
{
    uint64_t legal_moves_bitmask = get_legal_moves_bitmask(p, bitboard_index, square, occupancy_bitboards);
    
    int destsquare = 0;
    while(legal_moves_bitmask)
    {
        if(legal_moves_bitmask & 1)
        {
            Move m = {square, destsquare, 0};
            legal_moves[legal_moves_index] = m;
            legal_moves_index++;
        }
        legal_moves_bitmask >>= 1;
        destsquare++;
    }
}

// might make this async
void resolve_bitboard(int p, uint64_t bitboard, int bitboard_index, Move* legal_moves, uint64_t* occupancy_bitboards)
{
    int current_bit = 0;
    while(bitboard)
    {
        if(bitboard & 1)
            get_pieces_moves(p, bitboard_index, current_bit, legal_moves, occupancy_bitboards);
        
        bitboard >>= 1;
        ++current_bit;
    }
}

void generate_legal_moves(Position* position)
{
    int current_player = position->game_flags[0];

    printf("LEGAL MOVE GENERATION: current player is ");
    printf("%s", (current_player ? "WHITE" : "BLACK"));
    printf("\n");

    uint64_t enemy_attack_bitboard = position->attack_bitboards[!current_player];
    printf("Enemy attack bitboard is: \n");
    log_bitboard(&position->attack_bitboards[!current_player]);
    printf("\n");

    legal_moves_index = 0; // static
    int bitboard_start_index = current_player*6;
    for(int i = bitboard_start_index; i < bitboard_start_index + 6; i++)
    {
        resolve_bitboard(current_player, position->bitboards[i], i, position->legal_moves, position->occupancy);
    }

    int king_square = get_piece_square(position->bitboards[6*current_player+5]);
    printf("king is on square: %d\n", king_square);

    add_castling(king_square, position->bitboards, position->occupancy, position->game_flags, position->legal_moves, enemy_attack_bitboard);
    add_enpassant(current_player, position->bitboards, position->game_flags, position->legal_moves);
    position->legal_move_count = get_legal_move_count(position->legal_moves);
}

// TODO set a flag for pawn promotions to handle them in move_handler -> special move handler

