
#ifndef DOUBLE_PAWN_H
#define DOUBLE_PAWN_H

#include <stdio.h>
#include <stdint.h>

#include "engine/move.h"
#include "engine/castling.h"

void handle_castling(uint64_t* bitboards, Move* this_move, int* game_flags);
int is_double_pawn_push(uint64_t* bitboards, Move* this_move);
void handle_double_pawn_push(Move* legal_move, int* game_flags);
void handle_enpassant(uint64_t* bitboards, int* game_flags);

#endif
