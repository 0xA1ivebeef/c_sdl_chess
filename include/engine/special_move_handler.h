
#ifndef SPECIAL_MOVE_H
#define SPECIAL_MOVE_H

#include <stdio.h>
#include <stdint.h>

#include "engine/constants.h"
#include "engine/pieces.h"
#include "engine/move.h"

#include "utils/bitboard_utils.h"

int is_pawn_promotion(uint64_t* bitboards, Move* this_move);
void handle_pawn_promotion(uint64_t* bitboards, Move* this_move);

int is_double_pawn_push(Move* this_move, int ss_bb_i);
void handle_double_pawn_push(int current_player, Move* this_move, int* enpassant_square);

void handle_enpassant(int current_player, uint64_t* bitboards, int enpassant_square);
void handle_castling(uint64_t* bitboards, Move* this_move, int* castle_rights);

#endif
