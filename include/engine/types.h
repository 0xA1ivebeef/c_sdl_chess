
#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include "engine/constants.h"

#define BLACK 0
#define WHITE 1

typedef enum Piece
{
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING,
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    PIECE_COUNT
} Piece;

typedef struct 
{
    int running;
    uint8_t selected_square;
    int needs_update;
    int game_over;
} UIContext;

/*  
 *  POLYGLOT LAYOUT
    promo start  dest
    0xxx 000000 000000
*/
typedef uint16_t Move;

typedef struct LegalMoves
{
    Move moves[LEGAL_MOVES_SIZE];
    uint8_t count;
} LegalMoves;

typedef struct
{
    int nodes;
    Move move;
} MoveNode;

typedef struct
{
    uint64_t    bb[12];
    uint64_t    occ[3];

    uint8_t     player;
    uint8_t     castle_rights;
    uint8_t     enpassant;
    uint8_t     enpassant_hashed;
    uint8_t     halfmove;
    uint16_t    fullmove;
    uint64_t    zobrist_hash;
} Position;

typedef struct
{
    int         moved_piece; // can be neg
    int         captured_piece; 
    uint8_t     castle_rights;
    uint8_t     enpassant;
    uint8_t     enpassant_hashed;
    uint8_t     halfmove;
    uint16_t    fullmove;
    uint64_t    zobrist_hash;
} Undo;

// ! polyglot books are in BIG ENDIAN
typedef struct 
{
    uint64_t    key;
    Move        move;
    uint16_t    weight;
    uint32_t    learn;
} PolyglotEntry;

#endif

