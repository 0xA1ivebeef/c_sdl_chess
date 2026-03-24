
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "engine/types.h"

static inline int rank(int sq) { return sq >> 3; }
static inline int file(int sq) { return sq & 7; }

static inline int move_from(Move m) { return (m & FROM_MASK) >> 6; }
static inline int move_to(Move m)   { return m & TO_MASK; }

static inline int has_pawn(Position* pos)
{
    return (pos->bb[BLACK_PAWN] != 0) || (pos->bb[WHITE_PAWN] != 0);
}

static inline int has_rook(Position* pos)
{
    return (pos->bb[BLACK_ROOK] != 0) || (pos->bb[WHITE_ROOK] != 0);
}

static inline int has_queen(Position* pos)
{
    return (pos->bb[BLACK_QUEEN] != 0) || (pos->bb[WHITE_QUEEN] != 0);
}

int time_up(SearchInfo* s);
double get_time_seconds();

int enpassant_legal(Position* pos, Move m, Undo* undo);

int get_king_sq(Position* pos, uint8_t player);

void generate_piece_on_sq(Position* pos);
void generate_occ(Position* pos);

int abs_int(int x);

int is_pawn_promotion(Position* pos, Move m);
void choose_promotion_move(Move* m);

int square_string_to_int(char* square_string);
const char* square_to_notation(int sq);

// const uint16_t CASTLING_MOVES[4] 
int is_enpassant_move(uint8_t enpassant, Move m, int moved_piece);
int is_castling_move(Move m, int moved_piece);
int is_promotion_move(Move m);
int is_double_pawn_move(Move m, int moved_piece);

#endif 

