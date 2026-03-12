
#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <string.h>
#include <stdint.h>

#include "engine/constants.h"
#include "engine/types.h"
#include "engine/bitmask_loader.h"
#include "engine/castling.h"

#include "ui/console_logger.h"

#include "utils/utils.h"

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

void generate_legal_moves(Position* pos, LegalMoves* legal_moves);

#endif

