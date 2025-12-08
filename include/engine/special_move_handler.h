
#ifndef SPECIAL_MOVE_H
#define SPECIAL_MOVE_H

#include <stdio.h>
#include <stdint.h>

#include "engine/constants.h"
#include "engine/pieces.h"
#include "engine/move.h"
#include "engine/special_move_handler.h"

#include "utils/bitboard_utils.h"

void handle_pawn_promotion(uint64_t* bitboards, Move* this_move);

void handle_double_pawn_push(int current_player, Move* this_move, int* enpassant_square);

void handle_enpassant(int current_player, uint64_t* bitboards, int enpassant_square);
void handle_castling(uint64_t* bitboards, Move* this_move, int* castle_rights);

#endif
