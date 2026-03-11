
#include "engine/zobrist.h"

uint64_t pcg_r64()
{
    uint64_t high = (uint64_t)pcg32_random();
    uint64_t low  = (uint64_t)pcg32_random();
    return (high << 32) | low;
}

uint64_t zobrist_table[12*64];
uint64_t zobrist_castling[16];
uint64_t zobrist_enpassant[8];
uint64_t zobrist_black_to_move;

void init_zobrist()
{
    pcg32_srandom(time(NULL), 54ULL);

    for (int i = 0; i < 12*64; ++i)
        zobrist_table[i] = pcg_r64();

    for (int i = 0; i < 16; ++i)
        zobrist_castling[i] = pcg_r64();

    for (int i = 0; i < 8; ++i)
        zobrist_enpassant[i] = pcg_r64();

    zobrist_black_to_move = pcg_r64();
}

uint64_t get_zobrist_hash(Position* pos)
{
    uint64_t hash = 0;

    // bitboards - xor hash of piece on square for all pieces
    for (int i = 0; i < 12; ++i)
    {
        uint64_t bb = pos->bb[i];
        while(bb)
        {
            int sq = __builtin_ctzll(bb);
            hash ^= zobrist_table[i*64 + sq];
            bb &= bb - 1;
        }
    }

    hash ^= zobrist_castling[pos->castle_rights]; 
    
    if (pos->enpassant >= 16 && pos->enpassant <= 55)
        hash ^= zobrist_enpassant[pos->enpassant % 8];

    if (pos->player == BLACK)
        hash ^= zobrist_black_to_move;

    return hash;
}

/*
 * for incremental updates of zobrist hash
    | Change           | What to XOR                                                                       |
| ---------------- | --------------------------------------------------------------------------------- |
| **Piece moves**  | XOR the piece out of the old square, XOR it into the new square                   |
| **Capture**      | XOR the captured piece out of its square                                          |
| **Promotion**    | XOR the pawn out of the old square, XOR the promoted piece into the new square    |
| **Castling**     | XOR the old castling rights out, XOR the new castling rights in                   |
| **En passant**   | XOR the old en passant file out (if any), XOR the new en passant file in (if any) |
| **Side to move** | XOR `zobrist_black_to_move`                                                       |

*/
