
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "engine/types.h"

static inline int move_from(Move m) { return (m & FROM_MASK) >> 6; }
static inline int move_to(Move m)   { return m & TO_MASK; }

double get_time_seconds();

int enpassant_legal(Position* pos, Move m, Undo* undo);

int get_king_sq(Position* pos, uint8_t player);

void generate_piece_on_sq(Position* pos);
void generate_occ(Position* pos);

int get_bb_index(uint64_t* bb, int sq);
uint8_t get_legal_move_count(Move* lm);

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

