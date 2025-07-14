
important functions:
void generate_legal_moves(int p, uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags, move* legal_moves, uint64_t atk_bb)
uint64_t get_attack_bitboard(int p, uint64_t* bitboards, uint64_t* occupancy_bitboards, int* game_flags)

typedef struct
{
    uint64_t bitboards[12];
    uint64_t occupancy[3];
    int game_flags[5];
    Move legal_moves[LEGAL_MOVES_SIZE]
    int running;
    int needs_update;
    int selected_square;
} GameContext;

problems:

DONE:
enpassant still possible after inbetween-move 
fix:
remove enpassant after it held on one move
(enpassant is always only possible for a single move)
maybe just clear the game_flag after each move fixed it

castling:
cant castle when squares king +- 2 are under attack
but only for white (32bit int cast thingy)

discovered check:
filter moves that put the king in check in move generation
before a player makes a move

TODO:

stalemate / draw
pawn promotion

opponent ? 

!!! IF ROOK IS CAPTURED A NEW ROOK SPAWNS ON CASTLING !!! 
